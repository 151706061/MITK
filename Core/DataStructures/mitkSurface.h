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


#ifndef MITKSURFACEDATA_H_HEADER_INCLUDED_C19085D7
#define MITKSURFACEDATA_H_HEADER_INCLUDED_C19085D7

#include "mitkCommon.h"
#include "mitkBaseData.h"
#include "mitkTimeSlicedGeometry.h"
#include "itkImageRegion.h"
#include <vector>

class vtkPolyData;

namespace mitk {

//##ModelId=3E70F66001B9
//##Documentation
//## @brief Class for storing surfaces (vtkPolyData)
//## @ingroup Data
class Surface : public BaseData
{
protected:

public:
  // not yet the best chioce of a region-type for surfaces, but it works for the time being
  typedef itk::ImageRegion< 5 >  RegionType;  
	
  mitkClassMacro(Surface, BaseData);
  
  itkNewMacro(Self);
  
  //##ModelId=3E70F661009A
  virtual void SetVtkPolyData(vtkPolyData* polydata, unsigned int t = 0);
  
  //##ModelId=3E70F66100A5
  virtual vtkPolyData* GetVtkPolyData(unsigned int t = 0);
  
  //##ModelId=3E70F66100AE
  virtual void UpdateOutputInformation();
  
  virtual void SetRequestedRegionToLargestPossibleRegion();
  
  virtual bool RequestedRegionIsOutsideOfTheBufferedRegion();
  
  virtual bool VerifyRequestedRegion();
 
  virtual void SetRequestedRegion(itk::DataObject *data);

  virtual void SetRequestedRegion(Surface::RegionType *region);

  virtual void CopyInformation(const itk::DataObject *data);
  
  virtual void Update();
  
  virtual void Resize( unsigned int timeSteps );
  
  virtual void SetGeometry(Geometry3D* aGeometry3D);

  mitk::TimeSlicedGeometry* GetTimeSlicedGeometry()
  {
    return dynamic_cast<mitk::TimeSlicedGeometry*>( m_Geometry3D.GetPointer() );
  }

  const mitk::TimeSlicedGeometry* GetTimeSlicedGeometry() const
  {
    return dynamic_cast<mitk::TimeSlicedGeometry*>( m_Geometry3D.GetPointer() );
  }

  const RegionType& GetLargestPossibleRegion() const
  {
	  m_LargestPossibleRegion.SetIndex(3, 0);
	  m_LargestPossibleRegion.SetSize(3, GetTimeSlicedGeometry()->GetTimeSteps());
	  return m_LargestPossibleRegion;
  }

  //##Documentation
  //## Get the region object that defines the size and starting index
  //## for the region of the image requested (i.e., the region of the
  //## image to be operated on by a filter).
  virtual const RegionType& GetRequestedRegion() const
  {
    return m_RequestedRegion;
  }

  virtual void Initialize(unsigned int timeSteps=1);

  void CalculateBoundingBox();

protected:
 
  typedef std::vector< vtkPolyData* > VTKPolyDataSeries;

  //##ModelId=3E70F66100C4
  Surface();
  
  //##ModelId=3E70F66100CA
  virtual ~Surface();

  //##ModelId=3E70F6610099
  VTKPolyDataSeries m_PolyDataSeries;

  mutable RegionType m_LargestPossibleRegion;

  RegionType m_RequestedRegion;
  
  bool m_CalculateBoundingBox;
};

} // namespace mitk

#endif /* MITKSURFACEDATA_H_HEADER_INCLUDED_C19085D7 */
