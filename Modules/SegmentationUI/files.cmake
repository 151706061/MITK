set(CPP_FILES
  Qmitk/QmitkSegWithPreviewToolGUIBase.cpp
  Qmitk/QmitkMultiLabelSegWithPreviewToolGUIBase.cpp
  Qmitk/QmitkBinaryThresholdToolGUIBase.cpp
  Qmitk/QmitkBinaryThresholdToolGUI.cpp
  Qmitk/QmitkBinaryThresholdULToolGUI.cpp
  Qmitk/QmitkConfirmSegmentationDialog.cpp
  Qmitk/QmitkCopyLabelToGroupDialog.cpp
  Qmitk/QmitkCopyToClipBoardDialog.cpp
  Qmitk/QmitkDrawPaintbrushToolGUI.cpp
  Qmitk/QmitkErasePaintbrushToolGUI.cpp
  Qmitk/QmitkEditableContourToolGUIBase.cpp
  Qmitk/QmitkGrowCutToolGUI.cpp
  Qmitk/QmitkLiveWireTool2DGUI.cpp
  Qmitk/QmitkLassoToolGUI.cpp
  Qmitk/QmitkOtsuTool3DGUI.cpp
  Qmitk/QmitkPaintbrushToolGUI.cpp
  Qmitk/QmitkPickingToolGUI.cpp
  Qmitk/QmitkSlicesInterpolator.cpp
  Qmitk/QmitkToolGUI.cpp
  Qmitk/QmitkToolGUIArea.cpp
  Qmitk/QmitkToolSelectionBox.cpp
  Qmitk/QmitknnUNetFolderParser.cpp
  Qmitk/QmitknnUNetToolGUI.cpp
  Qmitk/QmitknnUNetWorker.cpp
  Qmitk/QmitknnUNetGPU.cpp
  Qmitk/QmitkSurfaceStampWidget.cpp
  Qmitk/QmitkStaticDynamicSegmentationDialog.cpp
  Qmitk/QmitkSimpleLabelSetListWidget.cpp
  Qmitk/QmitkSegmentationTaskListWidget.cpp
  Qmitk/QmitkTotalSegmentatorToolGUI.cpp
  Qmitk/QmitkSetupVirtualEnvUtil.cpp
  Qmitk/QmitkMultiLabelInspector.cpp
  Qmitk/QmitkMultiLabelManager.cpp
  Qmitk/QmitkMultiLabelTreeModel.cpp
  Qmitk/QmitkMultiLabelTreeView.cpp
  Qmitk/QmitkMultiLabelPresetHelper.cpp
  Qmitk/QmitkMultiLabelSelectionButton.cpp
  Qmitk/QmitkMultiLabelSelectionLabel.cpp
  Qmitk/QmitkMultiLabelSelectionWidget.cpp
  Qmitk/QmitkLabelColorItemDelegate.cpp
  Qmitk/QmitkLabelToggleItemDelegate.cpp
  Qmitk/QmitkFindSegmentationTaskDialog.cpp
  Qmitk/QmitkSegmentAnythingToolGUI.cpp
  Qmitk/QmitkMedSAMToolGUI.cpp
  Qmitk/QmitkMonaiLabelToolGUI.cpp
  Qmitk/QmitkMonaiLabel2DToolGUI.cpp
  Qmitk/QmitkMonaiLabel3DToolGUI.cpp
  SegmentationUtilities/QmitkBooleanOperationsWidget.cpp
  SegmentationUtilities/QmitkImageMaskingWidget.cpp
  SegmentationUtilities/QmitkMorphologicalOperationsWidget.cpp
  SegmentationUtilities/QmitkConvertToMultiLabelSegmentationWidget.cpp
  SegmentationUtilities/QmitkExtractFromMultiLabelSegmentationWidget.cpp
)

