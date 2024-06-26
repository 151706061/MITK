/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkExtractImageFilter_h
#define mitkExtractImageFilter_h

#include "mitkCommon.h"
#include "mitkImageToImageFilter.h"
#include <MitkImageExtractionExports.h>

#include "itkImage.h"

namespace mitk
{
  /**
    \deprecated This class is deprecated. Use mitk::ExtractSliceFilter instead.
    \sa ExtractSliceFilter

    \brief Extracts a 2D slice from a 3D image.

    \sa SegTool2D

    \ingroup Process
    \ingroup ToolManagerEtAl

    This class takes a 3D mitk::Image as input and tries to extract one slice from it.

    Two parameters determine which slice is extracted: the "slice dimension" is that one, which is constant for all
    points in the plane, e.g. axial would mean 2.
    The "slice index" is the slice index in the image direction you specified with "affected dimension". Indices count
    from zero.

    Output will not be set if there was a problem extracting the desired slice.

    Last contributor: $Author$
  */
  class MITKIMAGEEXTRACTION_EXPORT ExtractImageFilter : public ImageToImageFilter
  {
  public:
    mitkClassMacro(ExtractImageFilter, ImageToImageFilter);
    itkFactorylessNewMacro(Self);
    itkCloneMacro(Self);

      /**
        \brief Which slice to extract (first one has index 0).
      */
      itkSetMacro(SliceIndex, unsigned int);
    itkGetConstMacro(SliceIndex, unsigned int);

    /**
      \brief The orientation of the slice to be extracted.

      \a Parameter SliceDimension Number of the dimension which is constant for all pixels of the desired slice (e.g. 2
      for axial)
    */
    itkSetMacro(SliceDimension, unsigned int);
    itkGetConstMacro(SliceDimension, unsigned int);

    /**
      \brief Time step of the image to be extracted.
     */
    itkSetMacro(TimeStep, unsigned int);
    itkGetConstMacro(TimeStep, unsigned int);

    typedef enum DirectionCollapseStrategyEnum {
      DIRECTIONCOLLAPSETOUNKOWN = 0,
      DIRECTIONCOLLAPSETOIDENTITY = 1,
      DIRECTIONCOLLAPSETOSUBMATRIX = 2,
      DIRECTIONCOLLAPSETOGUESS = 3
    } DIRECTIONCOLLAPSESTRATEGY;

    /**
      \brief Collapse strategy to be used.
     */
    itkSetMacro(DirectionCollapseToStrategy, DIRECTIONCOLLAPSESTRATEGY);
    itkGetConstMacro(DirectionCollapseToStrategy, DIRECTIONCOLLAPSESTRATEGY);

  protected:
    ExtractImageFilter(); // purposely hidden
    ~ExtractImageFilter() override;

    void GenerateOutputInformation() override;
    void GenerateInputRequestedRegion() override;

    void GenerateData() override;

    template <typename TPixel, unsigned int VImageDimension>
    void ItkImageProcessing(const itk::Image<TPixel, VImageDimension> *image);

    unsigned int m_SliceIndex;
    unsigned int m_SliceDimension;
    unsigned int m_TimeStep;
    DIRECTIONCOLLAPSESTRATEGY m_DirectionCollapseToStrategy;
  };

} // namespace

#endif
