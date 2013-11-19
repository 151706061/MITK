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


#include "mitkTestingMacros.h"
#include "mitkPoint.h"
#include "mitkTypeOperations.h"
#include "mitkTypes.h" // for Equal method. TODO SW: Replace this once the Equal methods have their own header.

#include <iostream>

using namespace mitk;

static ScalarType bValues[3] = {14.123456, 10.2, 5.123456789};
static ScalarType cValues[3] = { 2.654321,  5.1, 2.543231111};

static Point3D a, b, c;


static void Setup(void)
{
  b = bValues;
  c = cValues;
}


template <typename TCoordRep, unsigned int NPointDimension>
static void TestForValuesHelper(itk::FixedArray<TCoordRep, NPointDimension>& result,
    ScalarType v0, ScalarType v1, ScalarType v2,
    std::string message, ScalarType eps = mitk::eps)
{
  MITK_TEST_CONDITION_REQUIRED(
         Equal(result[0], v0, eps)
      && Equal(result[1], v1, eps)
      && Equal(result[2], v2, eps), message)
}


static void Test_Addition(void)
{
  Setup();

  mitk::add(a, b, c);

  TestForValuesHelper(a, 16.777777, 15.3, 7.6666879, "summation a = b + c correctly performed");
}


static void Test_Substraction(void)
{
  Setup();
  itk::FixedArray<double, 3> test;

  a = mitk::sub(b, c);

  TestForValuesHelper(a, 11.469135, 5.1, 2.580225678, "difference a = b - c correctly performed");
}




int mitkTypeOperationTest(int /*argc*/ , char* /*argv*/[])
{
  // always start with this!
  MITK_TEST_BEGIN("TypeOperationTest")

  Test_Addition();
  Test_Substraction();

  MITK_TEST_END()
}
