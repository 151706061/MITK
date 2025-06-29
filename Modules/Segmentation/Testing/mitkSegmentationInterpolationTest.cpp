/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

// Testing
#include <mitkTestFixture.h>
#include <mitkTestingMacros.h>

// other
#include <mitkExtractSliceFilter.h>
#include <mitkIOUtil.h>
#include <mitkImage.h>
#include <mitkImagePixelReadAccessor.h>
#include <mitkImagePixelWriteAccessor.h>
#include <mitkSegmentationInterpolationController.h>
#include <mitkSliceNavigationController.h>
#include <mitkTool.h>
#include <mitkVtkImageOverwrite.h>

class mitkSegmentationInterpolationTestSuite : public mitk::TestFixture
{
  CPPUNIT_TEST_SUITE(mitkSegmentationInterpolationTestSuite);
  MITK_TEST(Equal_Axial_TestInterpolationAndReferenceInterpolation_ReturnsTrue);
  MITK_TEST(Equal_Coronal_TestInterpolationAndReferenceInterpolation_ReturnsTrue);
  MITK_TEST(Equal_Sagittal_TestInterpolationAndReferenceInterpolation_ReturnsTrue);
  CPPUNIT_TEST_SUITE_END();

private:
  // The tests all do the same, only in different directions
  void testRoutine(mitk::AnatomicalPlane viewDirection)
  {
    int dim;
    switch (viewDirection)
    {
      case (mitk::AnatomicalPlane::Axial):
        dim = 2;
        break;
      case (mitk::AnatomicalPlane::Coronal):
        dim = 1;
        break;
      case (mitk::AnatomicalPlane::Sagittal):
        dim = 0;
        break;
      default: // mitk::AnatomicalPlane::Original
        dim = -1;
        break;
    }

    /* Fill segmentation
     *
     * 1st slice: 3x3 square segmentation
     * 2nd slice: empty
     * 3rd slice: 1x1 square segmentation in corner
     * -> 2nd slice should become 2x2 square in corner
     *
     * put accessor in scope
     */

    itk::Index<3> currentPoint;
    {
      mitk::ImagePixelWriteAccessor<mitk::Tool::DefaultSegmentationDataType, 3> writeAccessor(m_SegmentationImage);

      // Fill 3x3 slice
      currentPoint[dim] = m_CenterPoint[dim] - 1;
      for (int i = -1; i <= 1; ++i)
      {
        for (int j = -1; j <= 1; ++j)
        {
          currentPoint[(dim + 1) % 3] = m_CenterPoint[(dim + 1) % 3] + i;
          currentPoint[(dim + 2) % 3] = m_CenterPoint[(dim + 2) % 3] + j;
          writeAccessor.SetPixelByIndexSafe(currentPoint, 1);
        }
      }
      // Now i=j=1, set point two slices up
      currentPoint[dim] = m_CenterPoint[dim] + 1;
      writeAccessor.SetPixelByIndexSafe(currentPoint, 1);
    }

    //        mitk::IOUtil::Save(m_SegmentationImage, "SOME PATH");

    m_InterpolationController->SetSegmentationVolume(m_SegmentationImage);

    // This could be easier...
    mitk::SliceNavigationController::Pointer navigationController = mitk::SliceNavigationController::New();
    navigationController->SetInputWorldTimeGeometry(m_SegmentationImage->GetTimeGeometry());
    navigationController->Update(viewDirection);
    mitk::Point3D pointMM;
    m_SegmentationImage->GetTimeGeometry()->GetGeometryForTimeStep(0)->IndexToWorld(m_CenterPoint, pointMM);
    navigationController->SelectSliceByPoint(pointMM);
    auto plane = navigationController->GetCurrentPlaneGeometry();
    mitk::Image::Pointer interpolationResult =
      m_InterpolationController->Interpolate(dim, m_CenterPoint[dim], plane, 0);

    //        mitk::IOUtil::Save(interpolationResult, "SOME PATH");

    // Write result into segmentation image
    vtkSmartPointer<mitkVtkImageOverwrite> reslicer = vtkSmartPointer<mitkVtkImageOverwrite>::New();
    reslicer->SetInputSlice(
      interpolationResult->GetSliceData()->GetVtkImageAccessor(interpolationResult)->GetVtkImageData());
    reslicer->SetOverwriteMode(true);
    reslicer->Modified();
    mitk::ExtractSliceFilter::Pointer extractor = mitk::ExtractSliceFilter::New(reslicer);
    extractor->SetInput(m_SegmentationImage);
    extractor->SetTimeStep(0);
    extractor->SetWorldGeometry(plane);
    extractor->SetVtkOutputRequest(true);
    extractor->SetResliceTransformByGeometry(m_SegmentationImage->GetTimeGeometry()->GetGeometryForTimeStep(0));
    extractor->Modified();
    extractor->Update();

    //        mitk::IOUtil::Save(m_SegmentationImage, "SOME PATH");

    // Check a 4x4 square, the center of which needs to be filled
    mitk::ImagePixelReadAccessor<mitk::Tool::DefaultSegmentationDataType, 3> readAccess(m_SegmentationImage);
    currentPoint = m_CenterPoint;

    for (int i = -1; i <= 2; ++i)
    {
      for (int j = -1; j <= 2; ++j)
      {
        currentPoint[(dim + 1) % 3] = m_CenterPoint[(dim + 1) % 3] + i;
        currentPoint[(dim + 2) % 3] = m_CenterPoint[(dim + 2) % 3] + j;

        if (i == -1 || i == 2 || j == -1 || j == 2)
        {
          CPPUNIT_ASSERT_MESSAGE("Have false positive segmentation.",
                                 readAccess.GetPixelByIndexSafe(currentPoint) == 0);
        }
        else
        {
          CPPUNIT_ASSERT_MESSAGE("Have false negative segmentation.",
                                 readAccess.GetPixelByIndexSafe(currentPoint) == 1);
        }
      }
    }
  }

