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

#include "mitkNodePredicateDataType.h"

#include "mitkDataTreeNode.h"
#include "mitkBaseData.h"

mitk::NodePredicateDataType::NodePredicateDataType(const char* datatype)
: NodePredicateBase()
{
  if (datatype == NULL)
    throw 1;  // Insert Exception Handling here

  m_ValidDataType = datatype;
}

mitk::NodePredicateDataType::~NodePredicateDataType()
{
}


bool mitk::NodePredicateDataType::CheckNode(mitk::DataTreeNode* node) const
{
  if (node == NULL)
    throw 1;  // Insert Exception Handling here

  mitk::BaseData* data = node->GetData();

  if (data == NULL)
    return false;  // or should we check if m_ValidDataType == "NULL" so that nodes without data can be requested?

  return ( m_ValidDataType.compare(data->GetNameOfClass()) == 0); // return true if data type matches 
}
