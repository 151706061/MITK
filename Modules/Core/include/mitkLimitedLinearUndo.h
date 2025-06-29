/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkLimitedLinearUndo_h
#define mitkLimitedLinearUndo_h

// MITK header
#include "mitkOperationEvent.h"
#include "mitkUndoModel.h"
#include <MitkCoreExports.h>
// STL header
#include <vector>
// ITK header
#pragma GCC visibility push(default)
#include <itkEventObject.h>
#pragma GCC visibility pop

#include <deque>

namespace mitk
{
  //##Documentation
  //## @brief A linear undo model with one undo and one redo stack.
  //##
  //## Derived from UndoModel AND itk::Object. Invokes ITK-events to signal listening
  //## GUI elements, whether each of the stacks is empty or not (to enable/disable button, ...)
  class MITKCORE_EXPORT LimitedLinearUndo : public UndoModel
  {
  public:
    typedef std::deque<UndoStackItem *> UndoContainer;
    typedef std::deque<UndoStackItem *>::reverse_iterator UndoContainerRevIter;

    mitkClassMacro(LimitedLinearUndo, UndoModel);
    itkFactorylessNewMacro(Self);
    itkCloneMacro(Self);

      bool SetOperationEvent(UndoStackItem *stackItem) override;

    //##Documentation
    //## @brief Undoes the last changes
    //##
    //##  Reads the top element of the Undo-Stack,
    //##  executes the operation,
    //##  swaps the OperationEvent-Undo with the Operation
    //##  and sets it to Redo-Stack
    bool Undo() override;
    bool Undo(bool) override;

    //##Documentation
    //## @brief Undoes all changes until ObjectEventID oeid
    virtual bool Undo(int oeid);

    //##Documentation
    //## @brief Undoes the last changes
    //##
    //## Reads the top element of the Redo-Stack,
    //## executes the operation,
    //## swaps the OperationEvent-Operation with the Undo-Operation
    //## and sets it to Undo-Stack
    bool Redo() override;
    bool Redo(bool) override;

    //##Documentation
    //## @brief Redoes all changes until ObjectEventID oeid
    virtual bool Redo(int oeid);

    //##Documentation
    //## @brief Clears UndoList and RedoList
    void Clear() override;

    //##Documentation
    //## @brief Clears the RedoList
    void ClearRedoList() override;

    //##Documentation
    //## @brief True, if RedoList is empty
    bool RedoListEmpty() override;

    //##Documentation
    //## @brief True, if UndoList is empty
    bool UndoListEmpty() override;

    //##Documentation
    //## @brief Gets the limit on the size of the undo history.
    //## The undo limit determines how many items can be stored
    //## in the undo stack. If the value is 0 that means that
    //## there is no limit.
    std::size_t GetUndoLimit() const override;

    //##Documentation
    //## @brief Sets a limit on the size of the undo history.
    //## If the limit is reached, the oldest undo items will
    //## be dropped from the bottom of the undo stack.
    //## The 0 value means that there is no limit.
    //## @param limit the maximum number of items on the stack
    void SetUndoLimit(std::size_t limit) override;

    //##Documentation
    //## @brief Returns the ObjectEventId of the
    //## top element in the OperationHistory
    int GetLastObjectEventIdInList() override;

    //##Documentation
    //## @brief Returns the GroupEventId of the
    //## top element in the OperationHistory
    int GetLastGroupEventIdInList() override;

    //##Documentation
    //## @brief Returns the last specified OperationEvent in Undo-list
    //## corresponding to the given values; if nothing found, then returns nullptr
    OperationEvent *GetLastOfType(OperationActor *destination, OperationType opType) override;

    /**
     * @brief Removes invalid operations from the undo/redo stack.
     * Iterates through m_UndoList and m_RedoList and removes invalid OperationEvents.
     * @return Number of invalid operations removed
     */
    unsigned int RemoveInvalidOperations() override;

  protected:
    //##Documentation
    //## Constructor
    LimitedLinearUndo();

    //##Documentation
    //## Destructor
    ~LimitedLinearUndo() override;

    //## @brief Convenience method to free the memory of
    //## elements in the list and to clear the list
    void ClearList(UndoContainer *list);

    UndoContainer m_UndoList;

    UndoContainer m_RedoList;

  private:
    int FirstObjectEventIdOfCurrentGroup(UndoContainer &stack);

    std::size_t m_UndoLimit;

  };

#pragma GCC visibility push(default)

  /// Some itk events to notify listening GUI elements, when the undo or redo stack is empty (disable undo button)
  /// or when there are items in the stack (enable button)
  itkEventMacroDeclaration(UndoStackEvent, itk::ModifiedEvent);
  itkEventMacroDeclaration(UndoEmptyEvent, UndoStackEvent);
  itkEventMacroDeclaration(RedoEmptyEvent, UndoStackEvent);
  itkEventMacroDeclaration(UndoNotEmptyEvent, UndoStackEvent);
  itkEventMacroDeclaration(RedoNotEmptyEvent, UndoStackEvent);
  /// Additional unused events, if anybody wants to put an artificial limit to the possible number of items in the stack
  itkEventMacroDeclaration(UndoFullEvent, UndoStackEvent);
  itkEventMacroDeclaration(RedoFullEvent, UndoStackEvent);

#pragma GCC visibility pop

} // namespace mitk

#endif
