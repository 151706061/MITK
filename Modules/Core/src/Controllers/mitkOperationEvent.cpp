/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "mitkOperationEvent.h"
#include <itkCommand.h>

int mitk::UndoStackItem::m_CurrObjectEventId = 0;
int mitk::UndoStackItem::m_CurrGroupEventId = 0;

mitk::UndoStackItem::UndoStackItem(std::string description) : m_Reversed(false), m_Description(description)
{
  m_ObjectEventId = GetCurrObjectEventId();
  m_GroupEventId = GetCurrGroupEventId();
}

mitk::UndoStackItem::~UndoStackItem()
{
  // nothing to do
}

int mitk::UndoStackItem::GetCurrObjectEventId()
{
  return m_CurrObjectEventId;
}

int mitk::UndoStackItem::GetCurrGroupEventId()
{
  return m_CurrGroupEventId;
}

void mitk::UndoStackItem::IncCurrObjectEventId()
{
  ++m_CurrObjectEventId;
}

void mitk::UndoStackItem::IncCurrGroupEventId()
{
  ++m_CurrGroupEventId;
}

int mitk::UndoStackItem::GetObjectEventId()
{
  return m_ObjectEventId;
}

int mitk::UndoStackItem::GetGroupEventId()
{
  return m_GroupEventId;
}

std::string mitk::UndoStackItem::GetDescription()
{
  return m_Description;
}

void mitk::UndoStackItem::ReverseOperations()
{
  m_Reversed = !m_Reversed;
}

void mitk::UndoStackItem::ReverseAndExecute()
{
  ReverseOperations();
}

// ******************** mitk::OperationEvent ********************

mitk::Operation *mitk::OperationEvent::GetOperation()
{
  return m_Operation;
}

mitk::OperationEvent::OperationEvent(OperationActor *destination,
                                     Operation *operation,
                                     Operation *undoOperation,
                                     std::string description)
  : UndoStackItem(description),
    m_Destination(destination),
    m_Operation(operation),
    m_UndoOperation(undoOperation),
    m_Invalid(false)
{
  // connect to delete event
  if (auto *object = dynamic_cast<itk::Object *>(m_Destination))
  {
    itk::SimpleMemberCommand<OperationEvent>::Pointer command = itk::SimpleMemberCommand<OperationEvent>::New();
    command->SetCallbackFunction(this, &OperationEvent::OnObjectDeleted);
    m_DelObserver.Reset(object, itk::DeleteEvent(), command);
  }
}

mitk::OperationEvent::~OperationEvent()
{
  delete m_Operation;
  delete m_UndoOperation;
}

//##Documentation
//##  swaps the Undo and Redo- operation and changes m_Reversed
void mitk::OperationEvent::ReverseOperations()
{
  if (m_Operation == nullptr)
    return;

  std::swap(m_Operation, m_UndoOperation);

  UndoStackItem::ReverseOperations();
}

void mitk::OperationEvent::ReverseAndExecute()
{
  ReverseOperations();
  if (m_Destination && m_Operation && !m_Invalid)
    m_Destination->ExecuteOperation(m_Operation);
}

mitk::OperationActor *mitk::OperationEvent::GetDestination()
{
  return m_Destination;
}

void mitk::OperationEvent::OnObjectDeleted()
{
  m_Invalid = true;
}

bool mitk::OperationEvent::IsValid() const
{
  return !m_Invalid
    && m_Operation != nullptr && m_Operation->IsValid()
    && m_UndoOperation != nullptr && m_UndoOperation->IsValid();
}
