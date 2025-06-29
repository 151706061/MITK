/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

// mitk qt widgets module
#include <QmitkSynchronizedNodeSelectionWidget.h>
#include <QmitkCustomVariants.h>
#include <QmitkEnums.h>
#include <QmitkNodeSelectionDialog.h>

// mitk core module
#include <mitkImage.h>
#include <mitkNodePredicateNot.h>
#include <mitkNodePredicateProperty.h>
#include <mitkRenderWindowLayerUtilities.h>

QmitkSynchronizedNodeSelectionWidget::QmitkSynchronizedNodeSelectionWidget(QWidget* parent)
  : QmitkAbstractNodeSelectionWidget(parent)
  , m_SyncGroupIndex(-1)
{
  m_Controls.setupUi(this);

  m_StorageModel = std::make_unique<QmitkRenderWindowDataNodeTableModel>(this);

  m_Controls.tableView->setModel(m_StorageModel.get());
  m_Controls.tableView->horizontalHeader()->setVisible(false);
  m_Controls.tableView->verticalHeader()->setVisible(false);
  m_Controls.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  m_Controls.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_Controls.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
  m_Controls.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  auto header = m_Controls.tableView->horizontalHeader();
  header->setSectionResizeMode(0, QHeaderView::Stretch);

  const int columnCount = m_StorageModel->columnCount();

  for (int column = 1; column < columnCount; ++column)
  {
    header->setSectionResizeMode(column, QHeaderView::ResizeToContents);
  }

  this->SetUpConnections();
  this->Initialize();
}

QmitkSynchronizedNodeSelectionWidget::~QmitkSynchronizedNodeSelectionWidget()
{

  emit DeregisterSynchronization();

  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  auto dataStorage = m_DataStorage.Lock();
  if (dataStorage.IsNull())
  {
    return;
  }

  // We know that renderer-specific properties exist for all nodes.
  // These properties need to be removed from the nodes.
  auto allNodes = dataStorage->GetAll();
  for (auto& node : *allNodes)
  {
    // Delete the relevant renderer-specific properties for the node using the current base renderer.
    mitk::RenderWindowLayerUtilities::DeleteRenderWindowProperties(node, baseRenderer);
  }
}

void QmitkSynchronizedNodeSelectionWidget::SetBaseRenderer(mitk::BaseRenderer* baseRenderer)
{
  if (m_BaseRenderer == baseRenderer)
  {
    // no need to do something
    return;
  }

  if (nullptr == baseRenderer)
  {
    return;
  }

  auto oldBaseRenderer = m_BaseRenderer.Lock();
  m_BaseRenderer = baseRenderer;
  m_StorageModel->SetCurrentRenderer(baseRenderer);

  auto dataStorage = m_DataStorage.Lock();
  if (dataStorage.IsNull())
  {
    return;
  }

  // We know that renderer-specific properties exist for all nodes.
  // These properties need to be removed from the nodes and
  // we need to transfer their values to new renderer-specific properties.
  auto allNodes = dataStorage->GetAll();
  for (auto& node : *allNodes)
  {
    // Set the relevant renderer-specific properties for the node using the new base renderer.
    // By transferring the values from the old property list,
    // the same property-state is kept when switching to another base renderer.
    mitk::RenderWindowLayerUtilities::TransferRenderWindowProperties(node, baseRenderer, oldBaseRenderer);
    // Delete the relevant renderer-specific properties for the node using the old base renderer.
    mitk::RenderWindowLayerUtilities::DeleteRenderWindowProperties(node, oldBaseRenderer);
  }

  this->Initialize();
}

QmitkRenderWindowDataNodeTableModel* QmitkSynchronizedNodeSelectionWidget::GetStorageModel() const
{
  return m_StorageModel.get();
}

void QmitkSynchronizedNodeSelectionWidget::SetSelectAll(bool selectAll)
{
  if (selectAll == m_Controls.selectionModeCheckBox->isChecked())
  {
    // no need to do something
    return;
  }

  m_Controls.selectionModeCheckBox->setChecked(selectAll);
}

bool QmitkSynchronizedNodeSelectionWidget::GetSelectAll() const
{
  return m_Controls.selectionModeCheckBox->isChecked();
}

void QmitkSynchronizedNodeSelectionWidget::OnSelectionModeChanged(bool selectAll)
{
  emit SelectionModeChanged(selectAll);

  if (selectAll)
  {
    this->SelectAll();
  }
}

