/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "mitkUnstructuredGridMapper2D.h"
#include <mitkGL.h>

#include "mitkAbstractTransformGeometry.h"
#include "mitkBaseRenderer.h"
#include "mitkColorProperty.h"
#include "mitkPlaneGeometry.h"
#include "mitkProperties.h"
#include "mitkTransferFunction.h"
#include "mitkTransferFunctionProperty.h"
#include "mitkUnstructuredGrid.h"
#include "mitkVtkMapper3D.h"
#include "mitkVtkScalarModeProperty.h"

#include <vtkAbstractMapper3D.h>
#include <vtkAbstractVolumeMapper.h>
#include <vtkAssembly.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkLinearTransform.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkProp3DCollection.h>
#include <vtkScalarsToColors.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

#include "vtkPointSetSlicer.h"

void mitk::UnstructuredGridMapper2D::GenerateDataForRenderer(mitk::BaseRenderer *renderer)
{
  BaseLocalStorage *ls = m_LSH.GetLocalStorage(renderer);
  bool needGenerateData = ls->IsGenerateDataRequired(renderer, this, GetDataNode());

  if (needGenerateData)
  {
    ls->UpdateGenerateDataTime();

    mitk::DataNode::ConstPointer node = this->GetDataNode();
    if (node.IsNull())
      return;

    if (!node->GetProperty(m_ScalarMode, "scalar mode"))
    {
      m_ScalarMode = mitk::VtkScalarModeProperty::New(0);
    }

    if (!node->GetProperty(m_ScalarVisibility, "scalar visibility"))
    {
      m_ScalarVisibility = mitk::BoolProperty::New(true);
    }

    if (!node->GetProperty(m_Outline, "outline polygons"))
    {
      m_Outline = mitk::BoolProperty::New(false);
    }

    if (!node->GetProperty(m_Color, "color"))
    {
      m_Color = mitk::ColorProperty::New(1.0f, 1.0f, 1.0f);
    }

    if (!node->GetProperty(m_LineWidth, "line width"))
    {
      m_LineWidth = mitk::IntProperty::New(1);
    }
  }
  mitk::BaseData::Pointer input = GetDataNode()->GetData();
  assert(input);

  input->Update();

  if (m_VtkPointSet)
    m_VtkPointSet->UnRegister(nullptr);
  m_VtkPointSet = this->GetVtkPointSet(renderer, this->GetTimestep());
  assert(m_VtkPointSet);
  m_VtkPointSet->Register(nullptr);

  if (m_ScalarVisibility->GetValue())
  {
    mitk::DataNode::ConstPointer node = this->GetDataNode();
    mitk::TransferFunctionProperty::Pointer transferFuncProp;
    node->GetProperty(transferFuncProp, "TransferFunction", renderer);
    if (transferFuncProp.IsNotNull())
    {
      mitk::TransferFunction::Pointer tf = transferFuncProp->GetValue();
      if (m_ScalarsToColors)
        m_ScalarsToColors->UnRegister(nullptr);
      m_ScalarsToColors = static_cast<vtkScalarsToColors *>(tf->GetColorTransferFunction());
      m_ScalarsToColors->Register(nullptr);

      if (m_ScalarsToOpacity)
        m_ScalarsToOpacity->UnRegister(nullptr);
      m_ScalarsToOpacity = tf->GetScalarOpacityFunction();
      m_ScalarsToOpacity->Register(nullptr);
    }
    else
    {
      if (m_ScalarsToColors)
        m_ScalarsToColors->UnRegister(nullptr);
      m_ScalarsToColors = this->GetVtkLUT(renderer);
      assert(m_ScalarsToColors);
      m_ScalarsToColors->Register(nullptr);

      float opacity;
      node->GetOpacity(opacity, renderer);
      if (m_ScalarsToOpacity)
        m_ScalarsToOpacity->UnRegister(nullptr);
      m_ScalarsToOpacity = vtkPiecewiseFunction::New();
      double range[2];
      m_VtkPointSet->GetScalarRange(range);
      m_ScalarsToOpacity->AddSegment(range[0], opacity, range[1], opacity);
    }
  }
}

