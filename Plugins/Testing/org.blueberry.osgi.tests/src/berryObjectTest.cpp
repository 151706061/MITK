/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "berryObjectTest.h"

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <berryObject.h>
//#include <cstdlib>

namespace berry
{

class TestObject: public Object
{
public:
  TestObject() :
    Object()
  {
  }
};

/**
 * used for listening to destroy events
 */
struct ObjectListener
{
  bool m_ObjectDeleted;

  ObjectListener() :
    m_ObjectDeleted(false)
  {

  }

  void DestroyListener()
  {
    m_ObjectDeleted = true;
  }

};

CppUnit::Test* ObjectTest::Suite()
{
  CppUnit::TestSuite* suite = new CppUnit::TestSuite("ObjectTest");

  CppUnit_addTest(suite, ObjectTest, TestReferenceCount);
  CppUnit_addTest(suite, ObjectTest, TestAddDestroyListener);
  CppUnit_addTest(suite, ObjectTest, TestRemoveDestroyListener);

  return suite;
}

ObjectTest::ObjectTest(const std::string& testName) :
  TestCase(testName)
{

}

void ObjectTest::TestReferenceCount()
{
  TestObject* obj = new TestObject();
  assertEqual(obj->GetReferenceCount(), 0);

  obj->Register();
  assertEqual(obj->GetReferenceCount(), 1);

  obj->SetReferenceCount(3);
  assertEqual(obj->GetReferenceCount(), 3);

  obj->SetReferenceCount(0);
}

void ObjectTest::TestAddDestroyListener()
{
  TestObject* obj = new TestObject();

  ObjectListener objectListener;

  obj->AddDestroyListener(berry::MessageDelegate<ObjectListener>(
      &objectListener, &ObjectListener::DestroyListener));
  delete obj;
  assertEqual(true, objectListener.m_ObjectDeleted);
}

void ObjectTest::TestRemoveDestroyListener()
{
  TestObject* obj = new TestObject();

  ObjectListener objectListener;

  obj->AddDestroyListener(berry::MessageDelegate<ObjectListener>(
      &objectListener, &ObjectListener::DestroyListener));
  obj->RemoveDestroyListener(berry::MessageDelegate<ObjectListener>(
      &objectListener, &ObjectListener::DestroyListener));
  delete obj;
  assertEqual(false, objectListener.m_ObjectDeleted);
}

}
