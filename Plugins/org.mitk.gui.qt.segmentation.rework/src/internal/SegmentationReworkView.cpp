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

// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// Qmitk
#include "SegmentationReworkView.h"

// Qt
#include <QMessageBox>

#include <mitkIOUtil.h>
#include "QmitkNewSegmentationDialog.h"
#include "mitkSegTool2D.h"
#include "mitkToolManagerProvider.h"

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp> 
#include <boost/lexical_cast.hpp>

const std::string SegmentationReworkView::VIEW_ID = "org.mitk.views.segmentationreworkview";

void SegmentationReworkView::SetFocus() {}

void SegmentationReworkView::CreateQtPartControl(QWidget *parent)
{
  // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi(parent);

  qRegisterMetaType< std::vector<std::string> >("std::vector<std::string>");

  connect(m_Controls.buttonUpload, &QPushButton::clicked, this, &SegmentationReworkView::UploadNewSegmentation);
  connect(m_Controls.buttonNewSeg, &QPushButton::clicked, this, &SegmentationReworkView::CreateNewSegmentationC);
  connect(m_Controls.cleanDicomBtn, &QPushButton::clicked, this, &SegmentationReworkView::CleanDicomFolder);

  m_downloadBaseDir = std::experimental::filesystem::path("/temp/");

  if (!std::experimental::filesystem::exists(m_downloadBaseDir))
  {
    std::experimental::filesystem::create_directory(m_downloadBaseDir);
  }


  utility::string_t port = U("2020");
  utility::string_t address = U("http://127.0.0.1:");
  address.append(port);

  m_HttpHandler = std::unique_ptr<SegmentationReworkREST>(new SegmentationReworkREST(address));

  connect(m_HttpHandler.get(),
    &SegmentationReworkREST::InvokeUpdateChartWidget,
    this,
    &SegmentationReworkView::UpdateChartWidget);
  connect(this, &SegmentationReworkView::InvokeLoadData, this, &SegmentationReworkView::LoadData);

  m_HttpHandler->SetPutCallback(std::bind(&SegmentationReworkView::RESTPutCallback, this, std::placeholders::_1));
  m_HttpHandler->SetGetCallback(std::bind(&SegmentationReworkView::RESTGetCallback, this, std::placeholders::_1));
  m_HttpHandler->Open().wait();

  MITK_INFO << "Listening for requests at: " << utility::conversions::to_utf8string(address);

  utility::string_t pacsHost = U("http://193.174.48.78:8090/dcm4chee-arc/aets/DCM4CHEE");
  m_RestClient = new mitk::RESTClient(pacsHost);
}

void SegmentationReworkView::RESTPutCallback(const SegmentationReworkREST::DicomDTO &dto)
{
  SetSimilarityGraph(dto.simScoreArray, dto.minSliceStart);

  MITK_INFO << "Load related dicom series ...";
  boost::uuids::random_generator generator;
    
  std::string folderPathSeries = std::experimental::filesystem::path(m_downloadBaseDir).append(boost::uuids::to_string(generator())).string() + "/";
  std::experimental::filesystem::create_directory(folderPathSeries);

  std::string pathSegA = std::experimental::filesystem::path(m_downloadBaseDir).append(boost::uuids::to_string(generator())).string() + "/";
  std::string pathSegB = std::experimental::filesystem::path(m_downloadBaseDir).append(boost::uuids::to_string(generator())).string() + "/";

  auto folderPathSegA = utility::conversions::to_string_t(pathSegA);
  auto folderPathSegB = utility::conversions::to_string_t(pathSegB);

  std::experimental::filesystem::create_directory(pathSegA);
  std::experimental::filesystem::create_directory(pathSegB);

  m_CurrentStudyUID = dto.studyUID;

  std::vector<pplx::task<std::string>> tasks;
  auto imageSeriesTask = m_RestClient->WadoRS(utility::conversions::to_string_t(folderPathSeries), dto.studyUID, dto.imageSeriesUID);
  auto segATask = m_RestClient->WadoRS(folderPathSegA, dto.studyUID, dto.segSeriesUIDA);
  auto segBTask = m_RestClient->WadoRS(folderPathSegB, dto.studyUID, dto.segSeriesUIDB);
  tasks.push_back(imageSeriesTask);
  tasks.push_back(segATask);
  tasks.push_back(segBTask);

  auto joinTask = pplx::when_all(begin(tasks), end(tasks));
  auto filePathList = joinTask.then([&](std::vector<std::string> filePathList) {
    auto fileNameA = std::experimental::filesystem::path(filePathList[1]).filename();
    auto fileNameB = std::experimental::filesystem::path(filePathList[2]).filename();
    m_Controls.labelSegA->setText(m_Controls.labelSegA->text() + " " + QString::fromUtf8(fileNameA.string().c_str()));
    m_Controls.labelSegB->setText(m_Controls.labelSegB->text() + " " + QString::fromUtf8(fileNameB.string().c_str()));
    InvokeLoadData(filePathList);
  });
}

