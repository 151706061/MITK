/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
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

#include "QmitkRenderingManager.h"


#include "mitkGeometry3D.h"
#include "mitkBaseRenderer.h"
#include "mitkSliceNavigationController.h"

#include <QApplication>


QmitkRenderingManager
::QmitkRenderingManager()
{
  
}


void
QmitkRenderingManager
::DoMonitorRendering()
{
  
}


void
QmitkRenderingManager
::DoFinishAbortRendering()
{
  
}


QmitkRenderingManager
::~QmitkRenderingManager()
{
}


void
QmitkRenderingManager
::GenerateRenderingRequestEvent()
{
  QApplication::postEvent( this, new QmitkRenderingRequestEvent );
}


bool 
QmitkRenderingManager
::event( QEvent *event ) 
{
  if ( event->type() == (QEvent::Type) QmitkRenderingRequestEvent::RenderingRequest )
{
    // Directly process all pending rendering requests
    this->ExecutePendingRequests();
    return true;
}

  return false;
}

