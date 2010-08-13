/*=========================================================================
 
 Program:   BlueBerry Platform
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

#include <Poco/ClassLibrary.h>

#include "src/internal/QmitkCommonExtPlugin.h"
#include "src/QmitkExtPreferencePage.h"
#include "src/QmitkInputDevicesPrefPage.h"

POCO_BEGIN_MANIFEST(berry::IBundleActivator)
  POCO_EXPORT_CLASS(QmitkCommonExtPlugin)
POCO_END_MANIFEST

POCO_BEGIN_NAMED_MANIFEST(berryIPreferencePage, berry::IPreferencePage)
  POCO_EXPORT_CLASS(QmitkExtPreferencePage)
  POCO_EXPORT_CLASS(QmitkInputDevicesPrefPage)
POCO_END_MANIFEST