  mitk::Image::Pointer m_ReferenceImage;
  mitk::Image::Pointer m_SegmentationImage;
  itk::Index<3> m_CenterPoint;
  mitk::SegmentationInterpolationController::Pointer m_InterpolationController;

public:
  void setUp() override
  {
    m_ReferenceImage = mitk::IOUtil::Load<mitk::Image>(GetTestDataFilePath("Pic3D.nrrd"));
    CPPUNIT_ASSERT_MESSAGE("Failed to load image for test: [Pic3D.nrrd]", m_ReferenceImage.IsNotNull());

    m_InterpolationController = mitk::SegmentationInterpolationController::GetInstance();

    // Create empty segmentation
    // Surely there must be a better way to get an image with all zeros?
    m_SegmentationImage = mitk::Image::New();
    const mitk::PixelType pixelType(mitk::MakeScalarPixelType<mitk::Tool::DefaultSegmentationDataType>());
    m_SegmentationImage->Initialize(pixelType, m_ReferenceImage->GetDimension(), m_ReferenceImage->GetDimensions());
    m_SegmentationImage->SetClonedTimeGeometry(m_ReferenceImage->GetTimeGeometry());
    unsigned int size = sizeof(mitk::Tool::DefaultSegmentationDataType);
    for (unsigned int dim = 0; dim < m_SegmentationImage->GetDimension(); ++dim)
    {
      size *= m_SegmentationImage->GetDimension(dim);
    }
    mitk::ImageWriteAccessor imageAccessor(m_SegmentationImage);
    memset(imageAccessor.GetData(), 0, size);

    // Work in the center of the image (Pic3D)
    m_CenterPoint = {{127, 127, 25}};
  }

  void tearDown() override
  {
    m_ReferenceImage = nullptr;
    m_SegmentationImage = nullptr;
    m_CenterPoint = {{0, 0, 0}};
  }

  void Equal_Axial_TestInterpolationAndReferenceInterpolation_ReturnsTrue()
  {
    mitk::AnatomicalPlane viewDirection = mitk::AnatomicalPlane::Axial;
    testRoutine(viewDirection);
  }

  void Equal_Coronal_TestInterpolationAndReferenceInterpolation_ReturnsTrue() // Coronal
  {
    mitk::AnatomicalPlane viewDirection = mitk::AnatomicalPlane::Coronal;
    testRoutine(viewDirection);
  }

  void Equal_Sagittal_TestInterpolationAndReferenceInterpolation_ReturnsTrue()
  {
    mitk::AnatomicalPlane viewDirection = mitk::AnatomicalPlane::Sagittal;
    testRoutine(viewDirection);
  }
};

MITK_TEST_SUITE_REGISTRATION(mitkSegmentationInterpolation)
