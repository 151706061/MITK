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


#include "mitkDisplayGeometry.h"

//##ModelId=3E3AE9FD00C5
void mitk::DisplayGeometry::Zoom(mitk::ScalarType factor, const mitk::Point2D& centerInDisplayUnits)
{
  assert(factor > 0);

  SetScaleFactor(m_ScaleFactorMMPerDisplayUnit/factor);
  SetOriginInMM(m_OriginInMM-centerInDisplayUnits.GetVectorFromOrigin()*(1-factor)*m_ScaleFactorMMPerDisplayUnit);

  Modified();
}

//##ModelId=3E3AEE700380
void mitk::DisplayGeometry::MoveBy(const mitk::Vector2D& shiftInDisplayUnits)
{
  SetOriginInMM(m_OriginInMM+shiftInDisplayUnits*m_ScaleFactorMMPerDisplayUnit);

  Modified();
}

//##ModelId=3E3AEA5101A2
void mitk::DisplayGeometry::SetWorldGeometry(const mitk::Geometry2D* aWorldGeometry)
{
  m_WorldGeometry = aWorldGeometry;

  Modified();
}

bool mitk::DisplayGeometry::Project(const mitk::Point3D &pt3d_mm, mitk::Point3D &projectedPt3d_mm) const
{
  if(m_WorldGeometry.IsNull()) return false;
  return m_WorldGeometry->Project(pt3d_mm, projectedPt3d_mm);
 }

bool mitk::DisplayGeometry::Project(const mitk::Point3D & atPt3d_mm, const mitk::Vector3D &vec3d_mm, mitk::Vector3D &projectedVec3d_mm) const
{
  if(m_WorldGeometry.IsNull()) return false;
  return m_WorldGeometry->Project(atPt3d_mm, vec3d_mm, projectedVec3d_mm);
}

//##ModelId=3E3AEB460259
bool mitk::DisplayGeometry::Map(const mitk::Point3D &pt3d_mm, mitk::Point2D &pt2d_mm) const
{
  if(m_WorldGeometry.IsNull()) return false;
  return m_WorldGeometry->Map(pt3d_mm, pt2d_mm);
}

//##ModelId=3E3AEB490380
void mitk::DisplayGeometry::Map(const mitk::Point2D &pt2d_mm, mitk::Point3D &pt3d_mm) const
{
  if(m_WorldGeometry.IsNull()) return;
  m_WorldGeometry->Map(pt2d_mm, pt3d_mm);    
}

//##ModelId=3E3AEB52008A
void mitk::DisplayGeometry::IndexToWorld(const mitk::Point2D &pt_units, mitk::Point2D &pt_mm) const
{
  if(m_WorldGeometry.IsNull()) return;
  m_WorldGeometry->IndexToWorld(pt_units, pt_mm);
}

//##ModelId=3E3AEB54017D
void mitk::DisplayGeometry::WorldToIndex(const mitk::Point2D &pt_mm, mitk::Point2D &pt_units) const
{
  if(m_WorldGeometry.IsNull()) return;
  m_WorldGeometry->WorldToIndex(pt_mm, pt_units);
}

//##ModelId=3E3B994802B6
void mitk::DisplayGeometry::IndexToWorld(const mitk::Point2D &atPt2d_units, const mitk::Vector2D &vec_units, mitk::Vector2D &vec_mm) const
{
  if(m_WorldGeometry.IsNull()) return;
  m_WorldGeometry->IndexToWorld(atPt2d_units, vec_units, vec_mm);
}

//##ModelId=3E3B994B02A6
void mitk::DisplayGeometry::WorldToIndex(const mitk::Point2D &atPt2d_mm, const mitk::Vector2D &vec_mm, mitk::Vector2D &vec_units) const
{
  if(m_WorldGeometry.IsNull()) return;
  m_WorldGeometry->WorldToIndex(atPt2d_mm, vec_mm, vec_units);
}

//##ModelId=3E48D5B40210
void mitk::DisplayGeometry::DisplayToMM(const mitk::Point2D &pt_display, mitk::Point2D &pt_mm) const
{
  pt_mm[0]=m_ScaleFactorMMPerDisplayUnit*pt_display[0]+m_OriginInMM[0];
  pt_mm[1]=m_ScaleFactorMMPerDisplayUnit*pt_display[1]+m_OriginInMM[1];
}