void mitk::UnstructuredGridMapper2D::Paint(mitk::BaseRenderer *renderer)
{
  bool visible = true;
  GetDataNode()->GetVisibility(visible, renderer, "visible");
  if (!visible)
    return;

  vtkLinearTransform *vtktransform = GetDataNode()->GetVtkTransform();
  vtkLinearTransform *inversetransform = vtktransform->GetLinearInverse();

  PlaneGeometry::ConstPointer worldGeometry = renderer->GetCurrentWorldPlaneGeometry();
  PlaneGeometry::ConstPointer worldPlaneGeometry = dynamic_cast<const PlaneGeometry *>(worldGeometry.GetPointer());

  Point3D point;
  Vector3D normal;

  if (worldPlaneGeometry.IsNotNull())
  {
    // set up vtkPlane according to worldGeometry
    point = worldPlaneGeometry->GetOrigin();
    normal = worldPlaneGeometry->GetNormal();
    normal.Normalize();
    m_Plane->SetTransform((vtkAbstractTransform *)nullptr);
  }
  else
  {
    //@FIXME: does not work correctly. Does m_Plane->SetTransform really transforms a "plane plane" into a "curved
    //plane"?
    return;
    AbstractTransformGeometry::ConstPointer worldAbstractGeometry =
      dynamic_cast<const AbstractTransformGeometry *>(renderer->GetCurrentWorldPlaneGeometry());
    if (worldAbstractGeometry.IsNotNull())
    {
      // set up vtkPlane according to worldGeometry
      point = worldAbstractGeometry->GetParametricBoundingBox()->GetMinimum();
      FillVector3D(normal, 0, 0, 1);
      m_Plane->SetTransform(worldAbstractGeometry->GetVtkAbstractTransform()->GetInverse());
    }
    else
      return;
  }

  double vp[3], vnormal[3];

  vnl2vtk(point.GetVnlVector(), vp);
  vnl2vtk(normal.GetVnlVector(), vnormal);

  // normally, we would need to transform the surface and cut the transformed surface with the cutter.
  // This might be quite slow. Thus, the idea is, to perform an inverse transform of the plane instead.
  //@todo It probably does not work for scaling operations yet:scaling operations have to be
  // dealt with after the cut is performed by scaling the contour.
  inversetransform->TransformPoint(vp, vp);
  inversetransform->TransformNormalAtPoint(vp, vnormal, vnormal);

  m_Plane->SetOrigin(vp);
  m_Plane->SetNormal(vnormal);

  // set data into cutter
  m_Slicer->SetInputData(m_VtkPointSet);
  //    m_Cutter->GenerateCutScalarsOff();
  //    m_Cutter->SetSortByToSortByCell();

  // calculate the cut
  m_Slicer->Update();

  // apply color and opacity read from the PropertyList
  ApplyColorAndOpacityProperties(renderer);

  // traverse the cut contour
  vtkPolyData *contour = m_Slicer->GetOutput();

  vtkPoints *vpoints = contour->GetPoints();
  vtkCellArray *vlines = contour->GetLines();
  vtkCellArray *vpolys = contour->GetPolys();
  vtkPointData *vpointdata = contour->GetPointData();
  vtkDataArray *vscalars = vpointdata->GetScalars();

  vtkCellData *vcelldata = contour->GetCellData();
  vtkDataArray *vcellscalars = vcelldata->GetScalars();

  const int numberOfLines = contour->GetNumberOfLines();
  const int numberOfPolys = contour->GetNumberOfPolys();

  const bool useCellData = m_ScalarMode->GetVtkScalarMode() == VTK_SCALAR_MODE_DEFAULT ||
                           m_ScalarMode->GetVtkScalarMode() == VTK_SCALAR_MODE_USE_CELL_DATA;
  const bool usePointData = m_ScalarMode->GetVtkScalarMode() == VTK_SCALAR_MODE_USE_POINT_DATA;

  Point3D p;
  Point2D p2d;

  vlines->InitTraversal();
  vpolys->InitTraversal();

  mitk::Color outlineColor = m_Color->GetColor();

  glLineWidth((float)m_LineWidth->GetValue());

  for (int i = 0; i < numberOfLines; ++i)
  {
    const vtkIdType *cell(nullptr);
    vtkIdType cellSize(0);

    vlines->GetNextCell(cellSize, cell);

    float rgba[4] = {outlineColor[0], outlineColor[1], outlineColor[2], 1.0f};
    if (m_ScalarVisibility->GetValue() && vcellscalars)
    {
      if (useCellData)
      { // color each cell according to cell data
        double scalar = vcellscalars->GetComponent(i, 0);
        double rgb[3] = {1.0f, 1.0f, 1.0f};
        m_ScalarsToColors->GetColor(scalar, rgb);
        rgba[0] = (float)rgb[0];
        rgba[1] = (float)rgb[1];
        rgba[2] = (float)rgb[2];
        rgba[3] = (float)m_ScalarsToOpacity->GetValue(scalar);
      }
      else if (usePointData)
      {
        double scalar = vscalars->GetComponent(i, 0);
        double rgb[3] = {1.0f, 1.0f, 1.0f};
        m_ScalarsToColors->GetColor(scalar, rgb);
        rgba[0] = (float)rgb[0];
        rgba[1] = (float)rgb[1];
        rgba[2] = (float)rgb[2];
        rgba[3] = (float)m_ScalarsToOpacity->GetValue(scalar);
      }
    }

    glColor4fv(rgba);

    glBegin(GL_LINE_LOOP);
    for (int j = 0; j < cellSize; ++j)
    {
      vpoints->GetPoint(cell[j], vp);
      // take transformation via vtktransform into account
      vtktransform->TransformPoint(vp, vp);

      vtk2itk(vp, p);

      // convert 3D point (in mm) to display coordinates (units )
      renderer->WorldToDisplay(p, p2d);

      // convert display coordinates ( (0,0) is top-left ) in GL coordinates ( (0,0) is bottom-left )
      // p2d[1]=toGL-p2d[1];

      // add the current vertex to the line
      glVertex2f(p2d[0], p2d[1]);
    }
    glEnd();
  }

  bool polyOutline = m_Outline->GetValue();
  bool scalarVisibility = m_ScalarVisibility->GetValue();

  // cache the transformed points
  // a fixed size array is way faster than 'new'
  // slices through 3d cells usually do not generated
  // polygons with more than 6 vertices
  const int maxPolySize = 10;
  auto *cachedPoints = new Point2D[maxPolySize * numberOfPolys];

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // only draw polygons if there are cell scalars
  // or the outline property is set to true
  if (scalarVisibility && vcellscalars)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (int i = 0; i < numberOfPolys; ++i)
    {
      const vtkIdType *cell(nullptr);
      vtkIdType cellSize(0);

      vpolys->GetNextCell(cellSize, cell);

      float rgba[4] = {1.0f, 1.0f, 1.0f, 0};
      if (scalarVisibility && vcellscalars)
      {
        if (useCellData)
        { // color each cell according to cell data
          double scalar = vcellscalars->GetComponent(i + numberOfLines, 0);
          double rgb[3] = {1.0f, 1.0f, 1.0f};
          m_ScalarsToColors->GetColor(scalar, rgb);
          rgba[0] = (float)rgb[0];
          rgba[1] = (float)rgb[1];
          rgba[2] = (float)rgb[2];
          rgba[3] = (float)m_ScalarsToOpacity->GetValue(scalar);
        }
        else if (usePointData)
        {
          double scalar = vscalars->GetComponent(i, 0);
          double rgb[3] = {1.0f, 1.0f, 1.0f};
          m_ScalarsToColors->GetColor(scalar, rgb);
          rgba[0] = (float)rgb[0];
          rgba[1] = (float)rgb[1];
          rgba[2] = (float)rgb[2];
          rgba[3] = (float)m_ScalarsToOpacity->GetValue(scalar);
        }
      }
      glColor4fv(rgba);

      glBegin(GL_POLYGON);
      for (int j = 0; j < cellSize; ++j)
      {
        vpoints->GetPoint(cell[j], vp);
        // take transformation via vtktransform into account
        vtktransform->TransformPoint(vp, vp);

        vtk2itk(vp, p);

        // convert 3D point (in mm) to display coordinates (units )
        renderer->WorldToDisplay(p, p2d);

        // convert display coordinates ( (0,0) is top-left ) in GL coordinates ( (0,0) is bottom-left )
        // p2d[1]=toGL-p2d[1];

        cachedPoints[i * 10 + j][0] = p2d[0];
        cachedPoints[i * 10 + j][1] = p2d[1];

        // add the current vertex to the line
        glVertex2f(p2d[0], p2d[1]);
      }
      glEnd();
    }

    if (polyOutline)
    {
      vpolys->InitTraversal();

      glColor4f(outlineColor[0], outlineColor[1], outlineColor[2], 1.0f);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      for (int i = 0; i < numberOfPolys; ++i)
      {
        const vtkIdType *cell(nullptr);
        vtkIdType cellSize(0);

        vpolys->GetNextCell(cellSize, cell);

        glBegin(GL_POLYGON);
        // glPolygonOffset(1.0, 1.0);
        for (int j = 0; j < cellSize; ++j)
        {
          // add the current vertex to the line
          glVertex2f(cachedPoints[i * 10 + j][0], cachedPoints[i * 10 + j][1]);
        }
        glEnd();
      }
    }
  }
  glDisable(GL_BLEND);
  delete[] cachedPoints;
}

