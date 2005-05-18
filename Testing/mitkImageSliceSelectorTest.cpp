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
#include "mitkDataTreeNodeFactory.h"
#include "mitkCylindricToCartesianFilter.h"
#include "mitkImageSliceSelector.h"
#include <itksys/SystemTools.hxx>

#include <fstream>
int mitkImageSliceSelectorTest(int argc, char* argv[])
{
  int slice_nr = 1;
  std::cout << "Loading file: ";
  if(argc==0)
  {
    std::cout<<"no file specified [FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  mitk::Image::Pointer image = NULL;
  mitk::DataTreeNodeFactory::Pointer factory = mitk::DataTreeNodeFactory::New();
  try
  {
    factory->SetFileName( argv[1] );
    factory->Update();

    if(factory->GetNumberOfOutputs()<1)
    {
      std::cout<<"file could not be loaded [FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    mitk::DataTreeNode::Pointer node = factory->GetOutput( 0 );
    image = dynamic_cast<mitk::Image*>(node->GetData());
    if(image.IsNull())
    {
      std::cout<<"file not an image - test will not be applied [PASSED]"<<std::endl;
      std::cout<<"[TEST DONE]"<<std::endl;
      return EXIT_SUCCESS;
    }
  }
  catch ( itk::ExceptionObject & ex )
  {
    std::cout << "Exception: " << ex << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
  }

  if(image->GetDimension(2)<2)
    slice_nr = 0;

  //Take a slice
	mitk::ImageSliceSelector::Pointer slice = mitk::ImageSliceSelector::New();
	  slice->SetInput(image);
	  slice->SetSliceNr(slice_nr);
	  slice->Update();

  std::cout << "Testing IsInitialized(): ";
  if(slice->GetOutput()->IsInitialized()==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing IsSliceSet(): ";
  if(slice->GetOutput()->IsSliceSet(0)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  if(itksys::SystemTools::LowerCase(itksys::SystemTools::GetFilenameExtension(argv[1])).find(".pic")!=std::string::npos)
  {
    std::cout << "Testing whether the slice is identical with a slice loaded by ipPicGetSlice:";
    ipPicDescriptor *picslice = ipPicGetSlice(argv[1], NULL, (image->GetDimension(2)-1-slice_nr)+1);
    int i, size = _ipPicSize(picslice);
    char * p1 = (char*)slice->GetPic()->data;
    char * p2 = (char*)picslice->data;
    //picslice->info->write_protect=ipFalse;
    //ipPicPut("C:\\1aaaaIPPIC.pic", picslice);
    //ipPicPut("C:\\1aaaaSEL.pic", slice->GetPic());
    for(i=0; i<size; ++i, ++p1, ++p2)
    {
      if((*p1) != (*p2))
      {
        std::cout<<"[FAILED]"<<std::endl;
        return EXIT_FAILURE;
      }
    }
    std::cout<<"[PASSED]"<<std::endl;
    ipPicFree(picslice);
  }

  try
  {
    std::cout << "Testing another, smaller (!!) input with the same slice-selector(): ";
    //Use CylindricToCartesianFilter
	  mitk::CylindricToCartesianFilter::Pointer cyl2cart = mitk::CylindricToCartesianFilter::New();
      cyl2cart->SetInput(image);
      //the output size of this filter is smaller than the of the input!!
      cyl2cart->SetTargetXSize( 64 );

    //Use the same slice-selector again, this time to take a slice of the filtered image
      //which is smaller than the one of the old input!!
	  slice->SetInput(cyl2cart->GetOutput());
      slice->SetSliceNr(1);
	    slice->Update();

    //Check that the requested region is now the one of the smaller image
    if(cyl2cart->GetOutput()->GetLargestPossibleRegion().GetSize()[0]!=64)
    {
      std::cout<<"Part 1 [FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"Part 1 [PASSED] ";

    //Check that the size of the output is now the one of the smaller image
    if((cyl2cart->GetOutput()->GetDimensions()[0]!=64) || (cyl2cart->GetOutput()->GetDimensions()[1]!=64))
    {
      std::cout<<"Part 2 [FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"Part 2 [PASSED] ";
  }
  catch ( itk::ExceptionObject &err)
  {
    std::cout<<"Part 1 [PASSED] - exception ... seems to be not ITK 2.0.0 ..."<<std::endl;
    std::cout<<err<<std::endl;
    return EXIT_FAILURE;
  }

  try
  {
	  slice->UpdateLargestPossibleRegion();
  }
  catch ( itk::ExceptionObject )
  {
    std::cout<<"Part 3 [FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"Part 3 [PASSED]"<<std::endl;

  std::cout << "Testing IsInitialized(): ";
  if(slice->GetOutput()->IsInitialized()==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing IsSliceSet(): ";
  if(slice->GetOutput()->IsSliceSet(0)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  if(image->GetDimension(3) > 1)
  {
    int time=image->GetDimension(3)-1;

    std::cout << "Testing 3D+t: Setting time to " << time << ": ";
	  slice->SetTimeNr(time);
    if(slice->GetTimeNr()!=time)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;

    std::cout << "Testing 3D+t: Updating slice: ";
    slice->Update();
    if(slice->GetOutput()->IsInitialized()==false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;

    std::cout << "Testing 3D+t: IsSliceSet(): ";
    if(slice->GetOutput()->IsSliceSet(0)==false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;

    std::cout << "Testing 3D+t: First slice in reader available: ";
    if(image->IsSliceSet(0, time)==false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;
  }

  std::cout<<"[TEST DONE]"<<std::endl;
  return EXIT_SUCCESS;
}
