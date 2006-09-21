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


#ifndef MITKGEOMETRYCLIPIMAGEFILTER_H_HEADER_INCLUDED_C1F48A22
#define MITKGEOMETRYCLIPIMAGEFILTER_H_HEADER_INCLUDED_C1F48A22

#include "mitkCommon.h"
#include "mitkImageToImageFilter.h"
#include "mitkImageTimeSelector.h"
#include "mitkGeometryData.h"

namespace itk {
template <class TPixel, unsigned int VImageDimension> class ITK_EXPORT Image;
}

namespace mitk {

//##Documentation
//## @brief Filter for clipping an image with a Geometry2D
//##
//## The given geometry for clipping can be either a Geometry2D
//## or a TimeSlicedGeometry containing multiple instances
//## of Geometry2D
//## @ingroup Process
class GeometryClipImageFilter : public ImageToImageFilter
{
public:
  mitkClassMacro(GeometryClipImageFilter, ImageToImageFilter);

  itkNewMacro(Self);

  //##Description 
  //## Set the geometry to be used for clipping 
  //##
  //## The given geometry for clipping can be either a Geometry2D
  //## or a TimeSlicedGeometry containing multiple instances
  //## of Geometry2D
  void SetClippingGeometry(const mitk::Geometry3D* aClippingGeometry);
  const mitk::Geometry3D* GetClippingGeometry() const;

  //##Description 
  //## @brief Get whether the part above or below the geometry 
  //## shall be clipped (default: @a true)
  itkGetConstMacro(ClipPartAboveGeometry, bool);
  //## @brief Set whether the part above or below the geometry 
  //## shall be clipped (default: @a true)
  itkSetMacro(ClipPartAboveGeometry, bool);
  //## @brief Set whether the part above or below the geometry 
  //## shall be clipped (default: @a true)
  itkBooleanMacro(ClipPartAboveGeometry);

  //##Description 
  //## @brief Set value for outside pixels (default: 0), 
  //## used when m_AutoOutsideValue is \a false
  itkSetMacro(OutsideValue, ScalarType);
  itkGetMacro(OutsideValue, ScalarType);

  //##Description 
  //## @brief If set to \a true the minimum of the ouput pixel type is
  //## used as outside value (default: \a false)
  itkSetMacro(AutoOutsideValue, bool);
  itkGetMacro(AutoOutsideValue, bool);
  itkBooleanMacro(AutoOutsideValue);
protected:
  GeometryClipImageFilter();

  ~GeometryClipImageFilter();

  virtual void GenerateInputRequestedRegion();

  virtual void GenerateOutputInformation();

  virtual void GenerateData();

  template < typename TPixel, unsigned int VImageDimension >
    friend void _InternalComputeClippedImage(itk::Image<TPixel, VImageDimension>* itkImage, mitk::GeometryClipImageFilter* geometryClipper, const mitk::Geometry2D* clippingGeometry2D);

  mitk::Geometry3D::ConstPointer m_ClippingGeometry;
  mitk::GeometryData::Pointer m_ClippingGeometryData;
  mitk::TimeSlicedGeometry::ConstPointer m_TimeSlicedClippingGeometry;
  mitk::ImageTimeSelector::Pointer m_InputTimeSelector;
  mitk::ImageTimeSelector::Pointer m_OutputTimeSelector;

  //##Description 
  //## @brief Defines whether the part above or below the geometry 
  //## shall be clipped (default: @a true)
  bool m_ClipPartAboveGeometry;

  //##Description 
  //## @brief Value for outside pixels (default: 0)
  //##
  //## Used only if m_AutoOutsideValue is \a false.
  ScalarType m_OutsideValue;
  //##Description 
  //## @brief If \a true the minimum of the ouput pixel type is
  //## used as outside value (default: \a false)
  bool m_AutoOutsideValue;

  //##Description 
  //## @brief Time when Header was last initialized
  itk::TimeStamp m_TimeOfHeaderInitialization;
};

} // namespace mitk

#endif /* MITKGEOMETRYCLIPIMAGEFILTER_H_HEADER_INCLUDED_C1F48A22 */

