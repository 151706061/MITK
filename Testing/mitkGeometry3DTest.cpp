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

#include "mitkGeometry3D.h"

#include <vnl/vnl_quaternion.h>
#include <vnl/vnl_quaternion.txx>

#include <fstream>

bool testGetAxisVectorVariants(mitk::Geometry3D* geometry)
{
  int direction;
  for(direction=0; direction<3; ++direction)
  {
    mitk::Vector3D frontToBack;
    switch(direction)
    {
      case 0: frontToBack = geometry->GetCornerPoint(false, false, false)-geometry->GetCornerPoint(true , false, false); break; //7-3
      case 1: frontToBack = geometry->GetCornerPoint(false, false, false)-geometry->GetCornerPoint(false, true , false); break; //7-5
      case 2: frontToBack = geometry->GetCornerPoint(false, false, false)-geometry->GetCornerPoint(false , false, true);  break; //7-2
    }
    std::cout << "Testing GetAxisVector(int) vs GetAxisVector(bool, bool, bool): ";
    if(mitk::Equal(geometry->GetAxisVector(direction), frontToBack) == false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return false;
    }
    std::cout<<"[PASSED]"<<std::endl;
  }
  return true;
}

bool testGetAxisVectorExtent(mitk::Geometry3D* geometry)
{
  int direction;
  for(direction=0; direction<3; ++direction)
  {
    if(mitk::Equal(geometry->GetAxisVector(direction).GetNorm(), geometry->GetExtentInMM(direction)) == false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return false;
    }
    std::cout<<"[PASSED]"<<std::endl;
  }
  return true;
}

int mitkGeometry3DTest(int /*argc*/, char* /*argv*/[])
{
  float bounds[ ] = {-10.0, 17.0, -12.0, 188.0, 13.0, 211.0};

  mitk::Geometry3D::Pointer geometry3d = mitk::Geometry3D::New();

  std::cout << "Initializing: ";
  geometry3d->Initialize();
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Setting bounds by SetFloatBounds(): ";
  geometry3d->SetFloatBounds(bounds);
  std::cout<<"[PASSED]"<<std::endl;
 
  if(testGetAxisVectorVariants(geometry3d) == false)
    return EXIT_FAILURE;

  if(testGetAxisVectorExtent(geometry3d) == false)
    return EXIT_FAILURE;

  std::cout<<"[TEST DONE]"<<std::endl;
  return EXIT_SUCCESS;
}
