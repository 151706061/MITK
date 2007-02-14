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


#ifndef QMITKPROGRESSBAR_H
#define QMITKPROGRESSBAR_H
#include <mitkProgressBarImplementation.h>
#include <mitkCommon.h>
#include <qprogressbar.h>

//##Documentation
//## @brief QT-Toolkit/GUI dependent class that provides the QT's ProgressBar
//##
//## The application sets the Instance to mitkProgressBar so that
//## all mitk-classes will call this class for output:
//## mitk::ProgressBar::SetInstance(QmitkProgressBar::GetInstance());
//## 
//## Then the applikation sets the MainWindow ProgressBar by:
//## QmitkProgressBar::SetProgressBar(QMainWindow::progressBar());

class QmitkProgressBar : public mitk::ProgressBarImplementation
{
public:

  mitkClassMacro(QmitkProgressBar, mitk::ProgressBarImplementation);
 
  //##Documentation
  //##@brief Constructor;
  //## holds param instance internaly and connects this to the mitkProgressBar
  QmitkProgressBar(QProgressBar* instance);
  
  //##Documentation
  //##@brief Destructor
  virtual ~QmitkProgressBar();

  //##Documentation
  //## @brief Sets the total number of steps to totalSteps.
  virtual void SetTotalSteps(int totalSteps);
  //##Documentation
  //## @brief Sets the current amount of progress to progress.
  virtual void SetProgress(int progress);
  //##Documentation
  //## @brief Sets the amount of progress to progress and the total number of steps to totalSteps.
  virtual void SetProgress(int progress, int totalSteps);

  //##Documentation
  //## @brief Reset the progress bar. The progress bar "rewinds" and shows no progress.
  virtual void Reset();

  //##Documentation
  //## @brief Sets whether the current progress value is displayed.
  virtual void SetPercentageVisible(bool visible);
    
private:
    //static Pointer m_Instance;
    QProgressBar* m_ProgressBar;

};

#endif /* define QMITKPROGRESSBAR_H */