vtkAbstractMapper3D *mitk::UnstructuredGridMapper2D::GetVtkAbstractMapper3D(mitk::BaseRenderer *renderer)
{
  // MITK_INFO << "GETVTKABSTRACTMAPPER3D\n";
  mitk::DataNode::ConstPointer node = this->GetDataNode();
  if (node.IsNull())
    return nullptr;

  mitk::VtkMapper::Pointer mitkMapper = dynamic_cast<mitk::VtkMapper *>(node->GetMapper(2));
  if (mitkMapper.IsNull())
  {
    return nullptr;
  }

  mitkMapper->Update(renderer);

  auto *assembly = dynamic_cast<vtkAssembly *>(mitkMapper->GetVtkProp(renderer));
  if (assembly)
  {
    vtkProp3DCollection *collection = assembly->GetParts();
    collection->InitTraversal();
    vtkProp3D *prop3d = nullptr;
    do
    {
      prop3d = collection->GetNextProp3D();
      auto *actor = dynamic_cast<vtkActor *>(prop3d);
      if (actor)
      {
        return dynamic_cast<vtkAbstractMapper3D *>(actor->GetMapper());
      }

      auto *volume = dynamic_cast<vtkVolume *>(prop3d);
      if (volume)
      {
        return dynamic_cast<vtkAbstractMapper3D *>(volume->GetMapper());
      }
    } while (prop3d != collection->GetLastProp3D());
  }
  else
  {
    auto *actor = dynamic_cast<vtkActor *>(mitkMapper->GetVtkProp(renderer));
    if (actor)
    {
      return dynamic_cast<vtkAbstractMapper3D *>(actor->GetMapper());
    }

    auto *volume = dynamic_cast<vtkVolume *>(mitkMapper->GetVtkProp(renderer));
    if (volume)
    {
      return dynamic_cast<vtkAbstractMapper3D *>(volume->GetMapper());
    }
  }
  return nullptr;
}

