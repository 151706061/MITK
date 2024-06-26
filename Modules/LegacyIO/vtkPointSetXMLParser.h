/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef _VTK_POINT_SET_XML_READER__H_
#define _VTK_POINT_SET_XML_READER__H_

#include <MitkLegacyIOExports.h>
#include <list>
#include <mitkPointSet.h>
#include <stack>
#include <string>
#include <vtkXMLParser.h>

namespace mitk
{
  /**
   * @brief Implementation of the vtkXMLParser interface for reading mitk::PointSets.
   *
   * This class implements the XMLParser interface of the vtkXMLParser which is based
   * on expat. It is used by the mitk::PointSetReader and is NOT INTENDED TO BE USED
   * FROM THE END-USER. If you want to read point sets, use the mitk::PointSetReader.
   * @ingroup MitkLegacyIOModule
   *
   * @deprecatedSince{2014_10} Use mitk::IOUtils or mitk::FileReaderRegistry instead.
   */
  class DEPRECATED() MITKLEGACYIO_EXPORT vtkPointSetXMLParser : public vtkXMLParser
  {
  public:
    vtkTypeMacro(vtkPointSetXMLParser, vtkXMLParser);

    static vtkPointSetXMLParser *New();

    typedef mitk::PointSet PointSetType;

    typedef std::stack<std::string> ParseStack;

    typedef std::list<PointSetType::Pointer> PointSetList;

    typedef PointSetType::DataType::PointIdentifier PointIdentifier;

    typedef PointSetType::PointType PointType;

    int InitializeParser() override;
    int CleanupParser() override;
    /**
     * Handler function which is called, when a new xml start-tag
     * has been parsed.
     */
    void StartElement(const char *name, const char **atts) override;

    /**
     * Handler function which is called, when a xml end-tag
     * has been parsed.
     */
    void EndElement(const char *name) override;

    /**
     * Handler function which is called, if character data has been
     * parsed by expat.
     * @param inData a char array containing the parsed string data
     * @param inLength the length of the parsed data string.
     */
    void CharacterDataHandler(const char *inData, int inLength) override;

    /**
     * Converts the given data to mitk::ScalarType.
     */
    virtual mitk::ScalarType ParseScalarType(const std::string &data);

    /**
     * Converts the given data to an PointIdentifier
     */
    virtual PointIdentifier ParsePointIdentifier(const std::string &data);

    /**
     * @returns the list of point sets which have been read from file.
     * NOTE: your have to call the Parse() function, before this function.
     */
    virtual PointSetList GetParsedPointSets();

  protected:
    vtkPointSetXMLParser();
    ~vtkPointSetXMLParser() override;

    /**
     * A stack containing the parsed start-tags.
     * If an end tag is encountered, it is matched with the
     * top element of the stack.
     */
    ParseStack m_ParseStack;

    /**
     * Contains the parsed point sets.
     */
    PointSetList m_PointSetList;

    /**
     * The current point set which is processed
     * by the parser.
     */
    PointSetType::Pointer m_CurrentPointSet;

    /**
     * The current point which is processed
     * by the parser.
     */
    PointType m_CurrentPoint;

    std::string m_CurId;
    std::string m_CurXString;
    std::string m_CurYString;
    std::string m_CurZString;

    /**
     * The current point id which is processed
     * by the parser.
     */
    PointIdentifier m_CurrentPointId;

    std::locale m_PreviousLocale;
  };
}
#endif // _VTK_POINT_SET_XML_READER__H_