set(H_FILES
  Qmitk/QmitkMultiLabelPresetHelper.h
)
set(MOC_H_FILES
  Qmitk/QmitkSegWithPreviewToolGUIBase.h
  Qmitk/QmitkMultiLabelSegWithPreviewToolGUIBase.h
  Qmitk/QmitkBinaryThresholdToolGUIBase.h
  Qmitk/QmitkBinaryThresholdToolGUI.h
  Qmitk/QmitkBinaryThresholdULToolGUI.h
  Qmitk/QmitkConfirmSegmentationDialog.h
  Qmitk/QmitkCopyLabelToGroupDialog.h
  Qmitk/QmitkCopyToClipBoardDialog.h
  Qmitk/QmitkDrawPaintbrushToolGUI.h
  Qmitk/QmitkErasePaintbrushToolGUI.h
  Qmitk/QmitkEditableContourToolGUIBase.h
  Qmitk/QmitkGrowCutToolGUI.h
  Qmitk/QmitkLiveWireTool2DGUI.h
  Qmitk/QmitkLassoToolGUI.h
  Qmitk/QmitkOtsuTool3DGUI.h
  Qmitk/QmitkPaintbrushToolGUI.h
  Qmitk/QmitkPickingToolGUI.h
  Qmitk/QmitkSlicesInterpolator.h
  Qmitk/QmitkToolGUI.h
  Qmitk/QmitkToolGUIArea.h
  Qmitk/QmitkToolSelectionBox.h
  Qmitk/QmitknnUNetFolderParser.h
  Qmitk/QmitknnUNetToolGUI.h
  Qmitk/QmitknnUNetGPU.h
  Qmitk/QmitknnUNetWorker.h
  Qmitk/QmitknnUNetEnsembleLayout.h
  Qmitk/QmitkSurfaceStampWidget.h
  Qmitk/QmitkStaticDynamicSegmentationDialog.h
  Qmitk/QmitkSimpleLabelSetListWidget.h
  Qmitk/QmitkSegmentationTaskListWidget.h
  Qmitk/QmitkTotalSegmentatorToolGUI.h
  Qmitk/QmitkSetupVirtualEnvUtil.h
  Qmitk/QmitkMultiLabelInspector.h
  Qmitk/QmitkMultiLabelManager.h
  Qmitk/QmitkMultiLabelSelectionLabel.h
  Qmitk/QmitkMultiLabelSelectionWidget.h
  Qmitk/QmitkMultiLabelSelectionButton.h
  Qmitk/QmitkMultiLabelTreeModel.h
  Qmitk/QmitkMultiLabelTreeView.h
  Qmitk/QmitkLabelColorItemDelegate.h
  Qmitk/QmitkLabelToggleItemDelegate.h
  Qmitk/QmitkFindSegmentationTaskDialog.h
  Qmitk/QmitkSegmentAnythingToolGUI.h
  Qmitk/QmitkMedSAMToolGUI.h
  Qmitk/QmitkMonaiLabelToolGUI.h
  Qmitk/QmitkMonaiLabel2DToolGUI.h
  Qmitk/QmitkMonaiLabel3DToolGUI.h

  SegmentationUtilities/QmitkBooleanOperationsWidget.h
  SegmentationUtilities/QmitkImageMaskingWidget.h
  SegmentationUtilities/QmitkMorphologicalOperationsWidget.h
  SegmentationUtilities/QmitkConvertToMultiLabelSegmentationWidget.h
  SegmentationUtilities/QmitkExtractFromMultiLabelSegmentationWidget.h
)

set(UI_FILES
  Qmitk/QmitkConfirmSegmentationDialog.ui
  Qmitk/QmitkCopyLabelToGroupDialog.ui
  Qmitk/QmitkGrowCutToolWidgetControls.ui
  Qmitk/QmitkOtsuToolWidgetControls.ui
  Qmitk/QmitkSurfaceStampWidgetGUIControls.ui
  Qmitk/QmitknnUNetToolGUIControls.ui
  Qmitk/QmitkEditableContourToolGUIControls.ui
  Qmitk/QmitkSegmentationTaskListWidget.ui
  Qmitk/QmitkTotalSegmentatorGUIControls.ui
  Qmitk/QmitkMultiLabelInspectorControls.ui
  Qmitk/QmitkMultiLabelManagerControls.ui
  Qmitk/QmitkMultiLabelSelectionLabel.ui
  Qmitk/QmitkMultiLabelSelectionWidget.ui
  Qmitk/QmitkFindSegmentationTaskDialog.ui
  Qmitk/QmitkSegmentAnythingGUIControls.ui
  Qmitk/QmitkMedSAMGUIControls.ui
  Qmitk/QmitkMonaiLabelToolGUIControls.ui
  SegmentationUtilities/QmitkBooleanOperationsWidgetControls.ui
  SegmentationUtilities/QmitkImageMaskingWidgetControls.ui
  SegmentationUtilities/QmitkMorphologicalOperationsWidgetControls.ui
  SegmentationUtilities/QmitkConvertToMultiLabelSegmentationWidgetControls.ui
  SegmentationUtilities/QmitkExtractFromMultiLabelSegmentationWidgetControls.ui
)

set(QRC_FILES
  resources/SegmentationUI.qrc
)
