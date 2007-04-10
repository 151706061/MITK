/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include "QmitkMemoryUsageIndicator.h"
#include "QmitkMemoryUsageIndicatorImages.h"
#include <mitkMemoryUtilities.h>

#include <qtimer.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qeventloop.h>

#include <iostream>
#include <sstream>
#include <iomanip>

void QmitkMemoryUsageIndicator::init()
{
  QTimer *timer = new QTimer( this );
  connect( timer, SIGNAL( timeout() ), this, SLOT( UpdateMemoryUsage() ) );
  timer->start(1000);
  m_LEDGreen.convertFromImage( qembed_findImage("led-green") );
  m_LEDYellow.convertFromImage( qembed_findImage("led-yellow") );
  m_LEDOrange.convertFromImage( qembed_findImage("led-orange") );
  m_LEDRed.convertFromImage( qembed_findImage("led-red") );
}

void QmitkMemoryUsageIndicator::UpdateMemoryUsage()
{
  size_t processSize = mitk::MemoryUtilities::GetProcessMemoryUsage();
  size_t totalSize =  mitk::MemoryUtilities::GetTotalSizeOfPhysicalRam();
  float percentage = ( (float) processSize / (float) totalSize ) * 100.0;
  m_Label->setText( GetMemoryDescription( processSize, percentage ) );
  if ( percentage < 50.0 )
    m_LED->setPixmap(m_LEDGreen);
  else if ( percentage < 65.0 )
    m_LED->setPixmap(m_LEDYellow);
  else if ( percentage < 80.0 )
    m_LED->setPixmap(m_LEDOrange);
  else
    m_LED->setPixmap(m_LEDRed);
  QApplication::eventLoop()->processEvents( QEventLoop::ExcludeUserInput );
}


std::string QmitkMemoryUsageIndicator::FormatMemorySize( size_t size )
{
  double val = size;
  std::string descriptor("B");
  if ( val >= 1000.0 )
  {
    val /= 1024.0;
    descriptor = "KB";
  }
  if ( val >= 1000.0 )
  {
    val /= 1024.0;
    descriptor = "MB";
  }
  if ( val >= 1000.0 )
  {
    val /= 1024.0;
    descriptor = "GB";
  }
  std::ostringstream str;
  str << std::fixed << std::setprecision(2) << val << " " << descriptor;
  return str.str();
}

std::string QmitkMemoryUsageIndicator::FormatPercentage( double val )
{
  std::ostringstream str;
  str << std::fixed << std::setprecision(2) << val << " " << "%";
  return str.str();
}

std::string QmitkMemoryUsageIndicator::GetMemoryDescription( size_t processSize, float percentage )
{
  std::ostringstream str;
  str << FormatMemorySize(processSize) << " (" << FormatPercentage( percentage ) <<")" ;
  return str.str();
}
