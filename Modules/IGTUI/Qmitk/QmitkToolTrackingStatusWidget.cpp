/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-03-21 19:27:37 +0100 (Sa, 21 Mrz 2009) $
Version:   $Revision: 16719 $ 

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "QmitkToolTrackingStatusWidget.h"





QmitkToolTrackingStatusWidget::QmitkToolTrackingStatusWidget(QWidget* parent)  
: QWidget(parent), m_Controls(NULL), m_StatusLabels (NULL), m_NavigationDatas(NULL)
{
  this->CreateQtPartControl( this );
}

QmitkToolTrackingStatusWidget::~QmitkToolTrackingStatusWidget()
{
  //m_Controls = NULL;
  delete m_StatusLabels;
  delete m_NavigationDatas;
}

void QmitkToolTrackingStatusWidget::CreateQtPartControl(QWidget *parent)
{
  if (!m_Controls)
  {
    // create GUI widgets
    m_Controls = new Ui::QmitkToolTrackingStatusWidgetControls;
    m_Controls->setupUi(parent);

    this->CreateConnections();
  }
}

void QmitkToolTrackingStatusWidget::CreateConnections()
{

}


void QmitkToolTrackingStatusWidget::SetNavigationDatas(std::vector<mitk::NavigationData::Pointer>* navDatas)
{  
  m_NavigationDatas = navDatas;
}


void QmitkToolTrackingStatusWidget::AddNavigationData(mitk::NavigationData::Pointer nd)
{
  if(m_NavigationDatas == NULL)
    m_NavigationDatas = new std::vector<mitk::NavigationData::Pointer>();

  m_NavigationDatas->push_back(nd);
}


void QmitkToolTrackingStatusWidget::Refresh()
{

  if(m_NavigationDatas == NULL || m_NavigationDatas->size() <= 0)
    return;

  mitk::NavigationData* navData; 

  for(unsigned int i = 0; i < m_NavigationDatas->size(); i++)
  {
    navData = m_NavigationDatas->at(i).GetPointer();
    QString name(navData->GetName());

    if(name.compare(m_StatusLabels->at(i)->text()) == 0)
    {
      if(navData->IsDataValid())
        m_StatusLabels->at(i)->setStyleSheet("QLabel{background-color: #8bff8b }");
      else
        m_StatusLabels->at(i)->setStyleSheet("QLabel{background-color: #ff7878 }");
    }
  }
}


void QmitkToolTrackingStatusWidget::ShowStatusLabels()
{

  if(m_NavigationDatas == NULL || m_NavigationDatas->size() <= 0)
    return;


  m_StatusLabels = new QVector<QLabel*>();
  mitk::NavigationData* navData;
  QLabel* label;


  for(unsigned int i = 0; i < m_NavigationDatas->size(); i++)
  {
    navData = m_NavigationDatas->at(i).GetPointer();

    QString name(navData->GetName());

    label = new QLabel(name, this);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    m_StatusLabels->append(label);
    m_Controls->m_GridLayout->addWidget(m_StatusLabels->at(i),0,i);
  }
}


void QmitkToolTrackingStatusWidget::RemoveStatusLabels()
{

  while(m_Controls->m_GridLayout->count() > 0)
  {
    QWidget* actWidget = m_Controls->m_GridLayout->itemAt(0)->widget();
    m_Controls->m_GridLayout->removeWidget(actWidget);
    delete actWidget;
  }

  if(m_StatusLabels != NULL && m_StatusLabels->size() > 0)
    m_StatusLabels->clear();
  if(m_NavigationDatas != NULL && m_NavigationDatas->size() > 0)
    m_NavigationDatas->clear();

}




