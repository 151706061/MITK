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

#ifndef COREOBJECTFACTORY_H_INCLUDED
#define COREOBJECTFACTORY_H_INCLUDED

#include <list>

#include "mitkCommon.h"
#include "mitkCoreObjectFactoryBase.h"
#include "mitkFileWriterWithInformation.h"
namespace mitk {

class Event;

class CoreObjectFactory : public CoreObjectFactoryBase
{
  public:
    mitkClassMacro(CoreObjectFactory,CoreObjectFactoryBase);
    itkFactorylessNewMacro(CoreObjectFactory);
    typedef std::list<mitk::FileWriterWithInformation::Pointer> FileWriterList;
    Mapper::Pointer CreateMapper(mitk::DataTreeNode* node, MapperSlotId slotId);
    virtual void SetDefaultProperties(mitk::DataTreeNode* node);
    virtual const char* GetFileExtensions();
    virtual const char* GetSaveFileExtensions();
    virtual FileWriterList GetFileWriters();
    itk::Object::Pointer CreateCoreObject( const std::string& className );
    virtual void MapEvent(const mitk::Event* event, const int eventID);
    static Pointer GetInstance();
  protected:
    CoreObjectFactory(); 
    FileWriterList m_FileWriters;
};

} // namespace mitk

#endif /* MAPPERFACTORY_H_HEADER_INCLUDED_C179C58F */
