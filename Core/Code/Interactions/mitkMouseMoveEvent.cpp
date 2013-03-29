/*===================================================================

 The Medical Imaging Interaction Toolkit (MITK)

 Copyright (c) German Cancer Research Center,
 Division of Medical and Biological Informatics.
 All rights reserved.

 This software is distributed WITHOUT ANY WARRANTY; without
 even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.

 See LICENSE.txt or http://www.mitk.org for details.

 ===================================================================*/

#include "mitkException.h"
#include "mitkMouseMoveEvent.h"

mitk::MouseMoveEvent::MouseMoveEvent(mitk::BaseRenderer* baseRenderer, const mitk::Point2D& mousePosition , mitk::MouseButtons buttonStates, mitk::ModifierKeys modifiers)
: InteractionPositionEvent(baseRenderer, mousePosition,  "MouseMoveEvent")
, m_ButtonStates(buttonStates)
, m_Modifiers(modifiers)
{
}

mitk::ModifierKeys mitk::MouseMoveEvent::GetModifiers() const
{
  return m_Modifiers;
}

mitk::MouseButtons mitk::MouseMoveEvent::GetButtonStates() const
{
  return m_ButtonStates;
}

void mitk::MouseMoveEvent::SetModifiers(ModifierKeys modifiers)
{
  m_Modifiers = modifiers;
}

void mitk::MouseMoveEvent::SetButtonStates(MouseButtons buttons)
{
  m_ButtonStates = buttons;
}

mitk::MouseMoveEvent::~MouseMoveEvent()
{
}

bool mitk::MouseMoveEvent::Equals(const mitk::InteractionEvent& interactionEvent) const
{
  const mitk::MouseMoveEvent* mpe = dynamic_cast<const mitk::MouseMoveEvent*>(&interactionEvent);
  if (mpe == NULL)
  {
    return false;
  }
  return (this->GetModifiers() == mpe->GetModifiers() && this->GetButtonStates() == mpe->GetButtonStates() &&
          Superclass::Equals(interactionEvent));
}

bool mitk::MouseMoveEvent::IsSuperClassOf(const InteractionEvent::Pointer& baseClass) const
{
  return (dynamic_cast<MouseMoveEvent*>(baseClass.GetPointer()) != NULL) ;
}
