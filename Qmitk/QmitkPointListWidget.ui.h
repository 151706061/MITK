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


/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qstring.h>

const int QmitkPointListWidget::UNLIMITED = 999;

void QmitkPointListWidget::init()
{
  m_DataChangedCommand = itk::SimpleMemberCommand<QmitkPointListWidget>::New();
  m_DataChangedCommand->SetCallbackFunction(this, &QmitkPointListWidget::ItemsOfListUpdate);
  m_CurrentObserverID = 0;
  m_CurrentInteraction = NULL;
  m_CurrentPolygonInteraction = NULL;
}



void QmitkPointListWidget::PointSelect( int ItemIndex )
//when a point is selected in the widget, then an event equal to an user event gets sent to the global interaction so that the data (point) is selected
//TODO: change the event EIDLEFTMOUSEBTN to something more defined, cause when user interaction changes in xml-file, 
//then this don't work anymore (then change the event here too)
{  
  assert(m_PointSet.IsNotNull());
  //assert(m_PointSet->IndexExists(ItemIndex));
  mitk::PointSet::PointType ppt;
  mitk::PointSet::PointsContainer::Iterator it, end;  
  int i=0;
  for (it = m_PointSet->GetPointSet()->GetPoints()->Begin(); i<ItemIndex; it++) i++; ///the iterator will be set to the position needed 
  m_PointSet->GetPointSet()->GetPoints()->GetElementIfIndexExists(it->Index(), &ppt);
  mitk::Point2D p2d;
  mitk::Point3D p3d;
  mitk::FillVector3D(p3d, (ppt)[0],(ppt)[1],(ppt)[2]);
  mitk::PositionEvent event(NULL, 0, 0, 0, mitk::Key_unknown, p2d, p3d);
  mitk::StateEvent *stateEvent = new mitk::StateEvent(mitk::EIDLEFTMOUSEBTN , &event);    

  mitk::GlobalInteraction::GetInstance()->HandleEvent( stateEvent );
  stateEvent->Set(mitk::EIDLEFTMOUSERELEASE , &event);
  mitk::GlobalInteraction::GetInstance()->HandleEvent( stateEvent );

  delete stateEvent;
}


void QmitkPointListWidget::ItemsOfListUpdate()
{
  if (m_DatatreeNode.IsNull())
    return;
  
  int lastSelectedPoint = InteractivePointList->currentItem();
  
  InteractivePointList->clear();
  m_PointSet = (mitk::PointSet*)(m_DatatreeNode->GetData());
  int size =m_PointSet->GetSize();
  m_NumberOfPointsLabel->setText(QString::number(size));
  if (size!=0)
  {
    const mitk::PointSet::DataType::Pointer Pointlist= m_PointSet->GetPointSet();
    ////
    mitk::PointSet::PointsContainer::Iterator it, end;
    end = m_PointSet->GetPointSet()->GetPoints()->End();
    unsigned int i=0;
    mitk::PointSet::PointType ppt;
    for (it = m_PointSet->GetPointSet()->GetPoints()->Begin(); it!=end; it++,i++)
    {
      m_PointSet->GetPointSet()->GetPoints()->GetElementIfIndexExists(it->Index(), &ppt);
      std::stringstream  aStrStream;
      aStrStream<<prefix<<i+1<<"  ("<< (ppt)[0]<<",  "<<(ppt)[1]<<",  "<<(ppt)[2]<<")"<<", Index "<< it->Index();
      const std::string s = aStrStream.str();
      const char * Item =s.c_str();
      this->InteractivePointList->insertItem(Item);
    }
    //
    // if there is a selected point in the point set, it will
    // also be highlighted in the pointlist widget.
    //
    int selectedPointIndex = m_PointSet->SearchSelectedPoint();
    if (selectedPointIndex == -1) 
      selectedPointIndex = lastSelectedPoint;
    if (selectedPointIndex > -1 && m_PointSet->GetPointSet()->GetPoints()->IndexExists(selectedPointIndex))
    {
      //
      // convert index into a number ranging from [0..N-1]
      // this is necessary, since if the user deletes points, the point
      // indices are not monotonically increasing.
      //
      int selectedItem = 0;
      for (it = m_PointSet->GetPointSet()->GetPoints()->Begin(); it->Index()!= (unsigned)selectedPointIndex; it++,selectedItem++)
      {}
      
      // select the currently active point in the point
      // list box      
      this->InteractivePointList->setSelected( selectedItem, true );
      
      // center the currently active point in the vertical
      // scroll bar
      this->InteractivePointList->centerCurrentItem();
    }
    else
    {
      this->InteractivePointList->clearSelection();
    }
  }
}

void QmitkPointListWidget::RemoveInteraction()
{
 mitk::PointSet::Pointer pointset;

    if (m_CurrentInteraction.IsNotNull())
    {
      //remove last Interactor
      mitk::GlobalInteraction::GetInstance()
        ->RemoveInteractor( m_CurrentInteraction );

      pointset = dynamic_cast<mitk::PointSet*>(m_DatatreeNode->GetData());
      assert(pointset.IsNotNull());
      pointset->RemoveObserver(m_CurrentObserverID);
    }
  
}

