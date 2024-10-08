/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

//#define MBILOG_ENABLE_DEBUG

#include "mitkEquiDistantBlocksSorter.h"

mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::SliceGroupingAnalysisResult() : m_SplitReason(IOVolumeSplitReason::New())
{
}

const mitk::DICOMDatasetList&
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::GetBlockDatasets() const
{
  return m_GroupedFiles;
}

const mitk::DICOMDatasetList&
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::GetUnsortedDatasets() const
{
  return m_UnsortedFiles;
}

mitk::IOVolumeSplitReason::ConstPointer
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult::GetSplitReason() const
{
  return m_SplitReason;
}

mitk::IOVolumeSplitReason::Pointer
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult::GetSplitReason()
{
  return m_SplitReason;
}

bool
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::ContainsGantryTilt()
{
  return m_TiltInfo.IsRegularGantryTilt();
}

void
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::AddFileToSortedBlock(DICOMDatasetAccess* dataset)
{
  m_GroupedFiles.push_back( dataset );
}

void
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::AddFileToUnsortedBlock(DICOMDatasetAccess* dataset)
{
  m_UnsortedFiles.push_back( dataset );
}

void
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::AddFilesToUnsortedBlock(const DICOMDatasetList& datasets)
{
  m_UnsortedFiles.insert( m_UnsortedFiles.end(), datasets.begin(), datasets.end() );
}

void
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::SetFirstFilenameOfBlock(const std::string& filename)
{
  m_FirstFilenameOfBlock = filename;
}

std::string
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::GetFirstFilenameOfBlock() const
{
  return m_FirstFilenameOfBlock;
}

void
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::SetLastFilenameOfBlock(const std::string& filename)
{
  m_LastFilenameOfBlock = filename;
}

std::string
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::GetLastFilenameOfBlock() const
{
  return m_LastFilenameOfBlock;
}


void
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::FlagGantryTilt(const GantryTiltInformation& tiltInfo)
{
  m_TiltInfo = tiltInfo;
}

const mitk::GantryTiltInformation&
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::GetTiltInfo() const
{
  return m_TiltInfo;
}

void
mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult
::UndoPrematureGrouping()
{
  assert( !m_GroupedFiles.empty() );
  m_UnsortedFiles.insert( m_UnsortedFiles.begin(), m_GroupedFiles.back() );
  m_GroupedFiles.pop_back();
  m_TiltInfo = GantryTiltInformation();
}

// ------------------------ end helper class

mitk::EquiDistantBlocksSorter
::EquiDistantBlocksSorter()
:DICOMDatasetSorter()
,m_AcceptTilt(false)
,m_ToleratedOriginOffset(0.3)
,m_ToleratedOriginOffsetIsAbsolute(false)
,m_AcceptTwoSlicesGroups(true)
{
}

mitk::EquiDistantBlocksSorter
::EquiDistantBlocksSorter(const EquiDistantBlocksSorter& other )
:DICOMDatasetSorter(other)
,m_AcceptTilt(other.m_AcceptTilt)
,m_ToleratedOriginOffset(other.m_ToleratedOriginOffset)
,m_ToleratedOriginOffsetIsAbsolute(other.m_ToleratedOriginOffsetIsAbsolute)
,m_AcceptTwoSlicesGroups(other.m_AcceptTwoSlicesGroups)
{
}

mitk::EquiDistantBlocksSorter
::~EquiDistantBlocksSorter()
{
}

bool
mitk::EquiDistantBlocksSorter
::operator==(const DICOMDatasetSorter& other) const
{
  if (const auto* otherSelf = dynamic_cast<const EquiDistantBlocksSorter*>(&other))
  {
    return this->m_AcceptTilt == otherSelf->m_AcceptTilt
        && this->m_ToleratedOriginOffsetIsAbsolute == otherSelf->m_ToleratedOriginOffsetIsAbsolute
        && this->m_AcceptTwoSlicesGroups == otherSelf->m_AcceptTwoSlicesGroups
        && (fabs(this->m_ToleratedOriginOffset - otherSelf->m_ToleratedOriginOffset) < eps);
  }
  else
  {
    return false;
  }
}

