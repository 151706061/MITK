/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

 //Testing
#include "mitkModelTestFixture.h"

//MITK includes
#include "mitkTwoStepLinearModel.h"


  class mitkTwoStepLinearModelTestSuite : public mitk::mitkModelTestFixture
{
  CPPUNIT_TEST_SUITE(mitkTwoStepLinearModelTestSuite);
  MITK_TEST(GetModelInfoTest);
  MITK_TEST(ComputeModelfunctionTest);
  MITK_TEST(ComputeDerivedParametersTest);
  CPPUNIT_TEST_SUITE_END();

private:
  mitk::TwoStepLinearModel::Pointer m_testmodel;

  json m_profile_json_obj;
  json m_modelValues_json_obj;

public:
  void setUp() override
  {
    // Parse JSON files
    m_profile_json_obj = ParseJSONFile("Pharmacokinetics/mitkTwoStepLinearModelTest_profile.json");
    m_modelValues_json_obj = ParseJSONFile("Pharmacokinetics/mitkTwoStepLinearModelTest_modelValues.json");

    // Generate test model
    m_testmodel = mitk::TwoStepLinearModel::New();
 }

  void tearDown() override
  {
  }

  void GetModelInfoTest()
  {
    // comparison of reference and testmodel profile
    CompareModelAndReferenceProfile(m_testmodel, m_profile_json_obj);
  }

  void ComputeModelfunctionTest()
  {
      CompareModelAndReferenceSignal(m_testmodel, m_modelValues_json_obj, m_profile_json_obj);
  }

  void ComputeDerivedParametersTest()
  {
      CompareModelAndReferenceDerivedParameters(m_testmodel, m_modelValues_json_obj);
  }
};

MITK_TEST_SUITE_REGISTRATION(mitkTwoStepLinearModel)
