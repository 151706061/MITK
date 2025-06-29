/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef QmitkSynchronizedNodeSelectionWidget_h
#define QmitkSynchronizedNodeSelectionWidget_h

#include <MitkQtWidgetsExports.h>

#include "ui_QmitkSynchronizedNodeSelectionWidget.h"

// mitk core
#include <mitkBaseRenderer.h>

// qt widgets module
#include <QmitkAbstractNodeSelectionWidget.h>
#include <QmitkRenderWindowDataNodeTableModel.h>

/*
* @brief The 'QmitkSynchronizedNodeSelectionWidget' implements the 'QmitkAbstractNodeSelectionWidget'
*        by providing a table view, using a 'QmitkRenderWindowDataNodeTableModel' and extending it
*        with base renderer-specific functionality.
* 
*        Given a base renderer, the selection widget is able to display and access render window specific properties
*        of the selected nodes. It can be connected with other QmitkSynchronizedNodeSelectionWidgets to synchronize
*        their state.
*        The widget can be used to decide if all data nodes of the data storage should be selected or
*        only an individually selected set of nodes, defined by a 'QmitkNodeSelectionDialog'.
*        If individual nodes are selected / removed from the selection, the widget can inform other
*        'QmitkSynchronizedNodeSelectionWidget' about the current selection, if desired.
*        Additionally the widget allows to reinitialize the corresponding base renderer with a specific
*        data node geometry.
*/
class MITKQTWIDGETS_EXPORT QmitkSynchronizedNodeSelectionWidget : public QmitkAbstractNodeSelectionWidget
{
  Q_OBJECT

public:

  QmitkSynchronizedNodeSelectionWidget(QWidget* parent);
  ~QmitkSynchronizedNodeSelectionWidget();

  using NodeList = QmitkAbstractNodeSelectionWidget::NodeList;

  void SetBaseRenderer(mitk::BaseRenderer* baseRenderer);

  QmitkRenderWindowDataNodeTableModel* GetStorageModel() const;

  void SetSelectAll(bool selectAll);
  bool GetSelectAll() const;
  void SelectAll();
  using GroupSyncIndexType = int;
  void SetSyncGroup(const GroupSyncIndexType index);
  GroupSyncIndexType GetSyncGroup() const;

Q_SIGNALS:

  void SelectionModeChanged(bool selectAll);
  void DeregisterSynchronization();
  void NodeVisibilityChanged(mitk::DataNode::Pointer node, const bool visibility);

public Q_SLOTS:
  void SetSelection(const NodeList& newSelection);
  void SetNodeVisibility(mitk::DataNode::Pointer node, const bool visibility);

private Q_SLOTS:

  void OnSelectionModeChanged(bool selectAll);
  void OnEditSelection();
  void OnTableClicked(const QModelIndex& index);

protected:

  void SetUpConnections();
  void Initialize();

  void UpdateInfo() override;
  void OnDataStorageChanged() override;
  void OnNodePredicateChanged() override;
  void ReviseSelectionChanged(const NodeList& oldInternalSelection, NodeList& newInternalSelection) override;
  void OnInternalSelectionChanged() override;
  void OnNodeAddedToStorage(const mitk::DataNode* node) override;
  void OnNodeModified(const itk::Object* caller, const itk::EventObject& event) override;

private:

  void ReinitNode(const mitk::DataNode* dataNode);
  void RemoveFromInternalSelection(mitk::DataNode* dataNode);
  bool IsParentNodeSelected(const mitk::DataNode* dataNode) const;
  void DeselectNode(mitk::DataNode* dataNode);

  Ui::QmitkSynchronizedNodeSelectionWidget m_Controls;
  mitk::WeakPointer<mitk::BaseRenderer> m_BaseRenderer;

  std::unique_ptr<QmitkRenderWindowDataNodeTableModel> m_StorageModel;
  GroupSyncIndexType m_SyncGroupIndex;

};

#endif
