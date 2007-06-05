/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include "QmitkTreeNodeSelector.h"



void QmitkEmptyComponentGUI::SetDataTreeIterator( mitk::DataTreeIteratorBase * it )
{
   m_DataTreeIterator = it;
//TODO   m_TreeNodeSelector->SetDataTreeNodeIterator(it);
   //m_VolumetryWidget->SetDataTreeNodeIterator(it);
}




QmitkDataTreeComboBox * QmitkEmptyComponentGUI::GetTreeNodeSelector()
{
 return m_TreeNodeSelector;
}


QGroupBox* QmitkEmptyComponentGUI::GetShowEmptyGroupBox()
{
  return m_ShowEmptyGroupBox;
}

QGroupBox* QmitkEmptyComponentGUI::GetEmptyFinderGroupBox()
{
  return m_Empty;
}

QGroupBox* QmitkEmptyComponentGUI::GetSelectDataGroupBox()
{
  return m_EmptySelectDataGroupBox;
}

QGroupBox* QmitkEmptyComponentGUI::GetEmptyValueContent()
{
  return m_EmptyValueContent;
}

QGroupBox* QmitkEmptyComponentGUI::GetImageContent()
{
  return m_ImageContent;
}


QCheckBox* QmitkEmptyComponentGUI::GetDeleteImageIfDeactivatedCheckBox()
{
  return m_DeleteImageIfDeactivatedCheckBox;
}


QGroupBox* QmitkEmptyComponentGUI::GetContainerContent()
{
  return m_ContainerContent;
}
