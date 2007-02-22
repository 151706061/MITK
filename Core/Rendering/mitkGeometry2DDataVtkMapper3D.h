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


#ifndef MITKGEOMETRY2DDATAVTKMAPPER3D_H_HEADER_INCLUDED_C196C71F
#define MITKGEOMETRY2DDATAVTKMAPPER3D_H_HEADER_INCLUDED_C196C71F

#include "mitkCommon.h"
#include "mitkBaseVtkMapper3D.h"
#include "mitkDataTree.h"

#include <vtkSystemIncludes.h>

class vtkActor;
class vtkTexture;
class vtkImageMapToWindowLevelColors;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkDataSetMapper;
class vtkLookupTable;
class vtkAssembly;
class vtkFeatureEdges;
class vtkTubeFilter;


namespace mitk {

class Geometry2DData;
class BaseRenderer;
class ImageMapper2D;

/**
 *  \brief Vtk-based mapper to display a Geometry2D in a 3D window
 * 
 *  \ingroup Mapper
 */
class Geometry2DDataVtkMapper3D : public BaseVtkMapper3D
{
public:
  mitkClassMacro(Geometry2DDataVtkMapper3D, BaseVtkMapper3D);

  itkNewMacro(Geometry2DDataVtkMapper3D);

  /**
   *  \brief Get the Geometry2DData to map
   */
  virtual const mitk::Geometry2DData *GetInput();

  /**
   * \brief All images found when traversing the (sub-) tree starting at
   * \a iterator which are resliced by an ImageMapper2D will be mapped.
   */
  virtual void SetDataIteratorForTexture(
    const mitk::DataTreeIteratorBase *iterator );

protected:
  Geometry2DDataVtkMapper3D();

  virtual ~Geometry2DDataVtkMapper3D();

  virtual void GenerateData(mitk::BaseRenderer* renderer);

  /**
   * \brief Construct an extended lookup table from the given one.
   *
   * In order to overlay differently sized images over each other, it is 
   * necessary to have a special translucent value, so that the empty
   * surroundings of the smaller image do not occlude the bigger image.
   *
   * The smallest possible short (-32768) is used for this. Due to the
   * implementation of vtkLookupTable, the lookup table needs to be extended
   * so that this value is included. Entries between -32768 and the actual
   * table minimum will be set to the lowest entry in the input lookup table.
   * From this point onward, the input lookup is just copied into the output
   * lookup table.
   *
   * See also mitk::ImageMapper2D, where -32768 is defined as BackgroundLevel
   * for resampling.
   *
   * NOTE: This method is currently not used; to make sure that the plane is
   * not rendered with translucent "holes", a black background plane is first
   * rendered under all other planes.
   */
  virtual void BuildPaddedLookupTable( 
    vtkLookupTable *inputLookupTable, vtkLookupTable *outputLookupTable,
    vtkFloatingPointType min, vtkFloatingPointType max );

  int FindPowerOfTwo( int i );


  /** \brief PropAssembly to hold the plane and its tubal frame */
  vtkAssembly *m_Prop3DAssembly;

  /** \brief Edge extractor for tube-shaped frame */
  vtkFeatureEdges *m_Edges;

  /** \brief Source to create the tube-shaped frame  */
  vtkTubeFilter *m_EdgeTuber;

  /** \brief Mapper for the tube-shaped frame  */
  vtkPolyDataMapper *m_EdgeMapper;

  /** \brief Actor for the tube-shaped frame */
  vtkActor *m_EdgeActor;


  mitk::DataTreeIteratorClone m_DataTreeIterator;


  /** A default grayscale lookup-table, used for reference */
  vtkLookupTable *m_DefaultLookupTable;


  /** \brief List holding the vtkActor to map the image into 3D for each 
   * ImageMapper 
   */
  typedef std::map< mitk::ImageMapper2D *, vtkActor * > ActorList;
  ActorList m_ImageActors;

  struct LookupTableProperties
  {
    vtkLookupTable *LookupTableSource;
    vtkFloatingPointType windowMin;
    vtkFloatingPointType windowMax;
  };

  typedef std::map< mitk::ImageMapper2D *, LookupTableProperties > 
    LookupTablePropertiesList;

  /** \brief List holding some lookup table properties of the previous pass */
  LookupTablePropertiesList m_LookupTableProperties;

  typedef std::multimap< int, vtkActor * > LayerSortedActorList;

  /** \brief List holding the last update times for each ImageMapper. */
  typedef std::map< mitk::ImageMapper2D *, int > LastUpdateTimeList;
  LastUpdateTimeList m_LastTextureUpdateTimes;

};

} // namespace mitk



#endif /* MITKGEOMETRY2DDATAVTKMAPPER3D_H_HEADER_INCLUDED_C196C71F */
