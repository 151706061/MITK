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


#include "mitkPointSetVtkMapper3D.h"
#include <mitkDataTreeNode.h>
#include <mitkProperties.h>
#include <mitkColorProperty.h>
#include <mitkOpenGLRenderer.h>
#include "mitkPointSet.h"

#include <vtkActor.h>

#include <vtkPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkPropAssembly.h>
#include <vtkTubeFilter.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkConeSource.h>
#include <vtkCylinderSource.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkCellArray.h>
#include <vtkVectorText.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <stdlib.h>



//##ModelId=3E70F60301D5
const mitk::PointSet* mitk::PointSetVtkMapper3D::GetInput()
{
  return static_cast<const mitk::PointSet * > ( GetData() );
}

//##ModelId=3E70F60301F4
mitk::PointSetVtkMapper3D::PointSetVtkMapper3D() 
: m_vtkSelectedPointList(NULL), m_vtkUnselectedPointList(NULL), m_vtkContourPolyData(NULL),
m_VtkSelectedPolyDataMapper(NULL), m_VtkUnselectedPolyDataMapper(NULL),m_vtkContourPolyDataMapper(NULL),
m_vtkTextList(NULL), m_contour(NULL), m_tubefilter(NULL),
m_NumberOfSelectedAdded(0), m_NumberOfUnselectedAdded(0)
{
  //propassembly
  m_PointsAssembly = vtkPropAssembly::New();

  //creating actors to be able to set transform
  m_SelectedActor = vtkActor::New();
  m_UnselectedActor = vtkActor::New();
  m_ContourActor = vtkActor::New();
}

//##ModelId=3E70F60301F5
mitk::PointSetVtkMapper3D::~PointSetVtkMapper3D()
{
  m_PointsAssembly->Delete();

  m_SelectedActor->Delete();
  m_UnselectedActor->Delete();
  m_ContourActor->Delete();
}


