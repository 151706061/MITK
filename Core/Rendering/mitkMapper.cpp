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


#include "mitkMapper.h"
#include "mitkDataTreeNode.h"
#include "mitkBaseRenderer.h"

const std::string mitk::Mapper::XML_NODE_NAME = "mapper";

mitk::Mapper::Mapper()
: m_TimeStep( 0 )
{

}


mitk::Mapper::~Mapper()
{
}

mitk::BaseData* mitk::Mapper::GetData() const
{
  return m_DataTreeNode->GetData();
}

mitk::DataTreeNode* mitk::Mapper::GetDataTreeNode() const
{
  itkDebugMacro("returning DataTreeNode address " << this->m_DataTreeNode );
  return this->m_DataTreeNode.GetPointer();
} 


bool mitk::Mapper::GetColor(float rgb[3], mitk::BaseRenderer* renderer, const char* name) const
{
    const mitk::DataTreeNode* node=GetDataTreeNode();
    if(node==NULL)
        return false;

    return node->GetColor(rgb, renderer, name);
}

bool mitk::Mapper::GetVisibility(bool &visible, mitk::BaseRenderer* renderer, const char* name) const
{
    const mitk::DataTreeNode* node=GetDataTreeNode();
    if(node==NULL)
        return false;

    return node->GetVisibility(visible, renderer, name);
}

bool mitk::Mapper::GetOpacity(float &opacity, mitk::BaseRenderer* renderer, const char* name) const
{
    const mitk::DataTreeNode* node=GetDataTreeNode();
    if(node==NULL)
        return false;

    return node->GetOpacity(opacity, renderer, name);
}

bool mitk::Mapper::GetLevelWindow(mitk::LevelWindow& levelWindow, mitk::BaseRenderer* renderer, const char* name) const
{
    const mitk::DataTreeNode* node=GetDataTreeNode();
    if(node==NULL)
        return false;

    return node->GetLevelWindow(levelWindow, renderer, name);
}

bool mitk::Mapper::IsVisible(mitk::BaseRenderer* renderer, const char* name) const
{
    bool visible=true;
    GetVisibility(visible, renderer, name);
    return visible;
}

void mitk::Mapper::GenerateData()
{
}

void mitk::Mapper::GenerateData(mitk::BaseRenderer* /*renderer*/)
{
}

void mitk::Mapper::CalculateTimeStep( mitk::BaseRenderer *renderer )
{
  if ( renderer != NULL )
  {
    m_TimeStep = renderer->GetTimeStep();
  }
  else
  {
    m_TimeStep = 0;
  }
  /*
  //
  // get the world time
  //
  const Geometry2D* worldGeometry = renderer->GetCurrentWorldGeometry2D();
  assert( worldGeometry != NULL );
  ScalarType time = worldGeometry->GetTimeBounds()[ 0 ];

  //
  // convert the world time in time steps of the input object
  //
  int m_TimeStep=0;
  if ( time > ScalarTypeNumericTraits::NonpositiveMin() )
  {
    m_TimeStep = inputTimeGeometry->MSToTimeStep( time );
  }
  */
}

void mitk::Mapper::Update(mitk::BaseRenderer *renderer)
{
  const DataTreeNode* node = GetDataTreeNode();
  assert(node!=NULL);

  this->CalculateTimeStep( renderer );
  
  //safty cause there are datatreenodes, that have no defined data (video-nodes and root)
  unsigned int dataMTime = 0;
  mitk::BaseData::Pointer data = static_cast<mitk::BaseData *>(node->GetData());
  if (data.IsNotNull())
  {
    dataMTime = data->GetMTime();
  }

  if(
      (m_LastUpdateTime < GetMTime()) ||
      (m_LastUpdateTime < node->GetDataReferenceChangedTime()) ||
      (m_LastUpdateTime < dataMTime) ||
      (m_LastUpdateTime < renderer->GetTimeStepUpdateTime())
    )
  {
    this->GenerateData();
    m_LastUpdateTime.Modified();
  }

  this->GenerateData(renderer);
}

const std::string& mitk::Mapper::GetXMLNodeName() const
{
  return XML_NODE_NAME;
}