void QmitkSynchronizedNodeSelectionWidget::OnEditSelection()
{
  QmitkNodeSelectionDialog* dialog = new QmitkNodeSelectionDialog(this);

  dialog->SetDataStorage(m_DataStorage.Lock());
  dialog->SetNodePredicate(m_NodePredicate);
  dialog->SetCurrentSelection(m_StorageModel->GetCurrentSelection());
  dialog->SetSelectionMode(QAbstractItemView::MultiSelection);

  m_Controls.changeSelectionButton->setChecked(true);
  if (dialog->exec())
  {
    if (m_Controls.selectionModeCheckBox->isChecked())
    {
      m_Controls.selectionModeCheckBox->setChecked(false);
      emit SelectionModeChanged(false);
    }

    auto selectedNodes = dialog->GetSelectedNodes();
    this->HandleChangeOfInternalSelection(selectedNodes);
  }

  m_Controls.changeSelectionButton->setChecked(false);

  delete dialog;
}

void QmitkSynchronizedNodeSelectionWidget::OnTableClicked(const QModelIndex& index)
{
  if (!index.isValid() || m_StorageModel.get() != index.model())
  {
    return;
  }

  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  QVariant dataNodeVariant = index.data(QmitkDataNodeRole);
  auto dataNode = dataNodeVariant.value<mitk::DataNode::Pointer>();

  if (index.column() == 1) // node visibility column
  {
    bool visibility = index.data(Qt::EditRole).toBool();
    m_StorageModel->setData(index, QVariant(!visibility), Qt::EditRole);
    emit NodeVisibilityChanged(dataNode, !visibility);
    return;
  }

  if (index.column() == 2) // reinit node column
  {
    this->ReinitNode(dataNode);
    return;
  }

  if (index.column() == 3) // remove node column
  {
    this->RemoveFromInternalSelection(dataNode);
    return;
  }
}

void QmitkSynchronizedNodeSelectionWidget::SetUpConnections()
{
  connect(m_Controls.selectionModeCheckBox, &QCheckBox::clicked,
    this, &QmitkSynchronizedNodeSelectionWidget::OnSelectionModeChanged);
  connect(m_Controls.changeSelectionButton, &QPushButton::clicked,
    this, &QmitkSynchronizedNodeSelectionWidget::OnEditSelection);

  connect(m_Controls.tableView, &QTableView::clicked,
    this, &QmitkSynchronizedNodeSelectionWidget::OnTableClicked);
}

void QmitkSynchronizedNodeSelectionWidget::SetSelection(const NodeList& newSelection)
{
  this->HandleChangeOfInternalSelection(newSelection);
  m_Controls.selectionModeCheckBox->setChecked(false);
}

void QmitkSynchronizedNodeSelectionWidget::Initialize()
{
  auto baseRenderer = m_BaseRenderer.Lock();
  auto dataStorage = m_DataStorage.Lock();
  m_StorageModel->SetDataStorage(dataStorage);
  m_StorageModel->SetCurrentRenderer(baseRenderer);

  if (baseRenderer.IsNull() || dataStorage.IsNull())
  {
    m_Controls.selectionModeCheckBox->setEnabled(false);
    m_Controls.changeSelectionButton->setEnabled(false);
    // reset the model if no data storage is defined
    m_StorageModel->removeRows(0, m_StorageModel->rowCount());
    return;
  }

  // Use the new data storage / node predicate to correctly set the list of
  // currently selected data nodes for the model.
  // If a new data storage or node predicate has been defined,
  // we switch to the "selectAll" mode and synchronize the selection for simplicity.
  // enable UI
  m_Controls.selectionModeCheckBox->setEnabled(true);
  m_Controls.changeSelectionButton->setEnabled(true);

  m_Controls.selectionModeCheckBox->setChecked(true);
}

void QmitkSynchronizedNodeSelectionWidget::UpdateInfo()
{

}

void QmitkSynchronizedNodeSelectionWidget::OnDataStorageChanged()
{
  this->Initialize();
}

void QmitkSynchronizedNodeSelectionWidget::OnNodePredicateChanged()
{
  this->Initialize();
}

