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
#include <QmitkStringPropertyEditor.h>

QmitkStringPropertyEditor::QmitkStringPropertyEditor( mitk::StringProperty* property, QWidget* parent, const char* name )
: PropertyEditor( property ),
  QLineEdit( parent, name ),
  m_StringProperty(property)
{
  PropertyChanged();
  connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
}

QmitkStringPropertyEditor::~QmitkStringPropertyEditor()
{
}

void QmitkStringPropertyEditor::PropertyChanged()
{
  if ( m_Property )
    setText( m_StringProperty->GetValue() );
}

void QmitkStringPropertyEditor::PropertyRemoved()
{
  m_Property = NULL;
  m_StringProperty = NULL;
  setText("n/a");
}

void QmitkStringPropertyEditor::onTextChanged(const QString& text)
{
  BeginModifyProperty();  // deregister from events
  
  m_StringProperty->SetValue(text);
  
  EndModifyProperty();  // again register for events
}

