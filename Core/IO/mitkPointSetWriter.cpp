#include "mitkPointSetWriter.h"
#include <iostream>
#include <fstream>


//
// Initialization of the xml tags.
//

const char* mitk::PointSetWriter::XML_POINT_SET_FILE = "point_set_file" ;

const char* mitk::PointSetWriter::XML_FILE_VERSION = "file_version" ;

const char* mitk::PointSetWriter::XML_POINT_SET = "point_set" ;

const char* mitk::PointSetWriter::XML_POINT = "point" ;

const char* mitk::PointSetWriter::XML_ID = "id" ;

const char* mitk::PointSetWriter::XML_X = "x" ;

const char* mitk::PointSetWriter::XML_Y = "y" ;

const char* mitk::PointSetWriter::XML_Z = "z" ;

const char* mitk::PointSetWriter::VERSION_STRING = "0.1" ;




mitk::PointSetWriter::PointSetWriter()
{
    this->SetNumberOfRequiredInputs( 1 );
    this->SetNumberOfOutputs( 1 );
    this->SetNthOutput( 0, mitk::PointSet::New().GetPointer() );
    m_Indent = 2;
    m_IndentDepth = 0;
}




mitk::PointSetWriter::~PointSetWriter()
{}




void mitk::PointSetWriter::GenerateData()
{
    m_IndentDepth = 0;

    //
    // Opening the file to write to
    //
    if ( m_FileName == "" )
    {
        itkWarningMacro( << "Sorry, filename has not been set!" );
        return ;
    }
    std::ofstream out( m_FileName.c_str() );
    if ( !out.good() )
    {
        itkWarningMacro( << "Sorry, file " << m_FileName << " could not be opened!" );
        out.close();
        return ;
    }

    //
    // Here the actual xml writing begins
    //
    WriteXMLHeader( out );
    WriteStartElement( XML_POINT_SET_FILE, out );
    WriteStartElement( XML_FILE_VERSION, out );
    WriteCharacterData( VERSION_STRING, out );
    WriteEndElement( XML_FILE_VERSION, out, false );

    //
    // for each input object write its xml representation to
    // the stream
    //
    for ( unsigned int i = 0 ; i < this->GetNumberOfInputs(); ++i )
    {
        InputType::Pointer pointSet = this->GetInput( i );
        assert( pointSet.IsNotNull() );
        WriteXML( pointSet.GetPointer(), out );
    }

    WriteEndElement( XML_POINT_SET_FILE, out );

    out.close();
}




void mitk::PointSetWriter::WriteXML( mitk::PointSet* pointSet, std::ofstream& out )
{
    WriteStartElement( XML_POINT_SET, out );
    mitk::PointSet::PointsContainer* pointsContainer = pointSet->GetPointSet()->GetPoints();
    mitk::PointSet::PointsContainer::Iterator it;
    for ( it = pointsContainer->Begin(); it != pointsContainer->End(); ++it )
    {
        WriteStartElement( XML_POINT, out );

        WriteStartElement( XML_ID, out );
        WriteCharacterData( ConvertToString<int>( it->Index() ).c_str() , out );
        WriteEndElement( XML_ID, out, false );

        mitk::PointSet::PointType point = it->Value();

        WriteStartElement( XML_X, out );
        WriteCharacterData( ConvertToString<mitk::ScalarType>( point[ 0 ] ).c_str(), out );
        WriteEndElement( XML_X, out, false );

        WriteStartElement( XML_Y, out );
        WriteCharacterData( ConvertToString<mitk::ScalarType>( point[ 1 ] ).c_str(), out );
        WriteEndElement( XML_Y, out, false );

        WriteStartElement( XML_Z, out );
        WriteCharacterData( ConvertToString<mitk::ScalarType>( point[ 2 ] ).c_str(), out );
        WriteEndElement( XML_Z, out, false );

        WriteEndElement( XML_POINT, out );
    }
    WriteEndElement( XML_POINT_SET, out );
}





void mitk::PointSetWriter::ResizeInputs( const unsigned int& num )
{
    unsigned int prevNum = this->GetNumberOfInputs();
    this->SetNumberOfInputs( num );
    for ( unsigned int i = prevNum; i < num; ++i )
    {
        this->SetNthInput( i, mitk::PointSet::New().GetPointer() );
    }
}




void mitk::PointSetWriter::SetInput( InputType* pointSet )
{
    this->ProcessObject::SetNthInput( 0, pointSet );
}




void mitk::PointSetWriter::SetInput( const unsigned int& id, InputType* pointSet )
{
    if ( id >= this->GetNumberOfInputs() )
        this->ResizeInputs( id + 1 );
    this->ProcessObject::SetNthInput( id, pointSet );
}



mitk::PointSet* mitk::PointSetWriter::GetInput()
{
    if ( this->GetNumberOfInputs() < 1 )
    {
        return 0;
    }
    else
    {
        return dynamic_cast<InputType*> ( this->GetInput( 0 ) );
    }
}




mitk::PointSet* mitk::PointSetWriter::GetInput( const unsigned int& num )
{
    return dynamic_cast<InputType*> ( this->ProcessObject::GetInput( num ) );
}





template < typename T>
std::string mitk::PointSetWriter::ConvertToString( T value )
{
    std::ostringstream o;
    if ( o << value )
        return o.str();
    else
        return "conversion error";
}




void mitk::PointSetWriter::WriteXMLHeader( std::ofstream &file )
{
    file << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";
}




void mitk::PointSetWriter::WriteStartElement( const char *const tag, std::ofstream &file )
{
    file << std::endl;
    WriteIndent( file );
    file << '<' << tag << '>';
    m_IndentDepth++;
}




void mitk::PointSetWriter::WriteEndElement( const char *const tag, std::ofstream &file, const bool& indent )
{
    m_IndentDepth--;
    if ( indent )
    {
        file << std::endl;
        WriteIndent( file );
    }
    file << '<' << '/' << tag << '>';
}




void mitk::PointSetWriter::WriteCharacterData( const char *const data, std::ofstream &file )
{
    file << data;
}




void mitk::PointSetWriter::WriteStartElement( std::string &tag, std::ofstream &file )
{
    WriteStartElement( tag.c_str(), file );
}




void mitk::PointSetWriter::WriteEndElement( std::string &tag, std::ofstream &file, const bool& indent )
{
    WriteEndElement( tag.c_str(), file, indent );
}




void mitk::PointSetWriter::WriteCharacterData( std::string &data, std::ofstream &file )
{
    WriteCharacterData( data.c_str(), file );
}




void mitk::PointSetWriter::WriteIndent( std::ofstream& file )
{
    std::string spaces( m_IndentDepth * m_Indent, ' ' );
    file << spaces.c_str();
}

