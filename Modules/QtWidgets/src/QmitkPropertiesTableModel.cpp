/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "QmitkPropertiesTableModel.h"

//# Own includes
#include "QmitkCustomVariants.h"
#include "mitkColorProperty.h"
#include "mitkEnumerationProperty.h"
#include "mitkProperties.h"
#include "mitkRenderingManager.h"
#include "mitkStringProperty.h"

//# Toolkit includes
#include <QBrush>
#include <QColor>
#include <QStringList>
#include <itkCommand.h>

//# PUBLIC CTORS,DTOR
QmitkPropertiesTableModel::QmitkPropertiesTableModel(QObject *parent, mitk::PropertyList::Pointer _PropertyList)
  : QAbstractTableModel(parent),
    m_PropertyList(nullptr),
    m_BlockEvents(false),
    m_SortDescending(false),
    m_FilterKeyWord("")
{
  this->SetPropertyList(_PropertyList);
}

QmitkPropertiesTableModel::~QmitkPropertiesTableModel()
{
  // remove all event listeners by setting the property list to 0
  this->SetPropertyList(nullptr);
}

//# PUBLIC GETTER
mitk::PropertyList::Pointer QmitkPropertiesTableModel::GetPropertyList() const
{
  return m_PropertyList.Lock();
}

Qt::ItemFlags QmitkPropertiesTableModel::flags(const QModelIndex &index) const
{
  // no editing so far, return default (enabled, selectable)
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);

  if (index.column() == PROPERTY_VALUE_COLUMN)
  {
    // there are also read only property items -> do not allow editing them
    if (index.data(Qt::EditRole).isValid())
      flags |= Qt::ItemIsEditable;

    if (index.data(Qt::CheckStateRole).isValid())
      flags |= Qt::ItemIsUserCheckable;
  }

  return flags;
}

QVariant QmitkPropertiesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal)
  {
    switch (section)
    {
      case PROPERTY_NAME_COLUMN:
        return tr("Name");

      case PROPERTY_VALUE_COLUMN:
        return tr("Value");

      default:
        return QVariant();
    }
  }

  return QVariant();
}

QVariant QmitkPropertiesTableModel::data(const QModelIndex &index, int role) const
{
  // empty data by default
  QVariant data;

  if (!index.isValid() || m_SelectedProperties.empty() || index.row() > (int)(m_SelectedProperties.size() - 1))
    return data;

  // the properties name
  if (index.column() == PROPERTY_NAME_COLUMN)
  {
    if (role == Qt::DisplayRole)
      data = QString::fromStdString(m_SelectedProperties[index.row()].first);
  }
  // the real properties value
  else if (index.column() == PROPERTY_VALUE_COLUMN)
  {
    mitk::BaseProperty *baseProp = m_SelectedProperties[index.row()].second;

    if (const mitk::ColorProperty *colorProp = dynamic_cast<const mitk::ColorProperty *>(baseProp))
    {
      mitk::Color col = colorProp->GetColor();
      QColor qcol((int)(col.GetRed() * 255), (int)(col.GetGreen() * 255), (int)(col.GetBlue() * 255));
      if (role == Qt::DisplayRole)
        data.setValue(qcol);
      else if (role == Qt::EditRole)
        data.setValue(qcol);
    }

    else if (mitk::BoolProperty *boolProp = dynamic_cast<mitk::BoolProperty *>(baseProp))
    {
      if (role == Qt::CheckStateRole)
        data = boolProp->GetValue() ? Qt::Checked : Qt::Unchecked;
    }

    else if (mitk::StringProperty *stringProp = dynamic_cast<mitk::StringProperty *>(baseProp))
    {
      if (role == Qt::DisplayRole)
        data.setValue<QString>(QString::fromStdString(stringProp->GetValue()));
      else if (role == Qt::EditRole)
        data.setValue<QString>(QString::fromStdString(stringProp->GetValue()));
    }

    else if (mitk::IntProperty *intProp = dynamic_cast<mitk::IntProperty *>(baseProp))
    {
      if (role == Qt::DisplayRole)
        data.setValue<int>(intProp->GetValue());
      else if (role == Qt::EditRole)
        data.setValue<int>(intProp->GetValue());
    }

    else if (mitk::FloatProperty *floatProp = dynamic_cast<mitk::FloatProperty *>(baseProp))
    {
      if (role == Qt::DisplayRole)
        data.setValue<float>(floatProp->GetValue());
      else if (role == Qt::EditRole)
        data.setValue<float>(floatProp->GetValue());
    }

    else if (mitk::EnumerationProperty *enumerationProp = dynamic_cast<mitk::EnumerationProperty *>(baseProp))
    {
      if (role == Qt::DisplayRole)
        data.setValue<QString>(QString::fromStdString(baseProp->GetValueAsString()));
      else if (role == Qt::EditRole)
      {
        QStringList values;
        for (auto it = enumerationProp->Begin(); it != enumerationProp->End(); it++)
        {
          values << QString::fromStdString(it->second);
        }
        data.setValue(values);
      }
    }

    else
    {
      if (role == Qt::DisplayRole)
        data.setValue(QString::fromStdString(m_SelectedProperties[index.row()].second->GetValueAsString()));
    }
  }

  return data;
}

