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

#include "AxioVisionV4ToJson.h"

#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>

#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/OutputFileFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagMapping.h"
#include "MetaXmlUtils.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AxioVisionV4ToJson::AxioVisionV4ToJson()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AxioVisionV4ToJson::~AxioVisionV4ToJson() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToJson::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToJson::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("AxioVision XML File (_meta.xml)", InputFile, FilterParameter::Parameter, AxioVisionV4ToJson, "*.xml"));
  parameters.push_back(SIMPL_NEW_OUTPUT_FILE_FP("JSON File", OutputFile, FilterParameter::Parameter, AxioVisionV4ToJson, "*.json"));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToJson::dataCheck()
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

  if(getOutputFile().isEmpty())
  {
    ss = QObject::tr("%1 needs the output JSON File set and it was not.").arg(ClassName());
    setErrorCondition(-390, ss);
    return;
  }

  QFileInfo outputFi(getOutputFile());
  if(outputFi.isDir())
  {
    ss = QObject::tr("The output JSON File path '%1' is a directory. Please select a JSON file.").arg(getInputFile());
    setErrorCondition(-391, ss);
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToJson::preflight()
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
void AxioVisionV4ToJson::execute()
{
  initialize();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  QJsonObject rootObj = createJsonObject();
  QJsonDocument doc(rootObj);
  QFile outputFile(getOutputFile());
  QFileInfo fi(getOutputFile());
  if(!outputFile.open(QFile::WriteOnly))
  {
    QString ss = QObject::tr("Could not open the output JSON File '%1' for writing.").arg(fi.fileName());
    setErrorCondition(-3005, ss);
    return;
  }

  outputFile.write(doc.toJson());
  outputFile.close();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QJsonObject AxioVisionV4ToJson::createJsonObject()
{
  QJsonObject rootObj;

  // Parse the XML file to get all the meta-data information and create all the
  // data structure that is needed.
  QFile xmlFile(getInputFile());
  QDomDocument domDocument;

  QString errorStr;
  int errorLine = -1;
  int errorColumn = -1;

  if(!domDocument.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn))
  {
    QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
    setErrorCondition(-71000, ss);
    return {};
  }
  QDomElement root = domDocument.documentElement();

  // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
  QDomElement tags = root.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
  if(tags.isNull())
  {
    QString ss = QObject::tr("Could not find the <ROOT><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file");
    setErrorCondition(-3000, ss);
    return {};
  }

  ZeissTagsXmlSection::Pointer rootTagsSection = MetaXmlUtils::ParseTagsSection(this, tags);
  if(nullptr == rootTagsSection.get())
  {
    return {};
  }

  ZeissTagMapping::Pointer tagMapping = ZeissTagMapping::instance();

  ZeissTagsXmlSection::MetaDataType tagMetaData = rootTagsSection->getMetaDataMap();
  QJsonObject tagObj;
  for(const auto& entry : tagMetaData)
  {
    int32_t tagId = entry->getZeissIdTag();
    QString name = tagMapping->nameForId(tagId);
    tagObj[name] = entry->toString();
  }
  rootObj["Tags"] = tagObj;

  QDomElement scaling = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling);
  ZeissTagsXmlSection::Pointer scalingTagsSection = MetaXmlUtils::ParseScalingSection(this, scaling);
  if(nullptr == scalingTagsSection.get())
  {
    QString ss = QObject::tr("The Scaling Tags Section inside the input AxioVision XML File was null.");
    setErrorCondition(-3001, ss);
    return {};
  }

  tagMetaData = scalingTagsSection->getMetaDataMap();
  QJsonObject scalingObj;
  for(const auto& entry : tagMetaData)
  {
    int32_t tagId = entry->getZeissIdTag();
    QString name = tagMapping->nameForId(tagId);
    scalingObj[name] = entry->toString();
  }
  rootObj["Scaling"] = scalingObj;

  int32_t imageCount = MetaXmlUtils::GetInt32Entry(this, rootTagsSection.get(), Zeiss::MetaXML::ImageCountRawId);

  int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(imageCount);

  // Loop over every image in the _meta.xml file
  for(int p = 0; p < imageCount; p++)
  {
    // Generate the xml tag that is for this image
    QString pTag;
    QTextStream out(&pTag);
    out << "p";
    out.setFieldWidth(imageCountPadding);
    out.setFieldAlignment(QTextStream::AlignRight);
    out.setPadChar('0');
    out << p;

    // Drill down into the XML document....
    QDomElement photoEle = root.firstChildElement(pTag);
    if(photoEle.isNull())
    {
      QString ss = QObject::tr("Could not find the <ROOT><%1> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-3002, ss);
      return {};
    }
    // Get the TAGS section
    QDomElement tags = photoEle.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
    if(tags.isNull())
    {
      QString ss = QObject::tr("Could not find the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-3003, ss);
      return {};
    }
    // Now Parse the TAGS section
    ZeissTagsXmlSection::Pointer photoTagsSection = MetaXmlUtils::ParseTagsSection(this, tags);
    if(nullptr == photoTagsSection.get())
    {
      QString ss = QObject::tr("Error Parsing the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss AxioVision _meta.xml file").arg(pTag);
      setErrorCondition(-3004, ss);
      return {};
    }
    tagMetaData = photoTagsSection->getMetaDataMap();
    QJsonObject pTagObj;
    for(const auto& entry : tagMetaData)
    {
      int32_t tagId = entry->getZeissIdTag();
      QString name = tagMapping->nameForId(tagId);
      pTagObj[name] = entry->toString();
    }
    rootObj[pTag] = pTagObj;
  }

  return rootObj;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer AxioVisionV4ToJson::newFilterInstance(bool copyFilterParameters) const
{
  AxioVisionV4ToJson::Pointer filter = AxioVisionV4ToJson::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToJson::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToJson::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToJson::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToJson::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToJson::getSubGroupName() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToJson::getHumanLabel() const
{
  return "ITK::Convert Zeiss AxioVision (V4) to Json";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid AxioVisionV4ToJson::getUuid()
{
  return QUuid("{8f37a0a3-aecd-5f87-b189-f2cc5229da51}");
}
