#include "QmitkLightBoxReaderDialog.h"

#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcursor.h>
#include <qapplication.h>

QmitkLightBoxReaderDialog::QmitkLightBoxReaderDialog( QWidget* parent, const char* name )
:QDialog( parent, name, true )
{
  QDialog::setMinimumSize(250, 300);

  QBoxLayout * verticalLayout = new QVBoxLayout( this );
  verticalLayout->setMargin(5);
  verticalLayout->setSpacing(5);

  verticalLayout->addWidget( new QLabel( "Slices contain different spacings. \n Please select the spacing you want to use.", this ) );
  m_Spacings = new QListBox( this );

  verticalLayout->addWidget( m_Spacings );

  m_Ok = new QPushButton( tr("OK"), this );
  m_Ok->setDefault(true);
  m_Ok->setFocus();
  connect( m_Ok, SIGNAL( clicked() ), this, SLOT( accept() ) );

  m_Abourt = new QPushButton( tr("Cancel"), this );
  connect( m_Abourt, SIGNAL( clicked() ), this, SLOT( reject() ) );

  QBoxLayout * horizontalLayout = new QHBoxLayout( verticalLayout );
  horizontalLayout->setSpacing(5);
  horizontalLayout->addStretch();
  horizontalLayout->addWidget( m_Ok );
  horizontalLayout->addWidget( m_Abourt );

  QApplication::restoreOverrideCursor();
}

QmitkLightBoxReaderDialog::~QmitkLightBoxReaderDialog()
{
}

mitk::Vector3D QmitkLightBoxReaderDialog::GetSpacing()
{
  QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
  return m_SpacingVector[ m_Spacings->currentItem() ];
}

void QmitkLightBoxReaderDialog::addSpacings( mitk::Vector3D spacing, int count )
{
  std::ostringstream stringHelper;
  stringHelper << "Spacing: ";
  stringHelper << spacing[2];
  stringHelper << ", Count: ";
  stringHelper << count;
  std::string resultString = stringHelper.str();

  m_Spacings->insertItem( resultString.c_str() );
  m_SpacingVector.push_back( spacing );

  m_Spacings->setCurrentItem(0);
}

