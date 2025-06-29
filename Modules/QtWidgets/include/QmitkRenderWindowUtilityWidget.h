/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef QmitkRenderWindowUtilityWidget_h
#define QmitkRenderWindowUtilityWidget_h

#include "MitkQtWidgetsExports.h"

// qt widgets module
#include <QmitkSynchronizedNodeSelectionWidget.h>
#include <QmitkSliceNavigationWidget.h>
#include <QmitkStepperAdapter.h>
#include <mitkRenderWindowLayerController.h>
#include <mitkRenderWindowViewDirectionController.h>

// qt
#include <QWidget>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QComboBox>

namespace mitk
{
  class DataStorage;
}

class QmitkRenderWindow;

/*
* @brief The 'QmitkRenderWindowUtilityWidget' can be added to a 'QMitkRenderWindowWidget' to extend it
*        with window-specific utilities.
*
*        It offers to select the viewing direction of the window, as well as a 'QmitkSliceNavigationWidget'
*        to scroll through the current view direction.
*        In addition, it contains a 'QmitkSynchronizedNodeSelectionWidget' that controls renderer-specific
*        properties and shown nodes, as well as a synchronization-group selector to share this state with
*        other render windows.
*/
class MITKQTWIDGETS_EXPORT QmitkRenderWindowUtilityWidget : public QWidget
{
	Q_OBJECT

public:

  QmitkRenderWindowUtilityWidget(
    QWidget* parent = nullptr,
    QmitkRenderWindow* renderWindow = nullptr,
    mitk::DataStorage* dataStorage = nullptr,
    const int nSyncGroups = 1
  );

  ~QmitkRenderWindowUtilityWidget() override;

  using GroupSyncIndexType = int;

  void SetSyncGroup(const GroupSyncIndexType index);
  GroupSyncIndexType GetSyncGroup() const;
  void OnSyncGroupSelectionChanged(int index);

  void SetGeometry(const itk::EventObject& event);
  QmitkSynchronizedNodeSelectionWidget* GetNodeSelectionWidget() const;

public Q_SLOTS:
  void UpdateViewPlaneSelection();
  void OnSyncGroupAdded(const GroupSyncIndexType index);

Q_SIGNALS:

  void SynchronizationToggled(QmitkSynchronizedNodeSelectionWidget* synchronizedWidget);
  void SyncGroupChanged(QmitkSynchronizedNodeSelectionWidget* synchronizedWidget, GroupSyncIndexType index);
  void SetDataSelection(const QList<mitk::DataNode::Pointer>& newSelection);

private:

  mitk::BaseRenderer* m_BaseRenderer;
  QmitkSynchronizedNodeSelectionWidget* m_NodeSelectionWidget;
  QPushButton* m_SynchPushButton;
  QComboBox* m_SyncGroupSelector;
  QmitkSliceNavigationWidget* m_SliceNavigationWidget;
  QmitkStepperAdapter* m_StepperAdapter;
  std::unique_ptr<mitk::RenderWindowLayerController> m_RenderWindowLayerController;
  std::unique_ptr<mitk::RenderWindowViewDirectionController> m_RenderWindowViewDirectionController;
  QComboBox* m_ViewDirectionSelector;

  void ChangeViewDirection(const QString& viewDirection);

};

#endif