void QmitkSynchronizedNodeSelectionWidget::ReviseSelectionChanged(const NodeList& oldInternalSelection, NodeList& newInternalSelection)
{
  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  // Since the model is synchronized to a group, all nodes need renderer-specific properties.
  // Thus we need to modify the renderer-specific properties of the selection:
  //  - a) set the renderer-specific properties of those nodes, which are part of the new selection AND
  //       have not been selected before (see 'SelectNode')
  //  - b) show those nodes, which are part of the new selection AND have not been
  //       selected before
  //  - c) keep the property values of those nodes, which are part of the new selection AND
  //       have been selected before
  //  - d) hide those nodes, which are part of the old selection AND
  //       have not been newly selected
  //  - e) set the renderer-specific properties of those nodes, which are part of the old selection AND
  //       have not been newly selected, to denote which nodes are selected
  for (auto& node : newInternalSelection)
  {
    // check if the node is part of the old selection
    auto finding = std::find(std::begin(oldInternalSelection), std::end(oldInternalSelection), node);
    if (finding == std::end(oldInternalSelection)) // node not found
    {
      // If the node is part of the new selection and was not already part of the old selection,
      // set the relevant renderer-specific properties.
      // This will set the "visible" and "layer" property for the renderer-specific property list
      // such that the global / default property list values are overwritten
      mitk::RenderWindowLayerUtilities::SetRenderWindowProperties(node, baseRenderer); // item a)
      // Explicitly set the visibility to true for selected nodes to show them in the render window.
      node->SetVisibility(true, baseRenderer); // item b)
    }
    // else: item c): node that was already selected before does not need to be modified
  }

  for (auto& node : oldInternalSelection)
  {
    // check if the node is part of the new selection
    auto finding = std::find(std::begin(newInternalSelection), std::end(newInternalSelection), node);
    if (finding == std::end(newInternalSelection)) // node not found
    {
      // If the node is not part of the new selection, unset the relevant renderer-specific properties.
      // This will unset the "visible" and "layer" property for the renderer-specific property list and
      // hide the node for this renderer.
      // ATTENTION: This is required, since the synchronized global property needs to be overwritten
      //            to make sure that the visibility is correctly set for the specific base renderer.
      this->DeselectNode(node); // item d) and e)
    }
    // else: item c): node that was already selected before does not need to be modified
  }

  // Since a new selection might have a different rendering tree the render windows
  // need to be updated.
  // Explicitly request an update since a renderer-specific property change does not mark the node as modified.
  // see https://phabricator.mitk.org/T22322
  mitk::RenderingManager::GetInstance()->RequestUpdate(baseRenderer->GetRenderWindow());
}

void QmitkSynchronizedNodeSelectionWidget::OnInternalSelectionChanged()
{
  m_StorageModel->SetCurrentSelection(this->GetCurrentInternalSelection());
}

void QmitkSynchronizedNodeSelectionWidget::OnNodeAddedToStorage(const mitk::DataNode* node)
{
  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  // For helper / hidden nodes
  if (m_NodePredicate.IsNotNull() && !m_NodePredicate->CheckNode(node))
  {
    // If the node predicate does not match, do not add the node to the current selection.
    // Leave the visibility as it is.
    return;
  }


  bool visibleInRenderer = false;
  if (node->PropertyIsOwned("visible", baseRenderer->GetName(), false))
    node->GetBoolProperty("visible", visibleInRenderer, baseRenderer);
  // The selection mode determines if we want to show all nodes from the data storage
  // or use a local selected list of nodes.
  // We need to hide each new incoming data node, if we use a local selection,
  // since we do not want to show / select newly added nodes immediately.
  // We need to add the incoming node to our selection, if the selection mode check box
  // is checked.
  // We want to add the incoming node to our selection, if the node is a child node
  // of an already selected node or it is already explicitly visible for this renderer
  // via properties.
  // Nodes added to the selection will be made visible.
  if (m_Controls.selectionModeCheckBox->isChecked() || this->IsParentNodeSelected(node) || visibleInRenderer)
  {
    auto currentSelection = this->GetCurrentInternalSelection();
    // Check if the nodes is already part of the internal selection.
    // That can happen if another render window already added the new node and sent out the new, updated
    // selection to be synchronized.
    auto finding = std::find(std::begin(currentSelection), std::end(currentSelection), node);
    if (finding != std::end(currentSelection)) // node found
    {
      // node already part of the selection
      return;
    }

    currentSelection.append(const_cast<mitk::DataNode*>(node));
    // This function will call 'QmitkSynchronizedNodeSelectionWidget::ReviseSelectionChanged'
    // which will take care of the visibility-property for newly added node.
    this->HandleChangeOfInternalSelection(currentSelection);
  }
  else
  {
    this->DeselectNode(const_cast<mitk::DataNode*>(node));
  }
}

void QmitkSynchronizedNodeSelectionWidget::OnNodeModified(const itk::Object* caller, const itk::EventObject& event)
{
  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  if (!itk::ModifiedEvent().CheckEvent(&event))
  {
    return;
  }

  auto node = dynamic_cast<const mitk::DataNode*>(caller);

  if (m_NodePredicate.IsNull() || m_NodePredicate->CheckNode(node))
  {
    auto currentSelection = this->GetCurrentInternalSelection();
    // check if the node to be modified is part of the current selection
    auto finding = std::find(std::begin(currentSelection), std::end(currentSelection), node);
    if (finding == std::end(currentSelection)) // node not found
    {
      // node not part of the selection
      return;
    }
    
    // We know that the node is relevant, but we don't know if the node modification was relevant
    // for the rendering. We just request an update here.
    // Explicitly request an update since a renderer-specific property change does not mark the node as modified.
    // see https://phabricator.mitk.org/T22322
    mitk::RenderingManager::GetInstance()->RequestUpdate(baseRenderer->GetRenderWindow());
    m_StorageModel->UpdateModelData();
  }
}

