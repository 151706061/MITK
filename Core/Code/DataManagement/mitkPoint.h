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


#ifndef MITKPOINT_H
#define MITKPOINT_H


#include <itkPoint.h>
#include "mitkTypeBasics.h"

namespace mitk {


//##Documentation
//##@brief enumeration of the type a point can be
enum PointSpecificationType
{
  PTUNDEFINED = 0,
  PTSTART,
  PTCORNER,
  PTEDGE,
  PTEND
};


template<class TCoordRep, unsigned int NPointDimension = 3>
class Point : public itk::Point<TCoordRep, NPointDimension>
{

public:
     /** Default constructor has nothing to do. */
  Point<TCoordRep, NPointDimension>() {}

  /** Pass-through constructors for the Array base class. */
  Point<TCoordRep, NPointDimension>(const mitk::Point<TCoordRep, NPointDimension>& r) : itk::Point<TCoordRep, NPointDimension>(r) {}
  Point<TCoordRep, NPointDimension>(const TCoordRep r[NPointDimension]):itk::Point<TCoordRep, NPointDimension>(r) {}
  Point<TCoordRep, NPointDimension>(const TCoordRep & v):itk::Point<TCoordRep, NPointDimension>(v) {}
  Point<TCoordRep, NPointDimension>(const itk::Point<TCoordRep, NPointDimension> r) : itk::Point<TCoordRep, NPointDimension>(r) {}

  /**
  * Warning: Array must have same dimension as Point
  */
  void CopyToArray(ScalarType* const array_p) const
  {
    for (int i = 0; i < this->GetPointDimension(); i++)
    {
      array_p[i] = this->GetElement(i);
    }
  }
};

typedef Point<ScalarType,2> Point2D;
typedef Point<ScalarType,3> Point3D;
typedef Point<ScalarType,4> Point4D;

typedef Point<int,2> Point2I;
typedef Point<int,3> Point3I;
typedef Point<int,4> Point4I;

} // namespace mitk


#endif /* MITKPOINT_H */
