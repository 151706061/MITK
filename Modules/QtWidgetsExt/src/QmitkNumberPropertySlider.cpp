/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include <QmitkNumberPropertySlider.h>

#include <mitkProperties.h>
#include <mitkPropertyObserver.h>

#include <mitkRenderingManager.h>

#define DT_SHORT 1
#define DT_INT 2
#define DT_FLOAT 3
#define DT_DOUBLE 4

#define ROUND(x) (((x) > 0) ? int((x) + 0.5) : int((x)-0.5))
#define ROUND_SHORT(x) (((x) > 0) ? short((x) + 0.5) : short((x)-0.5))

class QmitkNumberPropertySlider::Impl
{
public:
  Impl(QmitkNumberPropertySlider *q);

  void DisplayNumber();
  void adjustFactors(short, bool);

  class Editor : public mitk::PropertyEditor
  {
  public:
    Editor(mitk::IntProperty *, Impl *impl);
    Editor(mitk::FloatProperty *, Impl *impl);
    Editor(mitk::DoubleProperty *, Impl *impl);

    void PropertyChanged() override;
    void PropertyRemoved() override;

    void BeginModifyProperty() { mitk::PropertyEditor::BeginModifyProperty(); }
    void EndModifyProperty() { mitk::PropertyEditor::EndModifyProperty(); }
    union {
      mitk::GenericProperty<int> *m_IntProperty;
      mitk::GenericProperty<float> *m_FloatProperty;
      mitk::GenericProperty<double> *m_DoubleProperty;
    };

    const int m_DataType;

  private:
    Impl *m_Impl;
  };

  std::unique_ptr<Editor> m_PropEditor;

  short m_DecimalPlaces;           // how many decimal places are shown
  double m_FactorPropertyToSlider; // internal conversion factor. necessary because slider ranges work only with ints
  double m_FactorSliderToDisplay;  // internal conversion factor. necessary because slider ranges work only with ints
  bool m_ShowPercents;             // whether values are given in percent (0.5 -> 50%)

  bool m_SelfChangeLock;

private:
  void initialize();

  QmitkNumberPropertySlider *q;
};

QmitkNumberPropertySlider::Impl::Editor::Editor(mitk::IntProperty *property, Impl *impl)
  : mitk::PropertyEditor(property), m_IntProperty(property), m_DataType(DT_INT), m_Impl(impl)
{
}

QmitkNumberPropertySlider::Impl::Editor::Editor(mitk::FloatProperty *property, Impl *impl)
  : mitk::PropertyEditor(property), m_FloatProperty(property), m_DataType(DT_FLOAT), m_Impl(impl)
{
}

QmitkNumberPropertySlider::Impl::Editor::Editor(mitk::DoubleProperty *property, Impl *impl)
  : mitk::PropertyEditor(property), m_DoubleProperty(property), m_DataType(DT_DOUBLE), m_Impl(impl)
{
}

QmitkNumberPropertySlider::~QmitkNumberPropertySlider()
{
}

void QmitkNumberPropertySlider::SetProperty(mitk::IntProperty *property)
{
  if (property == nullptr)
  {
    d->m_PropEditor.reset();
    this->setEnabled(false);
  }
  else
  {
    d->m_PropEditor.reset(new Impl::Editor(property, d.get()));
    d->m_PropEditor->PropertyChanged();
    this->setEnabled(true);
  }
}

void QmitkNumberPropertySlider::SetProperty(mitk::FloatProperty *property)
{
  if (property == nullptr)
  {
    d->m_PropEditor.reset();
    this->setEnabled(false);
  }
  else
  {
    d->m_PropEditor.reset(new Impl::Editor(property, d.get()));
    d->m_PropEditor->PropertyChanged();
    this->setEnabled(true);
  }
}

void QmitkNumberPropertySlider::SetProperty(mitk::DoubleProperty *property)
{
  if (property == nullptr)
  {
    d->m_PropEditor.reset();
    this->setEnabled(false);
  }
  else
  {
    d->m_PropEditor.reset(new Impl::Editor(property, d.get()));
    d->m_PropEditor->PropertyChanged();
    this->setEnabled(true);
  }
}

QmitkNumberPropertySlider::Impl::Impl(QmitkNumberPropertySlider *q)
  : m_DecimalPlaces(0),
    m_FactorPropertyToSlider(1.0),
    m_FactorSliderToDisplay(1.0),
    m_ShowPercents(false),
    m_SelfChangeLock(false),
    q(q)
{
}

QmitkNumberPropertySlider::QmitkNumberPropertySlider(QWidget *parent) : QSlider(parent), d(new Impl(this))
{
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
  this->setEnabled(false);
}