void mitk::PointSetVtkMapper3D::GenerateData()
{
  if(m_PointsAssembly->GetParts()->IsItemPresent(m_SelectedActor))
    m_PointsAssembly->RemovePart(m_SelectedActor);
  if(m_PointsAssembly->GetParts()->IsItemPresent(m_UnselectedActor))
    m_PointsAssembly->RemovePart(m_UnselectedActor);
  if(m_PointsAssembly->GetParts()->IsItemPresent(m_ContourActor))
    m_PointsAssembly->RemovePart(m_ContourActor);

  mitk::PointSet::Pointer input  = const_cast<mitk::PointSet*>(this->GetInput());
  input->Update();
  mitk::PointSet::DataType::Pointer itkPointSet = input->GetPointSet();

  mitk::PointSet::PointsContainer::Iterator pointsIter;
  mitk::PointSet::PointDataContainer::Iterator pointDataIter;
  int j;

  m_NumberOfSelectedAdded = 0;
  m_NumberOfUnselectedAdded = 0;


  //check for color props and use it for rendering of selected/unselected points and contour 
  //due to different params in VTK (double/float) we have to convert!

  //vars to convert to
#if ((VTK_MAJOR_VERSION > 4) || ((VTK_MAJOR_VERSION==4) && (VTK_MINOR_VERSION>=4) ))
  double unselectedColor[4]={1.0f,1.0f,1.0f,1.0f};
  double selectedColor[4]={1.0f,0.0f,0.0f,0.0f};//red
  double contourColor[4]={1.0f,0.0f,0.0f,0.0f};//red
#else
  float unselectedColor[4]={1.0f,1.0f,1.0f,1.0f};
  float selectedColor[4]={1.0f,0.0f,0.0f,0.0f};//red
  float contourColor[4]={1.0f,0.0f,0.0f,0.0f};//red
#endif

  mitk::Color tmpColor;

  //get unselected from node
  float unselectedColorTMP[4]={1.0f,1.0f,1.0f,1.0f};
  m_DataTreeNode->GetColor(unselectedColorTMP, NULL);
  //check if there is an unselected property
  if (dynamic_cast<mitk::ColorProperty*>(this->GetDataTreeNode()->GetProperty("unselectedcolor").GetPointer()) != NULL)
  {
    tmpColor = dynamic_cast<mitk::ColorProperty *>(this->GetDataTreeNode()->GetProperty("unselectedcolor").GetPointer())->GetValue();
    unselectedColor[0] = tmpColor[0];
    unselectedColor[1] = tmpColor[1];
    unselectedColor[2] = tmpColor[2];
    unselectedColor[3] = 1.0f; //!!define a new ColorProp to be able to pass alpha value
  }
  else
  {
    unselectedColor[0] = unselectedColorTMP[0];
    unselectedColor[1] = unselectedColorTMP[1];
    unselectedColor[2] = unselectedColorTMP[2];
    //unselectedColor[3] stays 1.0f
  }

  //get selected property
  if (dynamic_cast<mitk::ColorProperty*>(this->GetDataTreeNode()->GetProperty("selectedcolor").GetPointer()) != NULL)
  {
    tmpColor = dynamic_cast<mitk::ColorProperty *>(this->GetDataTreeNode()->GetProperty("selectedcolor").GetPointer())->GetValue();
    selectedColor[0] = tmpColor[0];
    selectedColor[1] = tmpColor[1];
    selectedColor[2] = tmpColor[2];
    selectedColor[3] = 1.0f;
  }

  //get contour property
  if (dynamic_cast<mitk::ColorProperty*>(this->GetDataTreeNode()->GetProperty("contourcolor").GetPointer()) != NULL)
  {
    tmpColor = dynamic_cast<mitk::ColorProperty *>(this->GetDataTreeNode()->GetProperty("contourcolor").GetPointer())->GetValue();
    contourColor[0] = tmpColor[0];
    contourColor[1] = tmpColor[1];
    contourColor[2] = tmpColor[2];
    contourColor[3] = 1.0f;
  }
  //finished color fishing!

  //check if a contour shall be drawn
  bool makeContour;
  if (dynamic_cast<mitk::BoolProperty *>(this->GetDataTreeNode()->GetProperty("contour").GetPointer()) == NULL)
    makeContour = false;
  else
    makeContour = dynamic_cast<mitk::BoolProperty *>(this->GetDataTreeNode()->GetProperty("contour").GetPointer())->GetValue();

  //create contour
  if (makeContour)
  {
    m_vtkContourPolyData = vtkAppendPolyData::New();
    m_vtkContourPolyDataMapper = vtkPolyDataMapper::New(); 

    vtkPoints *points = vtkPoints::New();
    vtkCellArray *polys = vtkCellArray::New();

    for (j=0, pointsIter=itkPointSet->GetPoints()->Begin(); pointsIter!=itkPointSet->GetPoints()->End() ; pointsIter++,j++)
    {
      int cell[2] = {j-1,j};
      points->InsertPoint(j,pointsIter.Value()[0],pointsIter.Value()[1],pointsIter.Value()[2]);
      if (j>0)
        polys->InsertNextCell(2,cell);
    }

    bool close;
    if (dynamic_cast<mitk::BoolProperty *>(this->GetDataTreeNode()->GetProperty("close").GetPointer()) == NULL)
      close = false;
    else
      close = dynamic_cast<mitk::BoolProperty *>(this->GetDataTreeNode()->GetProperty("close").GetPointer())->GetValue();
    if (close) 
    {
      int cell[2] = {j-1,0};
      polys->InsertNextCell(2,cell);
    }

    m_contour = vtkPolyData::New();
    m_contour->SetPoints(points);
    points->Delete();
    m_contour->SetLines(polys);
    polys->Delete();
    m_contour->Update();

    m_tubefilter = vtkTubeFilter::New();
    m_tubefilter->SetInput(m_contour);
    m_contour->Delete();

    //check for property contoursize. If not present, then take pointsize
    int radius = 1;
    mitk::IntProperty::Pointer contourSizeProp = dynamic_cast<mitk::IntProperty *>(this->GetDataTreeNode()->GetProperty("contoursize").GetPointer());
    if (contourSizeProp.IsNull())
      contourSizeProp = dynamic_cast<mitk::IntProperty *>(this->GetDataTreeNode()->GetProperty("pointsize").GetPointer());
    if (contourSizeProp.IsNotNull())
      radius = contourSizeProp->GetValue();
    m_tubefilter->SetRadius(radius);

    //	  m_tubefilter->SetRadiusFactor(7);
    //	  m_tubefilter->SetNumberOfSides(6);
    m_tubefilter->Update();

    //add to pipeline
    m_vtkContourPolyData->AddInput(m_tubefilter->GetOutput());
    m_tubefilter->Delete();
    m_vtkContourPolyDataMapper->SetInput(m_vtkContourPolyData->GetOutput());
    m_vtkContourPolyData->Delete();

    //create a new instance of the actor
    m_ContourActor->Delete();
    m_ContourActor = vtkActor::New();

    m_ContourActor->SetMapper(m_vtkContourPolyDataMapper);
    m_vtkContourPolyDataMapper->Delete();
    m_ContourActor->GetProperty()->SetColor(contourColor);
    m_PointsAssembly->AddPart(m_ContourActor);
  }

  //now fill selected and unselected pointList
  //get size of Points in Property
  int pointSize = 2;
  mitk::IntProperty::Pointer pointSizeProp = dynamic_cast<mitk::IntProperty *>(this->GetDataTreeNode()->GetProperty("pointsize").GetPointer());
  if ( pointSizeProp.IsNotNull() )
    pointSize = pointSizeProp->GetValue();

  bool showLabel = true;
  this->GetDataTreeNode()->GetBoolProperty("show label", showLabel);
  const char * pointLabel=NULL;
  if(showLabel)
  {
    if(dynamic_cast<mitk::StringProperty *>(this->GetDataTreeNode()->GetProperty("label").GetPointer()) != NULL)
    {
      pointLabel =dynamic_cast<mitk::StringProperty *>(this->GetDataTreeNode()->GetProperty("label").GetPointer())->GetValue();
    }
    else
      showLabel = false;
  }

  m_vtkSelectedPointList = vtkAppendPolyData::New();
  m_vtkUnselectedPointList = vtkAppendPolyData::New();
  //now add an object for each point in data
  for (j=0, pointsIter=itkPointSet->GetPoints()->Begin(), pointDataIter = itkPointSet->GetPointData()->Begin(); 
    pointsIter!=itkPointSet->GetPoints()->End();
    pointsIter++, pointDataIter++, j++)
  {
    //check for the pointtype in data and decide which geom-object to take and then add to the selected or unselected list
    int pointType = pointDataIter.Value().pointSpec;
    vtkPolyDataSource *source;
    switch (pointType)
    {
    case mitk::PTUNDEFINED:
      {
        vtkSphereSource *sphere = vtkSphereSource::New();
        sphere->SetRadius(pointSize);
        sphere->SetCenter(pointsIter.Value()[0],pointsIter.Value()[1],pointsIter.Value()[2]);
        source = sphere;
      }
      break;
    case mitk::PTSTART:
      {
        vtkCubeSource *cube = vtkCubeSource::New();
        cube->SetXLength(pointSize/2);
        cube->SetYLength(pointSize/2);
        cube->SetZLength(pointSize/2);
        cube->SetCenter(pointsIter.Value()[0],pointsIter.Value()[1],pointsIter.Value()[2]);
        source = cube;
      }
      break;
    case mitk::PTCORNER:
      {
        vtkConeSource *cone = vtkConeSource::New();
        cone->SetRadius(pointSize);
        cone->SetCenter(pointsIter.Value()[0],pointsIter.Value()[1],pointsIter.Value()[2]);
        source = cone;
      }
      break;
    case mitk::PTEDGE:
      {
        vtkCylinderSource *cylinder = vtkCylinderSource::New();
        cylinder->SetRadius(pointSize);
        cylinder->SetCenter(pointsIter.Value()[0],pointsIter.Value()[1],pointsIter.Value()[2]);
        source = cylinder;
      }
      break;
    case mitk::PTEND:
      {
        vtkSphereSource *sphere = vtkSphereSource::New();
        sphere->SetRadius(pointSize);
        sphere->SetCenter(pointsIter.Value()[0],pointsIter.Value()[1],pointsIter.Value()[2]);
        source = sphere;
      }
      break;
    default:
      {
        vtkSphereSource *sphere = vtkSphereSource::New();
        sphere->SetRadius(pointSize);
        sphere->SetCenter(pointsIter.Value()[0],pointsIter.Value()[1],pointsIter.Value()[2]);
        source = sphere;
      }
      break;
    }
    if (pointDataIter.Value().selected)
    {
      m_vtkSelectedPointList->AddInput(source->GetOutput());
      ++m_NumberOfSelectedAdded;
    }
    else 
    {
      m_vtkUnselectedPointList->AddInput(source->GetOutput());
      ++m_NumberOfUnselectedAdded;
    }
    source->Delete();

    if (showLabel)
    {
      char buffer[20];
      std::string l = pointLabel;
      if ( input->GetSize()>1 )
      {
        sprintf(buffer,"%d",j+1);
        l.append(buffer);
      }
      // Define the text for the label
      vtkVectorText *label = vtkVectorText::New();
      label->SetText(l.c_str());

      //# Set up a transform to move the label to a new position.
      vtkTransform *aLabelTransform =vtkTransform::New();
      aLabelTransform->Identity();
      aLabelTransform->Translate(pointsIter.Value()[0]+2,pointsIter.Value()[1]+2,pointsIter.Value()[2]);
      aLabelTransform->Scale(5.7,5.7,5.7);

      //# Move the label to a new position.
      vtkTransformPolyDataFilter *labelTransform = vtkTransformPolyDataFilter::New();
      labelTransform->SetTransform(aLabelTransform);
      aLabelTransform->Delete();
      labelTransform->SetInput(label->GetOutput());
      label->Delete();

      //add it to the wright PointList
      if (pointDataIter.Value().selected)
      {
        m_vtkSelectedPointList->AddInput(labelTransform->GetOutput());
        ++m_NumberOfSelectedAdded;
      }
      else 
      {
        m_vtkUnselectedPointList->AddInput(labelTransform->GetOutput());
        ++m_NumberOfUnselectedAdded;
      }
      labelTransform->Delete();
    }
  }

  //now according to number of elements added to selected or unselected, build up the rendering pipeline
  if (m_NumberOfSelectedAdded > 0)
  {
    m_VtkSelectedPolyDataMapper = vtkPolyDataMapper::New();
    m_VtkSelectedPolyDataMapper->SetInput(m_vtkSelectedPointList->GetOutput());
    m_vtkSelectedPointList->Delete();

    //create a new instance of the actor
    m_SelectedActor->Delete();
    m_SelectedActor = vtkActor::New();

    m_SelectedActor->SetMapper(m_VtkSelectedPolyDataMapper);
    m_VtkSelectedPolyDataMapper->Delete();
    m_SelectedActor->GetProperty()->SetColor(selectedColor);
    m_PointsAssembly->AddPart(m_SelectedActor);
  }

  if (m_NumberOfUnselectedAdded > 0)
  {
    m_VtkUnselectedPolyDataMapper = vtkPolyDataMapper::New();
    m_VtkUnselectedPolyDataMapper->SetInput(m_vtkUnselectedPointList->GetOutput());
    m_vtkUnselectedPointList->Delete();

    //create a new instance of the actor
    m_UnselectedActor->Delete();
    m_UnselectedActor = vtkActor::New();

    m_UnselectedActor->SetMapper(m_VtkUnselectedPolyDataMapper);
    m_VtkUnselectedPolyDataMapper->Delete();
    m_UnselectedActor->GetProperty()->SetColor(unselectedColor);
    m_PointsAssembly->AddPart(m_UnselectedActor);
  }
}


