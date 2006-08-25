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

#include "mitkGenericProperty.h"
#include "mitkVector.h"

#include <iostream>
#include <string>

// call with testValue1 != testValue2 
template <typename T>
int TestGenericPropertyForDataType(T testValue1, T testValue2, std::string testValue1AsString, std::string testValue2AsString, std::string type)
{
  std::cout << "Testing mitk::GenericProperty<" << type << ">(" << testValue1 << ", " << testValue2 << ") ";
  
  typename mitk::GenericProperty<T>::Pointer prop = new mitk::GenericProperty<T>();
  typename mitk::GenericProperty<T>::Pointer prop2 = new mitk::GenericProperty<T>(testValue1);
  typename mitk::GenericProperty<T>::Pointer prop3 = new mitk::GenericProperty<T>(testValue2);
  
  unsigned long tBefore = prop->GetMTime();
  prop->SetValue(testValue1);
  unsigned long tAfter = prop->GetMTime();
  prop->SetValue(testValue1);
  unsigned long tAfterAll = prop->GetMTime();
  
  if (prop->GetValue() != testValue1 || prop->GetValueAsString() != testValue1AsString) 
  {
    std::cout << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;
    
  std::cout << "    Testing equality operator (operator==): ";
  if ( (! (*prop == *prop2)) || (*prop2 == *prop3) ) {
    std::cout << " [FAILED]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;
  
  prop->SetValue(testValue2);
  unsigned long tAfterEverything = prop->GetMTime();
 
  std::cout << "    Testing MTime correctness when changing property value: ";
  if (tBefore >= tAfter || tAfterAll != tAfter || tAfterEverything <= tAfterAll) {
     std::cout << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

  prop->SetValue(testValue1);
  std::cout << "    Testing assignment operator (operator=): ";
  *prop = *prop3;
  if ( (! (*prop == *prop3)) || (*prop == *prop2) ) {
    std::cout << " [FAILED]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;
  std::cout << std::endl;
  
  return EXIT_SUCCESS;
}  


int mitkGenericPropertyTest(int /*argc*/, char* /*argv*/[])
{
  int retVal(EXIT_SUCCESS);

  // testing for some different data types
  TestGenericPropertyForDataType<int>(1, 2, "1", "2", "int");
  TestGenericPropertyForDataType<bool>(true, false, "1", "0", "bool");
  TestGenericPropertyForDataType<float>(1.0, -1.0, "1", "-1", "float");
  TestGenericPropertyForDataType<double>(1.0, -1.0, "1", "-1", "double");
  
  TestGenericPropertyForDataType<std::string>("eins", "zwei", "eins", "zwei", "std::string");

  {
  mitk::Point3D p1; p1[0] = 2.0; p1[1] = 3.0; p1[2] = 4.0;
  mitk::Point3D p2; p2[0] =-1.0; p2[1] = 2.0; p2[2] = 3.0;
  TestGenericPropertyForDataType<mitk::Point3D>( p1, p2, "[2, 3, 4]", "[-1, 2, 3]", "mitk::Point3D");
  }
 
  {
  mitk::Point4D p1; p1[0] = 2.0; p1[1] = 3.0; p1[2] = 4.0; p1[3] =-2.0;
  mitk::Point4D p2; p2[0] =-1.0; p2[1] = 2.0; p2[2] = 3.0; p2[3] = 5.0;
  TestGenericPropertyForDataType<mitk::Point4D>( p1, p2, "[2, 3, 4, -2]", "[-1, 2, 3, 5]", "mitk::Point4D");
  }
 
  /*  THIS won't compile because of the interface of XMLWriter... that should be reworked perhaps
  {
  mitk::Vector2D p1; p1[0] = 2.0; p1[1] = 3.0;
  mitk::Vector2D p2; p2[0] =-1.0; p2[1] = 2.0;
  TestGenericPropertyForDataType<mitk::Vector2D>( p1, p2, "[2, 3]", "[-1, 2]", "mitk::Vector2D");
  }
  */

  {
  mitk::Vector3D p1; p1[0] = 2.0; p1[1] = 3.0; p1[2] = 4.0;
  mitk::Vector3D p2; p2[0] =-1.0; p2[1] = 2.0; p2[2] = 3.0;
  TestGenericPropertyForDataType<mitk::Vector3D>( p1, p2, "[2, 3, 4]", "[-1, 2, 3]", "mitk::Vector3D");
  }
 
  std::cout << "[TEST DONE]"<<std::endl;
  return retVal;
}
