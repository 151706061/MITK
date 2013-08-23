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


#ifndef MITKVECTOR_H_HEADER_INCLUDED_C1EBD0AD
#define MITKVECTOR_H_HEADER_INCLUDED_C1EBD0AD

// this is needed for memcopy in ITK
// can be removed when fixed in ITK
#include <cstring>

#include <itkPoint.h>
#include <float.h>
#include <itkIndex.h>
#include <itkContinuousIndex.h>
#include <itkVector.h>
#include <itkMatrix.h>
#include <itkTransform.h>
#include <vnl/vnl_quaternion.h>
#include <MitkExports.h>

#include "mitkTypeBasics.h"
#include "mitkTypedefs.h"
#include "mitkPoint.h" // TODO SW: should not be included here, maybe generate one "general datatype include" like mitkPrimitives.h
#include "mitkVector.h"
#include "mitkMatrix.h"

#include "mitkOldTypeConversions.h"


#ifndef DOXYGEN_SKIP

namespace mitk {


template <class T, unsigned int NVectorDimension>
  itk::Vector<T, NVectorDimension> operator+(const itk::Vector<T, NVectorDimension> &vector, const itk::Point<T, NVectorDimension> &point)
{
  itk::Vector<T, NVectorDimension> sub;
  for( unsigned int i=0; i<NVectorDimension; i++)
  {
    sub[i] = vector[i]+point[i];
  }
  return sub;
}

template <class T, unsigned int NVectorDimension>
  inline itk::Vector<T, NVectorDimension>& operator+=(itk::Vector<T, NVectorDimension> &vector, const itk::Point<T, NVectorDimension> &point)
{
  for( unsigned int i=0; i<NVectorDimension; i++)
  {
    vector[i] += point[i];
  }
  return vector;
}

template <class T, unsigned int NVectorDimension>
  itk::Vector<T, NVectorDimension> operator-(const itk::Vector<T, NVectorDimension> &vector, const itk::Point<T, NVectorDimension> &point)
{
  itk::Vector<T, NVectorDimension> sub;
  for( unsigned int i=0; i<NVectorDimension; i++)
  {
    sub[i] = vector[i]-point[i];
  }
  return sub;
}

template <class T, unsigned int NVectorDimension>
  inline itk::Vector<T, NVectorDimension>& operator-=(itk::Vector<T, NVectorDimension> &vector, const itk::Point<T, NVectorDimension> &point)
{
  for( unsigned int i=0; i<NVectorDimension; i++)
  {
    vector[i] -= point[i];
  }
  return vector;
}

/*!
\brief Check for matrix equality with a user defined accuracy. As an equality metric the root mean squared error (RMS) of all elements is calculated.
\param matrix1 first vnl matrix
\param matrix2 second vnl matrix
\param epsilon user defined accuracy bounds
*/
template <typename TCoordRep, unsigned int NRows, unsigned int NCols>
inline bool MatrixEqualRMS(const vnl_matrix_fixed<TCoordRep,NRows,NCols>& matrix1,const vnl_matrix_fixed<TCoordRep,NRows,NCols>& matrix2,mitk::ScalarType epsilon=mitk::eps)
{
  if ( (matrix1.rows() == matrix2.rows()) && (matrix1.cols() == matrix2.cols()) )
  {
    vnl_matrix_fixed<TCoordRep,NRows,NCols> differenceMatrix = matrix1-matrix2;
    if (differenceMatrix.rms()<epsilon)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

/*!
\brief Check for matrix equality with a user defined accuracy. As an equality metric the root mean squared error (RMS) of all elements is calculated.
\param matrix1 first itk matrix
\param matrix2 second itk matrix
\param epsilon user defined accuracy bounds
*/
template <typename TCoordRep, unsigned int NRows, unsigned int NCols>
inline bool MatrixEqualRMS(const itk::Matrix<TCoordRep, NRows, NCols>& matrix1,const itk::Matrix<TCoordRep, NRows, NCols>& matrix2,mitk::ScalarType epsilon=mitk::eps)
{
  return mitk::MatrixEqualRMS(matrix1.GetVnlMatrix(),matrix2.GetVnlMatrix(),epsilon);
}

/*!
\brief Check for element-wise matrix equality with a user defined accuracy.
\param matrix1 first vnl matrix
\param matrix2 second vnl matrix
\param epsilon user defined accuracy bounds
*/
template <typename TCoordRep, unsigned int NRows, unsigned int NCols>
inline bool MatrixEqualElementWise(const vnl_matrix_fixed<TCoordRep,NRows,NCols>& matrix1,const vnl_matrix_fixed<TCoordRep,NRows,NCols>& matrix2,mitk::ScalarType epsilon=mitk::eps)
{
  if ( (matrix1.rows() == matrix2.rows()) && (matrix1.cols() == matrix2.cols()) )
  {
    for( unsigned int r=0; r<NRows; r++)
    {
      for( unsigned int c=0; c<NCols; c++ )
      {
        TCoordRep difference =  fabs(matrix1(r,c)-matrix2(r,c));
        if (difference>epsilon)
        {
          return false;
        }
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

/*!
\brief Check for element-wise matrix equality with a user defined accuracy.
\param matrix1 first itk matrix
\param matrix2 second itk matrix
\param epsilon user defined accuracy bounds
*/
template <typename TCoordRep, unsigned int NRows, unsigned int NCols>
inline bool MatrixEqualElementWise(const itk::Matrix<TCoordRep, NRows, NCols>& matrix1,const itk::Matrix<TCoordRep, NRows, NCols>& matrix2,mitk::ScalarType epsilon=mitk::eps)
{
  return mitk::MatrixEqualElementWise(matrix1.GetVnlMatrix(),matrix2.GetVnlMatrix(),epsilon);
}

template <typename TCoordRep, unsigned int NPointDimension>
inline bool Equal(const itk::Vector<TCoordRep, NPointDimension>& vector1, const itk::Vector<TCoordRep, NPointDimension>& vector2, TCoordRep eps=mitk::eps)
{
  typename itk::Vector<TCoordRep, NPointDimension>::VectorType diff = vector1-vector2;
  for (unsigned int i=0; i<NPointDimension; i++)
      if (diff[i]>eps || diff[i]<-eps)
      return false;
  return true;
}

template <typename TCoordRep, unsigned int NPointDimension>
  inline bool Equal(const itk::Point<TCoordRep, NPointDimension>& vector1, const itk::Point<TCoordRep, NPointDimension>& vector2, TCoordRep eps=mitk::eps)
{
  typename itk::Point<TCoordRep, NPointDimension>::VectorType diff = vector1-vector2;
  for (unsigned int i=0; i<NPointDimension; i++)
    if (diff[i]>eps || diff[i]<-eps)
      return false;
  return true;
}

inline bool Equal(const mitk::VnlVector& vector1, const mitk::VnlVector& vector2, ScalarType eps=mitk::eps)
{
  mitk::VnlVector diff = vector1-vector2;
  for (unsigned int i=0; i<diff.size(); i++)
    if (diff[i]>eps || diff[i]<-eps)
      return false;
  return true;
}

inline bool Equal(ScalarType scalar1, ScalarType scalar2, ScalarType eps=mitk::eps)
{
  return fabs(scalar1-scalar2) < eps;
}

template <typename TCoordRep, unsigned int NPointDimension>
  inline bool Equal(const vnl_vector_fixed<TCoordRep, NPointDimension> & vector1, const vnl_vector_fixed<TCoordRep, NPointDimension>& vector2, TCoordRep eps=mitk::eps)
{
  vnl_vector_fixed<TCoordRep, NPointDimension> diff = vector1-vector2;
  bool returnValue = true;
  for( unsigned int i=0; i<diff.size(); i++)
  {
    if(diff[i]>eps || diff[i]<-eps)
    {
     returnValue = false;
    }
  }

  return returnValue;
}



} // namespace mitk

#endif //DOXYGEN_SKIP

/*
 * This part of the code has been shifted here to avoid compiler clashes
 * caused by including <itkAffineGeometryFrame.h> before the declaration of
 * the Equal() methods above. This problem occurs when using MSVC and is
 * probably related to a compiler bug.
 */

#include <itkAffineGeometryFrame.h>

namespace mitk
{
  typedef itk::AffineGeometryFrame<ScalarType, 3>::TransformType AffineTransform3D;
}


#define mitkSetConstReferenceMacro(name,type) \
  virtual void Set##name (const type & _arg) \
  { \
    itkDebugMacro("setting " << #name " to " << _arg ); \
    if (this->m_##name != _arg) \
      { \
      this->m_##name = _arg; \
      this->Modified(); \
      } \
  }

#define mitkSetVectorMacro(name,type) \
  mitkSetConstReferenceMacro(name,type)

#define mitkGetVectorMacro(name,type) \
  itkGetConstReferenceMacro(name,type)

#endif /* MITKVECTOR_H_HEADER_INCLUDED_C1EBD0AD */
