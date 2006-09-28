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


#include "mitkGeometry3D.h"
#include "mitkItkMatrixHack.h"
#include "mitkMatrixConvert.h"
#include "mitkOperation.h"
#include "mitkRotationOperation.h"
#include "mitkPointOperation.h"
#include "mitkInteractionConst.h"
#include "mitkStatusBar.h"
#include <mitkXMLWriter.h>
#include <mitkXMLReader.h>
#include <float.h>

#include <vtkMatrixToLinearTransform.h>
#include <vtkMatrix4x4.h>

const std::string mitk::Geometry3D::XML_NODE_NAME = "geometry";
const std::string mitk::Geometry3D::INDEX_TO_OBJECT_TRANSFORM = "INDEX_TO_OBJECT_TRANSFORM";
const std::string mitk::Geometry3D::OBJECT_TO_NODE_TRANSFORM = "OBJECT_TO_NODE_TRANSFORM";
const std::string mitk::Geometry3D::INDEX_TO_NODE_TRANSFORM = "INDEX_TO_NODE_TRANSFORM";
const std::string mitk::Geometry3D::INDEX_TO_WORLD_TRANSFORM = "INDEX_TO_WORLD_TRANSFORM";

// Standard Constructor for the new makro. sets the geometry to 3 dimensions
mitk::Geometry3D::Geometry3D()
    : m_ParametricBoundingBox(NULL),
    m_ImageGeometry(false), m_Valid(true), m_FrameOfReferenceID(0)
{
  FillVector3D(m_FloatSpacing, 1,1,1);
  m_VtkMatrix = vtkMatrix4x4::New();
  m_VtkIndexToWorldTransform = vtkMatrixToLinearTransform::New();
  m_VtkIndexToWorldTransform->SetInput(m_VtkMatrix);
  Initialize();
}

//##ModelId=3E3456C50067
mitk::Geometry3D::~Geometry3D()
{
}

static void CopySpacingFromTransform(mitk::AffineTransform3D* transform, mitk::Vector3D& spacing, float floatSpacing[3])
{
  mitk::AffineTransform3D::MatrixType::InternalMatrixType vnlmatrix;
  vnlmatrix = transform->GetMatrix().GetVnlMatrix();

  spacing[0]=vnlmatrix.get_column(0).magnitude();
  spacing[1]=vnlmatrix.get_column(1).magnitude();
  spacing[2]=vnlmatrix.get_column(2).magnitude();
  floatSpacing[0]=spacing[0];
  floatSpacing[1]=spacing[1];
  floatSpacing[2]=spacing[2];
}

//##ModelId=3E3453C703AF
void mitk::Geometry3D::Initialize()
{
  float b[6] = {0,1,0,1,0,1};
  SetFloatBounds(b);

  m_IndexToObjectTransform = TransformType::New();
  m_ObjectToNodeTransform = TransformType::New();

  if(m_IndexToWorldTransform.IsNull())
    m_IndexToWorldTransform = TransformType::New();
  else
    m_IndexToWorldTransform->SetIdentity();
  CopySpacingFromTransform(m_IndexToWorldTransform, m_Spacing, m_FloatSpacing);
  vtk2itk(m_IndexToWorldTransform->GetOffset(), m_Origin);

  m_VtkMatrix->Identity();

  m_ParametricTransform = m_IndexToWorldTransform;

  m_TimeBounds[0]=-ScalarTypeNumericTraits::max(); m_TimeBounds[1]=ScalarTypeNumericTraits::max();

  m_FrameOfReferenceID = 0;

  m_ImageGeometry = false;
}

void mitk::Geometry3D::TransferItkToVtkTransform()
{
  // copy m_IndexToWorldTransform into m_VtkIndexToWorldTransform
  TransferItkTransformToVtkMatrix(m_IndexToWorldTransform.GetPointer(), m_VtkMatrix);
  m_VtkIndexToWorldTransform->Modified();
}

