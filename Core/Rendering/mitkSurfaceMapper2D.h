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


#ifndef MITKSURFACEDATAMAPPER2D_H_HEADER_INCLUDED_C10EB2E8
#define MITKSURFACEDATAMAPPER2D_H_HEADER_INCLUDED_C10EB2E8

#include "mitkCommon.h"
#include "mitkGLMapper2D.h"
#include "mitkSurface.h"

class vtkCutter;
class vtkPlane;
class vtkLookupTable;
class vtkLinearTransform;


namespace mitk {

class BaseRenderer;
class Geometry2D;
class DisplayGeometry;

//##ModelId=3EF17FBD0315
//##Documentation
//## @brief OpenGL-based mapper to display a Geometry2D in a 2D window
//## OpenGL-based mapper to display a Geometry2D in a 2D window. The result is
//## normally a line. An important usage of this class is to show the
//## orientation of the slices displayed in other 2D windows.
class SurfaceMapper2D : public GLMapper2D
{
public:
  mitkClassMacro(SurfaceMapper2D, GLMapper2D);

  itkNewMacro(Self);

  //##ModelId=3EF18053036B
  const mitk::Surface * GetInput(void);

  //##ModelId=3EF18053039D
  virtual void Paint(BaseRenderer * renderer);

  //##Documentation
  //## @brief The Surface to map can be explicitly set by this method. If
  //## it is set, it is used instead of the data stored in the DataTreeNode.
  //## 
  //## This enables to use the mapper also internally from other mappers.
  itkSetConstObjectMacro(Surface, Surface);
  //##Documentation
  //## @brief Get the Surface set explicitly.
  //##
  //## @return NULL is returned if no Surface is set to be used instead of DataTreeNode::GetData().
  //## @sa SetSurface
  itkGetConstObjectMacro(Surface, Surface);

  // overwritten to initialize lookup table for point scalar data
 void SetDataTreeNode( DataTreeNode::Pointer node );

  void PaintCells(vtkPolyData* contour, const Geometry2D* worldGeometry, const DisplayGeometry* displayGeometry, vtkLinearTransform * vtktransform, vtkLookupTable *lut = NULL, bool useCellData=false);
protected:
  //##ModelId=3EF180540006
  SurfaceMapper2D();

  //##ModelId=3EF180540019
  virtual ~SurfaceMapper2D();

  //##ModelId=3EF18053031B
  vtkPlane* m_Plane;
  //##ModelId=3EF18053034D
  vtkCutter* m_Cutter;

  mitk::Surface::ConstPointer m_Surface;

  vtkLookupTable *m_LUT;
  
  int m_LineWidth;

};

} // namespace mitk
#endif /* MITKSURFACEDATAMAPPER2D_H_HEADER_INCLUDED_C10EB2E8 */
