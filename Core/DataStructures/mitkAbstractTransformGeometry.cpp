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


#include "mitkAbstractTransformGeometry.h"
#include <vtkAbstractTransform.h>

//##ModelId=3EF4A266029C
mitk::AbstractTransformGeometry::AbstractTransformGeometry() : m_Plane(NULL)
{
  Initialize();
}

//##ModelId=3EF4A266029D
mitk::AbstractTransformGeometry::~AbstractTransformGeometry()
{
}

void mitk::AbstractTransformGeometry::Initialize()
{
  Superclass::Initialize();

  m_ItkVtkAbstractTransform = itk::VtkAbstractTransform<ScalarType>::New();
  m_ParametricTransform = m_ItkVtkAbstractTransform;
}

//##ModelId=3EF4A2660237
vtkAbstractTransform* mitk::AbstractTransformGeometry::GetVtkAbstractTransform() const
{
  return m_ItkVtkAbstractTransform->GetVtkAbstractTransform();
}

mitk::ScalarType mitk::AbstractTransformGeometry::GetParametricExtentInMM(int direction) const
{
  if(m_Plane.IsNull())
  {
    itkExceptionMacro(<<"m_Plane is NULL.");
  }
  return m_Plane->GetExtentInMM(direction);
}

bool mitk::AbstractTransformGeometry::Project(const mitk::Point3D &pt3d_mm, mitk::Point3D &projectedPt3d_mm) const
{
  assert(m_BoundingBox.IsNotNull());

  mitk::Point2D pt2d_mm;
  bool isInside;
  isInside = Map(pt3d_mm, pt2d_mm);
  Map(pt2d_mm, projectedPt3d_mm);
  return isInside;
  //Point3D pt3d_units;
  //pt3d_units = m_ItkVtkAbstractTransform->BackTransform(pt3d_mm);
  //pt3d_units[2] = 0;
  //projectedPt3d_mm = m_ItkVtkAbstractTransform->TransformPoint(pt3d_units);
  //return const_cast<BoundingBox*>(m_BoundingBox.GetPointer())->IsInside(pt3d_units);
}

//##ModelId=3EF4A2660256
bool mitk::AbstractTransformGeometry::Map(const mitk::Point3D &pt3d_mm, mitk::Point2D &pt2d_mm) const
{
  assert((m_ItkVtkAbstractTransform.IsNotNull()) && (m_Plane.IsNotNull()));

  Point3D pt3d_units;
  pt3d_units = m_ItkVtkAbstractTransform->BackTransform(pt3d_mm);

  return m_Plane->Map(pt3d_units, pt2d_mm);
}

//##ModelId=3EF4A266025F
void mitk::AbstractTransformGeometry::Map(const mitk::Point2D &pt2d_mm, mitk::Point3D &pt3d_mm) const
{
  assert((m_ParametricTransform.IsNotNull()) && (m_Plane.IsNotNull()));

  m_Plane->Map(pt2d_mm, pt3d_mm);
  pt3d_mm = m_ParametricTransform->TransformPoint(pt3d_mm);
}

bool mitk::AbstractTransformGeometry::Project(const mitk::Point3D & atPt3d_mm, const mitk::Vector3D &vec3d_mm, mitk::Vector3D &projectedVec3d_mm) const
{
  itkExceptionMacro("not implemented yet - replace GetIndexToWorldTransform by m_ItkVtkAbstractTransform->GetInverseVtkAbstractTransform()");
  assert(m_BoundingBox.IsNotNull());

  Vector3D vec3d_units;
  vec3d_units = GetIndexToWorldTransform()->BackTransform(vec3d_mm);
  vec3d_units[2] = 0;
  projectedVec3d_mm = GetIndexToWorldTransform()->TransformVector(vec3d_units);

  Point3D pt3d_units;
  pt3d_units = GetIndexToWorldTransform()->BackTransformPoint(atPt3d_mm);
  return const_cast<BoundingBox*>(m_BoundingBox.GetPointer())->IsInside(pt3d_units);
}