void mitk::Geometry3D::TransferVtkToItkTransform()
{
  TransferVtkMatrixToItkTransform(m_VtkMatrix, m_IndexToWorldTransform.GetPointer());
  m_ParametricTransform = m_IndexToWorldTransform;
  CopySpacingFromTransform(m_IndexToWorldTransform, m_Spacing, m_FloatSpacing);
  vtk2itk(m_IndexToWorldTransform->GetOffset(), m_Origin);
}

void mitk::Geometry3D::SetIndexToWorldTransformByVtkMatrix(vtkMatrix4x4* vtkmatrix)
{
  m_VtkMatrix->DeepCopy(vtkmatrix);
  TransferVtkToItkTransform();
}

void mitk::Geometry3D::SetTimeBounds(const TimeBounds& timebounds)
{
  if(m_TimeBounds != timebounds)
  {
    m_TimeBounds = timebounds;
    Modified();
  }
}

void mitk::Geometry3D::SetFloatBounds(const float bounds[6])
{
  mitk::BoundingBox::BoundsArrayType b;
  const float *input = bounds;
  int i=0;
  for(mitk::BoundingBox::BoundsArrayType::Iterator it = b.Begin(); i < 6 ;++i) *it++ = (mitk::ScalarType)*input++;
  SetBoundsArray(b, m_BoundingBox);
}

void mitk::Geometry3D::SetFloatBounds(const double bounds[6])
{
  mitk::BoundingBox::BoundsArrayType b;
  const double *input = bounds;
  int i=0;
  for(mitk::BoundingBox::BoundsArrayType::Iterator it = b.Begin(); i < 6 ;++i) *it++ = (mitk::ScalarType)*input++;
  SetBoundsArray(b, m_BoundingBox);
}

void mitk::Geometry3D::SetParametricBounds(const BoundingBox::BoundsArrayType& bounds)
{
  //std::cout << " BOUNDS: " << bounds << std::endl;
  SetBoundsArray(bounds, m_ParametricBoundingBox);
}

void mitk::Geometry3D::WorldToIndex(const mitk::Point3D &pt_mm, mitk::Point3D &pt_units) const
{
  BackTransform(pt_mm, pt_units);
}

//##ModelId=3DDE65DC0151
void mitk::Geometry3D::IndexToWorld(const mitk::Point3D &pt_units, mitk::Point3D &pt_mm) const
{
  pt_mm = m_ParametricTransform->TransformPoint(pt_units);
}

//##ModelId=3E3B986602CF
void mitk::Geometry3D::WorldToIndex(const mitk::Point3D &atPt3d_mm, const mitk::Vector3D &vec_mm, mitk::Vector3D &vec_units) const
{
  BackTransform(atPt3d_mm, vec_mm, vec_units);
}

//##ModelId=3E3B987503A3
void mitk::Geometry3D::IndexToWorld(const mitk::Point3D &/*atPt3d_units*/, const mitk::Vector3D &vec_units, mitk::Vector3D &vec_mm) const
{
  vec_mm = m_ParametricTransform->TransformVector(vec_units);
}

void mitk::Geometry3D::SetIndexToWorldTransform(mitk::AffineTransform3D* transform)
{
  if(m_IndexToWorldTransform.GetPointer() != transform)
  {
    Superclass::SetIndexToWorldTransform(transform);
    m_ParametricTransform = m_IndexToWorldTransform;
    CopySpacingFromTransform(m_IndexToWorldTransform, m_Spacing, m_FloatSpacing);
    vtk2itk(m_IndexToWorldTransform->GetOffset(), m_Origin);
    TransferItkToVtkTransform();
    Modified();
  }
}

mitk::AffineGeometryFrame3D::Pointer mitk::Geometry3D::Clone() const
{
  Self::Pointer newGeometry = Self::New();
  newGeometry->Initialize();
  InitializeGeometry(newGeometry);
  return newGeometry.GetPointer();
}

