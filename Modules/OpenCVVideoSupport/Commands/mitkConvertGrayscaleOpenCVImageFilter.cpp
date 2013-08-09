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

#include "mitkConvertGrayscaleOpenCVImageFilter.h"

#include "cv.h"

namespace mitk {

bool ConvertGrayscaleOpenCVImageFilter::FilterImage( cv::Mat& image )
{
  cv::Mat buffer;

  cv::cvtColor(image, buffer, CV_RGB2GRAY, 1);
  image.release();
  image = buffer;

  return true;
}

} // namespace mitk