int QmitkPropertiesTableModel::rowCount(const QModelIndex & /*parent*/) const
{
  // return the number of properties in the properties list.
  return m_SelectedProperties.size();
}

int QmitkPropertiesTableModel::columnCount(const QModelIndex & /*parent*/) const
{
  return 2;
}

//# PUBLIC SETTER
void QmitkPropertiesTableModel::SetPropertyList(mitk::PropertyList *_PropertyList)
{
  // if propertylist really changed
  if (m_PropertyList != _PropertyList)
  {
    auto propertyList = m_PropertyList.Lock();
    // Remove delete listener if there was a propertylist before
    if (propertyList.IsNotNull())
      propertyList->RemoveObserver(m_PropertyListDeleteObserverTag);

    // set new list
    m_PropertyList = _PropertyList;
    propertyList = m_PropertyList.Lock();

    if (propertyList.IsNotNull())
    {
      auto command = itk::SimpleMemberCommand<QmitkPropertiesTableModel>::New();
      command->SetCallbackFunction(this, &QmitkPropertiesTableModel::PropertyListDelete);
      m_PropertyListDeleteObserverTag = propertyList->AddObserver(itk::DeleteEvent(), command);
    }
    this->Reset();
  }
}

void QmitkPropertiesTableModel::PropertyListDelete()
{
  if (!m_BlockEvents)
  {
    m_BlockEvents = true;
    this->Reset();
    m_BlockEvents = false;
  }
}

void QmitkPropertiesTableModel::PropertyModified(const itk::Object *caller, const itk::EventObject & /*event*/)
{
  if (!m_BlockEvents)
  {
    m_BlockEvents = true;
    int row = this->FindProperty(dynamic_cast<const mitk::BaseProperty *>(caller));

    QModelIndex indexOfChangedProperty = index(row, 1);

    emit dataChanged(indexOfChangedProperty, indexOfChangedProperty);
    m_BlockEvents = false;
  }
}

void QmitkPropertiesTableModel::PropertyDelete(const itk::Object *caller, const itk::EventObject & /*event*/)
{
  if (!m_BlockEvents)
  {
    m_BlockEvents = true;
    int row = this->FindProperty(dynamic_cast<const mitk::BaseProperty *>(caller));
    if (row >= 0)
      this->Reset();
    m_BlockEvents = false;
  }
}

