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


#include "mitkImage.h"
#include "mitkPlaneGeometry.h"
#include "mitkTimeSlicedGeometry.h"
#include "mitkSlicedGeometry3D.h"

#include <vnl/vnl_quaternion.h>
#include <vnl/vnl_quaternion.txx>

#include <fstream>

int mappingTests2D(const mitk::PlaneGeometry* planegeometry, const mitk::ScalarType& width, const mitk::ScalarType& height, const mitk::ScalarType& widthInMM, const mitk::ScalarType& heightInMM, const mitk::Point3D& origin, const mitk::Vector3D& right, const mitk::Vector3D& bottom)
{

  std::cout << "Testing mapping Map(pt2d_mm(x=widthInMM/2.3,y=heightInMM/2.5), pt3d_mm) and compare with expected: ";
  mitk::Point2D pt2d_mm;
  mitk::Point3D pt3d_mm, expected_pt3d_mm;
  pt2d_mm[0] = widthInMM/2.3; pt2d_mm[1] = heightInMM/2.5;
  expected_pt3d_mm = origin+right*(pt2d_mm[0]/right.GetNorm())+bottom*(pt2d_mm[1]/bottom.GetNorm());
  planegeometry->Map(pt2d_mm, pt3d_mm);
  if(mitk::Equal(pt3d_mm, expected_pt3d_mm) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing mapping Map(pt3d_mm, pt2d_mm) and compare with expected: ";
  mitk::Point2D testpt2d_mm;
  planegeometry->Map(pt3d_mm, testpt2d_mm);
  if(mitk::Equal(pt2d_mm, testpt2d_mm) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing UnitsToMM(pt2d_units, pt2d_mm) and compare with expected: ";
  mitk::Point2D pt2d_units;
  pt2d_units[0] = width/2.0;     pt2d_units[1] = height/2.0;
  pt2d_mm[0]    = widthInMM/2.0; pt2d_mm[1]    = heightInMM/2.0;
  planegeometry->UnitsToMM(pt2d_units, testpt2d_mm);
  if(mitk::Equal(pt2d_mm, testpt2d_mm) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing MMToUnits(pt2d_mm, pt2d_units) and compare with expected: ";
  mitk::Point2D testpt2d_units;
  planegeometry->MMToUnits(pt2d_mm, testpt2d_units);
  if(mitk::Equal(pt2d_units, testpt2d_units) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  return EXIT_SUCCESS;
}

int mitkPlaneGeometryTest(int argc, char* argv[])
{
  int result;

  mitk::PlaneGeometry::Pointer planegeometry = mitk::PlaneGeometry::New();
 
  mitk::Point3D origin;
  mitk::Vector3D right, bottom, normal;
  mitk::ScalarType width, height;
  mitk::ScalarType widthInMM, heightInMM, thicknessInMM;

  width  = 100;    widthInMM  = width;
  height = 200;    heightInMM = height;
  thicknessInMM = 1.0;
  mitk::FillVector3D(origin, 4.5,              7.3, 11.2);
  mitk::FillVector3D(right,  widthInMM,          0, 0);
  mitk::FillVector3D(bottom,         0, heightInMM, 0);
  mitk::FillVector3D(normal,         0,          0, thicknessInMM);

  std::cout << "Testing InitializeStandardPlane(rightVector, downVector, spacing = NULL): "<<std::endl;
  planegeometry->InitializeStandardPlane(right.Get_vnl_vector(), bottom.Get_vnl_vector());

  std::cout << "Testing width, height and thickness (in units): ";
  if((mitk::Equal(planegeometry->GetExtent(0),width)==false) || 
     (mitk::Equal(planegeometry->GetExtent(1),height)==false) || 
     (mitk::Equal(planegeometry->GetExtent(2),1)==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm): ";
  if((mitk::Equal(planegeometry->GetExtentInMM(0),widthInMM)==false) || 
     (mitk::Equal(planegeometry->GetExtentInMM(1),heightInMM)==false) || 
     (mitk::Equal(planegeometry->GetExtentInMM(2),thicknessInMM)==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector(): ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), right)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), bottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), normal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;



  std::cout << "Testing InitializeStandardPlane(rightVector, downVector, spacing = {1.0, 1.0, 1.5}): "<<std::endl;
  mitk::Vector3D spacing;
  thicknessInMM = 1.5;
  normal.Normalize(); normal *= thicknessInMM;
  mitk::FillVector3D(spacing, 1.0, 1.0, thicknessInMM);
  planegeometry->InitializeStandardPlane(right.Get_vnl_vector(), bottom.Get_vnl_vector(), &spacing);

  std::cout << "Testing width, height and thickness (in units): ";
  if((mitk::Equal(planegeometry->GetExtent(0),width)==false) || 
     (mitk::Equal(planegeometry->GetExtent(1),height)==false) || 
     (mitk::Equal(planegeometry->GetExtent(2),1)==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm): ";
  if((mitk::Equal(planegeometry->GetExtentInMM(0),widthInMM)==false) || 
     (mitk::Equal(planegeometry->GetExtentInMM(1),heightInMM)==false) || 
     (mitk::Equal(planegeometry->GetExtentInMM(2),thicknessInMM)==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector(): ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), right)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), bottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), normal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;



  std::cout << "Testing SetExtentInMM(2, ...), querying by GetExtentInMM(2): ";
  thicknessInMM = 3.5;
  normal.Normalize(); normal *= thicknessInMM;
  planegeometry->SetExtentInMM(2, thicknessInMM);
  if(mitk::Equal(planegeometry->GetExtentInMM(2),thicknessInMM)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing SetExtentInMM(2, ...), querying by GetAxisVector(2) and comparing to normal: ";
  if(mitk::Equal(planegeometry->GetAxisVector(2), normal)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing SetOrigin: ";
  planegeometry->SetOrigin(origin);
  if(mitk::Equal(planegeometry->GetOrigin(), origin)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector() after SetOrigin: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), right)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), bottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), normal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  result = mappingTests2D(planegeometry, width, height, widthInMM, heightInMM, origin, right, bottom);
  if(result!=EXIT_SUCCESS)
    return result;



  std::cout << "Changing the UnitsToMMTransform to a rotated version by SetUnitsToMMTransform() (keep origin): "<<std::endl;
  mitk::AffineTransform3D::Pointer transform = mitk::AffineTransform3D::New();
  mitk::AffineTransform3D::MatrixType::InternalMatrixType vnlmatrix;
  vnlmatrix = planegeometry->GetIndexToWorldTransform()->GetMatrix().GetVnlMatrix();
  mitk::VnlVector axis(3);
  mitk::FillVector3D(axis, 1.0, 1.0, 1.0); axis.normalize();
  vnl_quaternion<mitk::ScalarType> rotation(axis, 0.123);
  vnlmatrix = rotation.rotation_matrix_transpose()*vnlmatrix;
  mitk::Matrix3D matrix;
  matrix = vnlmatrix;
  transform->SetMatrix(matrix);
  transform->SetOffset(planegeometry->GetIndexToWorldTransform()->GetOffset());
  
  right.Set_vnl_vector( rotation.rotation_matrix_transpose()*right.Get_vnl_vector() );
  bottom.Set_vnl_vector(rotation.rotation_matrix_transpose()*bottom.Get_vnl_vector());
  normal.Set_vnl_vector(rotation.rotation_matrix_transpose()*normal.Get_vnl_vector());
  planegeometry->SetIndexToWorldTransform(transform);

  std::cout << "Testing whether SetIndexToWorldTransform kept origin: ";
  if(mitk::Equal(planegeometry->GetOrigin(), origin)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm) of rotated version: ";
  if((mitk::Equal(planegeometry->GetExtentInMM(0),widthInMM)==false) || 
     (mitk::Equal(planegeometry->GetExtentInMM(1),heightInMM)==false) || 
     (mitk::Equal(planegeometry->GetExtentInMM(2),thicknessInMM)==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector() of rotated version: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), right)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), bottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), normal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector(direction).GetNorm() != planegeometry->GetExtentInMM(direction) of rotated version: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0).GetNorm(),planegeometry->GetExtentInMM(0))==false) || 
     (mitk::Equal(planegeometry->GetAxisVector(1).GetNorm(),planegeometry->GetExtentInMM(1))==false) || 
     (mitk::Equal(planegeometry->GetAxisVector(2).GetNorm(),planegeometry->GetExtentInMM(2))==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing SetSizeInUnits() of rotated version: "<<std::endl;
  width  *= 2;
  height *= 3;
  planegeometry->SetSizeInUnits(width, height);

  std::cout << "Testing width, height and thickness (in units): ";
  if((mitk::Equal(planegeometry->GetExtent(0),width)==false) || 
     (mitk::Equal(planegeometry->GetExtent(1),height)==false) || 
     (mitk::Equal(planegeometry->GetExtent(2),1)==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm) of version with changed size in units: ";
  if(!mitk::Equal(planegeometry->GetExtentInMM(0), widthInMM) || !mitk::Equal(planegeometry->GetExtentInMM(1), heightInMM) || !mitk::Equal(planegeometry->GetExtentInMM(2), thicknessInMM))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector() of version with changed size in units: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), right)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), bottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), normal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector(direction).GetNorm() != planegeometry->GetExtentInMM(direction) of rotated version: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0).GetNorm(),planegeometry->GetExtentInMM(0))==false) || 
     (mitk::Equal(planegeometry->GetAxisVector(1).GetNorm(),planegeometry->GetExtentInMM(1))==false) || 
     (mitk::Equal(planegeometry->GetAxisVector(2).GetNorm(),planegeometry->GetExtentInMM(2))==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  result = mappingTests2D(planegeometry, width, height, widthInMM, heightInMM, origin, right, bottom);
  if(result!=EXIT_SUCCESS)
    return result;



  std::cout << "Testing Clone(): ";
  mitk::PlaneGeometry::Pointer clonedplanegeometry = dynamic_cast<mitk::PlaneGeometry*>(planegeometry->Clone().GetPointer());
  if((clonedplanegeometry.IsNull()) || (clonedplanegeometry->GetReferenceCount()!=1))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing origin of cloned version: ";
  if(mitk::Equal(clonedplanegeometry->GetOrigin(), origin)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in units) of cloned version: ";
  if((mitk::Equal(clonedplanegeometry->GetExtent(0),width)==false) || 
     (mitk::Equal(clonedplanegeometry->GetExtent(1),height)==false) || 
     (mitk::Equal(clonedplanegeometry->GetExtent(2),1)==false)
    )
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm) of cloned version: ";
  if(!mitk::Equal(clonedplanegeometry->GetExtentInMM(0), widthInMM) || !mitk::Equal(clonedplanegeometry->GetExtentInMM(1), heightInMM) || !mitk::Equal(clonedplanegeometry->GetExtentInMM(2), thicknessInMM))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector() of cloned version: ";
  if((mitk::Equal(clonedplanegeometry->GetAxisVector(0), right)==false) || (mitk::Equal(clonedplanegeometry->GetAxisVector(1), bottom)==false) || (mitk::Equal(clonedplanegeometry->GetAxisVector(2), normal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  result = mappingTests2D(clonedplanegeometry, width, height, widthInMM, heightInMM, origin, right, bottom);
  if(result!=EXIT_SUCCESS)
    return result;



  std::cout << "Testing InitializeStandardPlane(clonedplanegeometry, planeorientation = Transversal, zPosition = 0, frontside=true): " <<std::endl;
  planegeometry->InitializeStandardPlane(clonedplanegeometry);

  std::cout << "Testing origin of transversally initialized version: ";
  if(mitk::Equal(planegeometry->GetOrigin(), origin)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in units) of transversally initialized version (should be same as in mm due to unit spacing, except for thickness, which is always 1): ";
  if(!mitk::Equal(planegeometry->GetExtent(0), widthInMM) || !mitk::Equal(planegeometry->GetExtent(1), heightInMM) || !mitk::Equal(planegeometry->GetExtent(2), 1))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm) of transversally initialized version: ";
  if(!mitk::Equal(planegeometry->GetExtentInMM(0), widthInMM) || !mitk::Equal(planegeometry->GetExtentInMM(1), heightInMM) || !mitk::Equal(planegeometry->GetExtentInMM(2), thicknessInMM))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector() of transversally initialized version: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), right)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), bottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), normal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  result = mappingTests2D(planegeometry, widthInMM, heightInMM, widthInMM, heightInMM, origin, right, bottom);
  if(result!=EXIT_SUCCESS)
    return result;



  mitk::Vector3D newright, newbottom, newnormal;
  mitk::ScalarType newthicknessInMM;
  std::cout << "Testing InitializeStandardPlane(clonedplanegeometry, planeorientation = Frontal, zPosition = 0, frontside=true): " <<std::endl;
  planegeometry->InitializeStandardPlane(clonedplanegeometry, mitk::PlaneGeometry::Frontal);
  newright = right; 
  newbottom = normal; newbottom.Normalize();  newbottom *= thicknessInMM;
  newthicknessInMM = heightInMM/height*1.0/*extent in normal direction is 1*/;
  newnormal = -bottom; newnormal.Normalize(); newnormal *= newthicknessInMM;

  std::cout << "Testing origin of frontally initialized version: ";
  if(mitk::Equal(planegeometry->GetOrigin(), origin)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in units) of frontally initialized version: ";
  if(!mitk::Equal(planegeometry->GetExtent(0), widthInMM) || !mitk::Equal(planegeometry->GetExtent(1), thicknessInMM) || !mitk::Equal(planegeometry->GetExtent(2), 1))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm) of frontally initialized version: ";
  if(!mitk::Equal(planegeometry->GetExtentInMM(0), widthInMM) || !mitk::Equal(planegeometry->GetExtentInMM(1), thicknessInMM) || !mitk::Equal(planegeometry->GetExtentInMM(2), newthicknessInMM))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector() of frontally initialized version: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), newright)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), newbottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), newnormal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  result = mappingTests2D(planegeometry, widthInMM, thicknessInMM, widthInMM, thicknessInMM, origin, newright, newbottom);
  if(result!=EXIT_SUCCESS)
    return result;



  std::cout << "Testing InitializeStandardPlane(clonedplanegeometry, planeorientation = Sagittal, zPosition = 0, frontside=true): " <<std::endl;
  planegeometry->InitializeStandardPlane(clonedplanegeometry, mitk::PlaneGeometry::Sagittal);
  newright = bottom;
  newthicknessInMM = widthInMM/width*1.0/*extent in normal direction is 1*/;
  newnormal = right; newnormal.Normalize(); newnormal *= newthicknessInMM;

  std::cout << "Testing width, height and thickness (in units) of frontally initialized version: ";
  if(!mitk::Equal(planegeometry->GetExtent(0), heightInMM) || !mitk::Equal(planegeometry->GetExtent(1), thicknessInMM) || !mitk::Equal(planegeometry->GetExtent(2), 1))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm) of frontally initialized version: ";
  if(!mitk::Equal(planegeometry->GetExtentInMM(0), heightInMM) || !mitk::Equal(planegeometry->GetExtentInMM(1), thicknessInMM) || !mitk::Equal(planegeometry->GetExtentInMM(2), newthicknessInMM))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector() of frontally initialized version: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), newright)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), newbottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), newnormal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  result = mappingTests2D(planegeometry, heightInMM, thicknessInMM, heightInMM, thicknessInMM, origin, newright, newbottom);
  if(result!=EXIT_SUCCESS)
    return result;



  std::cout << "Testing backside initialization: InitializeStandardPlane(clonedplanegeometry, planeorientation = Transversal, zPosition = 0, frontside=false): " <<std::endl;
  planegeometry->InitializeStandardPlane(clonedplanegeometry, mitk::PlaneGeometry::Transversal, 0, false);
  mitk::Point3D backsideorigin;
  backsideorigin=origin+clonedplanegeometry->GetAxisVector(1);

  std::cout << "Testing origin of backsidedly, transversally initialized version: ";
  if(mitk::Equal(planegeometry->GetOrigin(), backsideorigin)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in units) of backsidedly, transversally initialized version (should be same as in mm due to unit spacing, except for thickness, which is always 1): ";
  if(!mitk::Equal(planegeometry->GetExtent(0), widthInMM) || !mitk::Equal(planegeometry->GetExtent(1), heightInMM) || !mitk::Equal(planegeometry->GetExtent(2), 1))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing width, height and thickness (in mm) of backsidedly, transversally initialized version: ";
  if(!mitk::Equal(planegeometry->GetExtentInMM(0), widthInMM) || !mitk::Equal(planegeometry->GetExtentInMM(1), heightInMM) || !mitk::Equal(planegeometry->GetExtentInMM(2), thicknessInMM))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetAxisVector() of backsidedly, transversally initialized version: ";
  if((mitk::Equal(planegeometry->GetAxisVector(0), right)==false) || (mitk::Equal(planegeometry->GetAxisVector(1), -bottom)==false) || (mitk::Equal(planegeometry->GetAxisVector(2), -normal)==false))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  result = mappingTests2D(planegeometry, widthInMM, heightInMM, widthInMM, heightInMM, backsideorigin, right, -bottom);
  if(result!=EXIT_SUCCESS)
    return result;



  std::cout<<"[TEST DONE]"<<std::endl;
  return EXIT_SUCCESS;
}