//##ModelId=3E48D5D7027E
void mitk::DisplayGeometry::MMToDisplay(const mitk::Point2D &pt_mm, mitk::Point2D &pt_display) const
{
  pt_display[0]=(pt_mm[0]-m_OriginInMM[0])*(1.0/m_ScaleFactorMMPerDisplayUnit);
  pt_display[1]=(pt_mm[1]-m_OriginInMM[1])*(1.0/m_ScaleFactorMMPerDisplayUnit);
}

//##ModelId=3E48E2AE03A7
void mitk::DisplayGeometry::DisplayToMM(const mitk::Vector2D &vec_display, mitk::Vector2D &vec_mm) const
{
  vec_mm=vec_display*m_ScaleFactorMMPerDisplayUnit;
}

//##ModelId=3E48E2B40374
void mitk::DisplayGeometry::MMToDisplay(const mitk::Vector2D &vec_mm, mitk::Vector2D &vec_display) const
{
  vec_display=vec_mm*(1.0/m_ScaleFactorMMPerDisplayUnit);
}

//##ModelId=3EF4364802A9
void mitk::DisplayGeometry::ULDisplayToDisplay(const mitk::Point2D &pt_ULdisplay, mitk::Point2D &pt_display) const
{
  pt_display[0]=pt_ULdisplay[0];
  pt_display[1]=GetDisplayHeight()-pt_ULdisplay[1];
}

//##ModelId=3EF43649007A
void mitk::DisplayGeometry::DisplayToULDisplay(const mitk::Point2D &pt_display, mitk::Point2D &pt_ULdisplay) const
{
  ULDisplayToDisplay(pt_display, pt_ULdisplay);
}

//##ModelId=3EF436490251
void mitk::DisplayGeometry::ULDisplayToDisplay(const mitk::Vector2D &vec_ULdisplay, mitk::Vector2D &vec_display) const
{
  vec_display[0]= vec_ULdisplay[0];
  vec_display[1]=-vec_ULdisplay[1];
}

//##ModelId=3EF4364A0049
void mitk::DisplayGeometry::DisplayToULDisplay(const mitk::Vector2D &vec_display, mitk::Vector2D &vec_ULdisplay) const
{
  ULDisplayToDisplay(vec_display, vec_ULdisplay);
}

//##ModelId=3EF436470027
void mitk::DisplayGeometry::ULDisplayToMM(const mitk::Point2D &pt_ULdisplay, mitk::Point2D &pt_mm) const
{
  ULDisplayToDisplay(pt_ULdisplay, pt_mm);
  DisplayToMM(pt_mm, pt_mm);
}

//##ModelId=3EF4364701AE
void mitk::DisplayGeometry::MMToULDisplay(const mitk::Point2D &pt_mm, mitk::Point2D &pt_ULdisplay) const
{
  MMToDisplay(pt_mm, pt_ULdisplay);
  DisplayToULDisplay(pt_ULdisplay, pt_ULdisplay);
}

//##ModelId=3EF436470348
void mitk::DisplayGeometry::ULDisplayToMM(const mitk::Vector2D &vec_ULdisplay, mitk::Vector2D &vec_mm) const
{
  ULDisplayToDisplay(vec_ULdisplay, vec_mm);
  DisplayToMM(vec_mm, vec_mm);
}

//##ModelId=3EF436480105
void mitk::DisplayGeometry::MMToULDisplay(const mitk::Vector2D &vec_mm, mitk::Vector2D &vec_ULdisplay) const
{
  MMToDisplay(vec_mm, vec_ULdisplay);
  DisplayToULDisplay(vec_ULdisplay, vec_ULdisplay);
}

//##ModelId=3E3AEB620231
const mitk::TimeBounds& mitk::DisplayGeometry::GetTimeBoundsInMS() const
{
  if(m_WorldGeometry.IsNull()) 
    return m_TimeBoundsInMS;

  return m_WorldGeometry->GetTimeBoundsInMS();
}

//##ModelId=3E3AE91A035E
mitk::DisplayGeometry::DisplayGeometry() : 
  m_ScaleFactorMMPerDisplayUnit(1.0),
  m_WorldGeometry(NULL)
{
  m_SizeInDisplayUnits.Fill(10);
  m_OriginInMM.Fill(0);
}

//##ModelId=3E3AE91A037C
mitk::DisplayGeometry::~DisplayGeometry()
{
}