void QmitkPointListWidget::SwitchInteraction( mitk::PointSetInteractor::Pointer *sop, mitk::DataTreeNode::Pointer * node, int numberOfPoints, mitk::Point3D c, std::string l )
{
  mitk::PointSet::Pointer pointset;
  if ((*sop).IsNull())
  {
    //new layer property
    mitk::IntProperty::Pointer layer = new mitk::IntProperty(1);
    mitk::ColorProperty::Pointer color = new mitk::ColorProperty(c[0],c[1],c[2]);

    mitk::BoolProperty::Pointer contour = new mitk::BoolProperty(false);
    mitk::BoolProperty::Pointer close = new mitk::BoolProperty(true);


    //if necessary create a TreeNode, to connect the data to...
    mitk::DataTreeNode::Pointer dataTreeNode;
    if((*node).IsNull())
      dataTreeNode = mitk::DataTreeNode::New();
    else
      dataTreeNode = *node;

    //if necessary create a DataElement that holds the points
    if(dynamic_cast<mitk::PointSet*>(dataTreeNode->GetData()) == NULL)
      pointset = mitk::PointSet::New();
    else
      pointset = dynamic_cast<mitk::PointSet*>(dataTreeNode->GetData());

    //declaring a new Interactor
    if (numberOfPoints!=UNLIMITED)//limited number of points
      *sop = new mitk::PointSetInteractor("pointsetinteractor", dataTreeNode, numberOfPoints);
    else   //unlimited number of points
      *sop = new mitk::PointSetInteractor("pointsetinteractor", dataTreeNode);

    //datatreenode: and give set the data, layer and Interactor
    dataTreeNode->SetData(pointset);
    dataTreeNode->SetProperty("layer",layer);
    dataTreeNode->SetProperty("color",color);
    dataTreeNode->SetProperty("contour",contour);
    dataTreeNode->SetProperty("close",close);

    mitk::StringProperty::Pointer label = new mitk::StringProperty(l);

    //const char *c="";
    //if ( !strcmp(l.c_str(),c) )
    // {

    dataTreeNode->SetProperty("label",label);
    // }

    dataTreeNode->SetInteractor(*sop);
    dataTreeNode->SetProperty("name", label); /// name is identical with label????
    *node = dataTreeNode;

    ////then add it to the existing DataTree
    //m_DataTreeIterator->add(dataTreeNode);
  }

  if (m_CurrentInteraction.IsNotNull())
  {
    //remove last Interactor
    mitk::GlobalInteraction::GetInstance()
      ->RemoveInteractor(m_CurrentInteraction);

    pointset = dynamic_cast<mitk::PointSet*>(m_DatatreeNode->GetData());
    assert(pointset.IsNotNull());
    pointset->RemoveObserver(m_CurrentObserverID);
  }

  //connect data to refresh method
  pointset = dynamic_cast<mitk::PointSet*>((*node)->GetData());
  assert(pointset.IsNotNull());
  m_CurrentObserverID = pointset->AddObserver(itk::EndEvent(), m_DataChangedCommand);

  //new Interactor
  m_CurrentInteraction = *sop;
  prefix= l;
  m_DatatreeNode= *node;
  //tell the global Interactor, that there is another one to ask if it can handle the event
  mitk::GlobalInteraction::GetInstance()->AddInteractor(m_CurrentInteraction);
  ItemsOfListUpdate();
}

void QmitkPointListWidget::SwitchInteraction( mitk::PolygonInteractor::Pointer *sop, mitk::DataTreeNode::Pointer * node, int itkNotUsed(numberOfPoints), mitk::Point3D c, std::string l )
{
  if ((*sop).IsNull())
  {
    //new layer property
    std::cout << "creating polygon interactor ..." << std::endl;

    mitk::IntProperty::Pointer layer = new mitk::IntProperty(1);
    mitk::ColorProperty::Pointer color = new mitk::ColorProperty(c[0],c[1],c[2]);

    //mitk::BoolProperty::Pointer contour = new mitk::BoolProperty(false);
    //mitk::BoolProperty::Pointer close = new mitk::BoolProperty(true);
    mitk::StringProperty::Pointer label = new mitk::StringProperty(l);
    mitk::StringProperty::Pointer name = new mitk::StringProperty(l);
    //create a DataElement that holds the points
    mitk::Mesh::Pointer meshpointset = mitk::Mesh::New();

    //connect data to refresh method
    meshpointset->AddObserver(itk::EndEvent(), m_DataChangedCommand);

    //then crate a TreeNode, to connect the data to...
    mitk::DataTreeNode::Pointer dataTreeNode = mitk::DataTreeNode::New();
    *sop = new mitk::PolygonInteractor("polygoninteractor", dataTreeNode);


    //datatreenode: and give set the data, layer and Interactor
    dataTreeNode->SetData(meshpointset);
    dataTreeNode->SetProperty("layer",layer);
    dataTreeNode->SetProperty("color",color);
    //dataTreeNode->SetProperty("contour",contour);
    //dataTreeNode->SetProperty("close",close);
    dataTreeNode->SetProperty("label",label);
    dataTreeNode->SetInteractor(*sop);
    dataTreeNode->SetProperty("name", name); /// name is identical with label????
    *node = dataTreeNode;

    ////then add it to the existing DataTree
    //m_DataTreeIterator->add(dataTreeNode);
  }

  if (m_CurrentPolygonInteraction.IsNotNull())
  {
    //remove last Interactor
    mitk::GlobalInteraction::GetInstance()
      ->RemoveInteractor(m_CurrentPolygonInteraction);
  }

  //new Interactor
  m_CurrentPolygonInteraction = *sop;
  prefix= l;
  m_DatatreeNode= *node;
  //tell the global Interactor, that there is another one to ask if it can handle the event
  mitk::GlobalInteraction::GetInstance()->AddInteractor(m_CurrentPolygonInteraction);
  ItemsOfListUpdate();
}
