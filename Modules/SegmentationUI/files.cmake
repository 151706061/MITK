set( CPP_FILES
Qmitk/QmitkSegWithPreviewToolGUIBase.cpp
Qmitk/QmitkMultiLabelSegWithPreviewToolGUIBase.cpp
Qmitk/QmitkBinaryThresholdToolGUIBase.cpp
Qmitk/QmitkBinaryThresholdToolGUI.cpp
Qmitk/QmitkBinaryThresholdULToolGUI.cpp
Qmitk/QmitkCalculateGrayValueStatisticsToolGUI.cpp
Qmitk/QmitkConfirmSegmentationDialog.cpp
Qmitk/QmitkCopyToClipBoardDialog.cpp
Qmitk/QmitkDrawPaintbrushToolGUI.cpp
Qmitk/QmitkErasePaintbrushToolGUI.cpp
Qmitk/QmitkLiveWireTool2DGUI.cpp
Qmitk/QmitkNewSegmentationDialog.cpp
Qmitk/QmitkOtsuTool3DGUI.cpp
Qmitk/QmitkPaintbrushToolGUI.cpp
Qmitk/QmitkPickingToolGUI.cpp
Qmitk/QmitkPixelManipulationToolGUI.cpp
Qmitk/QmitkSlicesInterpolator.cpp
Qmitk/QmitkToolGUI.cpp
Qmitk/QmitkToolGUIArea.cpp
Qmitk/QmitkToolSelectionBox.cpp
Qmitk/QmitknnUNetToolGUI.cpp
Qmitk/QmitknnUNetToolSlots.cpp
#Added from ML
Qmitk/QmitkLabelSetWidget.cpp
Qmitk/QmitkSurfaceStampWidget.cpp
Qmitk/QmitkMaskStampWidget.cpp
Qmitk/QmitkSliceBasedInterpolatorWidget.cpp
Qmitk/QmitkStaticDynamicSegmentationDialog.cpp
Qmitk/QmitkSurfaceBasedInterpolatorWidget.cpp
Qmitk/QmitkSimpleLabelSetListWidget.cpp
)

set(MOC_H_FILES
Qmitk/QmitkSegWithPreviewToolGUIBase.h
Qmitk/QmitkMultiLabelSegWithPreviewToolGUIBase.h
Qmitk/QmitkBinaryThresholdToolGUIBase.h
Qmitk/QmitkBinaryThresholdToolGUI.h
Qmitk/QmitkBinaryThresholdULToolGUI.h
Qmitk/QmitkCalculateGrayValueStatisticsToolGUI.h
Qmitk/QmitkConfirmSegmentationDialog.h
Qmitk/QmitkCopyToClipBoardDialog.h
Qmitk/QmitkDrawPaintbrushToolGUI.h
Qmitk/QmitkErasePaintbrushToolGUI.h
Qmitk/QmitkLiveWireTool2DGUI.h
Qmitk/QmitkNewSegmentationDialog.h
Qmitk/QmitkOtsuTool3DGUI.h
Qmitk/QmitkPaintbrushToolGUI.h
Qmitk/QmitkPickingToolGUI.h
Qmitk/QmitkPixelManipulationToolGUI.h
Qmitk/QmitkSlicesInterpolator.h
Qmitk/QmitkToolGUI.h
Qmitk/QmitkToolGUIArea.h
Qmitk/QmitkToolSelectionBox.h
Qmitk/QmitknnUNetToolGUI.h
Qmitk/QmitknnUNetGPU.h
Qmitk/QmitknnUNetEnsembleLayout.h
Qmitk/QmitknnUNetFolderParser.h
#Added from ML
Qmitk/QmitkLabelSetWidget.h
Qmitk/QmitkSurfaceStampWidget.h
Qmitk/QmitkMaskStampWidget.h
Qmitk/QmitkSliceBasedInterpolatorWidget.h
Qmitk/QmitkStaticDynamicSegmentationDialog.h
Qmitk/QmitkSurfaceBasedInterpolatorWidget.h
Qmitk/QmitkSimpleLabelSetListWidget.h
)

set(UI_FILES
Qmitk/QmitkConfirmSegmentationDialog.ui
Qmitk/QmitkOtsuToolWidgetControls.ui
Qmitk/QmitkLiveWireTool2DGUIControls.ui
#Added from ML
Qmitk/QmitkLabelSetWidgetControls.ui
Qmitk/QmitkSurfaceStampWidgetGUIControls.ui
Qmitk/QmitkMaskStampWidgetGUIControls.ui
Qmitk/QmitkSliceBasedInterpolatorWidgetGUIControls.ui
Qmitk/QmitkSurfaceBasedInterpolatorWidgetGUIControls.ui
Qmitk/QmitknnUNetToolGUIControls.ui
)

set(QRC_FILES
  resources/SegmentationUI.qrc
)
