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


#include "mitkSlicedData.h"
#include "mitkBaseProcess.h"

//##ModelId=3E141028018A
void mitk::SlicedData::UpdateOutputInformation()
{
  if (this->GetSource())
  {
    this->GetSource()->UpdateOutputInformation();
  }
  // If we don't have a source, then let's make our Image
  // span our buffer
  else
  {
    m_UseLargestPossibleRegion = true;
  }

  // Now we should know what our largest possible region is. If our 
  // requested region was not set yet, (or has been set to something 
  // invalid - with no data in it ) then set it to the largest possible
  // region.
  if ( ! m_RequestedRegionInitialized)
  {
    this->SetRequestedRegionToLargestPossibleRegion();
    m_RequestedRegionInitialized = true;
  }

  m_LastRequestedRegionWasOutsideOfTheBufferedRegion = 0;
}

//##ModelId=3E14102C029E
void mitk::SlicedData::SetRequestedRegionToLargestPossibleRegion()
{
  m_UseLargestPossibleRegion = true;
  if(GetGeometry()==NULL) 
    return;
  unsigned int i;
  const RegionType::IndexType & index = GetLargestPossibleRegion().GetIndex();
  const RegionType::SizeType & size = GetLargestPossibleRegion().GetSize();
  for(i=0;i<VmitkImageDimension;++i)
  {
    m_RequestedRegion.SetIndex(i, index[i]);
    m_RequestedRegion.SetSize(i, size[i]);
  }
}

//##ModelId=3E14104300AC
bool mitk::SlicedData::RequestedRegionIsOutsideOfTheBufferedRegion()
{
  if(VerifyRequestedRegion()==false) return true;

  // Is the requested region within the currently buffered data?
  // SlicedData and subclasses store entire volumes or slices. The
  // methods IsVolumeSet() and IsSliceSet are provided to check, 
  // a volume or slice, respectively, is available. Thus, these
  // methods used here.
  const IndexType &requestedRegionIndex = m_RequestedRegion.GetIndex();

  const SizeType& requestedRegionSize = m_RequestedRegion.GetSize();
  const SizeType& largestPossibleRegionSize
    = GetLargestPossibleRegion().GetSize();

  // are whole channels requested?
  int c, cEnd;
  c=requestedRegionIndex[4];
  cEnd=c+static_cast<long>(requestedRegionSize[4]);
  if(requestedRegionSize[3] == largestPossibleRegionSize[3])
  {
    for (; c< cEnd; ++c)
      if(IsChannelSet(c)==false) return true;
    return false;
  }

  // are whole volumes requested?
  int t, tEnd;
  t=requestedRegionIndex[3];
  tEnd=t+static_cast<long>(requestedRegionSize[3]);
  if(requestedRegionSize[2] == largestPossibleRegionSize[2])
  {
    for (; c< cEnd; ++c)
      for (; t< tEnd; ++t)
        if(IsVolumeSet(t, c)==false) return true;
    return false;
  }

  // ok, only slices are requested. Check if they are available.
  int s, sEnd;
  s=requestedRegionIndex[2];
  sEnd=s+static_cast<long>(requestedRegionSize[2]);
  for (; c< cEnd; ++c)
    for (; t< tEnd; ++t)
      for (; s< sEnd; ++s)
        if(IsSliceSet(s, t, c)==false) return true;

  return false;
}

//##ModelId=3E14105B00F7
bool mitk::SlicedData::VerifyRequestedRegion()
{
  if(m_Geometry3D.IsNull()) return false;

  unsigned int i;

  // Is the requested region within the LargestPossibleRegion?
  // Note that the test is indeed against the largest possible region
  // rather than the buffered region; see DataObject::VerifyRequestedRegion.
  const IndexType &requestedRegionIndex = m_RequestedRegion.GetIndex();
  const IndexType &largestPossibleRegionIndex
    = GetLargestPossibleRegion().GetIndex();

  const SizeType& requestedRegionSize = m_RequestedRegion.GetSize();
  const SizeType& largestPossibleRegionSize
    = GetLargestPossibleRegion().GetSize();

  for (i=0; i< VmitkImageDimension; ++i)
  {
    if ( (requestedRegionIndex[i] < largestPossibleRegionIndex[i]) ||
      ((requestedRegionIndex[i] + static_cast<long>(requestedRegionSize[i]))
    > (largestPossibleRegionIndex[i]+static_cast<long>(largestPossibleRegionSize[i]))))
    {
      return false;
    }
  }

  return true;
}

