#include "Step7.h"

#include <QmitkRenderWindow.h>
#include <QmitkSliceWidget.h>

#include <mitkProperties.h>

#include <mitkSurface.h>

#include <vtkMarchingCubes.h>
#include <vtkSTLWriter.h>

//##Documentation
//## @brief Convert result of region-grower into a surface
//## by means of a VTK filter
Step7::Step7( int argc, char* argv[], QWidget *parent, const char *name )
  : Step6( argc, argv, parent, name )
{
}


void Step7::StartRegionGrowing()
{
  Step6::StartRegionGrowing();

  if(m_ResultImage.IsNotNull())
  {
    m_ResultNode->SetProperty("volumerendering", new mitk::BoolProperty(false));

    vtkMarchingCubes* surfaceCreator = vtkMarchingCubes::New();
    surfaceCreator->SetInput(m_ResultImage->GetVtkImageData());
    surfaceCreator->SetValue(0, 1);

//    vtkSTLWriter* writer = vtkSTLWriter::New();
//    writer->SetInput(surfaceCreator->GetOutput());
//    writer->SetFileName("C:\\lungsOfOrig_volume.stl");
//writer->Write();

    mitk::Surface::Pointer surface = mitk::Surface::New();
    surface->SetVtkPolyData(surfaceCreator->GetOutput());

    mitk::DataTreeNode::Pointer surfaceNode = mitk::DataTreeNode::New();
    surfaceNode->SetData(surface);

    mitk::DataTreePreOrderIterator it(m_Tree);
    it.Add(surfaceNode);

    mitk::RenderWindow::UpdateAllInstances();
  }
}

/**
\example Step7.cpp
*/
