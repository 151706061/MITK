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


#ifndef QMITKABORTEVENTFILTER_H_HEADER_INCLUDED_C135A197
#define QMITKABORTEVENTFILTER_H_HEADER_INCLUDED_C135A197

#include <qobject.h>
#include <utility>
#include <queue>

/**
 */
class QmitkAbortEventFilter : public QObject
{
public:

  static QmitkAbortEventFilter* GetInstance();

  virtual ~QmitkAbortEventFilter();

  void ProcessEvents();
  void IssueQueuedEvents();
 
protected:
  QmitkAbortEventFilter();

  bool eventFilter( QObject* object, QEvent* event );
  
private:
  typedef std::pair< QObject*, QEvent* > ObjectEventPair;

  typedef std::queue< ObjectEventPair > EventQueue;

  EventQueue m_EventQueue;
};


#endif /* QMITKABORTEVENTFILTER_H_HEADER_INCLUDED_C135A197 */
