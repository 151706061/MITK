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

#include "QmitkAboutDialog.h"

#include <QPushButton>
#include <ui_QmitkAboutDialogGUI.h>

#include "QmitkModulesDialog.h"

#include <itkConfigure.h>
#include <vtkConfigure.h>
#include <mitkVersion.h>

QmitkAboutDialog::QmitkAboutDialog(QWidget* parent, Qt::WindowFlags f)
:QDialog(parent, f)
{
  Ui::QmitkAboutDialog gui;
  gui.setupUi(this);

  QString mitkRevision(MITK_REVISION);
  //mitkRevision.replace( QRegExp("[^0-9]+(\\d+).*"), "\\1");
  QString itkVersion = "%1.%2.%3";
  itkVersion = itkVersion.arg(ITK_VERSION_MAJOR).arg(ITK_VERSION_MINOR).arg(ITK_VERSION_PATCH);
  QString vtkVersion = "%1.%2.%3";
  vtkVersion = vtkVersion.arg(VTK_MAJOR_VERSION).arg(VTK_MINOR_VERSION).arg(VTK_BUILD_VERSION);

  gui.m_PropsLabel->setText(gui.m_PropsLabel->text().arg(itkVersion, QT_VERSION_STR, vtkVersion, mitkRevision));

  QPushButton* btnModules = new QPushButton(QIcon(":/qmitk/ModuleView.png"), "Modules");
  gui.m_ButtonBox->addButton(btnModules, QDialogButtonBox::ActionRole);

  connect(btnModules, SIGNAL(clicked()), this, SLOT(ShowModules()));
  connect(gui.m_ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QmitkAboutDialog::~QmitkAboutDialog()
{

}

void QmitkAboutDialog::ShowModules()
{
  QmitkModulesDialog dialog(this);
  dialog.exec();
}
