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

#ifndef MITKGRABCUTOPENCVIMAGEFILTER_H
#define MITKGRABCUTOPENCVIMAGEFILTER_H

#include "mitkAbstractOpenCVImageFilter.h"

//itk headers
#include "itkObjectFactory.h"
#include "itkMutexLock.h"

#include "cv.h"

namespace itk {
template<unsigned int T> class Index;
template<class T> class SmartPointer;
class MultiThreader;
class ConditionVariable;
class FastMutexLock;
}

namespace mitk {

class PointSet;

class MITK_OPENCVVIDEOSUPPORT_EXPORT GrabCutOpenCVImageFilter : public AbstractOpenCVImageFilter
{
public:
  typedef std::vector<itk::Index<2> > ModelPointsList;

  mitkClassMacro(GrabCutOpenCVImageFilter, AbstractOpenCVImageFilter)
  itkNewMacro(Self)

  GrabCutOpenCVImageFilter();
  virtual ~GrabCutOpenCVImageFilter();

  bool FilterImage( cv::Mat& image );

  void SetModelPoints(ModelPointsList foregroundPoints);
  void SetModelPoints(ModelPointsList foregroundPoints, ModelPointsList backgroundPoints);

  const cv::Mat& GetResultMask();

protected:
  bool UpdateResultMask();
  void UpdateMaskFromPointSets();
  void UpdateMaskWithPointSet(std::vector<itk::Index<2> > pointSet, int pixelValue);

  cv::Mat GetMaskFromPointSets();
  cv::Mat RunSegmentation(cv::Mat input, cv::Mat mask);

  bool                         m_PointSetsChanged;
  bool                         m_InputImageChanged;

  ModelPointsList              m_ForegroundPoints;
  ModelPointsList              m_BackgroundPoints;

  cv::Mat                      m_InputImage;
  cv::Mat                      m_GrabCutMask;
  cv::Mat                      m_ResultMask;

  unsigned int                 m_ProcessEveryNumImage;
  unsigned int                 m_CurrentProcessImageNum;

  unsigned int                 m_ResultCount;

private:
  static ITK_THREAD_RETURN_TYPE SegmentationWorker(void* pInfoStruct);

  int                                       m_ThreadId;

  itk::SmartPointer<itk::MultiThreader>     m_MultiThreader;
  itk::SmartPointer<itk::ConditionVariable> m_WorkerBarrier;
  itk::SimpleMutexLock                      m_WorkerMutex;
  itk::SmartPointer<itk::FastMutexLock>     m_ImageMutex;
  itk::SmartPointer<itk::FastMutexLock>     m_ResultMutex;
  itk::SmartPointer<itk::FastMutexLock>     m_PointSetsMutex;
};

} // namespace mitk

#endif // MITKGRABCUTOPENCVIMAGEFILTER_H
