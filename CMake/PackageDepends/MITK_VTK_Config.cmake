if(NOT MITK_VTK_Config_INCLUDED)
  set(MITK_VTK_Config_INCLUDED 1)
  find_package(VTK REQUIRED)
  if(VTK_FOUND AND NOT VTK_BUILD_SHARED_LIBS)
    message(FATAL_ERROR "MITK only supports a VTK which was built with shared libraries. Turn on BUILD_SHARED_LIBS in your VTK config.")
  endif()

  include(${VTK_USE_FILE})
  list(APPEND ALL_INCLUDE_DIRECTORIES ${VTK_INCLUDE_DIRS})
  list(APPEND ALL_LIBRARY_DIRS ${VTK_LIBRARY_DIRS})
  if(VTK_FOR_MITK_LIBRARIES)
    list(APPEND ALL_LIBRARIES ${VTK_FOR_MITK_LIBRARIES})
  else()
    # Libraries in mitk/Utilities may depend on VTK but
    # the VTK_FOR_MITK_LIBRARIES variable is not yet set.
    # Supply the VTK libraries manually
    ## VTK6_TODO Remove unneeded Libraries
    list(APPEND ALL_LIBRARIES
    vtkalglib
    vtkChartsCore
    vtkCommonColor
    vtkCommonComputationalGeometry
    vtkCommonCore
    vtkCommonDataModel
    vtkCommonExecutionModel
    vtkCommonMath
    vtkCommonMisc
    vtkCommonSystem
    vtkCommonTransforms
    vtkDICOMParser
    vtkDomainsChemistry
    vtkexoIIc
    vtkexpat
    vtkFiltersAMR
    vtkFiltersCore
    vtkFiltersExtraction
    vtkFiltersFlowPaths
    vtkFiltersGeneral
    vtkFiltersGeneric
    vtkFiltersGeometry
    vtkFiltersHybrid
    vtkFiltersHyperTree
    vtkFiltersImaging
    vtkFiltersModeling
    vtkFiltersParallel
    vtkFiltersParallelImaging
    vtkFiltersProgrammable
    vtkFiltersSelection
    vtkFiltersSources
    vtkFiltersStatistics
    vtkFiltersTexture
    vtkFiltersVerdict
    vtkfreetype
    vtkftgl
    vtkGeovisCore
    vtkgl2ps
    vtkhdf5
    vtkhdf5_hl
    vtkImagingColor
    vtkImagingCore
    vtkImagingFourier
    vtkImagingGeneral
    vtkImagingHybrid
    vtkImagingMath
    vtkImagingMorphological
    vtkImagingSources
    vtkImagingStatistics
    vtkImagingStencil
    vtkInfovisCore
    vtkInfovisLayout
    vtkInteractionImage
    vtkInteractionStyle
    vtkInteractionWidgets
    vtkIOAMR
    vtkIOCore
    vtkIOEnSight
    vtkIOExodus
    vtkIOExport
    vtkIOGeometry
    vtkIOImage
    vtkIOImport
    vtkIOInfovis
    vtkIOLegacy
    vtkIOLSDyna
    vtkIOMINC
    vtkIOMovie
    vtkIONetCDF
    vtkIOParallel
    vtkIOPLY
    vtkIOSQL
    vtkIOVideo
    vtkIOXML
    vtkIOXMLParser
    vtkjpeg
    vtkjsoncpp
    vtklibxml2
    vtkmetaio
    vtkNetCDF
    vtkNetCDF_cxx
    vtkoggtheora
    vtkParallelCore
    vtkpng
    vtkproj4
    vtkRenderingAnnotation
    vtkRenderingContext2D
    vtkRenderingCore
    vtkRenderingFreeType
    vtkRenderingFreeTypeOpenGL
    vtkRenderingGL2PS
    vtkRenderingHybridOpenGL
    vtkRenderingImage
    vtkRenderingLabel
    vtkRenderingLOD
    vtkRenderingOpenGL
    vtkRenderingVolume
    vtkRenderingVolumeAMR
    vtkRenderingVolumeOpenGL
    vtksqlite
    vtksys
    vtkTestingRendering
    vtktiff
    vtkViewsContext2D
    vtkViewsCore
    vtkViewsGeovis
    vtkViewsInfovis
    vtkzlib
    vtkGUISupportQt
    vtkGUISupportQtWebkit
      ${VTK_JPEG_LIBRARIES}
      ${VTK_PNG_LIBRARIES}
      ${VTK_ZLIB_LIBRARIES}
      ${VTK_EXPAT_LIBRARIES}
      ${VTK_FREETYPE_LIBRARIES}
    )
  endif()

endif(NOT MITK_VTK_Config_INCLUDED)