void
mitk::EquiDistantBlocksSorter
::PrintConfiguration(std::ostream& os, const std::string& indent) const
{
  std::stringstream ts;
  if (!m_ToleratedOriginOffsetIsAbsolute)
  {
    ts << "adaptive";
  }
  else
  {
    ts << m_ToleratedOriginOffset << "mm";
  }

  os << indent << "Sort into blocks of equidistant, well-aligned (tolerance "
     << ts.str() << ") slices "
     << (m_AcceptTilt ? "(accepting a gantry tilt)" : "")
     << std::endl;
}


void
mitk::EquiDistantBlocksSorter
::SetAcceptTilt(bool accept)
{
  m_AcceptTilt = accept;
}


bool
mitk::EquiDistantBlocksSorter
::GetAcceptTilt() const
{
  return m_AcceptTilt;
}

void
mitk::EquiDistantBlocksSorter
::SetAcceptTwoSlicesGroups(bool accept)
{
  m_AcceptTwoSlicesGroups = accept;
}

bool
mitk::EquiDistantBlocksSorter
::GetAcceptTwoSlicesGroups() const
{
  return m_AcceptTwoSlicesGroups;
}


mitk::EquiDistantBlocksSorter&
mitk::EquiDistantBlocksSorter
::operator=(const EquiDistantBlocksSorter& other)
{
  if (this != &other)
  {
    DICOMDatasetSorter::operator=(other);
    m_AcceptTilt = other.m_AcceptTilt;
    m_ToleratedOriginOffset = other.m_ToleratedOriginOffset;
    m_ToleratedOriginOffsetIsAbsolute = other.m_ToleratedOriginOffsetIsAbsolute;
    m_AcceptTwoSlicesGroups = other.m_AcceptTwoSlicesGroups;
  }
  return *this;
}

mitk::DICOMTagList
mitk::EquiDistantBlocksSorter
::GetTagsOfInterest()
{
  DICOMTagList tags;
  tags.push_back( DICOMTag(0x0020, 0x0032) ); // ImagePositionPatient
  tags.push_back( DICOMTag(0x0020, 0x0037) ); // ImageOrientationPatient
  tags.push_back( DICOMTag(0x0018, 0x1120) ); // GantryDetectorTilt

  return tags;
}

void
mitk::EquiDistantBlocksSorter
::Sort()
{
  DICOMDatasetList remainingInput = GetInput(); // copy

  m_SliceGroupingResults.clear();

  while (!remainingInput.empty()) // repeat until all files are grouped somehow
  {
    auto regularBlock = this->AnalyzeFileForITKImageSeriesReaderSpacingAssumption( remainingInput, m_AcceptTilt );

#ifdef MBILOG_ENABLE_DEBUG
    DICOMDatasetList inBlock = regularBlock.GetBlockDatasets();
    DICOMDatasetList laterBlock = regularBlock.GetUnsortedDatasets();
    MITK_DEBUG << "Result: sorted 3D group with " << inBlock.size() << " files";
    for (DICOMDatasetList::const_iterator diter = inBlock.cbegin(); diter != inBlock.cend(); ++diter)
      MITK_DEBUG << "  IN  " << (*diter)->GetFilenameIfAvailable();
    for (DICOMDatasetList::const_iterator diter = laterBlock.cbegin(); diter != laterBlock.cend(); ++diter)
      MITK_DEBUG << " OUT  " << (*diter)->GetFilenameIfAvailable();
#endif // MBILOG_ENABLE_DEBUG

    remainingInput = regularBlock->GetUnsortedDatasets();

    if (remainingInput.empty() && !m_SliceGroupingResults.empty() && m_SliceGroupingResults.back()->GetSplitReason()->HasReason(IOVolumeSplitReason::ReasonType::OverlappingSlices))
    {
      //if all inputs are processed and there is already a preceding grouping result that has overlapping as split reason, add also overlapping as split reason for the current block
      regularBlock->GetSplitReason()->AddReason(IOVolumeSplitReason::ReasonType::OverlappingSlices);
    }

    m_SliceGroupingResults.push_back( regularBlock );
  }

  unsigned int numberOfOutputs = m_SliceGroupingResults.size();
  this->SetNumberOfOutputs(numberOfOutputs);

  unsigned int outputIndex(0);
  for (auto oIter = m_SliceGroupingResults.cbegin();
       oIter != m_SliceGroupingResults.cend();
       ++outputIndex, ++oIter)
  {
    this->SetOutput(outputIndex, (*oIter)->GetBlockDatasets(), (*oIter)->GetSplitReason());
  }
}