void mitk::Geometry3D::InitializeGeometry(Geometry3D * newGeometry) const
{
  Superclass::InitializeGeometry(newGeometry);
  static_cast<mitk::ItkMatrixHack<AffineGeometryFrame3D::TransformType>*>(newGeometry->GetIndexToWorldTransform())->MatrixChanged();

  newGeometry->SetTimeBounds(m_TimeBounds);

  //newGeometry->GetVtkTransform()->SetMatrix(m_VtkIndexToWorldTransform->GetMatrix()); IW
  //newGeometry->TransferVtkToItkTransform(); //MH

  if(m_ParametricBoundingBox.IsNotNull())
    newGeometry->SetParametricBounds(m_ParametricBoundingBox->GetBounds());

  newGeometry->SetFrameOfReferenceID(GetFrameOfReferenceID());
  newGeometry->m_ImageGeometry = m_ImageGeometry;
}

void mitk::Geometry3D::SetExtentInMM(int direction, ScalarType extentInMM)
{
  ScalarType len = GetExtentInMM(direction);
  if(fabs(len - extentInMM)>=mitk::eps)
  {
    AffineTransform3D::MatrixType::InternalMatrixType vnlmatrix;
    vnlmatrix = m_IndexToWorldTransform->GetMatrix().GetVnlMatrix();
    if(len>extentInMM)
      vnlmatrix.set_column(direction, vnlmatrix.get_column(direction)/len*extentInMM);
    else
      vnlmatrix.set_column(direction, vnlmatrix.get_column(direction)*extentInMM/len);
    Matrix3D matrix;
    matrix = vnlmatrix;
    m_IndexToWorldTransform->SetMatrix(matrix);
    Modified();
  }
}

mitk::BoundingBox::Pointer mitk::Geometry3D::CalculateBoundingBoxRelativeToTransform(const mitk::AffineTransform3D* transform)
{
  mitk::BoundingBox::PointsContainer::Pointer pointscontainer=mitk::BoundingBox::PointsContainer::New();

  mitk::BoundingBox::PointIdentifier pointid=0;

  unsigned char i;
  if(transform!=NULL)
  {
    mitk::AffineTransform3D::Pointer inverse = mitk::AffineTransform3D::New();
    transform->GetInverse(inverse);
    for(i=0; i<8; ++i)
      pointscontainer->InsertElement( pointid++, inverse->TransformPoint( GetCornerPoint(i) ));
  }
  else
  {
    for(i=0; i<8; ++i)
      pointscontainer->InsertElement( pointid++, GetCornerPoint(i) );
  }

  mitk::BoundingBox::Pointer result = mitk::BoundingBox::New();
  result->SetPoints(pointscontainer);
  result->ComputeBoundingBox();

  return result;
}

