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


#include "mitkSurface.h"
#include "mitkCommon.h"
#include "mitkVector.h"
#include "mitkTimeSlicedGeometry.h"

#include "vtkPolyData.h"
#include "vtkSphereSource.h"

#include <fstream>

int mitkSurfaceTest(int /*argc*/, char* /*argv*/[])
{
  mitk::Surface::Pointer surface;
  std::cout << "Testing mitk::Surface::New(): ";
  surface = mitk::Surface::New();
  if (surface.IsNull()) {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  else {
  std::cout<<"[PASSED]"<<std::endl;
  } 

  vtkSphereSource* sphereSource = vtkSphereSource::New();
  sphereSource->SetCenter(0,0,0);
  sphereSource->SetRadius(5.0);
  sphereSource->SetThetaResolution(10);
  sphereSource->SetPhiResolution(10);
  sphereSource->Update();

  vtkPolyData* polys = sphereSource->GetOutput();
  surface->SetVtkPolyData( polys );
  std::cout << "Testing mitk::Surface::SetVtkPolyData(): ";
  if (surface->GetVtkPolyData() ==  NULL ) {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  else {
  std::cout<<"[PASSED]"<<std::endl;
  } 

  {
    vtkFloatingPointType bounds[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    polys->ComputeBounds();
    polys->GetBounds( bounds );

    std::cout << "Testing GetBoundingBox() ";
    surface->UpdateOutputInformation();
    surface->SetRequestedRegionToLargestPossibleRegion();
    //  mitk::BoundingBox bb = const_cast<mitk::BoundingBox*>(
    mitk::BoundingBox* bb = const_cast<mitk::BoundingBox*>(surface->GetGeometry()->GetBoundingBox());
    mitk::BoundingBox::BoundsArrayType surfBounds = bb->GetBounds();

    if ( bounds[0] != surfBounds[0] 
    || bounds[1] != surfBounds[1] 
    || bounds[2] != surfBounds[2] 
    || bounds[3] != surfBounds[3] 
    || bounds[4] != surfBounds[4] 
    || bounds[5] != surfBounds[5] 
    ) {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    else {
      std::cout<<"[PASSED]"<<std::endl;
    } 
  }


  std::cout << "Testing mitk::Surface::Initialize( timesteps): ";
  surface->Initialize(5);
  surface->Update();
  surface->SetRequestedRegionToLargestPossibleRegion();
  mitk::Surface::RegionType requestedRegion = surface->GetRequestedRegion();

  if ( requestedRegion.GetSize(3) != 5 ) {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  else {
  std::cout<<"[PASSED]"<<std::endl;
  } 

  std::cout << "Testing mitk::Surface::Testing 4D surface data creation: ";

//  surface = mitk::Surface::New();
  //mitk::Geometry3D::Pointer geometry = surface->GetTimeSlicedGeometry()->GetGeometry3D(0);
  //geometry->GetVtkTransform()->Identity();
  //geometry->GetVtkTransform()->Translate(10,10,10);
  //geometry->TransferVtkToItkTransform();
  //mitk::TimeSlicedGeometry* timeSlicedGeometry = surface->GetTimeSlicedGeometry();
  //timeSlicedGeometry->InitializeEvenlyTimed(geometry, 5);

  vtkFloatingPointType bounds[5][6];

  for (int i=0;i<5;i++) {
    vtkSphereSource* sphereSource = vtkSphereSource::New();
    sphereSource->SetCenter(0,0,0);
    sphereSource->SetRadius(1.0 * (i+1.0));
    sphereSource->SetThetaResolution(10);
    sphereSource->SetPhiResolution(10);
    sphereSource->Update();
    sphereSource->GetOutput()->ComputeBounds();
    sphereSource->GetOutput()->GetBounds( bounds[i] );
    surface->SetVtkPolyData( sphereSource->GetOutput(),i );
  }

  surface->UpdateOutputInformation();
  surface->SetRequestedRegionToLargestPossibleRegion();

  bool passed = true;
  for (int i=0;i<5;i++) 
  {
    mitk::BoundingBox::BoundsArrayType surfBounds = (const_cast<mitk::BoundingBox*>(surface->GetTimeSlicedGeometry()->GetGeometry3D(i)->GetBoundingBox()))->GetBounds();

    if ( bounds[i][0] != surfBounds[0] 
    || bounds[i][1] != surfBounds[1] 
    || bounds[i][2] != surfBounds[2] 
    || bounds[i][3] != surfBounds[3] 
    || bounds[i][4] != surfBounds[4] 
    || bounds[i][5] != surfBounds[5] )
    {
      passed = false;
      break;
    }
  }

  if (!passed)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  else 
  {
    std::cout<<"[PASSED]"<<std::endl;
  }
  
  std::cout << "Testing correctness of geometry for surface->GetUpdatedTimeSlicedGeometry(): ";
  const mitk::TimeSlicedGeometry* inputTimeGeometry = surface->GetUpdatedTimeSlicedGeometry();

  int time = 3;
  int timestep=0;
  timestep = inputTimeGeometry->MSToTimeStep( time );

  std::cout << "time: "<< time << std::endl;
  std::cout << "timestep: "<<timestep << std::endl;

  if (time != timestep)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  else 
  {
    std::cout<<"[PASSED]"<<std::endl;
  }

  std::cout << "Explicitly changing the data of timestep 3 and checking for timebounds ccorrectness of surface's geometry again.";
  
  sphereSource = vtkSphereSource::New();
  sphereSource->SetCenter(0,0,0);
  sphereSource->SetRadius( 100.0 );
  sphereSource->SetThetaResolution(10);
  sphereSource->SetPhiResolution(10);
  sphereSource->Update();
  surface->SetVtkPolyData( sphereSource->GetOutput(), 3 );
  
  inputTimeGeometry = surface->GetUpdatedTimeSlicedGeometry();
  time = 3;
  timestep=0;
  timestep = inputTimeGeometry->MSToTimeStep( time );

  std::cout << "time: "<< time << std::endl;
  std::cout << "timestep: "<<timestep << std::endl;

  if (time != timestep)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  else 
  {
    std::cout<<"[PASSED]"<<std::endl;
  }

  //std::cout << "Testing mitk::Surface::*TESTED_METHOD_DESCRIPTION: ";
  //// METHOD_TEST_CODE
  //if (surface.IsNull()) {
  //  std::cout<<"[FAILED]"<<std::endl;
  //  return EXIT_FAILURE;
  //}
  //else {
  //std::cout<<"[PASSED]"<<std::endl;
  //} 

  std::cout<<"[TEST DONE]"<<std::endl;
  return EXIT_SUCCESS;
}
