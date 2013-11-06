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

#include "mitkUSCombinedModality.h"
#include <mitkVirtualTrackingDevice.h>
#include <mitkUSVideoDevice.h>
#include <mitkTestingMacros.h>
#include <mitkTrackingDeviceSource.h>

class mitkCombinedModalityTestClass
{
public:

  /*
  * \brief Returns a reference string for serialized calibrations.
  */

  static std::string GetSerializedReference()
  {
    return "<calibrations>\n<default0 M00=\"1.1234\" M01=\"1.2234\" M02=\"1.3234\" M10=\"1.4234\" M11=\"1.5234\" M12=\"1.6234\" M20=\"1.7234\" M21=\"1.8234\" M22=\"1.9234\" O0=\"2.1234\" O1=\"2.2234\" O2=\"2.3234\" />\n<calibrations>";
  }

  /*
  * \brief Creates a dummy combined modality.
  */
  static mitk::USCombinedModality::Pointer CreateModality()
  {
    mitk::USVideoDevice::Pointer  usDevice =  mitk::USVideoDevice::New("IllegalPath", "Manufacturer", "Model");
    mitk::VirtualTrackingDevice::Pointer tracker = mitk::VirtualTrackingDevice::New();
    tracker->AddTool("tool1");
    tracker->AddTool("tool2");
    mitk::TrackingDeviceSource::Pointer source = mitk::TrackingDeviceSource::New();
    source->SetTrackingDevice(tracker);
    source->Connect();
    source->StartTracking();
    return mitk::USCombinedModality::New(usDevice.GetPointer(), source.GetPointer(), "MBI", "EchoTrack");
  }

  static void TestInstantiation()
  {
    mitk::USCombinedModality::Pointer combinedModality = CreateModality();
    MITK_TEST_CONDITION_REQUIRED(combinedModality.IsNotNull(), "CombinedModality should not be null after instantiation");
  }

  static void TestSerialization()
  {
    mitk::USCombinedModality::Pointer modality = CreateModality();

    mitk::AffineTransform3D::MatrixType matrix;
    matrix[0][0] = 1.1234f;
    matrix[0][1] = 1.2234f;
    matrix[0][2] = 1.3234f;
    matrix[1][0] = 1.4234f;
    matrix[1][1] = 1.5234f;
    matrix[1][2] = 1.6234f;
    matrix[2][0] = 1.7234f;
    matrix[2][1] = 1.8234f;
    matrix[2][2] = 1.9234f;

    mitk::AffineTransform3D::OffsetType offset;
    offset[0] = 2.1234f;
    offset[1] = 2.2234f;
    offset[2] = 2.3234f;

    mitk::AffineTransform3D::Pointer transform = mitk::AffineTransform3D::New();
    transform->SetMatrix(matrix);
    transform->SetOffset(offset);

    modality->SetCalibration(transform);

    MITK_TEST_CONDITION_REQUIRED(modality->SerializeCalibration() == GetSerializedReference(), "Testing correct Serialization...");
  }

  static void TestDeserialization()
  {
    mitk::USCombinedModality::Pointer modality = CreateModality();
    modality->DeserializeCalibration(GetSerializedReference());
    mitk::AffineTransform3D::Pointer transform = modality->GetCalibration();

    mitk::AffineTransform3D::MatrixType matrix = transform->GetMatrix();
    mitk::AffineTransform3D::OffsetType offset = transform->GetOffset();;

    bool identical = true;

    if (! matrix[0][0] == 1.1234f) identical = false;
    if (! matrix[0][1] == 1.2234f) identical = false;
    if (! matrix[0][2] == 1.3234f) identical = false;
    if (! matrix[1][0] == 1.4234f) identical = false;
    if (! matrix[1][1] == 1.5234f) identical = false;
    if (! matrix[1][2] == 1.6234f) identical = false;
    if (! matrix[2][0] == 1.7234f) identical = false;
    if (! matrix[2][1] == 1.8234f) identical = false;
    if (! matrix[2][2] == 1.9234f) identical = false;

    if (! offset[0] == 1.1234f) identical = false;
    if (! offset[1] == 1.2234f) identical = false;
    if (! offset[2] == 1.3234f) identical = false;

    MITK_TEST_CONDITION_REQUIRED(identical, "Testing if deserialized calibration is identical to serialized one...");
  }
};

/**
* This function is testing methods of the class USDevice.
*/
int mitkCombinedModalityTest(int /* argc */, char* /*argv*/[])
{
  MITK_TEST_BEGIN("mitkCombinedModalityTest");

  mitkCombinedModalityTestClass::TestInstantiation();
  mitkCombinedModalityTestClass::TestSerialization();
  mitkCombinedModalityTestClass::TestDeserialization();

  MITK_TEST_END();
}