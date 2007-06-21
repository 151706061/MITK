/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2007-04-19 15:02:32 +0200 (Do, 19 Apr 2007) $
Version:   $Revision: 10185 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if !defined(QmitkLoadSaveToChiliExample_H__INCLUDED)
#define QmitkLoadSaveToChiliExample_H__INCLUDED

#include "QmitkFunctionality.h"

class QmitkStdMultiWidget;
class QmitkLoadSaveToChiliExampleControls;

/*!
\brief QmitkLoadSaveToChiliExample 

One needs to reimplement the methods CreateControlWidget(..), CreateMainWidget(..) 
and CreateAction(..) from QmitkFunctionality. 

\sa QmitkFunctionality
\ingroup Functionalities
*/
class QmitkLoadSaveToChiliExample : public QmitkFunctionality
{
  Q_OBJECT

  public:
  /*!
  \brief default constructor
  */
  QmitkLoadSaveToChiliExample(QObject *parent=0, const char *name=0, QmitkStdMultiWidget *mitkStdMultiWidget = NULL, mitk::DataTreeIteratorBase* dataIt = NULL);

  /*!
  \brief default destructor
  */
  virtual ~QmitkLoadSaveToChiliExample();

  /*!
  \brief method for creating the widget containing the application controls, like sliders, buttons etc.
  */
  virtual QWidget * CreateControlWidget(QWidget *parent);

  /*!
  \brief method for creating the applications main widget
  */
  virtual QWidget * CreateMainWidget(QWidget * parent);

  /*!
  \brief method for creating the connections of main and control widget
  */
  virtual void CreateConnections();

  /*!
  \brief method for creating an QAction object, i.e. button & menu entry  @param parent the parent QWidget
  */
  virtual QAction * CreateAction(QActionGroup *parent);

  virtual void Activated();

protected slots:

  /*!
  \brief load an image from the current selected lightbox and add it to the MITK-DataTree
  */
  void LoadFromLightbox();

  /*!
  \brief add the current selected DataTreeNode (only images) to a new lightbox
  */
  void SaveToLightbox();

  /*!
  \brief change the description of the current selected DataTreeNode
  */
  void SetDescription();

  void FileDownload();
  void FileUpload();

  /*!
  \brief fill a textfield with the seriesdescription
  */
  void ImageSelected(mitk::DataTreeIteratorClone imageIt);

  /*!
  \brief method if the tree changed
  */
  void TreeChanged();

protected:

  /*!
  \brief if the user change the study in chili, mitk::ChiliPlugin throws an event, this method listen to it and change the listview (show the description of all series to the selected study)
  */
  void chiliStudySelected(const itk::EventObject&);

  /*!
  * default main widget containing 4 windows showing 3
  * orthogonal slices of the volume and a 3d render window
  */
  QmitkStdMultiWidget * m_MultiWidget;

  /*!
  * controls containing sliders for scrolling through the slices
  */
  QmitkLoadSaveToChiliExampleControls * m_Controls;

};
#endif // !defined(QmitkLoadSaveToChiliExample_H__INCLUDED)

