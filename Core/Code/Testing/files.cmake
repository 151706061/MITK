
# tests with no extra command line parameter
SET(MODULE_TESTS
  mitkCoreObjectFactoryTest.cpp 
  mitkPointSetWriterTest.cpp
  mitkMaterialTest.cpp
  mitkActionTest.cpp
  mitkDataStorageTest.cpp
  mitkEnumerationPropertyTest.cpp
  mitkEventMapperTest.cpp
  mitkEventTest.cpp
  mitkFocusManagerTest.cpp
  mitkGenericPropertyTest.cpp
  mitkGeometry3DTest.cpp
  mitkGeometryDataToSurfaceFilterTest.cpp
  mitkGlobalInteractionTest.cpp
  mitkImageDataItemTest.cpp
  #mitkImageMapper2DTest.cpp
  mitkImageTest.cpp
  mitkImageGeneratorTest.cpp
  mitkBaseDataTest.cpp
  #mitkImageToItkTest.cpp
  mitkInteractorTest.cpp
  mitkITKThreadingTest.cpp
  # mitkLevelWindowManagerTest.cpp
  mitkLevelWindowTest.cpp
  mitkMessageTest.cpp
  #mitkPipelineSmartPointerCorrectnessTest.cpp
  mitkPixelTypeTest.cpp
  mitkPlaneGeometryTest.cpp
  mitkPointSetFileIOTest.cpp
  mitkPointSetTest.cpp
  mitkPointSetInteractorTest.cpp
  mitkNodeDependentPointSetInteractorTest.cpp
  mitkPropertyListTest.cpp
  #mitkRegistrationBaseTest.cpp
  #mitkSegmentationInterpolationTest.cpp
  mitkSlicedGeometry3DTest.cpp
  mitkSliceNavigationControllerTest.cpp
  mitkStateMachineTest.cpp
  mitkStateMachineFactoryTest.cpp
  mitkStateTest.cpp
  mitkSurfaceTest.cpp
  mitkSurfaceToSurfaceFilterTest.cpp
  mitkTimeSlicedGeometryTest.cpp
  mitkTransitionTest.cpp
  mitkUndoControllerTest.cpp
  mitkVtkWidgetRenderingTest.cpp
  mitkVerboseLimitedLinearUndoTest.cpp
  mitkWeakPointerTest.cpp
  mitkTransferFunctionTest.cpp
  #mitkAbstractTransformGeometryTest.cpp
  #mitkPicFileIOTest.cpp
  mitkStepperTest.cpp
  itkTotalVariationDenoisingImageFilterTest.cpp
  mitkPointSetLocaleTest.cpp
  mitkRenderingManagerTest.cpp
  vtkMitkThickSlicesFilterTest.cpp
  mitkNodePredicateSourceTest.cpp
)

# test with image filename as an extra command line parameter
SET(MODULE_IMAGE_TESTS
  mitkSurfaceVtkWriterTest.cpp
  mitkPicFileWriterTest.cpp
  mitkImageWriterTest.cpp
  #mitkImageSliceSelectorTest.cpp
  mitkImageTimeSelectorTest.cpp
  mitkPicFileReaderTest.cpp
  # mitkVtkPropRendererTest.cpp
  mitkDataNodeFactoryTest.cpp
  #mitkSTLFileReaderTest.cpp
)

# list of images for which the tests are run
SET(MODULE_TESTIMAGES
  US4DCyl.pic.gz
  Pic3D.pic.gz
  Pic2DplusT.pic.gz
  BallBinary30x30x30.pic.gz
  binary.stl
  ball.stl
)

SET(MODULE_CUSTOM_TESTS 
    #mitkLabeledImageToSurfaceFilterTest.cpp
    #mitkExternalToolsTest.cpp
    mitkDataNodeTest.cpp
    mitkDicomSeriesReaderTest.cpp
    mitkDICOMLocaleTest.cpp
)