#include <vtkTransform.h>
void mitk::Geometry3D::ExecuteOperation(Operation* operation)
{
  vtkTransform *vtktransform = vtkTransform::New();
  vtktransform->SetMatrix(m_VtkMatrix);
  switch (operation->GetOperationType())
  {
  case OpNOTHING:
    break;
  case OpMOVE:
    {
      mitk::PointOperation *pointOp = dynamic_cast<mitk::PointOperation *>(operation);
      if (pointOp == NULL)
      {
        mitk::StatusBar::DisplayText("received wrong type of operation!See mitkAffineInteractor.cpp", 10000);
        return;
      }
      mitk::Point3D newPos = pointOp->GetPoint();
      ScalarType data[3];
      vtktransform->GetPosition(data);
      vtktransform->PostMultiply();
      vtktransform->Translate(newPos[0], newPos[1], newPos[2]);
      vtktransform->PreMultiply();
      break;
    }
  case OpSCALE:
    {
      mitk::PointOperation *pointOp = dynamic_cast<mitk::PointOperation *>(operation);
      if (pointOp == NULL)
      {
        mitk::StatusBar::DisplayText("received wrong type of operation!See mitkAffineInteractor.cpp", 10000);
        return;
      }
      mitk::Point3D newScale = pointOp->GetPoint();
      ScalarType data[3];
      /* calculate new scale: newscale = oldscale * (oldscale + scaletoadd)/oldscale */
      data[0] = 1 + (newScale[0] / GetMatrixColumn(0).magnitude());
      data[1] = 1 + (newScale[1] / GetMatrixColumn(1).magnitude());
      data[2] = 1 + (newScale[2] / GetMatrixColumn(2).magnitude());

      mitk::Point3D center = const_cast<mitk::BoundingBox*>(m_BoundingBox.GetPointer())->GetCenter();
      ScalarType pos[3];
      vtktransform->GetPosition(pos);
      vtktransform->PostMultiply();
      vtktransform->Translate(-pos[0], -pos[1], -pos[2]);
      vtktransform->Translate(-center[0], -center[1], -center[2]);
      vtktransform->PreMultiply();
      vtktransform->Scale(data[0], data[1], data[2]);
      vtktransform->PostMultiply();
      vtktransform->Translate(+center[0], +center[1], +center[2]);
      vtktransform->Translate(pos[0], pos[1], pos[2]);
      vtktransform->PreMultiply();
      break;
    }
  case OpROTATE:
    {
      mitk::RotationOperation *rotateOp = dynamic_cast<mitk::RotationOperation *>(operation);
      if (rotateOp == NULL)
      {
        mitk::StatusBar::DisplayText("received wrong type of operation!See mitkAffineInteractor.cpp", 10000);
        return;
      }
      Vector3D rotationVector = rotateOp->GetVectorOfRotation();
      Point3D center = rotateOp->GetCenterOfRotation();
      ScalarType angle = rotateOp->GetAngleOfRotation();
      angle = (angle < -360) ? -360 : angle;
      angle = (angle >  360) ?  360 : angle;
      vtktransform->PostMultiply();
      vtktransform->Translate(-center[0], -center[1], -center[2]);
      vtktransform->RotateWXYZ(angle, rotationVector[0], rotationVector[1], rotationVector[2]);
      vtktransform->Translate(center[0], center[1], center[2]);
      vtktransform->PreMultiply();
      break;
    }
  default:
    return;
  }
  m_VtkMatrix->DeepCopy(vtktransform->GetMatrix());
  TransferVtkToItkTransform();
  Modified();
}

void mitk::Geometry3D::BackTransform(const mitk::Point3D &in, mitk::Point3D& out) const
{
  ScalarType temp[3];
  unsigned int i, j;
  const TransformType::OffsetType& offset = m_IndexToWorldTransform->GetOffset();

  for (j = 0; j < 3; j++)
  {
    temp[j] = in[j] - offset[j];
  }

#if (ITK_VERSION_MAJOR == 1 && ITK_VERSION_MINOR <= 8)
  const TransformType::MatrixType& inverse = m_IndexToWorldTransform->GetInverse();
#else
  const TransformType::MatrixType& inverse = m_IndexToWorldTransform->GetInverseMatrix();
#endif
  for (i = 0; i < 3; i++)
  {
    out[i] = 0.0;
    for (j = 0; j < 3; j++)
    {
      out[i] += inverse[i][j]*temp[j];
    }
  }
}

void mitk::Geometry3D::BackTransform(const mitk::Point3D &/*at*/, const mitk::Vector3D &in, mitk::Vector3D& out) const
{
#if (ITK_VERSION_MAJOR == 1 && ITK_VERSION_MINOR <= 8)
  const TransformType::MatrixType& inverse = m_IndexToWorldTransform->GetInverse();
#else
  const TransformType::MatrixType& inverse = m_IndexToWorldTransform->GetInverseMatrix();
#endif
  out = inverse * in;
}

const float* mitk::Geometry3D::GetFloatSpacing() const
{
  return m_FloatSpacing;
}

void mitk::Geometry3D::SetSpacing(const float aSpacing[3])
{
  mitk::Vector3D tmp;
  tmp[0]= aSpacing[0];
  tmp[1]= aSpacing[1];
  tmp[2]= aSpacing[2];
  SetSpacing(tmp);
}

