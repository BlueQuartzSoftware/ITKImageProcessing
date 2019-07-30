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

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

class Observable;

class ITKImageProcessing_EXPORT AxioVisionV4Converter : public QObject
{
  Q_OBJECT

public:
  AxioVisionV4Converter();
  ~AxioVisionV4Converter() override;

  /**
   * @brief ConvertToJson
   * @param axioVisionFile
   * @return
   */
  static QJsonObject ConvertToJson(const QString& axioVisionFile, Observable* obs = nullptr);

  /**
   * @brief writeToJson
   * @param filepath
   */
  static void WriteToJson(const QString& axioVisionFile, const QString& outputJsonFile, Observable* obs);

public:
  AxioVisionV4Converter(const AxioVisionV4Converter&) = delete;            // Copy Constructor Not Implemented
  AxioVisionV4Converter(AxioVisionV4Converter&&) = delete;                 // Move Constructor Not Implemented
  AxioVisionV4Converter& operator=(const AxioVisionV4Converter&) = delete; // Copy Assignment Not Implemented
  AxioVisionV4Converter& operator=(AxioVisionV4Converter&&) = delete;      // Move Assignment Not Implemented
};
