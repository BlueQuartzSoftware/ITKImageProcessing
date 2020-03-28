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

#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/MetaXmlUtils.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MontageImportHelper::MontageImportHelper() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MontageImportHelper::~MontageImportHelper() = default;

// -----------------------------------------------------------------------------
//
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
//
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::map<int32_t, std::vector<size_t>> MontageImportHelper::Burn(int32_t tolerance, std::vector<int32_t>& input)
{
  int32_t halfTol = tolerance / 2;
  size_t count = input.size();
  int32_t seed = input[0];
  std::vector<bool> visited(input.size(), false);
  std::map<int32_t, std::vector<size_t>> avg_indices;

  bool completed = false;
  while(!completed)
  {
    std::vector<size_t> values;
    for(size_t i = 0; i < count; i++)
    {
      // const BoundsType& bound = bounds.at(i);
      if(input[i] < seed + halfTol && input[i] > seed - halfTol)
      {
        values.push_back(i);
        visited[i] = true;
      }
    }

    int32_t avg = 0;
    for(const auto& v : values)
    {
      avg = avg + input.at(v);
    }
    avg = avg / values.size();
    avg_indices[avg] = values;
    seed = 0;
    completed = true;
    for(size_t i = 0; i < count; i++)
    {
      if(!visited[i])
      {
        seed = input[i];
        completed = false;
        break;
      }
    }
  }
  return avg_indices;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString MontageImportHelper::GenerateDataContainerName(const QString& dataContainerPrefix, int32_t paddingDigits, int32_t row, int32_t col)
{
  QString dcName = dataContainerPrefix;
  QTextStream dcNameStream(&dcName);
  dcNameStream << "r";
  dcNameStream.setFieldWidth(paddingDigits);
  dcNameStream.setFieldAlignment(QTextStream::AlignRight);
  dcNameStream.setPadChar('0');
  dcNameStream << row;
  dcNameStream.setFieldWidth(0);
  dcNameStream << "c";
  dcNameStream.setFieldWidth(paddingDigits);
  dcNameStream << col;
  return dcName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString MontageImportHelper::GenerateDataContainerName(const QString& dataContainerPrefix, const IntVec2Type& montageMaxValues, int32_t row, int32_t col)
{
  int32_t rowCountPadding = MetaXmlUtils::CalculatePaddingDigits(montageMaxValues[1]);
  int32_t colCountPadding = MetaXmlUtils::CalculatePaddingDigits(montageMaxValues[0]);
  int charPaddingCount = std::max(rowCountPadding, colCountPadding);

  QString dcName = dataContainerPrefix;
  QTextStream dcNameStream(&dcName);
  dcNameStream << "r";
  dcNameStream.setFieldWidth(charPaddingCount);
  dcNameStream.setFieldAlignment(QTextStream::AlignRight);
  dcNameStream.setPadChar('0');
  dcNameStream << row;
  dcNameStream.setFieldWidth(0);
  dcNameStream << "c";
  dcNameStream.setFieldWidth(charPaddingCount);
  dcNameStream << col;
  return dcName;
}
