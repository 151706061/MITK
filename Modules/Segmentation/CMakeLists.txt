mitk_create_module(
  INCLUDE_DIRS Algorithms Controllers DataManagement Interactions Rendering SegmentationUtilities/BooleanOperations SegmentationUtilities/MorphologicalOperations
  DEPENDS MitkAlgorithmsExt MitkSurfaceInterpolation MitkGraphAlgorithms MitkContourModel MitkMultilabel MitkBoundingShape
  PACKAGE_DEPENDS
    PUBLIC ITK|QuadEdgeMesh+RegionGrowing httplib 
    PRIVATE ITK|LabelMap+MathematicalMorphology VTK|ImagingGeneral
  TARGET_DEPENDS PRIVATE GrowCut
)

add_subdirectory(cmdapps)
add_subdirectory(Testing)
