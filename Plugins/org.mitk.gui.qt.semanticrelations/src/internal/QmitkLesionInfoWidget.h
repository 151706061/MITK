/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef QMITKLESIONINFOWIDGET_H
#define QMITKLESIONINFOWIDGET_H

// semantic relations UI module
#include <ui_QmitkLesionInfoWidgetControls.h>

// semantic relations module
#include <mitkSemanticRelations.h>
#include <mitkISemanticRelationsObserver.h>

// mitk
#include <mitkDataStorage.h>

// qt
#include <QWidget>

/*
* @brief The QmitkLesionInfoWidget is a widget that shows and modifies the currently available lesion data of the semantic relations model.
*
*   The widget provides a dialogs to add nodes from the data storage to the semantic relations model.
*   It provides functionality to create new lesions and link them with segmentation nodes.
*
*   The QmitkLesionInfoWidget provides three QListWidgets, that show the lesion data and the referenced segmentation data, as
*   well as the connected image data, depending on the selected lesion.
*
*   The QmitkLesionInfoWidget implements the 'ISemanticRelationsObserver', so that it is automatically updated, if the
*   semantic relations model changes. Updating means freshly getting all lesion data and filling the lesion-ListWidget with the lesion data.
*/
class QmitkLesionInfoWidget : public QWidget, public mitk::ISemanticRelationsObserver
{
  Q_OBJECT

public:

  static const QBrush DEFAULT_BACKGROUND_COLOR;
  static const QBrush SELECTED_BACKGROUND_COLOR;
  static const QBrush CONNECTED_BACKGROUND_COLOR;

  QmitkLesionInfoWidget::QmitkLesionInfoWidget(mitk::DataStorage* dataStorage, QWidget* parent = nullptr);
  ~QmitkLesionInfoWidget();

  void SetCurrentCaseID(const mitk::SemanticTypes::CaseID& caseID);
  /*
  * @brief Updates the 'lesionListWidget' of the GUI with the current lesion-data from the semantic relations model.
  *
  *       Overridden from 'ISemanticRelationsObserver'.
  *       In order for the Update-function to be called, this widget has to be added as a observer of SemanticRelation
  *       (e.g. m_SemanticRelations->AddObserver(m_LesionInfoWidget);)
  *
  * @par caseID    The current case ID to identify the currently active patient / case.
  */
  virtual void Update(const mitk::SemanticTypes::CaseID& caseID) override;

  const mitk::SemanticTypes::Lesion& GetSelectedLesion() const { return m_CurrentLesion; }

  /*
  * @brief Resets all items from the lesion list widget.
  */
  void ResetLesionListWidget();
  /*
  * @brief Resets the background color of all items in each list widget.
  */
  void ResetBackgroundColors();

  void DarkenBackgroundColors();

Q_SIGNALS:

  void LesionChanged(const mitk::SemanticTypes::Lesion&);

private Q_SLOTS:

  /*
  * @brief Generates a new, empty lesion to add to the semantic relations model for the current case ID.
  */
  void OnAddLesionButtonClicked();

  // slots for the mouse click events of the list widgets
  void OnCurrentLesionItemChanged(QListWidgetItem*, QListWidgetItem*);
  void OnLesionItemDoubleClicked(QListWidgetItem*);

  void OnLesionListContextMenuRequested(const QPoint&);

  // slots for the context menu actions of the lesion list widget
  void OnLinkToSegmentation(const mitk::SemanticTypes::ID&);
  void OnSetLesionName(const mitk::SemanticTypes::ID&);
  void OnSetLesionClass(const mitk::SemanticTypes::ID&);
  void OnRemoveLesion(const mitk::SemanticTypes::ID&);

private:

  void Init();
  void SetUpConnections();

  Ui::QmitkLesionInfoWidgetControls m_Controls;
  mitk::DataStorage* m_DataStorage;
  std::unique_ptr<mitk::SemanticRelations> m_SemanticRelations;
  mitk::SemanticTypes::CaseID m_CaseID;

  mitk::SemanticTypes::Lesion m_CurrentLesion;
  mitk::DataNode::Pointer m_CurrentSegmentation;
  mitk::DataNode::Pointer m_CurrentImage;
};

#endif // QMITKLESIONINFOWIDGET_H
