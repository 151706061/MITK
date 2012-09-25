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

#include "ChangeTextDescriptor.h"
#include "ExtensionPointDefinitionConstants.h" // <= kann nach �nderungen weg!!!


ChangeTextDescriptor::ChangeTextDescriptor(berry::IConfigurationElement::Pointer changeTextExtensionPoint)
: m_ChangeTextExtensionPoint(changeTextExtensionPoint)
{
}

ChangeTextDescriptor::~ChangeTextDescriptor()
{
}

IChangeText::Pointer ChangeTextDescriptor::CreateChangeText()
{
  if(this->m_ChangeText == 0)
  {
    // "class" refers to xml attribute in a xml tag
    this->m_ChangeText = this->m_ChangeTextExtensionPoint
      ->CreateExecutableExtension<IChangeText>(ExtensionPointDefinitionConstants::CHANGETEXT_XMLATTRIBUTE_CLASS); // <= muss auch ge�ndert werden!!!
    if (this->m_ChangeText == 0)
    {
      // support legacy BlueBerry extensions                        // <= ist das �berhaupt notwendig?!
      this->m_ChangeText = this->m_ChangeTextExtensionPoint
          ->CreateExecutableExtension<IChangeText>(
            ExtensionPointDefinitionConstants::CHANGETEXT_XMLATTRIBUTE_CLASS, // <= m�sste auch ge�ndert werden!!!
            IChangeText::GetManifestName());
    }
  }
  return this->m_ChangeText;
}

std::string ChangeTextDescriptor::GetID() const 
{
  std::string idOfExtensionPoint;
  this->m_ChangeTextExtensionPoint->GetAttribute(ExtensionPointDefinitionConstants::CHANGETEXT_XMLATTRIBUTE_ID,idOfExtensionPoint); // <= muss auch ge�ndert werden!!!
  return idOfExtensionPoint;
}

std::string ChangeTextDescriptor::GetDescription() const
{
  std::vector<berry::IConfigurationElement::Pointer> 
    descriptions(this->m_ChangeTextExtensionPoint->GetChildren(ExtensionPointDefinitionConstants::CHANGETEXT_XMLATTRIBUTE_DESCRIPTION)); // <= muss auch ge�ndert werden!!!

  if(!descriptions.empty())
  {
    return descriptions[0]->GetValue();
  }
  return "";
}

std::string ChangeTextDescriptor::GetName() const
{
  std::string name;
  this->m_ChangeTextExtensionPoint->GetAttribute(ExtentionPointDefinitionConstants::CHANGETEXT_XMLATTRIBUTE_NAME,name);
  return name;
}

bool ChangeTextDescriptor::operator==(const Object* object) const
{
  if (const ChangeTextDescriptor* other = dynamic_cast<const ChangeTextDescriptor*>(object))
  {
    return this->GetID() == other->GetID();
  }
  return false;
}

