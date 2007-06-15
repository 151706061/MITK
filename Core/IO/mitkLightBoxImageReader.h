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

#ifndef LIGHTBOXIMAGEREADER_H_HEADER_INCLUDED_C1F48A22
#define LIGHTBOXIMAGEREADER_H_HEADER_INCLUDED_C1F48A22

#include "mitkImageSource.h"
#include "mitkChiliPlugin.h"
#include <mitkPropertyList.h>

class QcLightbox;

namespace mitk {

  /** Documentation
  @brief Read images from Chili LightBox
  @ingroup Process
  @ingroup Chili

  this is the default-implementation, for the real implementation look at mitkLightBoxImageReaderImpl
  */
  class LightBoxImageReader : public ImageSource
  {
    public:
      /** Standard class typedefs. */
      mitkClassMacro( LightBoxImageReader, ImageSource );

      /** Method for creation through the object factory. */
      itkNewMacro(Self);

      /** Set the lightbox to read from. */
      virtual void SetLightBox( QcLightbox* lightbox );

      /**Set the lightbox to read from to the current lightbox. */
      virtual void SetLightBoxToCurrentLightBox();

      /**
      Return a propertylist from the current selected lightBox.
      For the tags look at mitkLightBoxImageReaderImpl.cpp.
      This method reads the information from the pic-header or the dicom-header. 
      The property "name" is set and the same like the tag "Chili: SERIES DESCRIPTION".
      */
      virtual const mitk::PropertyList::Pointer GetImageTagsAsPropertyList();

      /** Get the lightbox to read from. */
      virtual QcLightbox* GetLightBox() const;

      virtual ~LightBoxImageReader() {}
    protected:
      LightBoxImageReader() {}
  };

} // namespace mitk


#endif /* LIGHTBOXIMAGEREADER_H_HEADER_INCLUDED_C1F48A22 */
