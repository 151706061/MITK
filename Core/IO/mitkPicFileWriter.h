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


#ifndef _MITK_PIC_FILE_WRITER__H_
#define _MITK_PIC_FILE_WRITER__H_

#if(_MSC_VER>=1200)
#include <iomanip>
#endif

#include <itkProcessObject.h>
#include <mitkFileWriter.h>
#include <mitkPointSet.h>
#include <mitkSurface.h>

namespace mitk
{
class Image;
/**
 * @brief Writer for mitk::Image
 * @ingroup IO
 */
class PicFileWriter : public itk::ProcessObject, public mitk::FileWriter
{
public:

    mitkClassMacro( PicFileWriter, itk::ProcessObject );

    itkNewMacro( Self );

    mitkWriterMacro;

    /**
     * Sets the filename of the file to write.
     * @param _arg the name of the file to write.
     */
    itkSetStringMacro( FileName );

    /**
     * @returns the name of the file to be written to disk.
     */
    itkGetStringMacro( FileName );

    /**
     * @warning multiple write not (yet) supported
     */
    itkSetStringMacro( FilePrefix );

    /**
     * @warning multiple write not (yet) supported
     */
    itkGetStringMacro( FilePrefix );

    /**
     * @warning multiple write not (yet) supported
     */
    itkSetStringMacro( FilePattern );

    /**
     * @warning multiple write not (yet) supported
     */
    itkGetStringMacro( FilePattern );

    /**
     * Sets the 0'th input object for the filter.
     * @param input the first input for the filter.
     */
    void SetInput( mitk::Image* input );

    /**
     * @returns the 0'th input object of the filter.
     */
    const mitk::Image* GetInput();

protected:

    /**
     * Constructor.
     */
    PicFileWriter();

    /**
     * Virtual destructor.
     */
    virtual ~PicFileWriter();

    virtual void GenerateData();

    std::string m_FileName;

    std::string m_FilePrefix;

    std::string m_FilePattern;
};

}

#endif //_MITK_PIC_FILE_WRITER__H_
