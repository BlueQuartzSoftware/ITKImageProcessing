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
#include "MontageImportHelper.h"

#include <QtCore/QObject>

// -----------------------------------------------------------------------------
MontageImportHelper::MontageImportHelper() = default;

// -----------------------------------------------------------------------------
MontageImportHelper::~MontageImportHelper() = default;

// -----------------------------------------------------------------------------
ITKImageReader::Pointer MontageImportHelper::CreateImageImportFilter(AbstractFilter* filter, const QString& imageFileName, const DataArrayPath& daPath)
{
  ITKImageReader::Pointer imageReader = ITKImageReader::New();

  // Connect up the Error/Warning/Progress object so the filter can report those things
  QObject::connect(imageReader.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), filter, SIGNAL(messageGenerated(const AbstractMessage::Pointer&)));
  DataContainerArrayShPtr dca = DataContainerArray::New();

  imageReader->setDataContainerArray(dca); // AbstractFilter implements this so no problem
  imageReader->setFileName(imageFileName);
  imageReader->setDataContainerName(daPath);
  imageReader->setCellAttributeMatrixName(daPath.getAttributeMatrixName());
  imageReader->setImageDataArrayName(daPath.getDataArrayName());

  return imageReader;
}

// -----------------------------------------------------------------------------
ConvertColorToGrayScale::Pointer MontageImportHelper::CreateColorToGrayScaleFilter(AbstractFilter* filter, const DataArrayPath& daPath, const FloatVec3Type& colorWeights,
                                                                                   const QString& outputArrayName)
{
  ConvertColorToGrayScale::Pointer rgbToGray = ConvertColorToGrayScale::New();

  // Connect up the Error/Warning/Progress object so the filter can report those things
  QObject::connect(rgbToGray.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), filter, SIGNAL(messageGenerated(const AbstractMessage::Pointer&)));

  rgbToGray->setDataContainerArray(filter->getDataContainerArray());
  rgbToGray->setConversionAlgorithm(0);
  rgbToGray->setColorWeights(colorWeights);
  QVector<DataArrayPath> inputDataArrayVector = {daPath};
  rgbToGray->setInputDataArrayVector(inputDataArrayVector);
  rgbToGray->setCreateNewAttributeMatrix(false);
  rgbToGray->setOutputAttributeMatrixName(daPath.getAttributeMatrixName());
  rgbToGray->setOutputArrayPrefix(outputArrayName);

  return rgbToGray;
}
