/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "mitkThreeDnTDICOMSeriesReader.h"
#include "mitkITKDICOMSeriesReaderHelper.h"

mitk::ThreeDnTDICOMSeriesReader
::ThreeDnTDICOMSeriesReader(unsigned int decimalPlacesForOrientation)
:DICOMITKSeriesGDCMReader(decimalPlacesForOrientation)
,m_Group3DandT(m_DefaultGroup3DandT), m_OnlyCondenseSameSeries(m_DefaultOnlyCondenseSameSeries)
{
}

mitk::ThreeDnTDICOMSeriesReader
::ThreeDnTDICOMSeriesReader(const ThreeDnTDICOMSeriesReader& other )
:DICOMITKSeriesGDCMReader(other)
,m_Group3DandT(m_DefaultGroup3DandT), m_OnlyCondenseSameSeries(m_DefaultOnlyCondenseSameSeries)
{
}

mitk::ThreeDnTDICOMSeriesReader
::~ThreeDnTDICOMSeriesReader()
{
}

mitk::ThreeDnTDICOMSeriesReader&
mitk::ThreeDnTDICOMSeriesReader
::operator=(const ThreeDnTDICOMSeriesReader& other)
{
  if (this != &other)
  {
    DICOMITKSeriesGDCMReader::operator=(other);
    this->m_Group3DandT = other.m_Group3DandT;
  }
  return *this;
}

bool
mitk::ThreeDnTDICOMSeriesReader
::operator==(const DICOMFileReader& other) const
{
  if (const auto* otherSelf = dynamic_cast<const Self*>(&other))
  {
    return
       DICOMITKSeriesGDCMReader::operator==(other)
    && this->m_Group3DandT == otherSelf->m_Group3DandT;
  }
  else
  {
    return false;
  }
}

void
mitk::ThreeDnTDICOMSeriesReader
::SetGroup3DandT(bool on)
{
  m_Group3DandT = on;
}

bool
mitk::ThreeDnTDICOMSeriesReader
::GetGroup3DandT() const
{
  return m_Group3DandT;
}

/** Helper function to make the code in mitk::ThreeDnTDICOMSeriesReader
::Condense3DBlocks(SortingBlockList& resultOf3DGrouping) more readable.*/
bool BlockShouldBeCondensed(bool onlyCondenseSameSeries, unsigned int currentBlockNumberOfSlices, unsigned int otherBlockNumberOfSlices,
  const mitk::DICOMDatasetFinding& currentBlockFirstOrigin, const mitk::DICOMDatasetFinding& currentBlockLastOrigin,
  const mitk::DICOMDatasetFinding& otherBlockFirstOrigin, const mitk::DICOMDatasetFinding& otherBlockLastOrigin,
  const mitk::DICOMDatasetFinding& currentBlockSeriesInstanceUID, const mitk::DICOMDatasetFinding& otherBlockSeriesInstanceUID)
{
  if (otherBlockNumberOfSlices != currentBlockNumberOfSlices)
    return false; //don't condense blocks that have unequal slice count

  if (!otherBlockFirstOrigin.isValid || !otherBlockLastOrigin.isValid)
    return false; //don't condense blocks that have invalid origins

  if (!currentBlockFirstOrigin.isValid || !currentBlockLastOrigin.isValid)
    return false; //don't condense blocks that have invalid origins

  const bool sameSeries = otherBlockSeriesInstanceUID.isValid
    && currentBlockSeriesInstanceUID.isValid
    && otherBlockSeriesInstanceUID.value == currentBlockSeriesInstanceUID.value;

  if (onlyCondenseSameSeries && !sameSeries)
    return false; //don't condense blocks if it is only allowed to condense same series and series are not defined or not equal.

  if (otherBlockFirstOrigin.value != currentBlockFirstOrigin.value)
    return false; //don't condense blocks that have unequal first origins

  if (otherBlockLastOrigin.value != currentBlockLastOrigin.value)
    return false; //don't condense blocks that have unequal last origins

  return true;
}

