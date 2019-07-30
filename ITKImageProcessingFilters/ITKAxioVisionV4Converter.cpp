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

#include "ITKAxioVisionV4Converter.h"

#include <QtCore/QFileInfo>

#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/OutputFileFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/AxioVisionV4Converter.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKAxioVisionV4Converter::ITKAxioVisionV4Converter()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKAxioVisionV4Converter::~ITKAxioVisionV4Converter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAxioVisionV4Converter::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAxioVisionV4Converter::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("AxioVision XML File (_meta.xml)", InputFile, FilterParameter::Parameter, ITKAxioVisionV4Converter, "*.xml"));

  {
    LinkedChoicesFilterParameter::Pointer parameter = LinkedChoicesFilterParameter::New();
    parameter->setHumanLabel("Select the desired output file type");
    parameter->setPropertyName("OutputFileType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKAxioVisionV4Converter, this, OutputFileType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKAxioVisionV4Converter, this, OutputFileType));
    parameter->setDefaultValue(0); // Always start with the first selection

    QVector<QString> choices;
    choices.push_back("Json (*.json)");
    choices.push_back("Text (*.txt)");
    parameter->setChoices(choices);
    QStringList linkedProps;
    linkedProps << "JsonOutputFile"
                << "TextOutputFile";

    parameter->setLinkedProperties(linkedProps);
    parameter->setEditable(false);
    parameter->setCategory(FilterParameter::Parameter);
    parameters.push_back(parameter);
  }

  parameters.push_back(SIMPL_NEW_OUTPUT_FILE_FP("JSON File", JsonOutputFile, FilterParameter::Parameter, ITKAxioVisionV4Converter, "*.json", "", 0));
  parameters.push_back(SIMPL_NEW_OUTPUT_FILE_FP("Text File", TextOutputFile, FilterParameter::Parameter, ITKAxioVisionV4Converter, "*.txt", "", 1));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAxioVisionV4Converter::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  QString ss;
  QFileInfo inputFi(getInputFile());
  if(getInputFile().isEmpty())
  {
    ss = QObject::tr("%1 needs the input AxioVision XML File set and it was not.").arg(ClassName());
    setErrorCondition(-387, ss);
    return;
  }

  if(!inputFi.exists())
  {
    ss = QObject::tr("The input AxioVision XML File '%1' does not exist.").arg(getInputFile());
    setErrorCondition(-388, ss);
    return;
  }

  if(inputFi.isDir())
  {
    ss = QObject::tr("The input AxioVision XML File path '%1' is a directory. Please select an XML file.").arg(getInputFile());
    setErrorCondition(-389, ss);
    return;
  }

  OutputFileTypeEnum fileTypeEnum = static_cast<OutputFileTypeEnum>(getOutputFileType());
  if(fileTypeEnum == OutputFileTypeEnum::Json)
  {
    if(getJsonOutputFile().isEmpty())
    {
      ss = QObject::tr("%1 needs the output JSON file set and it was not.").arg(ClassName());
      setErrorCondition(-390, ss);
      return;
    }

    QFileInfo outputFi(getJsonOutputFile());
    if(outputFi.isDir())
    {
      ss = QObject::tr("The output JSON file path '%1' is a directory. Please select a JSON file.").arg(getJsonOutputFile());
      setErrorCondition(-391, ss);
      return;
    }
  }
  else
  {
    if(getTextOutputFile().isEmpty())
    {
      ss = QObject::tr("%1 needs the output text file set and it was not.").arg(ClassName());
      setErrorCondition(-390, ss);
      return;
    }

    QFileInfo outputFi(getTextOutputFile());
    if(outputFi.isDir())
    {
      ss = QObject::tr("The output text file path '%1' is a directory. Please select a text file.").arg(getTextOutputFile());
      setErrorCondition(-391, ss);
      return;
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAxioVisionV4Converter::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck();                       // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted();          // We are done preflighting this filter
  setInPreflight(false);             // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAxioVisionV4Converter::execute()
{
  initialize();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  OutputFileTypeEnum fileTypeEnum = static_cast<OutputFileTypeEnum>(getOutputFileType());
  if(fileTypeEnum == OutputFileTypeEnum::Json)
  {
    AxioVisionV4Converter::ConvertToJsonFile(getInputFile(), getJsonOutputFile(), this);
  }
  else
  {
    AxioVisionV4Converter::ConvertToTextFile(getInputFile(), getTextOutputFile(), this);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKAxioVisionV4Converter::newFilterInstance(bool copyFilterParameters) const
{
  ITKAxioVisionV4Converter::Pointer filter = ITKAxioVisionV4Converter::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKAxioVisionV4Converter::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKAxioVisionV4Converter::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKAxioVisionV4Converter::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKAxioVisionV4Converter::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKAxioVisionV4Converter::getSubGroupName() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKAxioVisionV4Converter::getHumanLabel() const
{
  return "ITK::Convert Zeiss AxioVision (V4)";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKAxioVisionV4Converter::getUuid()
{
  return QUuid("{8f37a0a3-aecd-5f87-b189-f2cc5229da51}");
}
