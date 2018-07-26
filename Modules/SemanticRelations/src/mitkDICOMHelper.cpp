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

// semantic relations module
#include "mitkDICOMHelper.h"
#include "mitkSemanticRelationException.h"
#include "mitkUIDGeneratorBoost.h"

// mitk core
#include <mitkPropertyNameHelper.h>

// c++
#include <algorithm>

mitk::SemanticTypes::Date GetDateFromString(const std::string& dateAsString);

mitk::SemanticTypes::CaseID mitk::GetCaseIDFromDataNode(const mitk::DataNode* dataNode)
{
  if (nullptr == dataNode)
  {
    mitkThrowException(SemanticRelationException) << "Not a valid data node.";
  }

  mitk::BaseData* baseData = dataNode->GetData();
  if (nullptr == baseData)
  {
    mitkThrowException(SemanticRelationException) << "No valid base data.";
  }

  // extract suitable DICOM tag to use as the case id
  // two alternatives can be used:
  //        - DICOM tag "0x0010, 0x0010" is PatientName
  //        - DICOM tag "0x0010, 0x0020" is PatientID
  // in the current implementation the PatientID (0x0010, 0x0010) is used
  mitk::BaseProperty* dicomTag = baseData->GetProperty(mitk::GeneratePropertyNameForDICOMTag(0x0010, 0x0010).c_str());
  if (nullptr == dicomTag)
  {
    mitkThrowException(SemanticRelationException) << "Not a valid DICOM property.";
  }

  std::string dicomTagAsString = dicomTag->GetValueAsString();
  return dicomTagAsString;
}

mitk::SemanticTypes::ID mitk::GetIDFromDataNode(const mitk::DataNode* dataNode)
{
  if (nullptr == dataNode)
  {
    mitkThrowException(SemanticRelationException) << "Not a valid data node.";
  }

  mitk::BaseData* baseData = dataNode->GetData();
  if (nullptr == baseData)
  {
    mitkThrowException(SemanticRelationException) << "No valid base data.";
  }

  // extract suitable DICOM tag to use as the data node id
  // DICOM tag "0x0020, 0x000e" is SeriesInstanceUID
  mitk::BaseProperty* dicomTag = baseData->GetProperty(mitk::GeneratePropertyNameForDICOMTag(0x0020, 0x000e).c_str());
  if (nullptr == dicomTag)
  {
    mitkThrowException(SemanticRelationException) << "Not a valid DICOM property.";
  }
  std::string dicomTagAsString = dicomTag->GetValueAsString();
  return dicomTagAsString;
}

mitk::SemanticTypes::Date mitk::GetDICOMDateFromDataNode(const mitk::DataNode* dataNode)
{
  if (nullptr == dataNode)
  {
    mitkThrowException(SemanticRelationException) << "Not a valid data node.";
  }

  mitk::BaseData* baseData = dataNode->GetData();
  if (nullptr == baseData)
  {
    mitkThrowException(SemanticRelationException) << "No valid base data.";
  }

  // extract suitable DICOM tag to use as the data node id
  // DICOM tag "0x0008, 0x0022" is AcquisitionDate
  mitk::BaseProperty* acquisitionDateProperty = baseData->GetProperty(mitk::GeneratePropertyNameForDICOMTag(0x0008, 0x0022).c_str());
  if (nullptr == acquisitionDateProperty)
  {
    mitkThrowException(SemanticRelationException) << "Not a valid DICOM property.";
  }
  std::string acquisitionDateAsString = acquisitionDateProperty->GetValueAsString();
  return GetDateFromString(acquisitionDateAsString);
}

mitk::SemanticTypes::InformationType mitk::GetDICOMModalityFromDataNode(const mitk::DataNode* dataNode)
{
  if (nullptr == dataNode)
  {
    mitkThrowException(SemanticRelationException) << "Not a valid data node.";
  }

  mitk::BaseData* baseData = dataNode->GetData();
  if (nullptr == baseData)
  {
    mitkThrowException(SemanticRelationException) << "No valid base data.";
  }

  // extract suitable DICOM tag to use as the information type
  // DICOM tag "0x0008, 0x0060" is Modality
  mitk::BaseProperty* dicomTag = baseData->GetProperty(mitk::GeneratePropertyNameForDICOMTag(0x0008, 0x0060).c_str());
  if (nullptr == dicomTag)
  {
    mitkThrowException(SemanticRelationException) << "Not a valid DICOM property.";
  }
  std::string dicomTagAsString = dicomTag->GetValueAsString();
  return dicomTagAsString;
}

std::string mitk::TrimDICOM(const std::string& identifier)
{
  if (identifier.empty())
  {
    return identifier;
  }

  // leading whitespace
  std::size_t first = identifier.find_first_not_of(' ');
  if (std::string::npos == first)
  {
    return "";
  }
  // trailing whitespace
  std::size_t last = identifier.find_last_not_of(' ');
  return identifier.substr(first, last - first + 1);
}

mitk::SemanticTypes::Date GetDateFromString(const std::string& dateAsString)
{
  if (dateAsString.size() != 8) // string does not represent a DICOM date
  {
    return mitk::SemanticTypes::Date();
  }

  mitk::SemanticTypes::Date date;
  date.UID = mitk::UIDGeneratorBoost::GenerateUID();
  // date expected to be YYYYMMDD (8 characters)
  date.year = std::strtoul(dateAsString.substr(0, 4).c_str(), nullptr, 10);
  date.month = std::strtoul(dateAsString.substr(4, 2).c_str(), nullptr, 10);
  date.day = std::strtoul(dateAsString.substr(6, 2).c_str(), nullptr, 10);

  return date;
}
