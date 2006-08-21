/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <QmitkLevelWindowWidgetContextMenu.h>
#include <QmitkLevelWindowPresetDefinition.h>
#include <QmitkLevelWindowRangeChange.h>
#include <qcursor.h>

QmitkLevelWindowWidgetContextMenu::QmitkLevelWindowWidgetContextMenu(QWidget * parent, const char * name, WFlags f )
: QWidget( parent, name, f ), m_LevelWindowPreset()
{
  m_LevelWindowPreset.LoadPreset();
}

void QmitkLevelWindowWidgetContextMenu::setPreset(int presetID)
{
  QString item = m_PresetSubmenu->text(presetID);
  if (!(presetID == m_PresetID))
  {
    double dlevel = m_LevelWindowPreset.getLevel(std::string((const char*)item));
    double dwindow = m_LevelWindowPreset.getWindow(std::string((const char*)item));
    if ((dlevel + dwindow/2) > m_LevelWindow.GetRangeMax())
    {
      double lowerBound = (dlevel - dwindow/2);
      if (!(lowerBound > m_LevelWindow.GetRangeMax()))
      {
        dwindow = m_LevelWindow.GetRangeMax() - lowerBound;
        dlevel = lowerBound + dwindow/2;
      }
      else
      {
        dlevel = m_LevelWindow.GetRangeMax() - 1;
        dwindow = 2;
      }
    }
    else if ((dlevel - dwindow/2) < m_LevelWindow.GetRangeMin())
    {
      double upperBound = (dlevel + dwindow/2);
      if (!(upperBound < m_LevelWindow.GetRangeMin()))
      {
        dwindow = m_LevelWindow.GetRangeMin() + upperBound;
        dlevel = upperBound - dwindow/2;
      }
      else
      {
        dlevel = m_LevelWindow.GetRangeMin() + 1;
        dwindow = 2;
      }
    }
    m_LevelWindow.SetLevelWindow(dlevel, dwindow);
    m_Manager->SetLevelWindow(m_LevelWindow);
  }
}

void QmitkLevelWindowWidgetContextMenu::setLevelWindowManager(mitk::LevelWindowManager* levelWindowManager)
{
  m_Manager = levelWindowManager;
}

void QmitkLevelWindowWidgetContextMenu::addPreset()
{
  QmitkLevelWindowPresetDefinition addPreset(this, "newPreset", true);
  addPreset.setPresets(m_LevelWindowPreset.getLevelPresets(), m_LevelWindowPreset.getWindowPresets(), QString::number( (int) m_LevelWindow.GetLevel() ), QString::number( (int) m_LevelWindow.GetWindow() ));
  if(addPreset.exec())
  {
    m_LevelWindowPreset.newPresets(addPreset.getLevelPresets(), addPreset.getWindowPresets());
  }
}

void QmitkLevelWindowWidgetContextMenu::setDefaultLevelWindow()
{
  m_LevelWindow.SetLevelWindow(m_LevelWindow.GetDefaultLevel(), m_LevelWindow.GetDefaultWindow());
  m_Manager->SetLevelWindow(m_LevelWindow);
}

void QmitkLevelWindowWidgetContextMenu::setDefaultScaleRange()
{
  m_LevelWindow.SetRangeMinMax(m_LevelWindow.GetDefaultRangeMin(), m_LevelWindow.GetDefaultRangeMax());
  m_LevelWindow.SetLevelWindow(m_LevelWindow.GetLevel(), m_LevelWindow.GetWindow());
  m_Manager->SetLevelWindow(m_LevelWindow);
}

void QmitkLevelWindowWidgetContextMenu::changeScaleRange()
{
  QmitkLevelWindowRangeChange changeRange(this, "changeRange", true);
  changeRange.setLowerLimit((int)m_LevelWindow.GetRangeMin());
  changeRange.setUpperLimit((int)m_LevelWindow.GetRangeMax());
  if(changeRange.exec())
  {
    m_LevelWindow.SetRangeMinMax(changeRange.getLowerLimit(), changeRange.getUpperLimit());
    m_LevelWindow.SetLevelWindow(m_LevelWindow.GetLevel(), m_LevelWindow.GetWindow());
    m_Manager->SetLevelWindow(m_LevelWindow);
  }
}

