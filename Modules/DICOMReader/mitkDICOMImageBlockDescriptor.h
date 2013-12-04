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

#ifndef mitkDICOMImageBlockDescriptor_h
#define mitkDICOMImageBlockDescriptor_h

#include "mitkImage.h"

#include "mitkDICOMEnums.h"

#include "DICOMReaderExports.h"

namespace mitk
{

class DICOMReader_EXPORT DICOMImageBlockDescriptor
{
  public:

    DICOMImageBlockDescriptor();
    ~DICOMImageBlockDescriptor();

    DICOMImageBlockDescriptor(const DICOMImageBlockDescriptor& other);
    DICOMImageBlockDescriptor& operator=(const DICOMImageBlockDescriptor& other);

    void SetFilenames(StringList filenames);
    const StringList& GetFilenames() const;

    void SetMitkImage(Image::Pointer image);
    Image::Pointer GetMitkImage() const;

    void SetSliceIsLoaded(unsigned int index, bool isLoaded);
    bool IsSliceLoaded(unsigned int index) const;
    bool AllSlicesAreLoaded() const;

    void SetPixelsInterpolated(bool pixelsAreInterpolated);
    bool GetPixelsInterpolated() const;

    void SetPixelSpacingInterpretation( PixelSpacingInterpretation interpretation );
    PixelSpacingInterpretation GetPixelSpacingInterpretation() const;

  private:

    StringList m_Filenames;
    Image::Pointer m_MitkImage;
    BoolList m_SliceIsLoaded;
    bool m_PixelsInterpolated;
    PixelSpacingInterpretation m_PixelSpacingInterpretation;
};

}

#endif