void SegmentationReworkView::RESTGetCallback(const SegmentationReworkREST::DicomDTO &dto) 
{
  boost::uuids::random_generator generator;

  std::string folderPathSeries = std::experimental::filesystem::path(m_downloadBaseDir).append(boost::uuids::to_string(generator())).string() + "/";
  std::experimental::filesystem::create_directory(folderPathSeries);

  std::string pathSeg = std::experimental::filesystem::path(m_downloadBaseDir).append(boost::uuids::to_string(generator())).string() + "/";
  auto folderPathSeg = utility::conversions::to_string_t(pathSeg);
  std::experimental::filesystem::create_directory(pathSeg);

  std::vector<pplx::task<std::string>> tasks;
  auto imageSeriesTask = m_RestClient->WadoRS(utility::conversions::to_string_t(folderPathSeries), dto.studyUID, dto.imageSeriesUID);
  auto segATask = m_RestClient->WadoRS(folderPathSeg, dto.studyUID, dto.segSeriesUIDA);
  tasks.push_back(imageSeriesTask);
  tasks.push_back(segATask);

  auto joinTask = pplx::when_all(begin(tasks), end(tasks));
  auto filePathList = joinTask.then([&](std::vector<std::string> filePathList) { InvokeLoadData(filePathList); });
}

void SegmentationReworkView::LoadData(std::vector<std::string> filePathList)
{
  MITK_INFO << "Loading finished. Pushing data to data storage ...";
  auto ds = GetDataStorage();
  auto dataNodes = mitk::IOUtil::Load(filePathList, *ds);
  // reinit view
  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(ds);

  // find data nodes
  m_Image = dataNodes->at(0);
  m_SegA = dataNodes->at(1);
  m_SegB = dataNodes->at(2);
}

void SegmentationReworkView::UpdateChartWidget()
{
  m_Controls.chartWidget->Show();
}

void SegmentationReworkView::SetSimilarityGraph(std::vector<double> simScoreArray, int sliceMinStart)
{
  std::map<double, double> map;

  double sliceIndex = sliceMinStart;
  for (double score : simScoreArray)
  {
    map.insert(std::map<double, double>::value_type(sliceIndex, score));
    sliceIndex++;
  }
  m_Controls.chartWidget->AddData2D(map, "similarity score");
  m_Controls.chartWidget->SetChartType("similarity score", QmitkChartWidget::ChartType::line);
  m_Controls.chartWidget->SetXAxisLabel("slice number");
  m_Controls.chartWidget->SetYAxisLabel("similarity score");
}

void SegmentationReworkView::OnSelectionChanged(berry::IWorkbenchPart::Pointer /*source*/,
  const QList<mitk::DataNode::Pointer> &nodes)
{
  // iterate all selected objects, adjust warning visibility
  foreach(mitk::DataNode::Pointer node, nodes)
  {
    if (node.IsNotNull() && dynamic_cast<mitk::Image *>(node->GetData()))
    {
      m_Controls.labelWarning->setVisible(false);
      return;
    }
  }

  m_Controls.labelWarning->setVisible(true);
}

void SegmentationReworkView::UploadNewSegmentation()
{
  auto filePath = U("1.2.276.0.7230010.3.1.4.296485376.8.1533635734.141264.dcm");
  m_CurrentStudyUID = "1.2.840.113654.2.70.1.159145727925405623564217141386659468090";
  try {
    m_RestClient->StowRS(filePath, m_CurrentStudyUID).wait();
  }
  catch (const std::exception &exception)
  {
    std::cout << exception.what() << std::endl;
  }
}

void SegmentationReworkView::CreateNewSegmentationC() 
{
  mitk::ToolManager* toolManager = mitk::ToolManagerProvider::GetInstance()->GetToolManager();
  toolManager->SetReferenceData(m_Image);
  toolManager->SetWorkingData(m_SegC);
}

void SegmentationReworkView::CleanDicomFolder() 
{
  std::experimental::filesystem::remove_all(m_downloadBaseDir);
  std::experimental::filesystem::create_directory(m_downloadBaseDir);
}

void SegmentationReworkView::DoImageProcessing()
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  if (nodes.empty())
    return;

  mitk::DataNode *node = nodes.front();

  if (!node)
  {
    // Nothing selected. Inform the user and return
    QMessageBox::information(nullptr, "Template", "Please load and select an image before starting image processing.");
    return;
  }

  // here we have a valid mitk::DataNode

  // a node itself is not very useful, we need its data item (the image)
  mitk::BaseData *data = node->GetData();
  if (data)
  {
    // test if this data item is an image or not (could also be a surface or something totally different)
    mitk::Image *image = dynamic_cast<mitk::Image *>(data);
    if (image)
    {
      std::stringstream message;
      std::string name;
      message << "Performing image processing for image ";
      if (node->GetName(name))
      {
        // a property called "name" was found for this DataNode
        message << "'" << name << "'";
      }
      message << ".";
      MITK_INFO << message.str();

      // actually do something here...
    }
  }
}