void mitk::Geometry3D::SetSpacing(const mitk::Vector3D& aSpacing)
{
  if(mitk::Equal(m_Spacing, aSpacing) == false)
  {
    assert(aSpacing[0]>0 && aSpacing[1]>0 && aSpacing[2]>0);

    m_Spacing = aSpacing;

    AffineTransform3D::MatrixType::InternalMatrixType vnlmatrix;

    vnlmatrix = m_IndexToWorldTransform->GetMatrix().GetVnlMatrix();

    mitk::VnlVector col;
    col = vnlmatrix.get_column(0); col.normalize(); col*=aSpacing[0]; vnlmatrix.set_column(0, col);
    col = vnlmatrix.get_column(1); col.normalize(); col*=aSpacing[1]; vnlmatrix.set_column(1, col);
    col = vnlmatrix.get_column(2); col.normalize(); col*=aSpacing[2]; vnlmatrix.set_column(2, col);

    Matrix3D matrix;
    matrix = vnlmatrix;

    AffineTransform3D::Pointer transform = AffineTransform3D::New();
    transform->SetMatrix(matrix);
    transform->SetOffset(m_IndexToWorldTransform->GetOffset());

    SetIndexToWorldTransform(transform.GetPointer());

    itk2vtk(m_Spacing, m_FloatSpacing);
  }
}

void mitk::Geometry3D::SetOrigin(const Point3D & origin)
{
  if(origin!=GetOrigin())
  {
    m_Origin = origin;
    m_IndexToWorldTransform->SetOffset(m_Origin.GetVectorFromOrigin());
    Modified();
    TransferItkToVtkTransform();
  }
}

void mitk::Geometry3D::Translate(const Vector3D & vector)
{
  if((vector[0] != 0) || (vector[1] != 0) || (vector[2] != 0))
  {
    m_IndexToWorldTransform->SetOffset(m_IndexToWorldTransform->GetOffset()+vector);
    TransferItkToVtkTransform();
    Modified();
  }
}

void mitk::Geometry3D::SetIdentity()
{
  m_IndexToWorldTransform->SetIdentity();
  m_Origin.Fill(0);
  Modified();
  TransferItkToVtkTransform();
}

void mitk::Geometry3D::Compose( const mitk::AffineGeometryFrame3D::TransformType * other, bool pre )
{
  m_IndexToWorldTransform->Compose(other, pre);
  vtk2itk(m_IndexToWorldTransform->GetOffset(), m_Origin);
  Modified();
  TransferItkToVtkTransform();
}

void mitk::Geometry3D::Compose( const vtkMatrix4x4 * vtkmatrix, bool pre )
{
  mitk::AffineGeometryFrame3D::TransformType::Pointer itkTransform = mitk::AffineGeometryFrame3D::TransformType::New();
  TransferVtkMatrixToItkTransform(vtkmatrix, itkTransform.GetPointer());
  Compose(itkTransform, pre);
}

const char* mitk::Geometry3D::GetTransformAsString( TransformType* transformType ) 
{
	static char buffer[255];  
  for ( int j=0; j<255; j++) buffer[j] = '\0';
	ostrstream out( buffer, 255 );

	out << '[';

	for( int i=0; i<3; ++i )
	{
		out << '[';
		for( int j=0; j<3; ++j )
			out << transformType->GetMatrix().GetVnlMatrix().get(i, j) << ' ';
		out << ']';
	}

	out << "][";

	for( int i=0; i<3; ++i )
		out << transformType->GetOffset()[i] << ' ';

	out << "]\0";

	return buffer;
}

bool mitk::Geometry3D::WriteXMLData( XMLWriter& xmlWriter )
{
	if ( m_IndexToObjectTransform.IsNotNull() )
		xmlWriter.WriteProperty( INDEX_TO_OBJECT_TRANSFORM, GetTransformAsString( m_IndexToObjectTransform.GetPointer() ) );

	if ( m_ObjectToNodeTransform.IsNotNull() )
		xmlWriter.WriteProperty( OBJECT_TO_NODE_TRANSFORM, GetTransformAsString( m_ObjectToNodeTransform.GetPointer() ) );

	if ( m_IndexToNodeTransform.IsNotNull() )
		xmlWriter.WriteProperty( INDEX_TO_NODE_TRANSFORM, GetTransformAsString( m_IndexToNodeTransform.GetPointer() ) );

	if ( m_IndexToWorldTransform.IsNotNull() )
		xmlWriter.WriteProperty( INDEX_TO_WORLD_TRANSFORM, GetTransformAsString( m_IndexToWorldTransform.GetPointer() ) );	

  return true;
}