//##ModelId=3E1410760114
void mitk::SlicedData::SetRequestedRegion(itk::DataObject *data)
{
  m_UseLargestPossibleRegion=false;

  mitk::SlicedData *slicedData;

  slicedData = dynamic_cast<mitk::SlicedData*>(data);

  if (slicedData)
  {
    m_RequestedRegion = slicedData->GetRequestedRegion();
    m_RequestedRegionInitialized = true;
  }
  else
  {
    // pointer could not be cast back down
    itkExceptionMacro( << "mitk::SlicedData::SetRequestedRegion(DataObject*) cannot cast " << typeid(data).name() << " to " << typeid(SlicedData*).name() );
  }
}

void mitk::SlicedData::SetRequestedRegion(SlicedData::RegionType *region)
{
  m_UseLargestPossibleRegion=false;

  if(region!=NULL)
  {
    m_RequestedRegion = *region;
    m_RequestedRegionInitialized = true;
  }
  else
  {
    // pointer could not be cast back down
    itkExceptionMacro( << "mitk::SlicedData::SetRequestedRegion(SlicedData::RegionType*) cannot cast " << typeid(region).name() << " to " << typeid(SlicedData*).name() );
  }
}


//##ModelId=3E19EA3300BA
mitk::SlicedData::SlicedData() : m_UseLargestPossibleRegion(false), m_TimeSlicedGeometry(NULL)
{
  unsigned int i;
  for(i=0;i<4;++i)
  {
    m_LargestPossibleRegion.SetIndex(i, 0);
    m_LargestPossibleRegion.SetSize (i, 1);
  }
}


//##ModelId=3E19EA3300CE
mitk::SlicedData::~SlicedData()
{
}

//##ModelId=3E34513B016D
void mitk::SlicedData::CopyInformation(const itk::DataObject *data)
{
  const mitk::SlicedData *slicedData;

  slicedData = dynamic_cast<const mitk::SlicedData*>(data);

  if (slicedData)
  {
    SetGeometry(dynamic_cast<Geometry3D*>(GetGeometry()->Clone().GetPointer()));//new SlicedGeometry3D(*slicedData->GetGeometry().GetPointer());
  }
  else
  {
    // pointer could not be cast back down
    itkExceptionMacro( << "mitk::SlicedData::CopyInformation(const DataObject *data) cannot cast " << typeid(data).name() << " to " << typeid(SlicedData*).name() );
  }
}

//const mitk::Geometry2D* mitk::SlicedData::GetGeometry2D(int s, int t) const
//{
//  const_cast<SlicedData*>(this)->SetRequestedRegionToLargestPossibleRegion();
//
//  const_cast<SlicedData*>(this)->UpdateOutputInformation();
//
//  return GetSlicedGeometry(t)->GetGeometry2D(s);
//}
//
mitk::SlicedGeometry3D* mitk::SlicedData::GetSlicedGeometry(unsigned int t) const
{
  if(m_TimeSlicedGeometry == NULL)
    return NULL;
  return dynamic_cast<SlicedGeometry3D*>(m_TimeSlicedGeometry->GetGeometry3D(t));
}

const mitk::SlicedGeometry3D* mitk::SlicedData::GetUpdatedSlicedGeometry(unsigned int t)
{
  SetRequestedRegionToLargestPossibleRegion();

  UpdateOutputInformation();

  return GetSlicedGeometry(t);
}

void mitk::SlicedData::SetGeometry(Geometry3D* aGeometry3D)
{
  if(aGeometry3D!=NULL)
  {
    m_TimeSlicedGeometry = dynamic_cast<TimeSlicedGeometry*>(aGeometry3D);
    if(m_TimeSlicedGeometry==NULL)
    {
      SlicedGeometry3D::Pointer slicedGeometry = dynamic_cast<SlicedGeometry3D*>(aGeometry3D);
      if(slicedGeometry.IsNull())
      {
        Geometry2D* geometry2d = dynamic_cast<Geometry2D*>(aGeometry3D);
        assert(geometry2d!=NULL);
        slicedGeometry = SlicedGeometry3D::New();
        slicedGeometry->InitializeEvenlySpaced(geometry2d, 1);
      }
      assert(slicedGeometry.IsNotNull());

      TimeSlicedGeometry::Pointer timeSlicedGeometry = TimeSlicedGeometry::New();
      m_Geometry3D = m_TimeSlicedGeometry = timeSlicedGeometry;   
      m_TimeSlicedGeometry->InitializeEvenlyTimed(slicedGeometry, 1);
    }
    Superclass::SetGeometry(m_TimeSlicedGeometry);
  }
  else
    Superclass::SetGeometry(NULL);
}