mitk::DICOMITKSeriesGDCMReader::SortingBlockList
mitk::ThreeDnTDICOMSeriesReader
::Condense3DBlocks(SortingBlockList& resultOf3DGrouping)
{
  if (!m_Group3DandT)
  {
    return resultOf3DGrouping; // don't work if nobody asks us to
  }

  SortingBlockList remainingBlocks = resultOf3DGrouping;

  SortingBlockList non3DnTBlocks;
  SortingBlockList true3DnTBlocks;
  std::vector<unsigned int> true3DnTBlocksTimeStepCount;

  // we should describe our need for this tag as needed via a function
  // (however, we currently know that the superclass will always need this tag)
  const DICOMTag tagImagePositionPatient(0x0020, 0x0032);
  const DICOMTag tagSeriesInstaceUID(0x0020, 0x000e);

  while (!remainingBlocks.empty())
  {
    // new block to fill up
    const DICOMDatasetAccessingImageFrameList& firstBlock = remainingBlocks.front().first;
    DICOMDatasetAccessingImageFrameList current3DnTBlock = firstBlock;
    auto currentSplitReason = remainingBlocks.front().second;

    int current3DnTBlockNumberOfTimeSteps = 1;

    // get block characteristics of first block
    const unsigned int currentBlockNumberOfSlices = firstBlock.size();
    const auto currentBlockFirstOrigin = firstBlock.front()->GetTagValueAsString( tagImagePositionPatient );
    const auto currentBlockLastOrigin  =  firstBlock.back()->GetTagValueAsString( tagImagePositionPatient );
    const auto currentBlockSeriesInstanceUID = firstBlock.back()->GetTagValueAsString(tagSeriesInstaceUID);

    remainingBlocks.erase( remainingBlocks.begin() );

    // compare all other blocks against the first one
    for (auto otherBlockIter = remainingBlocks.begin();
         otherBlockIter != remainingBlocks.cend();
         /*++otherBlockIter*/) // <-- inside loop
    {
      // get block characteristics from first block
      const DICOMDatasetAccessingImageFrameList otherBlock = otherBlockIter->first;

      const unsigned int otherBlockNumberOfSlices = otherBlock.size();
      const auto otherBlockFirstOrigin = otherBlock.front()->GetTagValueAsString( tagImagePositionPatient );
      const auto otherBlockLastOrigin  =  otherBlock.back()->GetTagValueAsString( tagImagePositionPatient );
      const auto otherBlockSeriesInstanceUID = otherBlock.back()->GetTagValueAsString(tagSeriesInstaceUID);

      // add matching blocks to current3DnTBlock
      // keep other blocks for later
      if ( BlockShouldBeCondensed(m_OnlyCondenseSameSeries, currentBlockNumberOfSlices, otherBlockNumberOfSlices,
        currentBlockFirstOrigin, currentBlockLastOrigin,
        otherBlockFirstOrigin, otherBlockLastOrigin,
        currentBlockSeriesInstanceUID, otherBlockSeriesInstanceUID))
      { // matching block
        ++current3DnTBlockNumberOfTimeSteps;
        current3DnTBlock.insert( current3DnTBlock.end(), otherBlock.begin(), otherBlock.end() ); // append
        //also merge split reasons
        currentSplitReason = currentSplitReason->ExtendReason(otherBlockIter->second);

        // remove this block from remainingBlocks
        otherBlockIter = remainingBlocks.erase(otherBlockIter); // make sure iterator otherBlockIter is valid afterwards
      }
      else
      {
        ++otherBlockIter;
      }
    }

    // in any case, we now know all about the first block of our list ...
    // ... and we either call it 3D o 3D+t
    if (current3DnTBlockNumberOfTimeSteps > 1)
    {
      true3DnTBlocks.emplace_back(current3DnTBlock,currentSplitReason);
      true3DnTBlocksTimeStepCount.push_back(current3DnTBlockNumberOfTimeSteps);
    }
    else
    {
      non3DnTBlocks.emplace_back(current3DnTBlock, currentSplitReason);
    }
  }

  // create output for real 3D+t blocks (other outputs will be created by superclass)
  // set 3D+t flag on output block
  this->SetNumberOfOutputs( true3DnTBlocks.size() );
  unsigned int o = 0;
  for (auto blockIter = true3DnTBlocks.cbegin();
       blockIter != true3DnTBlocks.cend();
       ++o, ++blockIter)
  {
    // bad copy&paste code from DICOMITKSeriesGDCMReader, should be handled in a better way
    const auto& gdcmFrameInfoList = blockIter->first;
    assert(!gdcmFrameInfoList.empty());

    // reverse frames if necessary
    // update tilt information from absolute last sorting
    const DICOMDatasetList datasetList = ConvertToDICOMDatasetList( gdcmFrameInfoList );
    m_NormalDirectionConsistencySorter->SetInput( datasetList );
    m_NormalDirectionConsistencySorter->Sort();
    const DICOMDatasetAccessingImageFrameList sortedGdcmInfoFrameList = ConvertToDICOMDatasetAccessingImageFrameList( m_NormalDirectionConsistencySorter->GetOutput(0) );
    const GantryTiltInformation& tiltInfo = m_NormalDirectionConsistencySorter->GetTiltInformation();

    // set frame list for current block
    const DICOMImageFrameList frameList = ConvertToDICOMImageFrameList( sortedGdcmInfoFrameList );
    assert(!frameList.empty());

    DICOMImageBlockDescriptor block;
    block.SetTagCache( this->GetTagCache() ); // important: this must be before SetImageFrameList(), because SetImageFrameList will trigger reading of lots of interesting tags!
    block.SetAdditionalTagsOfInterest(GetAdditionalTagsOfInterest());
    block.SetTagLookupTableToPropertyFunctor(GetTagLookupTableToPropertyFunctor());
    block.SetImageFrameList( frameList );
    block.SetTiltInformation( tiltInfo );
    block.SetSplitReason(blockIter->second->Clone());

    if (true3DnTBlocks.size() == 1 && non3DnTBlocks.empty())
    {
      //if we have condensed everything into just on 3DnT block, we can remove the overlap reason,
      //because no real overlap is existent any more.
      block.GetSplitReason()->RemoveReason(IOVolumeSplitReason::ReasonType::OverlappingSlices);
    }

    block.SetFlag("3D+t", true);
    block.SetIntProperty("timesteps", true3DnTBlocksTimeStepCount[o]);
    MITK_DEBUG << "Found " << true3DnTBlocksTimeStepCount[o] << " timesteps";

    this->SetOutput( o, block );
  }

  return non3DnTBlocks;
}

