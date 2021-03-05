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

#pragma once

#include <QtCore/QString>
#include <QtWidgets/QWidget>

#include "SVWidgetsLib/FilterParameterWidgets/FilterParameterWidget.h"

namespace Ui
{
class EbsdWarpPolynomialWidget;
}
class AbstractFilter;
class EbsdWarpPolynomialFilterParameter;

/**
 * @brief
 * @author
 * @version
 */
class EbsdWarpPolynomialWidget : public FilterParameterWidget
{
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param parameter The FilterParameter object that this widget represents
   * @param filter The instance of the filter that this parameter is a part of
   * @param parent The parent QWidget for this Widget
   */
  EbsdWarpPolynomialWidget(FilterParameter* parameter, AbstractFilter* filter = nullptr, QWidget* parent = nullptr);

  ~EbsdWarpPolynomialWidget() override;

  /**
   * @brief This method does additional GUI widget connections
   */
  void setupGui() override;

public Q_SLOTS:
  void widgetChanged(const QString& msg);
  void beforePreflight();
  void afterPreflight();
  void filterNeedsInputParameters(AbstractFilter* filter);

private:
  QSharedPointer<Ui::EbsdWarpPolynomialWidget> m_Ui;

  EbsdWarpPolynomialFilterParameter* m_FilterParameter = nullptr;

public:
  EbsdWarpPolynomialWidget(const EbsdWarpPolynomialWidget&) = delete;            // Copy Constructor Not Implemented
  EbsdWarpPolynomialWidget(EbsdWarpPolynomialWidget&&) = delete;                 // Move Constructor Not Implemented
  EbsdWarpPolynomialWidget& operator=(const EbsdWarpPolynomialWidget&) = delete; // Copy Assignment Not Implemented
  EbsdWarpPolynomialWidget& operator=(EbsdWarpPolynomialWidget&&) = delete;      // Move Assignment Not Implemented
};