//##ModelId=3E3C36920345
void mitk::DisplayGeometry::SetSizeInDisplayUnits(unsigned int width, unsigned int height, bool keepDisplayedRegion)
{
  Vector2D centerInMM;
  centerInMM =  m_SizeInMM*0.5;

  m_SizeInDisplayUnits[0]=width;
  m_SizeInDisplayUnits[1]=height;

  if(m_SizeInDisplayUnits[0] <= 0)
    m_SizeInDisplayUnits[0] = 1;
  if(m_SizeInDisplayUnits[1] <= 0)
    m_SizeInDisplayUnits[1] = 1;

  DisplayToMM(m_SizeInDisplayUnits, m_SizeInMM);

  if(keepDisplayedRegion)
  {
    Vector2D newCenterInMM;
    newCenterInMM = m_SizeInMM*0.5;

    Vector2D shift;
    shift=centerInMM-newCenterInMM;
    MMToDisplay(shift, shift);

    m_ScaleFactorMMPerDisplayUnit *= sqrt(centerInMM.GetSquaredNorm()/newCenterInMM.GetSquaredNorm());
    assert(m_ScaleFactorMMPerDisplayUnit<ScalarTypeNumericTraits::infinity());

    MoveBy(shift);
  }

  Modified();
}

//##ModelId=3E3C36CD02D2
void mitk::DisplayGeometry::SetOriginInMM(const mitk::Vector2D& origin_mm)
{
  m_OriginInMM = origin_mm;
  MMToDisplay(m_OriginInMM, m_OriginInDisplayUnits);

  Modified();
}

//##ModelId=3E3C38660226
void mitk::DisplayGeometry::Fit()
{
  if((m_WorldGeometry.IsNull()) || (m_WorldGeometry->IsValid() == false)) return;

  mitk::ScalarType   x,y,w,h;

  int width, height;
  // @FIXME: try to remove all the casts

  width=(int)m_SizeInDisplayUnits[0];
  height=(int)m_SizeInDisplayUnits[1];

  w = width;
  h = height;

  const ScalarType& widthInMM(m_WorldGeometry->GetParametricExtentInMM(0));
  const ScalarType& heightInMM(m_WorldGeometry->GetParametricExtentInMM(1));
  mitk::ScalarType aspRatio=((mitk::ScalarType)widthInMM)/heightInMM;

  x = (mitk::ScalarType)w/widthInMM;
  y = (mitk::ScalarType)h/heightInMM;
  if (x > y)
    w = (int) (aspRatio*h);
  else
    h = (int) (w/aspRatio);

  if(w>0)
    SetScaleFactor(widthInMM/w);

  mitk::Vector2D origin_display;
  origin_display[0]=-(width-w)/2.0;
  origin_display[1]=-(height-h)/2.0;
  SetOriginInMM(origin_display*m_ScaleFactorMMPerDisplayUnit);

  Modified();
}

//##ModelId=3E3D883B0132
void mitk::DisplayGeometry::SetScaleFactor(mitk::ScalarType mmPerDisplayUnit)
{
  if(mmPerDisplayUnit<0.0001) mmPerDisplayUnit=0.0001;

  m_ScaleFactorMMPerDisplayUnit = mmPerDisplayUnit;
  assert(m_ScaleFactorMMPerDisplayUnit<ScalarTypeNumericTraits::infinity());

  DisplayToMM(m_SizeInDisplayUnits, m_SizeInMM);
}

//##ModelId=3E3ED45900A1
mitk::ScalarType mitk::DisplayGeometry::GetScaleFactorMMPerDisplayUnit() const
{
  return m_ScaleFactorMMPerDisplayUnit;
}

//##ModelId=3E66CC5A0118
unsigned long mitk::DisplayGeometry::GetMTime() const
{
  if((m_WorldGeometry.IsNotNull()) && (Geometry2D::GetMTime() < m_WorldGeometry->GetMTime()))
    Modified();
  return Geometry2D::GetMTime();
}

//##ModelId=3EF4935A01B6
bool mitk::DisplayGeometry::Map(const mitk::Point3D & atPt3d_mm, const mitk::Vector3D &vec3d_mm, mitk::Vector2D &vec2d_mm) const
{
  if(m_WorldGeometry.IsNull()) return false;

  return m_WorldGeometry->Map(atPt3d_mm, vec3d_mm, vec2d_mm);
}

//##ModelId=3EF4935C03B8
void mitk::DisplayGeometry::Map(const mitk::Point2D & atPt2d_mm, const mitk::Vector2D &vec2d_mm, mitk::Vector3D &vec3d_mm) const
{
  if(m_WorldGeometry.IsNull()) return;

  m_WorldGeometry->Map(atPt2d_mm, vec2d_mm, vec3d_mm);    
}

bool mitk::DisplayGeometry::IsValid() const
{
  return m_Valid && m_WorldGeometry.IsNotNull() && m_WorldGeometry->IsValid();
}
