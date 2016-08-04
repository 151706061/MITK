/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef QmitkIGTTrackingDataEvaluationView_h
#define QmitkIGTTrackingDataEvaluationView_h

#include <berryISelectionListener.h>

#include <QmitkFunctionality.h>

#include "ui_QmitkIGTTrackingDataEvaluationViewControls.h"

#include <mitkNavigationDataEvaluationFilter.h>



/*!
  \brief QmitkIGTTrackingDataEvaluationView

  \warning  This application module is not yet documented. Use "svn blame/praise/annotate" and ask the author to provide basic documentation.

  \sa QmitkFunctionality
  \ingroup Functionalities
*/
class QmitkIGTTrackingDataEvaluationView : public QmitkFunctionality
{
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT

  public:

    static const std::string VIEW_ID;

    QmitkIGTTrackingDataEvaluationView();
    virtual ~QmitkIGTTrackingDataEvaluationView();

    virtual void CreateQtPartControl(QWidget *parent);

    virtual void StdMultiWidgetAvailable (QmitkStdMultiWidget &stdMultiWidget);
    virtual void StdMultiWidgetNotAvailable();

  protected slots:

    void OnLoadFileList();
    void OnAddToCurrentList();
    void OnEvaluateData();
    void OnGeneratePointSet();
    void OnGeneratePointSetsOfSinglePositions();
    void OnGenerateRotationLines();
    void OnGenerateGroundTruthPointSet();
    void OnConvertCSVtoXMLFile();
    void OnCSVtoXMLLoadInputList();
    void OnCSVtoXMLLoadOutputList();

    /** Reads in exactly three position files als reference. */
    void OnOrientationCalculation_CalcRef();
    /** Uses always three positions (1,2,3: first orientation; 4,5,6: second orientation; and so on) in every file to calcualte a orientation. */
    void OnOrientationCalculation_CalcOrientandWriteToFile();


  protected:

    Ui::QmitkIGTTrackingDataEvaluationViewControls* m_Controls;

    QmitkStdMultiWidget* m_MultiWidget;

    std::vector<std::string> m_FilenameVector;

    void MessageBox(std::string s);

    std::fstream m_CurrentWriteFile;
    void WriteHeader();
    void WriteDataSet(mitk::NavigationDataEvaluationFilter::Pointer evaluationFilter, std::string dataSetName);

    //members for orientation calculation
    mitk::Point3D m_RefPoint1;
    mitk::Point3D m_RefPoint2;
    mitk::Point3D m_RefPoint3;

    double m_scalingfactor; //scaling factor for visualization, 1 by default

    //angle diffrences: seperated file
    std::fstream m_CurrentAngleDifferencesWriteFile;
    void CalculateDifferenceAngles();
    void WriteDifferenceAnglesHeader();
    void WriteDifferenceAnglesDataSet(std::string pos1, std::string pos2, int idx1, int idx2, double angle);

    //different help methods to read a csv logging file
    std::vector<mitk::NavigationData::Pointer> GetNavigationDatasFromFile(std::string filename);
    std::vector<std::string> GetFileContentLineByLine(std::string filename);
    mitk::NavigationData::Pointer GetNavigationDataOutOfOneLine(std::string line);

    //CSV to XML members
    std::vector<std::string> m_CSVtoXMLInputFilenameVector;
    std::vector<std::string> m_CSVtoXMLOutputFilenameVector;

    //returns the number of converted lines
    int ConvertOneFile(std::string inputFilename, std::string outputFilename);

    /** @brief calculates the angle in the plane perpendicular to the rotation axis of the two quaterions. */
    double GetAngleBetweenTwoQuaterions(mitk::Quaternion a, mitk::Quaternion b);

    /** @brief calculates the slerp average of a set of quaternions which is stored in the navigation data evaluation filter */
    mitk::Quaternion GetSLERPAverage(mitk::NavigationDataEvaluationFilter::Pointer);
};



#endif // _QMITKIGTTRACKINGDATAEVALUATIONVIEW_H_INCLUDED