vtkPointSet *mitk::UnstructuredGridMapper2D::GetVtkPointSet(mitk::BaseRenderer *renderer, int time)
{
  // MITK_INFO << "GETVTKPOINTSET\n";
  vtkAbstractMapper3D *abstractMapper = GetVtkAbstractMapper3D(renderer);
  if (abstractMapper == nullptr)
  {
    // try to get data from the node
    mitk::DataNode::ConstPointer node = this->GetDataNode();
    if (node.IsNull())
      return nullptr;
    mitk::BaseData::Pointer data = node->GetData();
    mitk::UnstructuredGrid::Pointer grid = dynamic_cast<mitk::UnstructuredGrid *>(data.GetPointer());
    if (!grid.IsNull())
      return static_cast<vtkPointSet *>(grid->GetVtkUnstructuredGrid(time));

    return nullptr;
  }
  else
  {
    auto *mapper = dynamic_cast<vtkMapper *>(abstractMapper);
    if (mapper)
    {
      return dynamic_cast<vtkPointSet *>(mapper->GetInput());
    }
    auto *volMapper = dynamic_cast<vtkAbstractVolumeMapper *>(abstractMapper);
    if (volMapper)
    {
      return dynamic_cast<vtkPointSet *>(volMapper->GetDataSetInput());
    }
  }

  return nullptr;
}