void QmitkSynchronizedNodeSelectionWidget::ReinitNode(const mitk::DataNode* dataNode)
{
  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  auto selectedImage = dynamic_cast<mitk::Image*>(dataNode->GetData());
  if (nullptr == selectedImage)
  {
    return;
  }

  auto boundingBoxPredicate = mitk::NodePredicateNot::New(
    mitk::NodePredicateProperty::New("includeInBoundingBox", mitk::BoolProperty::New(false), baseRenderer));
  if (!boundingBoxPredicate->CheckNode(dataNode))
  {
    return;
  }

  mitk::RenderingManager::GetInstance()->InitializeView(baseRenderer->GetRenderWindow(), selectedImage->GetTimeGeometry());
}

void QmitkSynchronizedNodeSelectionWidget::RemoveFromInternalSelection(mitk::DataNode* dataNode)
{
  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  if (m_Controls.selectionModeCheckBox->isChecked())
  {
    m_Controls.selectionModeCheckBox->setChecked(false);
    emit SelectionModeChanged(false);
  }

  this->RemoveNodeFromInternalSelection(dataNode);
}

bool QmitkSynchronizedNodeSelectionWidget::IsParentNodeSelected(const mitk::DataNode* dataNode) const
{
  auto dataStorage = m_DataStorage.Lock();
  if (dataStorage.IsNull())
  {
    return false;
  }

  auto currentSelection = this->GetCurrentInternalSelection();
  auto parentNodes = dataStorage->GetSources(dataNode, m_NodePredicate, false);
  for (auto it = parentNodes->Begin(); it != parentNodes->End(); ++it)
  {
    const mitk::DataNode* parentNode = it->Value();
    auto finding = std::find(std::begin(currentSelection), std::end(currentSelection), parentNode);
    if (finding != std::end(currentSelection)) // parent node found
    {
      // at least one parent node is part of the selection
      return true;
    }
  }

  return false;
}

void QmitkSynchronizedNodeSelectionWidget::DeselectNode(mitk::DataNode* dataNode)
{
  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  if (nullptr == dataNode)
  {
    return;
  }

  if (m_NodePredicate.IsNull() || m_NodePredicate->CheckNode(dataNode))
  {
    // If the node should not be part of the selection, set the relevant renderer-specific properties.
    // This will set the "visible" and "layer" property for the renderer-specific property list,
    // such that the global / default property list values are overwritten.
    mitk::RenderWindowLayerUtilities::SetRenderWindowProperties(dataNode, baseRenderer);
    // Explicitly set the visibility to false for the node to hide them in the render window.
    dataNode->SetVisibility(false, baseRenderer);
  }
}

void QmitkSynchronizedNodeSelectionWidget::SelectAll()
{
  auto dataStorage = m_DataStorage.Lock();
  if (dataStorage.IsNull())
  {
    return;
  }

  auto allNodes = m_NodePredicate ? dataStorage->GetSubset(m_NodePredicate) : dataStorage->GetAll();
  NodeList currentSelection;
  for (auto& node : *allNodes)
  {
    currentSelection.append(node);
  }

  this->HandleChangeOfInternalSelection(currentSelection);
}

void QmitkSynchronizedNodeSelectionWidget::SetSyncGroup(const GroupSyncIndexType index)
{
  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }

  if (index == 0)
  {
    MITK_ERROR << "Invalid call to SetSyncGroup. Group index can't be 0.";
    return;
  }

  m_SyncGroupIndex = index;

  // Since the synchronization might lead to a different node order depending on the layer properties, the render window
  // needs to be updated.
  // Explicitly request an update since a renderer-specific property change does not mark the node as modified.
  // see https://phabricator.mitk.org/T22322
  mitk::RenderingManager::GetInstance()->RequestUpdate(baseRenderer->GetRenderWindow());
}

QmitkSynchronizedNodeSelectionWidget::GroupSyncIndexType QmitkSynchronizedNodeSelectionWidget::GetSyncGroup() const
{
  return m_SyncGroupIndex;
}

void QmitkSynchronizedNodeSelectionWidget::SetNodeVisibility(mitk::DataNode::Pointer node, const bool visibility)
{
  auto baseRenderer = m_BaseRenderer.Lock();
  if (baseRenderer.IsNull())
  {
    return;
  }
  node->SetVisibility(visibility, baseRenderer);

  // Explicitly request an update since a renderer-specific property change does not mark the node as modified.
  // see https://phabricator.mitk.org/T22322
  mitk::RenderingManager::GetInstance()->RequestUpdate(baseRenderer->GetRenderWindow());
}
