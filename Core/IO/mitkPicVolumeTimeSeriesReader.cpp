/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "mitkPicVolumeTimeSeriesReader.h"
#include "mitkPicFileReader.h"
#include <itkImageFileReader.h>
#include <string>

extern "C" 
{
ipPicDescriptor * MITKipPicGet( char *infile_name, ipPicDescriptor *pic );
ipPicDescriptor * MITKipPicGetTags( char *infile_name, ipPicDescriptor *pic );
}


void mitk::PicVolumeTimeSeriesReader::GenerateOutputInformation()
{
    mitk::Image::Pointer output = this->GetOutput();

    if ( ( output->IsInitialized() ) && ( this->GetMTime() <= m_ReadHeaderTime.GetMTime() ) )
        return ;

    itkDebugMacro( << "Reading file for GenerateOutputInformation()" << m_FileName );

    if ( ! this->GenerateFileList() )
    {
        itkWarningMacro( "Sorry, file list could not be generated!" );
        return ;
    }

    //
    // Read the first file of the image sequence. Assume equal size and spacings for all
    // other volumes. @TODO Integrate support for different sizes and spacings
    //
    char* filename = const_cast<char *> ( m_MatchedFileNames[ 0 ].c_str() );
    ipPicDescriptor * header = ipPicGetHeader( filename, NULL );
    header = MITKipPicGetTags( filename, header );

    if ( header == NULL )
    {
        itk::ImageFileReaderException e( __FILE__, __LINE__ );
        itk::OStringStream msg;
        msg << " Could not read file " << m_FileName.c_str();
        e.SetDescription( msg.str().c_str() );
        throw e;
        return ;
    }
    if ( header->dim != 3 )
    {
        itk::ImageFileReaderException e( __FILE__, __LINE__ , "Only 3D-pic volumes are supported! " );
        throw e;
        return ;
    }

    //
    // modify the header to match the real temporal extent
    //
    header->dim = 4;
    header->n[ 3 ] = m_MatchedFileNames.size();

    output->Initialize( header );

    ipPicFree( header );

    m_ReadHeaderTime.Modified();
}


void mitk::PicVolumeTimeSeriesReader::GenerateData()
{
    mitk::Image::Pointer output = this->GetOutput();

    //
    // Check to see if we can read the file given the name or prefix
    //
    if ( m_FilePrefix == "" || m_FilePattern == "" )
    {
        throw itk::ImageFileReaderException( __FILE__, __LINE__, "Both FilePattern and FilePrefix must be non-empty" );
    }

    if ( m_MatchedFileNames.size() == 0 )
    {
        throw itk::ImageFileReaderException( __FILE__, __LINE__, "Sorry, there are no files to read!" );
    }


    //
    // read 3d volumes and copy them to the 4d volume
    //
    ipPicDescriptor* volume3d = NULL;
    for ( unsigned int t = 0 ; t < m_MatchedFileNames.size() ; ++t )
    {
        char* filename = const_cast< char* >( m_MatchedFileNames[ t ].c_str() );
        std::cout << "Reading file " << filename << "..." << std::endl;
        volume3d = MITKipPicGet( filename, NULL );

        if ( volume3d == NULL )
        {
            throw itk::ImageFileReaderException( __FILE__, __LINE__, "File could not be read!" );
        }

        //
        // @TODO do some error checking
        //

        //
        // copy the 3d data volume to the 4d volume
        //

        // \todo use memory of Image as in PicFileReader (or integrate everything into the PicFileReader!)
        PicFileReader::ConvertHandedness(volume3d);
        output->SetPicVolume( volume3d, t );
        ipPicFree ( volume3d );
    }
}

mitk::PicVolumeTimeSeriesReader::PicVolumeTimeSeriesReader()
{
    //this->DebugOn();
}

mitk::PicVolumeTimeSeriesReader::~PicVolumeTimeSeriesReader()
{}

