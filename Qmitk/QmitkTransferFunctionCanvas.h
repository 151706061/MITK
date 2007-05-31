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

#ifndef QMITKTRANSFERFUNCTIONCANVAS_H_INCLUDED
#define QMITKTRANSFERFUNCTIONCANVAS_H_INCLUDED

#include "mitkHistogramGenerator.h"

#include <set>
#include <qwidget.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

#include "QmitkTransferFunctionWidget.h"

#include <vtkPiecewiseFunction.h>


class vtkColorTransferFunction;
class QmitkTransferFunctionCanvas : public QWidget
{

  Q_OBJECT

  public:

    QmitkTransferFunctionCanvas( QWidget * parent=0, const char * name=0, WFlags f = false );
    mitk::HistogramGenerator::HistogramType::ConstPointer GetHistogram()
    {
      return m_Histogram;
    };
    void SetHistogram(const mitk::HistogramGenerator::HistogramType* histogram)
    {
      m_Histogram = histogram;
    }
    vtkFloatingPointType GetMin()
    {
      return m_Min;
    }
    void SetMin(vtkFloatingPointType min)
    {
      this->m_Min = min;
      SetLower(min);
    }
    vtkFloatingPointType GetMax()
    {
      return m_Max;
    }
    void SetMax(vtkFloatingPointType max)
    {
      this->m_Max = max;
      SetUpper(max);
    }
    vtkFloatingPointType GetLower()
    {
      return m_Lower;
    }
    void SetLower(vtkFloatingPointType lower)
    {
      this->m_Lower = lower;
    }
    vtkFloatingPointType GetUpper()
    {
      return m_Upper;
    }
    void SetUpper(vtkFloatingPointType upper)
    {
      this->m_Upper = upper;
    }

    // itkGetConstObjectMacro(Histogram,mitk::HistogramGenerator::HistogramType);
    // itkSetObjectMacro(Histogram,mitk::HistogramGenerator::HistogramType);
    void mousePressEvent( QMouseEvent* mouseEvent );
    virtual void paintEvent( QPaintEvent* e );
    virtual void DoubleClickOnHandle(int handle) = 0;

    void mouseMoveEvent( QMouseEvent* mouseEvent );
    void mouseReleaseEvent( QMouseEvent* mouseEvent );
    void mouseDoubleClickEvent( QMouseEvent* mouseEvent );
    void PaintHistogram(QPainter &p);
    void PaintHistogramGO(QPainter &p);

    virtual int GetNearHandle(int x,int y,unsigned int maxSquaredDistance = 32) = 0;
    virtual void AddFunctionPoint(vtkFloatingPointType x,vtkFloatingPointType val) = 0;
    virtual void RemoveFunctionPoint(vtkFloatingPointType x) = 0;
    virtual void MoveFunctionPoint(int index, std::pair<vtkFloatingPointType,vtkFloatingPointType> pos) = 0;
    virtual vtkFloatingPointType GetFunctionX(int index) = 0;
    virtual float GetFunctionY(int index) = 0;
    virtual int GetFunctionSize() = 0;
    int m_GrabbedHandle;
    std::pair<int,int> m_MoveStart;
    vtkFloatingPointType m_Lower, m_Upper, m_Min, m_Max;
    std::pair<int,int> FunctionToCanvas(std::pair<vtkFloatingPointType,vtkFloatingPointType>);
    std::pair<vtkFloatingPointType,vtkFloatingPointType> CanvasToFunction(std::pair<int,int>);
    mitk::HistogramGenerator::HistogramType::ConstPointer m_Histogram;
    void keyPressEvent ( QKeyEvent * e ); 
    
    void SetImmediateUpdate(bool state);
    
    void SetTFWidget(QmitkTransferFunctionWidget* widget)
    {
      m_TFWidget = widget;
      m_TFWidgetAvailable = true;
    }
    
  protected: 
    bool m_ImmediateUpdate;
    float m_Range;
    bool m_TFWidgetAvailable;
    QmitkTransferFunctionWidget* m_TFWidget;
};
#endif