void QmitkNumberPropertySlider::Impl::adjustFactors(short newDecimalPlaces, bool newShowPercents)
{
  int oldMax = q->maxValue();
  int oldMin = q->minValue();

  m_DecimalPlaces = newDecimalPlaces;
  m_ShowPercents = newShowPercents;

  m_FactorPropertyToSlider = pow(10.0, m_DecimalPlaces);
  m_FactorSliderToDisplay = 1.0 / m_FactorPropertyToSlider;

  if (m_ShowPercents)
    m_FactorPropertyToSlider *= 100.0;

  q->setMinimum(oldMin);
  q->setMaximum(oldMax);
}

short QmitkNumberPropertySlider::getDecimalPlaces() const
{
  return d->m_DecimalPlaces;
}

void QmitkNumberPropertySlider::setDecimalPlaces(short places)
{
  if (d->m_PropEditor.get() == nullptr)
    return;

  switch (d->m_PropEditor->m_DataType)
  {
    case DT_FLOAT:
    case DT_DOUBLE:
    {
      d->adjustFactors(places, d->m_ShowPercents);
      d->DisplayNumber();
      break;
    }
    default:
      break;
  }
}

bool QmitkNumberPropertySlider::getShowPercent() const
{
  return d->m_ShowPercents;
}

void QmitkNumberPropertySlider::setShowPercent(bool showPercent)
{
  if (showPercent == d->m_ShowPercents)
    return; // nothing to change

  if (d->m_PropEditor.get() == nullptr)
    return;

  switch (d->m_PropEditor->m_DataType)
  {
    case DT_FLOAT:
    case DT_DOUBLE:
    {
      d->adjustFactors(d->m_DecimalPlaces, showPercent);
      break;
    }
    default:
    {
      break;
    }
  }

  d->DisplayNumber();
}

int QmitkNumberPropertySlider::minValue() const
{
  return ROUND(QSlider::minimum() / d->m_FactorPropertyToSlider);
}

void QmitkNumberPropertySlider::setMinValue(int value)
{
  QSlider::setMinimum(ROUND(value * d->m_FactorPropertyToSlider));
}

int QmitkNumberPropertySlider::maxValue() const
{
  return ROUND(QSlider::maximum() / d->m_FactorPropertyToSlider);
}

void QmitkNumberPropertySlider::setMaxValue(int value)
{
  QSlider::setMaximum(ROUND(value * d->m_FactorPropertyToSlider));
}

double QmitkNumberPropertySlider::doubleValue() const
{
  return static_cast<double>((QSlider::value()) / d->m_FactorPropertyToSlider);
}

void QmitkNumberPropertySlider::setDoubleValue(double value)
{
  QSlider::setValue(ROUND(value * d->m_FactorPropertyToSlider));
}

void QmitkNumberPropertySlider::onValueChanged(int value)
{
  if (d->m_PropEditor.get() == nullptr)
    return;

  if (d->m_SelfChangeLock)
    return; // valueChanged is even emitted, when this widget initiates a change to its value
            // this may be useful some times, but in this use, it would be wrong:
            //   (A) is an editor with 3 decimal places
            //   (B) is an editor with 2 decimal places
            //   User changes A's displayed value to 4.002
            //   A's onValueChanged gets called, sets the associated Property to 4.002
            //   B's onPropertyChanged gets called, sets its display to 4.00
            //   B's onValueChanged gets called and sets the associated Property to 4.00
            //   A's onPropertyChanged gets called, sets its display to 4.000

  d->m_PropEditor->BeginModifyProperty();

  double newValue(value / d->m_FactorPropertyToSlider);

  switch (d->m_PropEditor->m_DataType)
  {
    case DT_INT:
    {
      d->m_PropEditor->m_IntProperty->SetValue(ROUND(newValue));
      break;
    }
    case DT_FLOAT:
    {
      d->m_PropEditor->m_FloatProperty->SetValue(newValue);
      break;
    }
    case DT_DOUBLE:
    {
      d->m_PropEditor->m_DoubleProperty->SetValue(newValue);
      break;
    }
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();

  d->m_PropEditor->EndModifyProperty();
}

void QmitkNumberPropertySlider::Impl::Editor::PropertyChanged()
{
  m_Impl->DisplayNumber();
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void QmitkNumberPropertySlider::Impl::Editor::PropertyRemoved()
{
  this->m_Property = nullptr;
}

void QmitkNumberPropertySlider::Impl::DisplayNumber()
{
  if (m_PropEditor.get() == nullptr)
    return;

  m_SelfChangeLock = true;
  switch (m_PropEditor->m_DataType)
  {
    case DT_INT:
    {
      int i = m_PropEditor->m_IntProperty->GetValue();
      q->setValue(i);
      break;
    }
    case DT_FLOAT:
    {
      float f = m_PropEditor->m_FloatProperty->GetValue();
      q->setDoubleValue(f);
      break;
    }
    case DT_DOUBLE:
    {
      double d = m_PropEditor->m_DoubleProperty->GetValue();
      q->setDoubleValue(d);
      break;
    }
    default:
      break;
  }
  m_SelfChangeLock = false;
}