bool mitk::Geometry3D::ReadXMLData( XMLReader& xmlReader ) 
{ 
  xmlReader.GetAttribute( INDEX_TO_OBJECT_TRANSFORM, *m_IndexToObjectTransform.GetPointer() );
  xmlReader.GetAttribute( OBJECT_TO_NODE_TRANSFORM, *m_ObjectToNodeTransform.GetPointer() );
  xmlReader.GetAttribute( INDEX_TO_NODE_TRANSFORM, *m_IndexToNodeTransform.GetPointer() );
  xmlReader.GetAttribute( INDEX_TO_WORLD_TRANSFORM, *m_IndexToWorldTransform.GetPointer() );
  TransferItkToVtkTransform();
	return true;
}

/**
 *
 */
const std::string& mitk::Geometry3D::GetXMLNodeName() const
{
  return XML_NODE_NAME;
}


void mitk::Geometry3D::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << " IndexToWorldTransform: ";
  if(m_IndexToWorldTransform.IsNull())
    os << "NULL" << std::endl;
  else
    m_IndexToWorldTransform->Print(os, indent);

  os << indent << " BoundingBox: ";
  if(m_BoundingBox.IsNull())
    os << "NULL" << std::endl;
  else
    m_BoundingBox->Print(os, indent);

  os << indent << " Origin: " << m_Origin << std::endl;
  os << indent << " ImageGeometry: " << m_ImageGeometry << std::endl;
  os << indent << " Spacing: " << m_Spacing << std::endl;
  os << indent << " TimeBounds: " << m_TimeBounds << std::endl;

  Superclass::PrintSelf(os,indent);
}

mitk::Point3D mitk::Geometry3D::GetCornerPoint(int id) const
{
  assert(id >= 0);
  assert(m_BoundingBox.IsNotNull());

  BoundingBox::BoundsArrayType bounds = m_BoundingBox->GetBounds();

  Point3D cornerpoint;
  switch(id)
  {
  case 0: FillVector3D(cornerpoint, bounds[0],bounds[2],bounds[4]); break;
  case 1: FillVector3D(cornerpoint, bounds[0],bounds[2],bounds[5]); break;
  case 2: FillVector3D(cornerpoint, bounds[0],bounds[3],bounds[4]); break;
  case 3: FillVector3D(cornerpoint, bounds[0],bounds[3],bounds[5]); break;
  case 4: FillVector3D(cornerpoint, bounds[1],bounds[2],bounds[4]); break;
  case 5: FillVector3D(cornerpoint, bounds[1],bounds[2],bounds[5]); break;
  case 6: FillVector3D(cornerpoint, bounds[1],bounds[3],bounds[4]); break;
  case 7: FillVector3D(cornerpoint, bounds[1],bounds[3],bounds[5]); break;
  default: assert(id < 8);
  }
  if(m_ImageGeometry)
  {
    FillVector3D(cornerpoint, cornerpoint[0]-0.5, cornerpoint[1]-0.5, cornerpoint[2]-0.5);
  }
  return m_IndexToWorldTransform->TransformPoint(cornerpoint);
}

mitk::Point3D mitk::Geometry3D::GetCornerPoint(bool xFront, bool yFront, bool zFront) const
{
  assert(m_BoundingBox.IsNotNull());
  BoundingBox::BoundsArrayType bounds = m_BoundingBox->GetBounds();

  Point3D cornerpoint;
  cornerpoint[0] = (xFront ? bounds[0] : bounds[1]);
  cornerpoint[1] = (yFront ? bounds[2] : bounds[3]);
  cornerpoint[2] = (zFront ? bounds[4] : bounds[5]);
  if(m_ImageGeometry)
  {
    FillVector3D(cornerpoint, cornerpoint[0]-0.5, cornerpoint[1]-0.5, cornerpoint[2]-0.5);
  }

  return m_IndexToWorldTransform->TransformPoint(cornerpoint);
}

