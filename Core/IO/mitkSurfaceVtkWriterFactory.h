/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2006-04-25 14:34:04 +0200 (Di, 25 Apr 2006) $
Version:   $Revision: 6719 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef SURFACEWRITERFACTORY_H_HEADER_INCLUDED
#define SURFACEWRITERFACTORY_H_HEADER_INCLUDED

#include "itkObjectFactoryBase.h"
#include "mitkBaseData.h"

namespace mitk
{

class SurfaceVtkWriterFactory : public itk::ObjectFactoryBase
{
public:

  mitkClassMacro( mitk::SurfaceVtkWriterFactory, itk::ObjectFactoryBase )

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    static bool IsRegistered = false;
    if ( !IsRegistered )
    {
      SurfaceVtkWriterFactory::Pointer surfaceVtkWriterFactory = SurfaceVtkWriterFactory::New();
      ObjectFactoryBase::RegisterFactory( surfaceVtkWriterFactory );
      IsRegistered = true;
    }
  }

protected:
  SurfaceVtkWriterFactory();
  ~SurfaceVtkWriterFactory();

private:
  SurfaceVtkWriterFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace mitk

#endif


