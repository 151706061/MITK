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
#include "mitkMouseReleaseEvent.h"

mitk::MouseReleaseEvent::MouseReleaseEvent(mitk::BaseRenderer* baseRenderer,
    const mitk::Point2D& mousePosition,
    mitk::MouseButtons buttonStates,
    mitk::ModifierKeys modifiers,
    mitk::MouseButtons eventButton)
: InteractionPositionEvent(baseRenderer, mousePosition, "MouseReleaseEvent")
, m_EventButton(eventButton)
, m_ButtonStates(buttonStates)
, m_Modifiers(modifiers)
{
}

mitk::MouseButtons mitk::MouseReleaseEvent::GetEventButton() const
{
  return m_EventButton;
}

void mitk::MouseReleaseEvent::SetEventButton(MouseButtons buttons)
{
  m_EventButton = buttons;
}


mitk::ModifierKeys mitk::MouseReleaseEvent::GetModifiers() const
{
  return m_Modifiers;
}

mitk::MouseButtons mitk::MouseReleaseEvent::GetButtonStates() const
{
  return m_ButtonStates;
}

void mitk::MouseReleaseEvent::SetModifiers(ModifierKeys modifiers)
{
  m_Modifiers = modifiers;
}

void mitk::MouseReleaseEvent::SetButtonStates(MouseButtons buttons)
{
  m_ButtonStates = buttons;
}

mitk::MouseReleaseEvent::~MouseReleaseEvent()
{
}

bool mitk::MouseReleaseEvent::IsEqual(const mitk::InteractionEvent& interactionEvent) const
{
  const mitk::MouseReleaseEvent& mre = static_cast<const mitk::MouseReleaseEvent&>(interactionEvent);
  return (this->GetEventButton() == mre.GetEventButton() && this->GetModifiers() == mre.GetModifiers()
          && this->GetButtonStates() == mre.GetButtonStates() &&
          Superclass::IsEqual(interactionEvent));
}

bool mitk::MouseReleaseEvent::IsSuperClassOf(const InteractionEvent::Pointer& baseClass) const
{
  return (dynamic_cast<MouseReleaseEvent*>(baseClass.GetPointer()) != NULL) ;
}
