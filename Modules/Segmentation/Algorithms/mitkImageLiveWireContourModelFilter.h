/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkImageLiveWireContourModelFilter_h
#define mitkImageLiveWireContourModelFilter_h

#include "mitkCommon.h"
#include "mitkContourModel.h"
#include "mitkContourModelSource.h"
#include <MitkSegmentationExports.h>

#include <mitkImage.h>
#include <mitkImageAccessByItk.h>
#include <mitkImageCast.h>

#include <itkShortestPathCostFunctionLiveWire.h>
#include <itkShortestPathImageFilter.h>

namespace mitk
{
  /**

   \brief Calculates a LiveWire contour between two points in an image.

   For defining costs between two pixels specific features are extracted from the image and transformed into a single cost
   value.
   \sa ShortestPathCostFunctionLiveWire

   The filter is able to create dynamic cost transfer map and thus use on the fly training.
   \note On the fly training will only be used for next update.
   The computation uses the last calculated segment to map cost according to features in the area of the segment.

   Caution: time support currently not available. Filter will always work on the first
   timestep in its current implementation.

   \ingroup ContourModelFilters
   \ingroup Process
  */
  class MITKSEGMENTATION_EXPORT ImageLiveWireContourModelFilter : public ContourModelSource
  {
  public:
    mitkClassMacro(ImageLiveWireContourModelFilter, ContourModelSource);
    itkFactorylessNewMacro(Self);
    itkCloneMacro(Self);

      typedef ContourModel OutputType;
    typedef OutputType::Pointer OutputTypePointer;
    typedef mitk::Image InputType;

    typedef itk::Image<float, 2> InternalImageType;
    typedef itk::ShortestPathImageFilter<InternalImageType, InternalImageType> ShortestPathImageFilterType;
    typedef itk::ShortestPathCostFunctionLiveWire<InternalImageType> CostFunctionType;
    typedef std::vector<itk::Index<2>> ShortestPathType;

    /** \brief start point in world coordinates*/
    itkSetMacro(StartPoint, mitk::Point3D);
    itkGetMacro(StartPoint, mitk::Point3D);

    /** \brief end point in woorld coordinates*/
    itkSetMacro(EndPoint, mitk::Point3D);
    itkGetMacro(EndPoint, mitk::Point3D);

    /** \brief Create dynamic cost transfer map - use on the fly training.
    \note On the fly training will be used for next update only.
    The computation uses the last calculated segment to map cost according to features in the area of the segment.
    */
    itkSetMacro(UseDynamicCostMap, bool);
    itkGetMacro(UseDynamicCostMap, bool);

    /** \brief Clear all repulsive points used in the cost function
    */
    void ClearRepulsivePoints();

    /** \brief Set a vector with repulsive points to use in the cost function
    */
    void SetRepulsivePoints(const ShortestPathType &points);

    /** \brief Add a single repulsive point to the cost function
    */
    void AddRepulsivePoint(const itk::Index<2> &idx);

    /** \brief Remove a single repulsive point from the cost function
    */
    void RemoveRepulsivePoint(const itk::Index<2> &idx);

    virtual void SetInput(const InputType *input);

    using Superclass::SetInput;
    virtual void SetInput(unsigned int idx, const InputType *input);

    const InputType *GetInput(void);

    const InputType *GetInput(unsigned int idx);

    virtual OutputType *GetOutput();

    virtual void DumpMaskImage();

    /** \brief Create dynamic cost transfer map - on the fly training*/
    bool CreateDynamicCostMap(mitk::ContourModel *path = nullptr);

    void SetUseCostFunction(bool doUseCostFunction) { m_ShortestPathFilter->SetUseCostFunction(doUseCostFunction); };

  protected:
    ImageLiveWireContourModelFilter();

    ~ImageLiveWireContourModelFilter() override;

    void GenerateOutputInformation() override{};

    void GenerateData() override;

    void UpdateLiveWire();

    /** \brief start point in worldcoordinates*/
    mitk::Point3D m_StartPoint;

    /** \brief end point in woorldcoordinates*/
    mitk::Point3D m_EndPoint;

    /** \brief Start point in index*/
    mitk::Point3D m_StartPointInIndex;

    /** \brief End point in index*/
    mitk::Point3D m_EndPointInIndex;

    /** \brief The cost function to compute costs between two pixels*/
    CostFunctionType::Pointer m_CostFunction;

    /** \brief Shortest path filter according to cost function m_CostFunction*/
    ShortestPathImageFilterType::Pointer m_ShortestPathFilter;

    /** \brief Flag to use a dynamic cost map or not*/
    bool m_UseDynamicCostMap;

    unsigned int m_TimeStep;

    template <typename TPixel, unsigned int VImageDimension>
    void ItkPreProcessImage(const itk::Image<TPixel, VImageDimension> *inputImage);

    template <typename TPixel, unsigned int VImageDimension>
    void CreateDynamicCostMapByITK(const itk::Image<TPixel, VImageDimension> *inputImage,
                                   mitk::ContourModel *path = nullptr);

    InternalImageType::Pointer m_InternalImage;
  };
}

#endif