vtkScalarsToColors *mitk::UnstructuredGridMapper2D::GetVtkLUT(mitk::BaseRenderer *renderer)
{
  // MITK_INFO << "GETVTKLUT\n";
  auto *mapper = dynamic_cast<vtkMapper *>(GetVtkAbstractMapper3D(renderer));
  if (mapper)
    return mapper->GetLookupTable();
  else
  {
    mitk::DataNode::ConstPointer node = this->GetDataNode();
    if (node.IsNull())
      return nullptr;

    mitk::VtkMapper::Pointer mitkMapper = dynamic_cast<mitk::VtkMapper *>(node->GetMapper(2));
    if (mitkMapper.IsNull())
    {
      // MITK_INFO << "mitkMapper is null\n";
      return nullptr;
    }

    mitkMapper->Update(renderer);

    auto *volume = dynamic_cast<vtkVolume *>(mitkMapper->GetVtkProp(renderer));
    if (volume)
    {
      // MITK_INFO << "found volume prop\n";
      return static_cast<vtkScalarsToColors *>(volume->GetProperty()->GetRGBTransferFunction());
    }

    auto *assembly = dynamic_cast<vtkAssembly *>(mitkMapper->GetVtkProp(renderer));
    if (assembly)
    {
      // MITK_INFO << "found assembly prop\n";
      mitk::TransferFunctionProperty::Pointer transferFuncProp;
      node->GetProperty(transferFuncProp, "TransferFunction", nullptr);
      if (transferFuncProp.IsNotNull())
      {
        MITK_INFO << "return colortransferfunction\n";
        return static_cast<vtkScalarsToColors *>(transferFuncProp->GetValue()->GetColorTransferFunction());
      }
    }
    return nullptr;
  }
}

bool mitk::UnstructuredGridMapper2D::IsConvertibleToVtkPointSet(mitk::BaseRenderer *renderer)
{
  return (GetVtkPointSet(renderer, this->GetTimestep()) != nullptr);
}

mitk::UnstructuredGridMapper2D::UnstructuredGridMapper2D()
{
  m_Plane = vtkPlane::New();
  m_Slicer = vtkPointSetSlicer::New();

  m_Slicer->SetSlicePlane(m_Plane);

  m_ScalarsToColors = nullptr;
  m_ScalarsToOpacity = nullptr;
  m_VtkPointSet = nullptr;

  // m_LUT = vtkLookupTable::New();
  // m_LUT->SetTableRange( 0, 255 );
  // m_LUT->SetNumberOfColors( 255 );
  // m_LUT->SetRampToLinear ();
  // m_LUT->Build();
}

mitk::UnstructuredGridMapper2D::~UnstructuredGridMapper2D()
{
  m_Slicer->Delete();
  m_Plane->Delete();

  if (m_ScalarsToOpacity != nullptr)
    m_ScalarsToOpacity->UnRegister(nullptr);
  if (m_ScalarsToColors != nullptr)
    m_ScalarsToColors->UnRegister(nullptr);
  if (m_VtkPointSet != nullptr)
    m_VtkPointSet->UnRegister(nullptr);
}