void
mitk::EquiDistantBlocksSorter
::SetToleratedOriginOffsetToAdaptive(double fractionOfInterSliceDistance)
{
  m_ToleratedOriginOffset = fractionOfInterSliceDistance;
  m_ToleratedOriginOffsetIsAbsolute = false;

  if (m_ToleratedOriginOffset < 0.0)
  {
    MITK_WARN << "Call SetToleratedOriginOffsetToAdaptive() only with positive numbers between 0.0 and 1.0, read documentation!";
  }

  if (m_ToleratedOriginOffset > 0.5)
  {
    MITK_WARN << "EquiDistantBlocksSorter is now accepting large errors, take care of measurements, they could appear at imprecise locations!";
  }
}

void
mitk::EquiDistantBlocksSorter
::SetToleratedOriginOffset(double millimeters)
{
  m_ToleratedOriginOffset = millimeters;
  m_ToleratedOriginOffsetIsAbsolute = true;
  if (m_ToleratedOriginOffset < 0.0)
  {
    MITK_WARN << "Negative tolerance set to SetToleratedOriginOffset()!";
  }
}

double
mitk::EquiDistantBlocksSorter
::GetToleratedOriginOffset() const
{
  return m_ToleratedOriginOffset;
}

bool
mitk::EquiDistantBlocksSorter
::IsToleratedOriginOffsetAbsolute() const
{
  return m_ToleratedOriginOffsetIsAbsolute;
}


std::string
mitk::EquiDistantBlocksSorter
::ConstCharStarToString(const char* s)
{
  return s ?  std::string(s) : std::string();
}

