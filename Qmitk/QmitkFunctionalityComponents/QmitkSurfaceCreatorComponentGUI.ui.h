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



void QmitkSurfaceCreatorComponentGUI::SetDataTreeIterator( mitk::DataTreeIteratorBase * it )
{
   m_DataTreeIterator = it;
//TODO   m_TreeNodeSelector->SetDataTreeNodeIterator(it);
   //m_VolumetryWidget->SetDataTreeNodeIterator(it);
}

QmitkDataTreeComboBox * QmitkSurfaceCreatorComponentGUI::GetTreeNodeSelector()
{
 return m_TreeNodeSelector;
}

QGroupBox* QmitkSurfaceCreatorComponentGUI::GetSelectDataGroupBox()
{
  return m_SurfaceDataGroupBox;
}


QGroupBox * QmitkSurfaceCreatorComponentGUI::GetImageContent()
{
   return m_ImageContent;
}

QGroupBox * QmitkSurfaceCreatorComponentGUI::GetCreateSurfaceGroupBox()
{
   return m_CreateSurfaceGroupBox;
}

QGroupBox * QmitkSurfaceCreatorComponentGUI::GetSurfaceCreatorGroupBox()
{
  return m_SurfaceCreatorGroupBox;
}


QPushButton* QmitkSurfaceCreatorComponentGUI::GetCreateSurfaceButton()
{
   return m_CreateSurfaceButton;
}


QLineEdit* QmitkSurfaceCreatorComponentGUI::GetThresholdLineEdit()
{
  return m_ThresholdLineEdit;
}


int QmitkSurfaceCreatorComponentGUI::GetThreshold()
{
    int value = atoi(m_ThresholdLineEdit->text());
  return value;
}


void QmitkSurfaceCreatorComponentGUI::SetThreshold( int threshold )
{
  m_ThresholdLineEdit->setText(QString::number(threshold));
}


QGroupBox* QmitkSurfaceCreatorComponentGUI::GetShowExpertModeGroupBox()
{
  return m_ShowExpertModeGroupBox;
}


QGroupBox * QmitkSurfaceCreatorComponentGUI::GetExpertModeMedianGroupBox()
{
  return m_ExpertModeMedianGroupBox;
}


QGroupBox* QmitkSurfaceCreatorComponentGUI::GetExpertModeInterpolateGoupBox()
{
return m_ExpertModeInterpolateGroupBox;
}


QGroupBox * QmitkSurfaceCreatorComponentGUI::GetExpertModeSmoothGroupBox()
{
return m_ExpertModeSmoothGroupBox;
}


QGroupBox * QmitkSurfaceCreatorComponentGUI::GetExpertModeGaussGroupBox()
{
return m_ExpertModeGaussGroupBox;
}


QGroupBox * QmitkSurfaceCreatorComponentGUI::GetExpertModeDecimateGroupBox()
{
return m_ExpertModeDecimateGroupBox;
}


QCheckBox* QmitkSurfaceCreatorComponentGUI::GetMedian3DCheckBox()
{
  return m_Median3DCheckBox;
}


QCheckBox * QmitkSurfaceCreatorComponentGUI::GetInterpolateCheckBox()
{
  return m_InterpolateCheckBox;
}


QCheckBox* QmitkSurfaceCreatorComponentGUI::GetSmoothCheckBox()
{
 return m_SmoothCheckBox;
}


QCheckBox* QmitkSurfaceCreatorComponentGUI::GetGaussCheckBox()
{
 return m_GaussCheckBox;
}


QCheckBox * QmitkSurfaceCreatorComponentGUI::GetDecimateCheckBox()
{
 return m_DecimateCheckBox;
}


QGroupBox * QmitkSurfaceCreatorComponentGUI::GetShowSurfaceParameterGroupBox()
{
  return m_ShowSurfaceParameterGroupBox;
}


float QmitkSurfaceCreatorComponentGUI::GetGaussValue()
{
  float value;
  value = atof(m_GausValueLineEdit->text());
  return value;
}

float QmitkSurfaceCreatorComponentGUI::GetDecimateValue()
{
  float value;
  value = atof(m_DecimateValueLineEdit->text());
  return value;
}

int QmitkSurfaceCreatorComponentGUI::GetSmoothIterationValue()
{
  int value;
  value = atoi(m_SmoothIterationValueLineEdit->text());
  return value;
}

float QmitkSurfaceCreatorComponentGUI::GetSmoothRelaxationValue()
{
   float value;
  value = atof(m_SmoothIRelaxationValueLineEdit->text());
  return value;
}