bool QmitkPropertiesTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  // TODO: check 'role' condition
  if (index.isValid() && !m_SelectedProperties.empty() && index.row() < (int)(m_SelectedProperties.size()) &&
      (role == Qt::EditRole || role == Qt::CheckStateRole))
  {
    // block all events now!
    m_BlockEvents = true;

    auto propertyList = m_PropertyList.Lock();

    // the properties name
    if (index.column() == PROPERTY_VALUE_COLUMN)
    {
      mitk::BaseProperty *baseProp = m_SelectedProperties[index.row()].second;

      if (mitk::ColorProperty *colorProp = dynamic_cast<mitk::ColorProperty *>(baseProp))
      {
        QColor qcolor = value.value<QColor>();
        if (!qcolor.isValid())
          return false;

        mitk::Color col = colorProp->GetColor();
        col.SetRed(qcolor.red() / 255.0);
        col.SetGreen(qcolor.green() / 255.0);
        col.SetBlue(qcolor.blue() / 255.0);
        colorProp->SetColor(col);
        propertyList->InvokeEvent(itk::ModifiedEvent());
        propertyList->Modified();

        mitk::RenderingManager::GetInstance()->RequestUpdateAll();
      }

      else if (mitk::BoolProperty *boolProp = dynamic_cast<mitk::BoolProperty *>(baseProp))
      {
        boolProp->SetValue(value.toInt() == Qt::Checked ? true : false);
        propertyList->InvokeEvent(itk::ModifiedEvent());
        propertyList->Modified();

        mitk::RenderingManager::GetInstance()->RequestUpdateAll();
      }

      else if (mitk::StringProperty *stringProp = dynamic_cast<mitk::StringProperty *>(baseProp))
      {
        stringProp->SetValue((value.value<QString>()).toStdString());
        propertyList->InvokeEvent(itk::ModifiedEvent());
        propertyList->Modified();

        mitk::RenderingManager::GetInstance()->RequestUpdateAll();
      }

      else if (mitk::IntProperty *intProp = dynamic_cast<mitk::IntProperty *>(baseProp))
      {
        int intValue = value.value<int>();
        if (intValue != intProp->GetValue())
        {
          intProp->SetValue(intValue);
          propertyList->InvokeEvent(itk::ModifiedEvent());
          propertyList->Modified();

          mitk::RenderingManager::GetInstance()->RequestUpdateAll();
        }
      }

      else if (mitk::FloatProperty *floatProp = dynamic_cast<mitk::FloatProperty *>(baseProp))
      {
        float floatValue = value.value<float>();
        if (floatValue != floatProp->GetValue())
        {
          floatProp->SetValue(floatValue);
          propertyList->InvokeEvent(itk::ModifiedEvent());
          propertyList->Modified();

          mitk::RenderingManager::GetInstance()->RequestUpdateAll();
        }
      }

      else if (mitk::EnumerationProperty *enumerationProp = dynamic_cast<mitk::EnumerationProperty *>(baseProp))
      {
        std::string activatedItem = value.value<QString>().toStdString();
        if (activatedItem != enumerationProp->GetValueAsString())
        {
          if (enumerationProp->IsValidEnumerationValue(activatedItem))
          {
            enumerationProp->SetValue(activatedItem);
            propertyList->InvokeEvent(itk::ModifiedEvent());
            propertyList->Modified();

            mitk::RenderingManager::GetInstance()->RequestUpdateAll();
          }
        }
      }
    }

    // property was changed by us, now we can accept property changes triggered by someone else
    m_BlockEvents = false;
    emit dataChanged(index, index);
    return true;
  }

  return false;
}

void QmitkPropertiesTableModel::sort(int column, Qt::SortOrder order /*= Qt::AscendingOrder */)
{
  bool sortDescending = (order == Qt::DescendingOrder) ? true : false;

  // do not sort twice !!! (dont know why, but qt calls this func twice. STUPID!)
  if (sortDescending != m_SortDescending)
  {
    m_SortDescending = sortDescending;

    PropertyDataSetCompareFunction::CompareCriteria _CompareCriteria = PropertyDataSetCompareFunction::CompareByName;

    PropertyDataSetCompareFunction::CompareOperator _CompareOperator =
      m_SortDescending ? PropertyDataSetCompareFunction::Greater : PropertyDataSetCompareFunction::Less;

    if (column == PROPERTY_VALUE_COLUMN)
      _CompareCriteria = PropertyDataSetCompareFunction::CompareByValue;

    PropertyDataSetCompareFunction compareFunc(_CompareCriteria, _CompareOperator);
    std::sort(m_SelectedProperties.begin(), m_SelectedProperties.end(), compareFunc);

    QAbstractTableModel::beginResetModel();
    QAbstractTableModel::endResetModel();
  }
}

//# PROTECTED GETTER
int QmitkPropertiesTableModel::FindProperty(const mitk::BaseProperty *_Property) const
{
  int row = -1;

  if (_Property)
  {
    // search for property that changed and emit datachanged on the corresponding ModelIndex
    std::vector<PropertyDataSet>::const_iterator propertyIterator;

    for (propertyIterator = m_SelectedProperties.begin(); propertyIterator != m_SelectedProperties.end();
         propertyIterator++)
    {
      if (propertyIterator->second == _Property)
        break;
    }

    if (propertyIterator != m_SelectedProperties.end())
      row = std::distance(m_SelectedProperties.begin(), propertyIterator);
  }

  return row;
}

//# PROTECTED SETTER
void QmitkPropertiesTableModel::AddSelectedProperty(PropertyDataSet &_PropertyDataSet)
{
  // subscribe for modified event
  itk::MemberCommand<QmitkPropertiesTableModel>::Pointer _PropertyDataSetModifiedCommand =
    itk::MemberCommand<QmitkPropertiesTableModel>::New();
  _PropertyDataSetModifiedCommand->SetCallbackFunction(this, &QmitkPropertiesTableModel::PropertyModified);
  m_PropertyModifiedObserverTags.push_back(
    _PropertyDataSet.second->AddObserver(itk::ModifiedEvent(), _PropertyDataSetModifiedCommand));

  // subscribe for delete event
  itk::MemberCommand<QmitkPropertiesTableModel>::Pointer _PropertyDataSetDeleteCommand =
    itk::MemberCommand<QmitkPropertiesTableModel>::New();
  _PropertyDataSetDeleteCommand->SetCallbackFunction(this, &QmitkPropertiesTableModel::PropertyDelete);
  m_PropertyDeleteObserverTags.push_back(
    _PropertyDataSet.second->AddObserver(itk::DeleteEvent(), _PropertyDataSetDeleteCommand));

  // add to the selection
  m_SelectedProperties.push_back(_PropertyDataSet);
}