std::shared_ptr<mitk::EquiDistantBlocksSorter::SliceGroupingAnalysisResult>
mitk::EquiDistantBlocksSorter
::AnalyzeFileForITKImageSeriesReaderSpacingAssumption(
  const DICOMDatasetList& datasets,
  bool groupImagesWithGantryTilt)
{
  auto result = std::make_shared<SliceGroupingAnalysisResult>();

  const DICOMTag tagImagePositionPatient = DICOMTag(0x0020,0x0032); // Image Position (Patient)
  const DICOMTag    tagImageOrientation = DICOMTag(0x0020, 0x0037); // Image Orientation

  Vector3D fromFirstToSecondOrigin; fromFirstToSecondOrigin.Fill(0.0);
  bool fromFirstToSecondOriginInitialized(false);
  Point3D thisOrigin;
  thisOrigin.Fill(0.0f);
  Point3D lastOrigin;
  lastOrigin.Fill(0.0f);
  Point3D lastDifferentOrigin;
  lastDifferentOrigin.Fill(0.0f);

  bool lastOriginInitialized(false);
  int missingSlicesCount = 0;

  MITK_DEBUG << "--------------------------------------------------------------------------------";
  MITK_DEBUG << "Analyzing " << datasets.size() << " files for z-spacing assumption of ITK's ImageSeriesReader (group tilted: " << groupImagesWithGantryTilt << ")";
  unsigned int fileIndex(0);
  double toleratedOriginError(0.005); // default: max. 1/10mm error when measurement crosses 20 slices in z direction (too strict? we don't know better)
  for (auto dsIter = datasets.cbegin();
       dsIter != datasets.cend();
       ++dsIter, ++fileIndex)
  {
    bool fileFitsIntoPattern(false);
    std::string thisOriginString;
    // Read tag value into point3D. PLEASE replace this by appropriate GDCM code if you figure out how to do that
    thisOriginString = (*dsIter)->GetTagValueAsString(tagImagePositionPatient).value;

    if (thisOriginString.empty())
    {
      // don't let such files be in a common group. Everything without position information will be loaded as a single slice:
      // with standard DICOM files this can happen to: CR, DX, SC
      MITK_DEBUG << "    ==> Sort away " << *dsIter << " for later analysis (no position information)"; // we already have one occupying this position

      if ( result->GetBlockDatasets().empty() ) // nothing WITH position information yet
      {
        // ==> this is a group of its own, stop processing, come back later
        result->AddFileToSortedBlock( *dsIter );

        DICOMDatasetList remainingFiles;
        remainingFiles.insert( remainingFiles.end(), dsIter+1, datasets.end() );
        result->AddFilesToUnsortedBlock( remainingFiles );
        if (!remainingFiles.empty())
          //if there are remaining files add a split reason
          result->GetSplitReason()->AddReason(IOVolumeSplitReason::ReasonType::ImagePostionMissing);
        fileFitsIntoPattern = false;
        break; // no files anymore
      }
      else
      {
        // ==> this does not match, consider later
        result->AddFileToUnsortedBlock( *dsIter ); // sort away for further analysis
        fileFitsIntoPattern = false;
        continue; // next file
      }
    }

    bool ignoredConversionError(-42); // hard to get here, no graceful way to react
    thisOrigin = DICOMStringToPoint3D( thisOriginString, ignoredConversionError );

    MITK_DEBUG << "  " << fileIndex << " " << (*dsIter)->GetFilenameIfAvailable()
                       << " at "
                       /* << thisOriginString */ << "(" << thisOrigin[0] << "," << thisOrigin[1] << "," << thisOrigin[2] << ")";

    if ( lastOriginInitialized && (thisOrigin == lastOrigin) )
    {
      MITK_DEBUG << "    ==> Sort away " << *dsIter << " for separate time step"; // we already have one occupying this position
      result->AddFileToUnsortedBlock( *dsIter ); // sort away for further analysis
      result->GetSplitReason()->AddReason(IOVolumeSplitReason::ReasonType::OverlappingSlices);
      fileFitsIntoPattern = false;
    }
    else
    {
      if (!fromFirstToSecondOriginInitialized && lastOriginInitialized) // calculate vector as soon as possible when we get a new position
      {
        fromFirstToSecondOrigin = thisOrigin - lastDifferentOrigin;
        fromFirstToSecondOriginInitialized = true;

        // classic mode without tolerance!
        if (!m_ToleratedOriginOffsetIsAbsolute)
        {
          MITK_DEBUG << "Distance of two slices: " << fromFirstToSecondOrigin.GetNorm() << "mm";
          toleratedOriginError =
            fromFirstToSecondOrigin.GetNorm() * 0.3; // a third of the slice distance
          //  (less than half, which would mean that a slice is displayed where another slice should actually be)
        }
        else
        {
          toleratedOriginError = m_ToleratedOriginOffset;
        }
        MITK_DEBUG << "Accepting errors in actual versus expected origin up to " << toleratedOriginError << "mm";

        // Here we calculate if this slice and the previous one are well aligned,
        // i.e. we test if the previous origin is on a line through the current
        // origin, directed into the normal direction of the current slice.

        // If this is NOT the case, then we have a data set with a TILTED GANTRY geometry,
        // which cannot be simply loaded into a single mitk::Image at the moment.
        // For this case, we flag this finding in the result and DicomSeriesReader
        // can correct for that later.

        Vector3D right; right.Fill(0.0);
        Vector3D up; right.Fill(0.0); // might be down as well, but it is just a name at this point
        std::string orientationValue = (*dsIter)->GetTagValueAsString( tagImageOrientation ).value;
        DICOMStringToOrientationVectors( orientationValue, right, up, ignoredConversionError );

        GantryTiltInformation tiltInfo( lastDifferentOrigin, thisOrigin, right, up, 1 );

        if ( tiltInfo.IsSheared() )
        {
          /* optimistic approach, accepting gantry tilt: save file for later, check all further files */

          // at this point we have TWO slices analyzed! if they are the only two files, we still split, because there is no third to verify our tilting assumption.
          // later with a third being available, we must check if the initial tilting vector is still valid. if yes, continue.
          // if NO, we need to split the already sorted part (result->first) and the currently analyzed file (*dsIter)

          // tell apart gantry tilt from overall skewedness
          // sort out irregularly sheared slices, that IS NOT tilting

          if ( groupImagesWithGantryTilt && tiltInfo.IsRegularGantryTilt() )
          {
            assert(!datasets.empty());

            result->FlagGantryTilt(tiltInfo);
            result->AddFileToSortedBlock( *dsIter ); // this file is good for current block
            result->SetFirstFilenameOfBlock( datasets.front()->GetFilenameIfAvailable() );
            result->SetLastFilenameOfBlock( datasets.back()->GetFilenameIfAvailable() );
            fileFitsIntoPattern = true;
          }
          else // caller does not want tilt compensation OR shearing is more complicated than tilt
          {
            result->AddFileToUnsortedBlock( *dsIter ); // sort away for further analysis
            result->GetSplitReason()->AddReason(IOVolumeSplitReason::ReasonType::GantryTiltDifference);
            fileFitsIntoPattern = false;
          }
        }
        else // not sheared
        {
          result->AddFileToSortedBlock( *dsIter ); // this file is good for current block
          fileFitsIntoPattern = true;
        }
      }
      else if (fromFirstToSecondOriginInitialized) // we already know the offset between slices
      {
        Point3D assumedOrigin = lastDifferentOrigin + fromFirstToSecondOrigin;

        Vector3D originError = assumedOrigin - thisOrigin;
        double norm = originError.GetNorm();

        if (norm > toleratedOriginError)
        {
          MITK_DEBUG << "  File does not fit into the inter-slice distance pattern (diff = "
                               << norm << ", allowed "
                               << toleratedOriginError << ").";
          MITK_DEBUG << "  Expected position (" << assumedOrigin[0] << ","
                                            << assumedOrigin[1] << ","
                                            << assumedOrigin[2] << "), got position ("
                                            << thisOrigin[0] << ","
                                            << thisOrigin[1] << ","
                                            << thisOrigin[2] << ")";
          MITK_DEBUG  << "    ==> Sort away " << *dsIter << " for later analysis";

          // At this point we know we deviated from the expectation of ITK's ImageSeriesReader
          // We split the input file list at this point, i.e. all files up to this one (excluding it)
          // are returned as group 1, the remaining files (including the faulty one) are group 2

          /* Optimistic approach: check if any of the remaining slices fits in */
          result->AddFileToUnsortedBlock( *dsIter ); // sort away for further analysis

          const auto fromLastToThisOriginDistance = (lastDifferentOrigin - thisOrigin).GetNorm();
          const auto fromFirstToSecondOriginDistance = fromFirstToSecondOrigin.GetNorm();

          auto currentMissCount = static_cast<int>(std::round((fromLastToThisOriginDistance / fromFirstToSecondOriginDistance)-1));
          if (missingSlicesCount == 0 || missingSlicesCount > currentMissCount)
          {
            if (missingSlicesCount != 0 && currentMissCount == 0)
            {
              result->GetSplitReason()->RemoveReason(IOVolumeSplitReason::ReasonType::MissingSlices);
            }

            missingSlicesCount = currentMissCount;
            result->GetSplitReason()->AddReason(IOVolumeSplitReason::ReasonType::SliceDistanceInconsistency, std::to_string(fromLastToThisOriginDistance));
          }

          if (missingSlicesCount < 0)
          {
            result->GetSplitReason()->AddReason(IOVolumeSplitReason::ReasonType::OverlappingSlices);
          }
          else if (missingSlicesCount > 0 && !result->GetSplitReason()->HasReason(IOVolumeSplitReason::ReasonType::OverlappingSlices))
          {
            //If the missing slice count is positive, but no overlapping was flagged, add the missing slice reason.
            //We only do it if overlapping was not flagged, to avoid false positives, that could be triggered by slices
            //of the overlapping volume.
            result->GetSplitReason()->AddReason(IOVolumeSplitReason::ReasonType::MissingSlices, std::to_string(missingSlicesCount));
          }

          fileFitsIntoPattern = false;
        }
        else
        {
          result->AddFileToSortedBlock( *dsIter ); // this file is good for current block
          result->GetSplitReason()->RemoveReason(IOVolumeSplitReason::ReasonType::SliceDistanceInconsistency);
          result->GetSplitReason()->RemoveReason(IOVolumeSplitReason::ReasonType::MissingSlices);
          fileFitsIntoPattern = true;
          missingSlicesCount = 0;
        }
      }
      else // this should be the very first slice
      {
        result->AddFileToSortedBlock( *dsIter ); // this file is good for current block
        fileFitsIntoPattern = true;
      }
    }

    // record current origin for reference in later iterations
    if ( !lastOriginInitialized || ( fileFitsIntoPattern && (thisOrigin != lastOrigin) ) )
    {
      lastDifferentOrigin = thisOrigin;
    }

    lastOrigin = thisOrigin;
    lastOriginInitialized = true;
  }

  if ( result->ContainsGantryTilt() )
  {
    // check here how many files were grouped.
    // IF it was only two files AND we assume tiltedness (e.g. save "distance")
    // THEN we would want to also split the two previous files (simple) because
    // we don't have any reason to assume they belong together

    // Above behavior can be configured via m_AcceptTwoSlicesGroups, the default being "do accept"
    if ( result->GetBlockDatasets().size() == 2 && !m_AcceptTwoSlicesGroups )
    {
      result->UndoPrematureGrouping();
      result->GetSplitReason()->AddReason(IOVolumeSplitReason::ReasonType::GantryTiltDifference);
    }
  }

  // update tilt info to get maximum precision
  // earlier, tilt was only calculated from first and second slice.
  // now that we know the whole range, we can re-calculate using the very first and last slice
  if ( result->ContainsGantryTilt() && result->GetBlockDatasets().size() > 1 )
  {
    try
    {
      DICOMDatasetList datasets = result->GetBlockDatasets();
      DICOMDatasetAccess* firstDataset = datasets.front();
      DICOMDatasetAccess* lastDataset = datasets.back();
      unsigned int numberOfSlicesApart = datasets.size() - 1;

      std::string orientationString = firstDataset->GetTagValueAsString( tagImageOrientation ).value;
      std::string firstOriginString = firstDataset->GetTagValueAsString( tagImagePositionPatient ).value;
      std::string lastOriginString = lastDataset->GetTagValueAsString( tagImagePositionPatient ).value;

      result->FlagGantryTilt( GantryTiltInformation::MakeFromTagValues( firstOriginString, lastOriginString, orientationString, numberOfSlicesApart ));
    }
    catch (...)
    {
      // just do not flag anything, we are ok
    }
  }

  return result;
}
