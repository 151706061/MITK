

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
#ifndef MITK_PIXELGREYVALUEMANIPULATORCOMPONENT_H
#define MITK_PIXELGREYVALUEMANIPULATORCOMPONENT_H

#include "QmitkFunctionalityComponentContainer.h"
#include <itkImage.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>

//NUR FUER POINTSET
#include "mitkPointSetInteractor.h"
#include "mitkPointSet.h"


class QmitkPixelGreyValueManipulatorComponentGUI;
class QmitkStdMultiWidget;


/**
* \brief ComponentClass to create a surface based on a threshold for a selected image
* \ingroup QmitkFunctionalityComponent
* 
* This class inherits from
* - \ref QmitkFunctionalityComponentContainer

* 
* \section QmitkPixelGreyValueManipulatorComponent Overview
* 
* The PixelGreyValueManipulatorComponent is a class to create a surface, based on a threshold. The user can input a threshold 
* (maybe found with the ThresholdComponent) and create a surface that will be added into the DataTree. The suface
* is made of polygones and can be saved as vtk or stl-Format.
* Like all other componentes the QmitkPixelGreyValueManipulatorComponent inherits from QmitkFunctionalityComponentContainer.
* 
*/


class QmitkPixelGreyValueManipulatorComponent : public QmitkFunctionalityComponentContainer
{

  Q_OBJECT

public:
  /***************       CONSTRUCTOR      ***************/
  /** \brief Constructor. */
  QmitkPixelGreyValueManipulatorComponent(QObject *parent=0, const char *name=0, QmitkStdMultiWidget *mitkStdMultiWidget = NULL, mitk::DataTreeIteratorBase* dataIt = NULL, bool updateSelector = false, bool showSelector = true);

  /***************        DESTRUCTOR      ***************/
  /** \brief Destructor. */
  virtual ~QmitkPixelGreyValueManipulatorComponent();

  /***************        CREATE          ***************/

  /** \brief Method to create the GUI for the component from the .ui-File. This Method is obligatory */
  virtual QWidget* CreateControlWidget(QWidget* parent);

  /** \brief Method to create the connections for the component. This Method is obligatory even if no connections is needed*/
  virtual void CreateConnections();


  /***************      SET AND GET       ***************/

  /** \brief Method to set the Iterator to the DataTree */
  void SetDataTreeIterator(mitk::DataTreeIteratorBase* it);

  /** \brief Method to get the Iterator to the DataTree */
  mitk::DataTreeIteratorBase* GetDataTreeIterator();

  ///** \brief Method to set the Name of the Functionality */
  //void SetFunctionalityName(QString parentName);

  ///** \brief Method to get the Name of the Functionality */
  //virtual QString GetFunctionalityName();

  ///** \brief Method to get the Name of the FunctionalityComponent */
  //virtual QString GetComponentName();
  
  /** \brief Method to get the GUI of this component. This Method is obligatory */
  //virtual QWidget* GetGUI();

  /** \brief Method to return the TreeNodeSelector-QmitkDataTreeComboBox */
  virtual QmitkDataTreeComboBox* GetTreeNodeSelector();
   
  /** \brief Method to set the Image Selector visible or invisible */
  virtual void SetSelectorVisibility(bool visibility);


  /***************      (DE)ACTIVATED     ***************/

  ///** \brief Method to set m_Activated to true */
  virtual void Activated();

  ///** \brief Method to set m_Activated to false */
  virtual void Deactivated();


  ///***************        ATTRIBUTES      ***************/

  ///** \brief Vector with all added components */
  //std::vector<QmitkFunctionalityComponentContainer*> m_AddedChildList; 


public slots:  
  /***************      OHTER METHODS     ***************/

  /** \brief Slot method that will be called if TreeNodeSelector widget was activated to select the current image. */
  virtual void ImageSelected(const mitk::DataTreeFilter::Item * imageIt);

  /** \brief Slot method that will be called if TreeNodeSelector widget was activated to select the current segmentation. */
  virtual void SegmentationSelected(const mitk::DataTreeFilter::Item * imageIt);

  /** \brief Slot method that will be called if the CheckBox at the Threshold-Group-Box was toggled to show the threshold image or not. */ 
  void ShowPixelGreyValueManipulatorContent(bool show = true);

  /** \brief Slot method that will be called if the CheckBox at the TreeNodeSelector-Group-Box was toggled to show the TreeNodeSelector or not. */ 
  void ShowImageContent(bool show = true);

  ///** \brief Method is called when the DataTree was changed. This Method is 	self-evident obligatory */
  //virtual void TreeChanged();

  /** \brief Method is called when the "create new manipulated image" - was pressed. This Method coordinates all steps to create a new manipulated image  */
  virtual void PipelineControllerToCreateManipulatedImage();

  /** \brief Method is called when the ManipulationMode was selected to decide 	whether value2 shall be visible or not */
  virtual void HideOrShowValue2(int index);

  /** \brief Slot-Method to set the Threshold from extern, for example by signal-slot from the surfaceCreatorComponent */
  void SetThreshold(const QString&);

protected:

  /** \brief Method to update the content of all DataTreeComboBoxes. */
  virtual void UpdateDataTreeComboBoxes();

  /** \brief Method to call the linearShift-TemplateMethod */
  void LinearShift();

  /** \brief Method to call the gradientShift-TemplateMethod */
  void GradientShift();

