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


#ifndef MAPPER2D_H_HEADER_INCLUDED_C1C5453B
#define MAPPER2D_H_HEADER_INCLUDED_C1C5453B

#include "mitkCommon.h"
#include "mitkMapper.h"
#include "mitkGeometry3D.h"
#include "mitkDisplayGeometry.h"
#include "mitkBaseRenderer.h"
#include "mitkImageChannelSelector.h"

namespace mitk {

//##ModelId=3E3AA308024B
//##Documentation
//## @brief Base class of all Mappers for 2D display
//## @ingroup Mapper
//## Base class of all Mappers for 2D display, i.e., a frontal view on a plane
//## display area, so nothing rotated in 3D space as, e.g., a plane in 3D space
//## (such things are done by subclasses of Mapper3D).
//## @note [not yet used:] SetGeometry3D() tells the Mapper2D which slices will
//## potentially be requested.
class Mapper2D : public Mapper
{
public:
	mitkClassMacro(Mapper2D,Mapper);

    //##ModelId=3E3C45A0009E
    //##Documentation
    //## @brief Set Geometry3D containing the all possible Geometry2D that may be requested for mapping
    //## @sa m_Geometry3D
    virtual void SetGeometry3D(const mitk::Geometry3D* aGeometry3D);

	/** Method for creation through the object factory. */
//	itkNewMacro(Self);
protected:
    //##ModelId=3E3C46810341
    Mapper2D();

    //##ModelId=3E3C4681035F
    virtual ~Mapper2D();

    //##ModelId=3EDD039F03E6
    //##Documentation
    //## @brief Set Geometry3D containing the all possible Geometry2D that may be requested for mapping
    //## @note not yet implemented
    //## The idea was that this allows storing/pre-fetching of data required for mapping.
    //## Should be called by SliceNavigationController.
    //## @todo check design and implement when implementing (sub-)classes of SliceNavigationController
    Geometry3D::ConstPointer m_Geometry3D;
};

} // namespace mitk



#endif /* MAPPER2D_H_HEADER_INCLUDED_C1C5453B */
