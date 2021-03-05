/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "EbsdWarpPolynomialWidget.h"

#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Utilities/FilterCompatibility.hpp"

#include "ITKImageProcessing/FilterParameters/EbsdWarpPolynomialFilterParameter.h"

#include "ui_EbsdWarpPolynomialWidget.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
/* These macros are mainly used by the 2nd, 3rd and 4rth Order Polynomial Widgets
 * We put them here so all three can use these macros and we do not duplicate
 * the code into 3 different locations
 */

#define EWPW_SETUP_WIDGET(cell)                                                                                                                                                                        \
  connect(m_Ui->cell, SIGNAL(textChanged(const QString&)), this, SLOT(widgetChanged(const QString&)));                                                                                                 \
  QDoubleValidator* cell##Val = new QDoubleValidator(m_Ui->cell);                                                                                                                                      \
  cell##Val->setLocale(loc);                                                                                                                                                                           \
  m_Ui->cell->setValidator(cell##Val);

#define EWPW_CHECK_LINEEDIT(cell)                                                                                                                                                                      \
  {                                                                                                                                                                                                    \
    if(sender() == m_Ui->cell && m_Ui->cell->text().isEmpty())                                                                                                                                         \
    {                                                                                                                                                                                                  \
      SVStyle::Instance()->LineEditErrorStyle(m_Ui->cell);                                                                                                                                             \
      SVStyle::Instance()->SetErrorColor("QLabel", m_Ui->errorLabel);                                                                                                                                  \
      m_Ui->errorLabel->setText("No value entered. Filter will use default value of " + getFilterParameter()->getDefaultValue().toString());                                                           \
      m_Ui->errorLabel->show();                                                                                                                                                                        \
    }                                                                                                                                                                                                  \
  }

#define EWPW_EXTRACT_VALUE(cell)                                                                                                                                                                       \
  data[cell] = loc.toFloat(m_Ui->a##cell->text(), &ok);                                                                                                                                                \
  if(!ok)                                                                                                                                                                                              \
  {                                                                                                                                                                                                    \
    if(m_Ui->errorLabel)                                                                                                                                                                               \
    {                                                                                                                                                                                                  \
      SVStyle::Instance()->SetErrorColor("QLabel", m_Ui->errorLabel);                                                                                                                                  \
      m_Ui->errorLabel->setText("Value entered is beyond the representable range for the value type.\nThe filter will use the default value of " +                                                     \
                                getFilterParameter()->getDefaultValue().toString());                                                                                                                   \
      m_Ui->errorLabel->show();                                                                                                                                                                        \
    }                                                                                                                                                                                                  \
    SVStyle::Instance()->LineEditErrorStyle(m_Ui->a##cell);                                                                                                                                            \
    data[cell] = defValue[cell];                                                                                                                                                                       \
  }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdWarpPolynomialWidget::EbsdWarpPolynomialWidget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent)
: FilterParameterWidget(parameter, filter, parent)
, m_Ui(new Ui::EbsdWarpPolynomialWidget)
{
  m_FilterParameter = SIMPL_FILTER_PARAMETER_COMPATIBILITY_CHECK(filter, parameter, EbsdWarpPolynomialWidget, EbsdWarpPolynomialFilterParameter);
  m_Ui->setupUi(this);
  setupGui();
}
// -----------------------------------------------------------------------------
EbsdWarpPolynomialWidget::~EbsdWarpPolynomialWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdWarpPolynomialWidget::setupGui()
{
  // Catch when the filter is about to execute the preflight
  connect(getFilter(), SIGNAL(preflightAboutToExecute()), this, SLOT(beforePreflight()));

  // Catch when the filter is finished running the preflight
  connect(getFilter(), SIGNAL(preflightExecuted()), this, SLOT(afterPreflight()));

  // Catch when the filter wants its values updated
  connect(getFilter(), SIGNAL(updateFilterParameters(AbstractFilter*)), this, SLOT(filterNeedsInputParameters(AbstractFilter*)));

  QLocale loc = QLocale::system();

  EWPW_SETUP_WIDGET(a0)
  EWPW_SETUP_WIDGET(a1)
  EWPW_SETUP_WIDGET(a2)
  EWPW_SETUP_WIDGET(a3)
  EWPW_SETUP_WIDGET(a4)
  EWPW_SETUP_WIDGET(a5)
  EWPW_SETUP_WIDGET(a6)

  if(getFilterParameter() != nullptr)
  {
    m_Ui->label->setText(getFilterParameter()->getHumanLabel());

    FloatVec7Type data = m_FilterParameter->getGetterCallback()();
    m_Ui->a0->setText(loc.toString(data[0]));
    m_Ui->a1->setText(loc.toString(data[1]));
    m_Ui->a2->setText(loc.toString(data[2]));
    m_Ui->a3->setText(loc.toString(data[3]));
    m_Ui->a4->setText(loc.toString(data[4]));
    m_Ui->a5->setText(loc.toString(data[5]));
    m_Ui->a6->setText(loc.toString(data[6]));
  }

  m_Ui->errorLabel->hide();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdWarpPolynomialWidget::widgetChanged(const QString& text)
{
  Q_UNUSED(text)
  m_Ui->errorLabel->hide();

  EWPW_CHECK_LINEEDIT(a0)
  EWPW_CHECK_LINEEDIT(a1)
  EWPW_CHECK_LINEEDIT(a2)
  EWPW_CHECK_LINEEDIT(a3)
  EWPW_CHECK_LINEEDIT(a4)
  EWPW_CHECK_LINEEDIT(a5)
  EWPW_CHECK_LINEEDIT(a6)

  Q_EMIT parametersChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdWarpPolynomialWidget::filterNeedsInputParameters(AbstractFilter* filter)
{
  bool ok = false;
  FloatVec7Type data;
  FloatVec7Type defValue = m_FilterParameter->getDefaultValue().value<FloatVec7Type>();

  QLocale loc;

  EWPW_EXTRACT_VALUE(0)
  EWPW_EXTRACT_VALUE(1)
  EWPW_EXTRACT_VALUE(2)
  EWPW_EXTRACT_VALUE(3)
  EWPW_EXTRACT_VALUE(4)
  EWPW_EXTRACT_VALUE(5)
  EWPW_EXTRACT_VALUE(6)

  m_FilterParameter->getSetterCallback()(data);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdWarpPolynomialWidget::beforePreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdWarpPolynomialWidget::afterPreflight()
{
}