  /** \brief Method to call the ChangeGreyValuet-TemplateMethod */
  void ChangeGreyValue();

  /** \brief Method to call the lightenOrShade-TemplateMethod */
  void LightenOrShade();



  /***************        ATTRIBUTES      ***************/

  ///*!
  //the name of the parent-component
  //*/
  //QString m_ParentName;

  ///*!
  //the name of the component
  //*/
  //QString m_ComponentName;

  ///*!
  //a reference to the MultiWidget
  //*/
  //QmitkStdMultiWidget * m_MultiWidget;

  ///*!
  //a reference to a data tree iterator object
  //*/
  //mitk::DataTreeIteratorClone m_DataTreeIteratorClone;

  ///** \brief Attribute to decide whether the selector shall be updated when a parent-Selector is updatet or not */
  //bool m_UpdateSelector;

  ///** \brief Attribute to decide whether the selector shall be shown or not */
  //bool m_ShowSelector;
 
  ///** \brief Attribute to that will be set to true by the first activation of the functionality */
  //bool m_Active;


private:


  /***************        ATTRIBUTES      ***************/

  /** \brief The created GUI from the .ui-File. This Attribute is	obligatory*/
  QmitkPixelGreyValueManipulatorComponentGUI * m_PixelGreyValueManipulatorComponentGUI;

  ///** \brief This Attribute holds the current ImageItem from the TreeNodeSelector if his selection was changed*/
  //const mitk::DataTreeFilter::Item * m_SelectedImage; 

  ///** \brief Spacer added at the end of the component */
  //QSpacerItem* m_Spacer;

  /*!
  * base image for the pixelmanipulated image
  */
  mitk::Image* m_MitkImage;

  /*!
  * Image which includes the segmentation, if existing (to be raised) 
  */
  mitk::Image::Pointer m_Segmentation;

  /*!
  * iterator on current image
  */
  mitk::DataTreeIteratorClone m_MitkImageIterator;

    /*!
  * Image after ValueChange 
  */
  mitk::Image::Pointer m_PixelChangedImage;

    /*!
  * Node which includes the changed image (shift, gradient or normal change)
  */
  mitk::DataTreeNode::Pointer m_PixelChangedImageNode;

  
  /*!
  * Node which includes the Shifted Resultimage 
  */
  mitk::DataTreeNode::Pointer m_SegmentedShiftResultNode;

    /*!
  * iterator on the shifted Image node 
  */
  mitk::DataTreeIteratorClone m_ItNewBuildSeg;

  /*!
  * Node which includes the "eris"-segmentation, if existing (to be raised) 
  */
  mitk::DataTreeNode::ConstPointer m_SegmentationNode;

  /*!
  *  member to count PixelChangedImage an give it to name in DataTree 
  */
  int m_PixelChangedImageCounter;

  /*!
  *  member for the mode with that the new image shall be created. Values are:  0 = Nothing selected, 1 = linear shift, 2 = gradient shift, 3 = change grey value, 4 = lighten / shade
  */
  int m_ManipulationMode;

  /*!
  *  member for the area on that the new image shall be manipulated: 0 and 1 is the whole image, 2 only on segmented areas.
  */
  int m_ManipulationArea;

  //BEGIN ONLY FOR SEEDPOINTS******************************************************************************************************************************************
    /*!
  * Node for the seed-points for threshold-gradient
  */
  mitk::DataTreeNode::Pointer m_SeedPointSetNode;

  /*!
  * Interactor for the seed-points for threshold-gradient
  */
  mitk::PointSetInteractor::Pointer m_SeedPointSetInteractor;

    /*!
  * PointSetPointer for the seed-points for threshold-gradient
  */
  mitk::PointSet::Pointer m_Seeds;
  //END ONLY FOR SEEDPOINTS******************************************************************************************************************************************


  /** \brief Method to calculate the shiftvalue for the gradient shift and add it into new image */
  template < typename ItkImageType >  
  void InternalGradientShiftCalculation(int & shiftedThresholdOne, int & shiftedThresholdTwo, int & normalThreshold, itk::ImageRegionIterator<ItkImageType> & itShifted, itk::ImageRegionConstIterator<ItkImageType> & it, mitk::PointSet::PointType & pointOne, mitk::PointSet::PointType & pointTwo);

  template < typename ItkImageType>
  void AddManipulatedImageIntoTree(typename ItkImageType::Pointer & itkShiftedImage);

  void GetManipulationModeAndAreaFromGUI(int & manipulationMode, int & manipulationArea);
  void GetManipulationValueFromGUI(int & value1, int & value2, int & baseValue);

  template < typename TPixel, unsigned int VImageDimension > 
  void CreateLinearShiftedImage( itk::Image< TPixel, VImageDimension >* itkImage, const mitk::Image* segmentation);

  template < typename TPixel, unsigned int VImageDimension > 
  void CreateGradientShiftedImage( itk::Image< TPixel, VImageDimension >* itkImage, const mitk::Image* segmentation);

  template < typename TPixel, unsigned int VImageDimension > 
  void CreateChangedGreyValueImage( itk::Image< TPixel, VImageDimension >* itkImage, const mitk::Image* segmentation);

  template < typename TPixel, unsigned int VImageDimension > 
  void CreateLightenOrShadeImage( itk::Image< TPixel, VImageDimension >* itkImage, const mitk::Image* segmentation);
};

#endif