void QmitkLevelWindowWidgetContextMenu::setImage(int imageID)
{
  if (imageID == m_ImageID)
    m_Manager->SetAutoTopMostImage();
  else
    m_Manager->SetLevelWindowProperty(m_Images[imageID]);
}

void QmitkLevelWindowWidgetContextMenu::getContextMenu(QPopupMenu* contextmenu)
{
  try
  {
    m_LevelWindow = m_Manager->GetLevelWindow();

    mitk::DataTree* dataTree = m_Manager->GetDataTree();
    mitk::DataTreeIteratorClone dataTreeIteratorClone = dataTree->GetIteratorToNode( dataTree, NULL );

    QPopupMenu* contextMenu = contextmenu;
    Q_CHECK_PTR( contextMenu );
    contextMenu->insertItem(tr("Default Level/Window"), this, SLOT(setDefaultLevelWindow()));
    contextMenu->insertSeparator();
    contextMenu->insertItem(tr("Change Scale Range"), this, SLOT(changeScaleRange()));
    contextMenu->insertItem(tr("Default Scale Range"), this, SLOT(setDefaultScaleRange()));
    contextMenu->insertSeparator();
    
    m_PresetSubmenu = new QPopupMenu( this );
    Q_CHECK_PTR( m_PresetSubmenu );
    m_PresetID = m_PresetSubmenu->insertItem(tr("Preset Definition"), this, SLOT(addPreset()));
    m_PresetSubmenu->insertSeparator();
    std::map<std::string, double> preset = m_LevelWindowPreset.getLevelPresets();
    for( std::map<std::string, double>::iterator iter = preset.begin(); iter != preset.end(); iter++ ) {
      QString item = ((*iter).first.c_str());
      m_PresetSubmenu->insertItem(item);
    }
    connect(m_PresetSubmenu, SIGNAL(activated(int)), this, SLOT(setPreset(int)));
    contextMenu->insertItem( "Presets",  m_PresetSubmenu );
    contextMenu->insertSeparator();

    m_ImageSubmenu = new QPopupMenu( this );
    m_ImageSubmenu->setCheckable(true);
    m_ImageID = m_ImageSubmenu->insertItem(tr("Set Topmost Image"));
    if (m_Manager->isAutoTopMost())
      m_ImageSubmenu->setItemChecked(m_ImageID, true);
    m_ImageSubmenu->insertSeparator();
    Q_CHECK_PTR( m_ImageSubmenu );
    dataTreeIteratorClone->GoToBegin();
    while ( !dataTreeIteratorClone->IsAtEnd() )
    {
      if ( (dataTreeIteratorClone->Get().IsNotNull()) && (dataTreeIteratorClone->Get()->IsVisible(NULL)) )
      {
        bool binary = false;
        dataTreeIteratorClone->Get()->GetBoolProperty("binary", binary);
        if( binary == false)
        { 
          mitk::LevelWindowProperty::Pointer levelWindowProperty = dynamic_cast<mitk::LevelWindowProperty*>(dataTreeIteratorClone->Get()->GetProperty("levelwindow").GetPointer());
          if (levelWindowProperty.IsNotNull())
          {
            std::string name;
            dataTreeIteratorClone->Get()->GetName(name);
            QString item = name.c_str();
            int id = m_ImageSubmenu->insertItem(item);
            m_Images[id] = levelWindowProperty;
            if (levelWindowProperty == m_Manager->GetLevelWindowProperty())
            {
              m_ImageSubmenu->setItemChecked(id, true);
            }
          }
        }
      }
      ++dataTreeIteratorClone;
    }
    connect(m_ImageSubmenu, SIGNAL(activated(int)), this, SLOT(setImage(int)));
    contextMenu->insertItem( "Images",  m_ImageSubmenu );

    contextMenu->exec( QCursor::pos() );
    delete contextMenu;
  }
  catch(...)
  {
  }
}

