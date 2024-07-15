/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "mitkIOVolumeSplitReason.h"

#include <nlohmann/json.hpp>

void mitk::IOVolumeSplitReason::AddReason(ReasonType type, std::string_view detail)
{
  m_ReasonMap.insert(std::make_pair(type, detail));
}

void mitk::IOVolumeSplitReason::RemoveReason(ReasonType type)
{
  m_ReasonMap.erase(type);
}

bool mitk::IOVolumeSplitReason::ReasonExists() const
{
  return !m_ReasonMap.empty();
}

bool mitk::IOVolumeSplitReason::ReasonExists(ReasonType type) const
{
  return m_ReasonMap.cend() != m_ReasonMap.find(type);
}

std::string mitk::IOVolumeSplitReason::GetReasonDetails(ReasonType type) const
{
  auto finding = m_ReasonMap.find(type);
  if (m_ReasonMap.cend() == finding)
    mitkThrow() << "Cannot get details for inexistent type.";

  return finding->second;
};

mitk::IOVolumeSplitReason::Pointer mitk::IOVolumeSplitReason::ExtendReason(const Self* otherReason) const
{
  if (nullptr == otherReason)
    mitkThrow() << "Cannot extend reason. Pass other reason is in valid.";

  Pointer result = this->Clone();

  result->m_ReasonMap.insert(otherReason->m_ReasonMap.cbegin(), otherReason->m_ReasonMap.cend());

  return result;
}

mitk::IOVolumeSplitReason::IOVolumeSplitReason(): itk::LightObject()
{
}

mitk::IOVolumeSplitReason::~IOVolumeSplitReason()
{
}

mitk::IOVolumeSplitReason::IOVolumeSplitReason(const IOVolumeSplitReason& other)
{
  m_ReasonMap = other.m_ReasonMap;
}

std::string mitk::IOVolumeSplitReason::TypeToString(const IOVolumeSplitReason::ReasonType& reasonType)
{
  switch (reasonType)
  {
  case IOVolumeSplitReason::ReasonType::GantryTiltDifference:
    return "gantry_tilt_difference";
  case IOVolumeSplitReason::ReasonType::ImagePostionMissing:
    return "image_position_missing";
  case IOVolumeSplitReason::ReasonType::OverlappingSlices:
    return "overlapping_slices";
  case IOVolumeSplitReason::ReasonType::SliceDistanceInconsistency:
    return "slice_distance_inconsistency";
  case IOVolumeSplitReason::ReasonType::ValueSortDistance:
    return "value_sort_distance";
  case IOVolumeSplitReason::ReasonType::ValueSplitDifference:
    return "value_split_difference";
  case IOVolumeSplitReason::ReasonType::MissingSlices:
    return "missing_slices";
  }
  return "unknown";
}

mitk::IOVolumeSplitReason::ReasonType mitk::IOVolumeSplitReason::StringToType(const std::string& reasonStr)
{
  if (reasonStr == "gantry_tilt_difference")
    return IOVolumeSplitReason::ReasonType::GantryTiltDifference;
  else if (reasonStr == "image_position_missing")
    return IOVolumeSplitReason::ReasonType::ImagePostionMissing;
  else if (reasonStr == "overlapping_slices")
    return IOVolumeSplitReason::ReasonType::OverlappingSlices;
  else if (reasonStr == "slice_distance_inconsistency")
    return IOVolumeSplitReason::ReasonType::SliceDistanceInconsistency;
  else if (reasonStr == "value_sort_distance")
    return IOVolumeSplitReason::ReasonType::ValueSortDistance;
  else if (reasonStr == "value_split_difference")
    return IOVolumeSplitReason::ReasonType::ValueSplitDifference;
  else if (reasonStr == "missing_slices")
    return IOVolumeSplitReason::ReasonType::MissingSlices;

  return IOVolumeSplitReason::ReasonType::Unkown;
}

std::string mitk::IOVolumeSplitReason::SerializeToJSON(const IOVolumeSplitReason* reason)
{
  if (nullptr == reason)
    mitkThrow() << "Cannot extend reason. Pass other reason is in valid.";

  auto data = nlohmann::json::array();

  for (const auto& [type, detail] : reason->m_ReasonMap)
  {
    auto details = nlohmann::json::array();

    details.push_back(TypeToString(type));
    if (!detail.empty())
    {
      details.push_back(detail);
    }
    data.push_back(details);
  }

  return data.dump();
}

mitk::IOVolumeSplitReason::Pointer mitk::IOVolumeSplitReason::DeserializeFromJSON(const std::string& reasonStr)
{
  if (reasonStr.empty())
    return nullptr;

  auto reason = IOVolumeSplitReason::New();

  auto data = nlohmann::json::parse(reasonStr);

  for (const auto& jItem : data)
  {
    if (!jItem.empty())
    {
      ReasonType reasonType = StringToType(jItem.at(0).get<std::string>());

      std::string detail;
      if (jItem.size() > 1)
      {
        detail = jItem.at(1).get<std::string>();
      }
      reason->AddReason(reasonType, detail);
    }
  }

  return reason;
}
