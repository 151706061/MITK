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
#include "mitkImageAccessByItk.h"
#include "mitkITKImageImport.h"

#include <fstream>

int compareGeometries(mitk::Geometry3D* geometry1, mitk::Geometry3D* geometry2)
{
  std::cout << "Testing transfer of GetGeometry()->GetOrigin(): " << std::flush;
  if(mitk::Equal(geometry1->GetOrigin(), geometry2->GetOrigin()) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing transfer of GetGeometry()->GetSpacing(): " << std::flush;
  if(mitk::Equal(geometry1->GetSpacing(), geometry2->GetSpacing()) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  int i;
  for(i=0; i<3; ++i)
  {
    std::cout << "Testing transfer of GetGeometry()->GetAxisVector(" << i << "): " << std::flush;
    if(mitk::Equal(geometry1->GetAxisVector(i), geometry2->GetAxisVector(i)) == false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;
  }
  return EXIT_SUCCESS;
}

template <class ImageType>
int testBackCasting(mitk::Image* imgMem, typename ImageType::Pointer & itkImage)
{
  int result;

  if(itkImage.IsNull())
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  int *p = (int*)itkImage->GetBufferPointer();
  if(p==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing mitk::CastToMitkImage: " << std::flush;
  mitk::Image::Pointer mitkImage = mitk::Image::New();
  mitk::CastToMitkImage( itkImage, mitkImage );
  std::cout<<"[PASSED]"<<std::endl;

  result = compareGeometries(imgMem->GetGeometry(), mitkImage->GetGeometry());
  if(result != EXIT_SUCCESS)
    return result;

  std::cout << "Testing whether data after mitk::CastToMitkImage is available: " << std::flush;
  if(mitkImage->IsChannelSet()==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing mitk::ImportItkImage: " << std::flush;
  mitkImage = mitk::ImportItkImage(itkImage);
  std::cout<<"[PASSED]"<<std::endl;

  result = compareGeometries(imgMem->GetGeometry(), mitkImage->GetGeometry());
  if(result != EXIT_SUCCESS)
    return result;

  std::cout << "Testing whether data after mitk::ImportItkImage is available: " << std::flush;
  if(mitkImage->IsChannelSet()==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  return EXIT_SUCCESS;
}


template <unsigned int dim>
int testImageToItkAndBack(mitk::Image* imgMem)
{
  int result;

  int *p = (int*)imgMem->GetData();
  if(p==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing for dimension " << dim << ": " << std::flush;
  std::cout << "Testing mitk::ImageToItk: " << std::flush;
  typedef itk::Image<int,dim> ImageType;

  typename mitk::ImageToItk<ImageType>::Pointer toItkFilter = mitk::ImageToItk<ImageType>::New();
  toItkFilter->SetInput(imgMem);
  toItkFilter->Update();
  typename ImageType::Pointer itkImage = toItkFilter->GetOutput();

  result = testBackCasting<ImageType>(imgMem, itkImage);
  if(result != EXIT_SUCCESS)
    return result;

  return EXIT_SUCCESS;
}

int mitkImageToItkTest(int /*argc*/, char* /*argv*/[])
{
  int result;

  //Create Image out of nowhere
	mitk::Image::Pointer imgMem;
	mitk::PixelType pt(typeid(int));

  std::cout << "Testing creation of Image: ";
	imgMem=mitk::Image::New();
  if(imgMem.IsNull())
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  // geometry information for image
  mitk::Point3D origin;
  mitk::Vector3D right, bottom;
  mitk::Vector3D spacing;
  mitk::FillVector3D(origin, 17.0, 19.92, 7.83);
  mitk::FillVector3D(right, 1.0, 2.0, 3.0);
  mitk::FillVector3D(bottom, 0.0, -3.0, 2.0);
  mitk::FillVector3D(spacing, 0.78, 0.91, 2.23);

  std::cout << "Testing InitializeStandardPlane(rightVector, downVector, spacing): " << std::flush;
  mitk::PlaneGeometry::Pointer planegeometry = mitk::PlaneGeometry::New();
  planegeometry->InitializeStandardPlane(100, 100, right, bottom, &spacing);
  planegeometry->SetOrigin(origin);
  std::cout << "done" << std::endl;

  std::cout << "Testing Initialize(const mitk::PixelType& type, int sDim, const mitk::PlaneGeometry& geometry) and GetData(): ";
  imgMem->Initialize(mitk::PixelType(typeid(int)), 40, *planegeometry); //XXXXXXXXXXXXXXXXXXXXXCHANGE!

  result = testImageToItkAndBack<3>(imgMem);
  if(result != EXIT_SUCCESS)
    return result;

  std::cout << "Testing mitk::CastToItkImage: " << std::flush;
  typedef itk::Image<float,3> ImageType;
  ImageType::Pointer itkImage;
  mitk::CastToItkImage( imgMem, itkImage );

  result = testBackCasting<ImageType>(imgMem, itkImage);
  if(result != EXIT_SUCCESS)
    return result;

  std::cout << "Testing Initialize(const mitk::PixelType& type, int sDim, const mitk::PlaneGeometry& geometry) and GetData(): ";
  imgMem->Initialize(mitk::PixelType(typeid(int)), 40, *planegeometry, false, 1, 6);
  result = testImageToItkAndBack<4>(imgMem);
  if(result != EXIT_SUCCESS)
    return result;

  std::cout<<"[TEST DONE]"<<std::endl;
  return EXIT_SUCCESS;
}