void QmitkLevelWindowWidgetContextMenu::getContextMenu()
{
  try
  {
    m_LevelWindow = m_Manager->GetLevelWindow();
    mitk::DataTree* dataTree = m_Manager->GetDataTree();
    mitk::DataTreeIteratorClone dataTreeIteratorClone = dataTree->GetIteratorToNode( dataTree, NULL );

    QPopupMenu* contextMenu = new QPopupMenu( this );
    Q_CHECK_PTR( contextMenu );
    contextMenu->insertItem(tr("Default Level/Window"), this, SLOT(setDefaultLevelWindow()));
    contextMenu->insertSeparator();
    contextMenu->insertItem(tr("Change Scale Range"), this, SLOT(changeScaleRange()));
    contextMenu->insertItem(tr("Default Scale Range"), this, SLOT(setDefaultScaleRange()));
    contextMenu->insertSeparator();
    
    m_PresetSubmenu = new QPopupMenu( this );
    Q_CHECK_PTR( m_PresetSubmenu );
    m_PresetID = m_PresetSubmenu->insertItem(tr("Preset Definition"), this, SLOT(addPreset()));
    m_PresetSubmenu->insertSeparator();
    std::map<std::string, double> preset = m_LevelWindowPreset.getLevelPresets();
    for( std::map<std::string, double>::iterator iter = preset.begin(); iter != preset.end(); iter++ ) {
      QString item = ((*iter).first.c_str());
      m_PresetSubmenu->insertItem(item);
    }
    connect(m_PresetSubmenu, SIGNAL(activated(int)), this, SLOT(setPreset(int)));
    contextMenu->insertItem( "Presets",  m_PresetSubmenu );
    contextMenu->insertSeparator();

    m_ImageSubmenu = new QPopupMenu( this );
    m_ImageSubmenu->setCheckable(true);
    m_ImageID = m_ImageSubmenu->insertItem(tr("Set Topmost Image"));
    if (m_Manager->isAutoTopMost())
      m_ImageSubmenu->setItemChecked(m_ImageID, true);
    m_ImageSubmenu->insertSeparator();
    Q_CHECK_PTR( m_ImageSubmenu );
    dataTreeIteratorClone->GoToBegin();
    while ( !dataTreeIteratorClone->IsAtEnd() )
    {
      if ( (dataTreeIteratorClone->Get().IsNotNull()) && (dataTreeIteratorClone->Get()->IsVisible(NULL)) )
      {
        bool binary = false;
        dataTreeIteratorClone->Get()->GetBoolProperty("binary", binary);
        if( binary == false)
        { 
          mitk::LevelWindowProperty::Pointer levelWindowProperty = dynamic_cast<mitk::LevelWindowProperty*>(dataTreeIteratorClone->Get()->GetProperty("levelwindow").GetPointer());
          if (levelWindowProperty.IsNotNull())
          {
            std::string name;
            dataTreeIteratorClone->Get()->GetName(name);
            QString item = name.c_str();
            int id = m_ImageSubmenu->insertItem(item);
            m_Images[id] = levelWindowProperty;
            if (levelWindowProperty == m_Manager->GetLevelWindowProperty())
            {
              m_ImageSubmenu->setItemChecked(id, true);
            }
          }
        }
      }
      ++dataTreeIteratorClone;
    }
    connect(m_ImageSubmenu, SIGNAL(activated(int)), this, SLOT(setImage(int)));
    contextMenu->insertItem( "Images",  m_ImageSubmenu );

    contextMenu->exec( QCursor::pos() );
    delete contextMenu;
  }
  catch(...)
  {
  }
}
