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


#include "mitkDisplayInteractor.h"

#include <mitkOperationActor.h>
#include <mitkEventMapper.h>
#include <mitkGlobalInteraction.h>
#include <mitkCoordinateSupplier.h>
#include <mitkDisplayCoordinateOperation.h>
#include <mitkDisplayVectorInteractor.h>
#include <mitkBaseRenderer.h>
#include <mitkRenderWindow.h>

#include <mitkInteractionConst.h>

mitk::DisplayInteractor::DisplayInteractor(mitk::BaseRenderer * ren)
{
  m_ParentRenderer = ren;
}

void mitk::DisplayInteractor::ExecuteOperation(mitk::Operation * operation)
{

  bool ok;//as return type

  mitk::DisplayCoordinateOperation* dcOperation=dynamic_cast<mitk::DisplayCoordinateOperation*>(operation);
  if ( dcOperation != NULL )
  {
    /****ZOOM & MOVE of the whole volume****/
    mitk::BaseRenderer* renderer = dcOperation->GetRenderer();
    if( renderer == NULL || (m_ParentRenderer != NULL && m_ParentRenderer != renderer))
      return;
    switch (operation->GetOperationType())
    {
    case OpMOVE :
      {
        renderer->GetDisplayGeometry()->MoveBy(dcOperation->GetLastToCurrentDisplayVector()*(-1.0));
        renderer->GetRenderWindow()->RequestUpdate();
        ok = true;
      }
      break;
    case OpZOOM :
      {
        float distance = dcOperation->GetLastToCurrentDisplayVector()[1];
        distance = (distance > 0 ? 1 : (distance < 0 ? -1 : 0));
        float factor= 1.0 + distance * 0.05;
        //renderer->GetDisplayGeometry()->Zoom(factor, dcOperation->GetStartDisplayCoordinate());
        Point2D center;
        center[0] = renderer->GetDisplayGeometry()->GetDisplayWidth()/2;
        center[1] = renderer->GetDisplayGeometry()->GetDisplayHeight()/2;
        renderer->GetDisplayGeometry()->Zoom(factor, center);
        renderer->GetRenderWindow()->RequestUpdate();
        ok = true;
      }
      break;
    default:
      ;
    }
  }
}