void QmitkPropertiesTableModel::RemoveSelectedProperty(unsigned int _Index)
{
  PropertyDataSet &_PropertyDataSet = m_SelectedProperties.at(_Index);

  // remove modified event listener
  _PropertyDataSet.second->RemoveObserver(m_PropertyModifiedObserverTags[_Index]);
  m_PropertyModifiedObserverTags.erase(m_PropertyModifiedObserverTags.begin() + _Index);
  // remove delete event listener
  _PropertyDataSet.second->RemoveObserver(m_PropertyDeleteObserverTags[_Index]);
  m_PropertyDeleteObserverTags.erase(m_PropertyDeleteObserverTags.begin() + _Index);
  // remove from selection
  m_SelectedProperties.erase(m_SelectedProperties.begin() + _Index);
}

void QmitkPropertiesTableModel::Reset()
{
  // remove all selected properties
  while (!m_SelectedProperties.empty())
  {
    this->RemoveSelectedProperty(m_SelectedProperties.size() - 1);
  }

  std::vector<PropertyDataSet> allPredicates;
  auto propertyList = m_PropertyList.Lock();

  if (propertyList.IsNotNull())
  {
    // first of all: collect all properties from the list
    for (auto it = propertyList->GetMap()->begin(); it != propertyList->GetMap()->end(); it++)
    {
      allPredicates.push_back(*it); //% TODO
    }
  }
  // make a subselection if a keyword is specified
  if (!m_FilterKeyWord.empty())
  {
    std::vector<PropertyDataSet> subSelection;

    for (auto it = allPredicates.begin(); it != allPredicates.end(); it++)
    {
      // add this to the selection if it is matched by the keyword
      if ((*it).first.find(m_FilterKeyWord) != std::string::npos)
        subSelection.push_back((*it));
    }
    allPredicates.clear();
    allPredicates = subSelection;
  }

  PropertyDataSet tmpPropertyDataSet;
  // add all selected now to the Model
  for (auto it = allPredicates.begin(); it != allPredicates.end(); it++)
  {
    tmpPropertyDataSet = *it;
    this->AddSelectedProperty(tmpPropertyDataSet);
  }

  // sort the list as indicated by m_SortDescending
  this->sort(m_SortDescending);

  // model was reset
  QAbstractTableModel::beginResetModel();
  QAbstractTableModel::endResetModel();
}

void QmitkPropertiesTableModel::SetFilterPropertiesKeyWord(std::string _FilterKeyWord)
{
  m_FilterKeyWord = _FilterKeyWord;
  this->Reset();
}

QmitkPropertiesTableModel::PropertyDataSetCompareFunction::PropertyDataSetCompareFunction(
  CompareCriteria _CompareCriteria, CompareOperator _CompareOperator)
  : m_CompareCriteria(_CompareCriteria), m_CompareOperator(_CompareOperator)
{
}

bool QmitkPropertiesTableModel::PropertyDataSetCompareFunction::operator()(const PropertyDataSet &_Left,
                                                                           const PropertyDataSet &_Right) const
{
  switch (m_CompareCriteria)
  {
    case CompareByValue:
      if (m_CompareOperator == Less)
        return (_Left.second->GetValueAsString() < _Right.second->GetValueAsString());
      else
        return (_Left.second->GetValueAsString() > _Right.second->GetValueAsString());
      break;

    // CompareByName:
    default:
      if (m_CompareOperator == Less)
        return (_Left.first < _Right.first);
      else
        return (_Left.first > _Right.first);
      break;
  }
}
QmitkPropertiesTableModel::PropertyListElementFilterFunction::PropertyListElementFilterFunction(
  const std::string &_FilterKeyWord)
  : m_FilterKeyWord(_FilterKeyWord)
{
}

bool QmitkPropertiesTableModel::PropertyListElementFilterFunction::operator()(const PropertyDataSet &_Elem) const
{
  if (m_FilterKeyWord.empty())
    return true;
  return (_Elem.first.find(m_FilterKeyWord) == 0);
}
