#ifndef MITK_BASE_XML_WRITER
#define MITK_BASE_XML_WRITER

#include <fstream>
#include <string>
#include <stack>
#include <sstream>

namespace mitk{

  /// Brief Base class for writing XML nodes and XML attributes.
  ///
  /// Implements methods for writing a XML file (XML nodes and XML attributes).
  /// The data is stored with XML attributes.
  /// The functions WriteProperty() are used to write XML attributes. The first parameter specifies the name of the attribute. The second parameter holds the data of the attribute.
  /// Ingroup IO
  class BaseXMLWriter {
    
    /// encapsulates all information of a node in a stream
    class StreamNode
    {
    public:
      StreamNode( std::string name ): m_Name(name){};
      std::string GetName() { return m_Name; };
      std::ostream& GetPropertyStream() { return m_PropertyStream; };
      std::ostream& GetChildrenStream() { return m_ChildrenStream; };
      std::ostream& GetComment() { return m_Comment; };      

      void Write( std::ostream& out, int steps );
    private:
      std::string m_Name;
      std::stringstream m_PropertyStream;
      std::stringstream m_ChildrenStream;
      std::stringstream m_Comment;
    };

    std::stack<StreamNode*> m_Stack;
    std::ostream* m_Out;
    int m_Increase;
    int m_Space;
    int m_NodeCount;
    bool m_File;
    bool m_FirstNode;

  public:

    /// constructor
    BaseXMLWriter( const char* filename, int space = 3);

    /// constructor
    BaseXMLWriter( std::ostream& out, int space = 3 );

    /// destructor
    virtual ~BaseXMLWriter();

    /// begins a new XML node
    //  the parameter name specifies the name of the XML node
    void BeginNode( const std::string& name );

    /// closes an open XML node
    void EndNode( );

    /// writes a XML attribute that datatype is a string
    void WriteProperty( const std::string& key, const char* value ) const;

    /// writes a XML attribute that datatype is a string
    void WriteProperty( const std::string& key, const std::string& value ) const;

    /// writes a XML attribute that datatype is an integer
    void WriteProperty( const std::string& key, int value ) const;

    /// writes a XML attribute that datatype is a float
    void WriteProperty( const std::string& key, float value ) const;

    /// writes a XML attribute that datatype is a double
    void WriteProperty( const std::string& key, double value ) const;

    /// writes a XML attribute that datatype is a bool
    void WriteProperty( const std::string& key, bool value ) const;

    /// writes a comment in the XML file
    void WriteComment( const std::string& key );
  
    /// returns the current node depth
    int GetCurrentDeph() const;

    /// returns the current count of nodes
    int GetNodeCount() const;

    int GetSpace() const;

    void SetSpace( int space );

protected:

    /// replaces characters "<" and ">" with "{" and "}"
    const char* ConvertString( const char* string ) const;
  };
}
#endif // MITK_BASE_XML_WRITER