//##ModelId=3EF19FA803BF
void mitk::PointSetVtkMapper3D::GenerateData(mitk::BaseRenderer* renderer)
{
  bool makeContour;
  if (dynamic_cast<mitk::BoolProperty *>(this->GetDataTreeNode()->GetProperty("contour").GetPointer()) == NULL)
    makeContour = false;
  else
    makeContour = dynamic_cast<mitk::BoolProperty *>(this->GetDataTreeNode()->GetProperty("contour").GetPointer())->GetValue();

  if(IsVisible(renderer)==false)
  {
    m_UnselectedActor->VisibilityOff();
    m_SelectedActor->VisibilityOff();
    m_ContourActor->VisibilityOff();
    return;
  }

  m_UnselectedActor->VisibilityOn();
  m_SelectedActor->VisibilityOn();

  if (makeContour)
  {
    m_ContourActor->VisibilityOn();
  }
  else
  {
    m_ContourActor->VisibilityOff();
  }
}

vtkProp* mitk::PointSetVtkMapper3D::GetProp()
{
  if(GetDataTreeNode()!=NULL && 
    m_PointsAssembly != NULL) 
  {
    m_SelectedActor->SetUserTransform(GetDataTreeNode()->GetVtkTransform());
    m_UnselectedActor->SetUserTransform(GetDataTreeNode()->GetVtkTransform());
    m_ContourActor->SetUserTransform(GetDataTreeNode()->GetVtkTransform());
  } 
  return m_PointsAssembly;
}