//##ModelId=3EF4A2660288
bool mitk::AbstractTransformGeometry::Map(const mitk::Point3D & atPt3d_mm, const mitk::Vector3D &vec3d_mm, mitk::Vector2D &vec2d_mm) const
{
  assert((m_ItkVtkAbstractTransform.IsNotNull()) && (m_Plane.IsNotNull()));

  float vtkpt[3], vtkvec[3];
  itk2vtk(atPt3d_mm, vtkpt);
  itk2vtk(vec3d_mm, vtkvec);
  m_ItkVtkAbstractTransform->GetInverseVtkAbstractTransform()->TransformVectorAtPoint(vtkpt, vtkvec, vtkvec);
  mitk::Vector3D vec3d_units;
  vtk2itk(vtkvec, vec3d_units);
  return m_Plane->Map(atPt3d_mm, vec3d_units, vec2d_mm);
}

//##ModelId=3EF4A2660292
void mitk::AbstractTransformGeometry::Map(const mitk::Point2D & atPt2d_mm, const mitk::Vector2D &vec2d_mm, mitk::Vector3D &vec3d_mm) const
{
  m_Plane->Map(atPt2d_mm, vec2d_mm, vec3d_mm);
  Point3D atPt3d_mm;
  Map(atPt2d_mm, atPt3d_mm);
  float vtkpt[3], vtkvec[3];
  itk2vtk(atPt3d_mm, vtkpt);
  itk2vtk(vec3d_mm, vtkvec);
  m_ItkVtkAbstractTransform->GetVtkAbstractTransform()->TransformVectorAtPoint(vtkpt, vtkvec, vtkvec);
  vtk2itk(vtkvec, vec3d_mm);
}

void mitk::AbstractTransformGeometry::IndexToWorld(const mitk::Point2D &pt_units, mitk::Point2D &pt_mm) const
{
  m_Plane->IndexToWorld(pt_units, pt_mm);
}

void mitk::AbstractTransformGeometry::WorldToIndex(const mitk::Point2D &pt_mm, mitk::Point2D &pt_units) const
{
  m_Plane->WorldToIndex(pt_mm, pt_units);
}

void mitk::AbstractTransformGeometry::IndexToWorld(const mitk::Point2D &atPt2d_units, const mitk::Vector2D &vec_units, mitk::Vector2D &vec_mm) const
{
  m_Plane->IndexToWorld(atPt2d_units, vec_units, vec_mm);
}

void mitk::AbstractTransformGeometry::WorldToIndex(const mitk::Point2D &atPt2d_mm, const mitk::Vector2D &vec_mm, mitk::Vector2D &vec_units) const
{
  m_Plane->WorldToIndex(atPt2d_mm, vec_mm, vec_units);
}

void mitk::AbstractTransformGeometry::SetVtkAbstractTransform(vtkAbstractTransform* aVtkAbstractTransform)
{
  m_ItkVtkAbstractTransform->SetVtkAbstractTransform(aVtkAbstractTransform);
}

void mitk::AbstractTransformGeometry::SetPlane(const mitk::PlaneGeometry* aPlane)
{
  if(aPlane!=NULL)
  {
    m_Plane = static_cast<mitk::PlaneGeometry*>(aPlane->Clone().GetPointer());

		BoundingBox::BoundsArrayType b=m_Plane->GetBoundingBox()->GetBounds();

		SetParametricBounds(b);

    //@warning affine-transforms and bounding-box should be set by specific sub-classes!
    SetBounds(m_Plane->GetBoundingBox()->GetBounds());
  }
  else
  {
    if(m_Plane.IsNull())
      return;
    m_Plane=NULL;
  }
  Modified();
}

unsigned long mitk::AbstractTransformGeometry::GetMTime() const
{
  if(Superclass::GetMTime()<m_ItkVtkAbstractTransform->GetMTime())
    return m_ItkVtkAbstractTransform->GetMTime();

  return Superclass::GetMTime();
}

void mitk::AbstractTransformGeometry::SetOversampling(float oversampling)
{
  if(m_Plane.IsNull())
  {
    itkExceptionMacro(<< "m_Plane is not set.");
  }

  mitk::BoundingBox::BoundsArrayType bounds = m_Plane->GetBounds();
  bounds[1]*=oversampling; bounds[3]*=oversampling; bounds[5]*=oversampling;
  SetParametricBounds(bounds);
}

mitk::AffineGeometryFrame3D::Pointer mitk::AbstractTransformGeometry::Clone() const
{
  Self::Pointer newGeometry = Self::New();
  newGeometry->Initialize();
  InitializeGeometry(newGeometry);
  return newGeometry.GetPointer();
}

void mitk::AbstractTransformGeometry::InitializeGeometry(Self * newGeometry) const
{
  Superclass::InitializeGeometry(newGeometry);
  newGeometry->SetPlane(m_Plane);
}