bool
mitk::ThreeDnTDICOMSeriesReader
::LoadImages()
{
  bool success = true;

  unsigned int numberOfOutputs = this->GetNumberOfOutputs();
  for (unsigned int o = 0; o < numberOfOutputs; ++o)
  {
    const DICOMImageBlockDescriptor& block = this->InternalGetOutput(o);

    if (block.GetFlag("3D+t", false))
    {
      success &= this->LoadMitkImageForOutput(o);
    }
    else
    {
      success &= DICOMITKSeriesGDCMReader::LoadMitkImageForOutput(o); // let superclass handle non-3D+t
    }
  }

  return success;
}

bool
mitk::ThreeDnTDICOMSeriesReader
::LoadMitkImageForImageBlockDescriptor(DICOMImageBlockDescriptor& block) const
{
  PushLocale();
  const DICOMImageFrameList& frames = block.GetImageFrameList();
  const GantryTiltInformation tiltInfo = block.GetTiltInformation();
  const bool hasTilt = tiltInfo.IsRegularGantryTilt();

  const int numberOfTimesteps = block.GetNumberOfTimeSteps();

  if (numberOfTimesteps == 1)
  {
    return DICOMITKSeriesGDCMReader::LoadMitkImageForImageBlockDescriptor(block);
  }

  const int numberOfFramesPerTimestep = block.GetNumberOfFramesPerTimeStep();

  ITKDICOMSeriesReaderHelper::StringContainerList filenamesPerTimestep;
  for (int timeStep = 0; timeStep<numberOfTimesteps; ++timeStep)
  {
    // use numberOfFramesPerTimestep frames for a new item in filenamesPerTimestep
    ITKDICOMSeriesReaderHelper::StringContainer filenamesOfThisTimeStep;
    auto timeStepStart = frames.cbegin() + timeStep * numberOfFramesPerTimestep;
    auto timeStepEnd   = frames.cbegin() + (timeStep+1) * numberOfFramesPerTimestep;
    for (auto frameIter = timeStepStart;
        frameIter != timeStepEnd;
        ++frameIter)
    {
      filenamesOfThisTimeStep.push_back( (*frameIter)->Filename );
    }
    filenamesPerTimestep.push_back( filenamesOfThisTimeStep );
  }

  mitk::ITKDICOMSeriesReaderHelper helper;
  mitk::Image::Pointer mitkImage = helper.Load3DnT( filenamesPerTimestep, m_FixTiltByShearing && hasTilt, tiltInfo );

  block.SetMitkImage( mitkImage );

  PopLocale();

  return true;
}
