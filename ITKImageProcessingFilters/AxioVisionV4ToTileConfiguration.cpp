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

#include "AxioVisionV4ToTileConfiguration.h"

#include <array>
#include <fstream>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtXml/QDomDocument>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/Utilities/FileSystemPathHelper.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/OutputFileFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagsXmlSection.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/MetaXmlUtils.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagMappingConstants.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagsXmlSection.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AxioVisionV4ToTileConfiguration::AxioVisionV4ToTileConfiguration()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AxioVisionV4ToTileConfiguration::~AxioVisionV4ToTileConfiguration() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToTileConfiguration::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToTileConfiguration::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Input File", InputFile, FilterParameter::Parameter, AxioVisionV4ToTileConfiguration, "*.xml"));
  parameters.push_back(SIMPL_NEW_OUTPUT_FILE_FP("Output File", OutputFile, FilterParameter::Parameter, AxioVisionV4ToTileConfiguration, ".txt"));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToTileConfiguration::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  QFileInfo fi(getInputFile());

  if(m_InputFile.isEmpty())
  {
    QString ss = QObject::tr("The input file must be set for property %1").arg("InputFile");
    setErrorCondition(-1, ss);
    return;
  }

  QString ext = fi.suffix();
  if(ext != "xml")
  {
    QString ss = QObject::tr("The file extension '%1' was not recognized. The reader only recognizes the .xml file extension").arg(ext);
    setErrorCondition(-997, ss);
    return;
  }

  if(!fi.exists())
  {
    QString ss = QObject::tr("The input file does not exist: '%1'").arg(getInputFile());
    setErrorCondition(-388, ss);
    return;
  }

  fi = QFileInfo(getOutputFile());
  if(fi.suffix().compare("") == 0)
  {
    setOutputFile(getOutputFile().append(".txt"));
  }
  FileSystemPathHelper::CheckOutputFile(this, "Output File Name", getOutputFile(), true);

  // Parse the XML file to get all the meta-data information and create all the
  // data structure that is needed.
  QFile xmlFile(getInputFile());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToTileConfiguration::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToTileConfiguration::execute()
{
  initialize();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  if (getCancel()) { return; }

  // Parse the XML file to get all the meta-data information and create all the
  // data structure that is needed.
  QFile xmlFile(getInputFile());
  readMetaXml(&xmlFile);
}

// -----------------------------------------------------------------------------
void AxioVisionV4ToTileConfiguration::readMetaXml(QIODevice* device)
{
  QString errorStr;
  int errorLine;
  int errorColumn;

  QDomDocument domDocument;
  QDomElement root;
  ZeissTagsXmlSection::Pointer rootTagsSection;

  // We are reading from the file, so set the FileWasRead flag to true
  if(!domDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn))
  {
    QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
    setErrorCondition(-70000, ss);
    return;
  }

  root = domDocument.documentElement();

  QDomElement tags = root.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
  if(tags.isNull())
  {
    QString ss = QObject::tr("Could not find the <ROOT><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file");
    setErrorCondition(-70001, ss);
    return;
  }

  // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
  rootTagsSection = MetaXmlUtils::ParseTagsSection(this, tags);
  if(nullptr == rootTagsSection.get())
  {
    return;
  }

  QDomElement rootCopy = root.cloneNode().toElement();

  // Now parse each of the <pXXX> tags
  parseImages(root, rootTagsSection.get());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4ToTileConfiguration::parseImages(QDomElement& root, ZeissTagsXmlSection* rootTagsSection)
{

  int32_t imageCount = MetaXmlUtils::GetInt32Entry(this, rootTagsSection, Zeiss::MetaXML::ImageCountRawId);
  if(getErrorCode() < 0)
  {
    return;
  }

  AbstractZeissMetaData::Pointer fileNamePtr = rootTagsSection->getEntry(Zeiss::MetaXML::FilenameId);
  StringZeissMetaEntry::Pointer imageNamePtr = std::dynamic_pointer_cast<StringZeissMetaEntry>(fileNamePtr);
  QString imageName = imageNamePtr->getValue();

  QFileInfo fi(imageName);

  // Figure out the max padding digits for both the imageCount (we need that to generate the proper xml tag) and
  // the row/col values because we need to have a consistent numbering format for later filters.
  int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(imageCount);
  //  int32_t rowCountPadding = MetaXmlUtils::CalculatePaddingDigits(rowCount);
  //  int32_t colCountPadding = MetaXmlUtils::CalculatePaddingDigits(colCount);
  //int charPaddingCount = std::max(rowCountPadding, colCountPadding);

  DataContainerArray::Pointer dca = getDataContainerArray();
  QTextStream outTextStream;
  QFile outFile(getOutputFile());
  if(!getInPreflight())
  {
    // Create our Output file Stream
    outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    outTextStream.setDevice(&outFile);
    outTextStream << "# File Written by DREAM.3D based on values from the _meta.xml" << "\n";
    outTextStream << "# Files are ordered Columns moving fastest, then rows." << "\n";
    outTextStream << "# Coordinate values are in pixel coords." << "\n";
    outTextStream << "# Define the number of dimensions we are working on." << "\n";
    outTextStream << "dim = 2" << "\n";
    outTextStream << "\n";
    outTextStream << "# Define the image coordinates" << "\n";
  }


  std::vector<std::array<int32_t, 3>> allDims;
  std::vector<std::array<float, 3>> allResolution;
  std::vector<std::array<float, 3>> allOrigins;
  std::vector<std::array<int32_t, 2>> allRCIndices;
  std::vector<QString> allNames;

  std::array<float, 3> minOrigin = {{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), 0.0f}};
  std::array<int32_t, 2> maxRC = {{ 0,0}};

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

    // Send a status update on the progress
    QString msg = QString("%1: Importing file %2 of %3").arg(getHumanLabel()).arg(p).arg(imageCount);
    notifyStatusMessage(msg);

    // Drill down into the XML document....
    QDomElement photoEle = root.firstChildElement(pTag);
    if(photoEle.isNull())
    {
      QString ss = QObject::tr("Could not find the <ROOT><%1> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-70002, ss);
      return;
    }
    // Get the TAGS section
    QDomElement tags = photoEle.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
    if(tags.isNull())
    {
      QString ss = QObject::tr("Could not find the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-70003, ss);
      return;
    }
    // Now Parse the TAGS section
    ZeissTagsXmlSection::Pointer photoTagsSection = MetaXmlUtils::ParseTagsSection(this, tags);
    if(nullptr == photoTagsSection.get())
    {
      QString ss = QObject::tr("Error Parsing the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss AxioVision _meta.xml file").arg(pTag);
      setErrorCondition(-70004, ss);
      return;
    }

    // Get the Row Index (Zero Based)
    int32_t rowIndex = MetaXmlUtils::GetInt32Entry(this, photoTagsSection.get(), Zeiss::MetaXML::ImageIndexVId);
    if(getErrorCode() < 0)
    {
      return;
    }
    // Get the Columnn Index (Zero Based)
    int32_t colIndex = MetaXmlUtils::GetInt32Entry(this, photoTagsSection.get(), Zeiss::MetaXML::ImageIndexUId);
    if(getErrorCode() < 0)
    {
      return;
    }

    std::array<int32_t, 2> rcIndex = {{ rowIndex, colIndex}};
    allRCIndices.push_back(rcIndex);

    if(rcIndex[0] > maxRC[0]) { maxRC[0] = rcIndex[0];}
    if(rcIndex[1] > maxRC[1]) { maxRC[1] = rcIndex[1];}


    //#######################################################################
    // Get Pixel Dimensions of the image.
    std::array<int32_t, 3> dims;
    AbstractZeissMetaData::Pointer ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageWidthPixelId);
    Int32ZeissMetaEntry::Pointer int32Entry = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
    Q_ASSERT_X(int32Entry.get() != nullptr, "Could not Cast to Int32ZeissMetaEntry", "");

    dims[0] = int32Entry->getValue();

    ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageHeightPixelId);
    int32Entry = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
    Q_ASSERT_X(int32Entry.get() != nullptr, "Could not Cast to Int32ZeissMetaEntry", "");

    dims[1] = int32Entry->getValue();
    dims[2] = 1;

    allDims.push_back(dims);

    //#######################################################################
    // Get the Origin to the Stage Positions
    std::array<float, 3> origin;
    origin[0] = MetaXmlUtils::GetFloatEntry(this, photoTagsSection.get(), Zeiss::MetaXML::StagePositionXId);
    origin[1] = MetaXmlUtils::GetFloatEntry(this, photoTagsSection.get(), Zeiss::MetaXML::StagePositionYId);
    origin[2] = 0.0f;

    allOrigins.push_back(origin);

    if(origin[0] < minOrigin[0]) { minOrigin[0] = origin[0];}
    if(origin[1] < minOrigin[1]) { minOrigin[1] = origin[1];}
    if(origin[2] < minOrigin[2]) { minOrigin[2] = origin[2];}

    //#######################################################################
    // Get the Spacing of the geometry
    bool ok = false;
    std::array<float, 3> scaling = {{1.0f, 1.0f, 1.0f}};
    QDomElement scalingDom = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling).firstChildElement("Factor_0");
    scaling[0] = scalingDom.text().toFloat(&ok);
    scalingDom = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling).firstChildElement("Factor_1");
    scaling[1] = scalingDom.text().toFloat(&ok);
    scaling[2] = 1.0;

    allResolution.push_back(scaling);

    QString name = fi.completeBaseName() + "_" + pTag + "." + fi.suffix();
    allNames.push_back(name);
  }

  /* The Montage config file must be written as Rows going fastest and not in a Snake style */
  for(int32_t row = 0; row <= maxRC[0]; row++)
  {
    for(int32_t col = 0; col <= maxRC[1]; col++)
    {
      for (size_t p = 0; p < imageCount; p++)
      {
        std::array<int32_t, 2>& rcIndex = allRCIndices.at(p);
        if(rcIndex[0] == row && rcIndex[1] == col)
        {
          std::array<float, 3> origin = allOrigins[p];
          std::array<float, 3> resolution = allResolution[p];

          origin[0] = (origin[0] - minOrigin[0])/resolution[0];
          origin[1] = (origin[1] - minOrigin[1])/resolution[1];
          origin[2] = (origin[2] - minOrigin[2])/resolution[2];
          if(!getInPreflight())
          {
            outTextStream << allNames[p] << "; ; (" << origin[0] << ", " << origin[1] << ")" << "\n";
          }
        }
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer AxioVisionV4ToTileConfiguration::newFilterInstance(bool copyFilterParameters) const
{
  AxioVisionV4ToTileConfiguration::Pointer filter = AxioVisionV4ToTileConfiguration::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToTileConfiguration::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToTileConfiguration::getBrandingString() const
{
  return ITKImageProcessingConstants::ITKImageProcessingPluginDisplayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToTileConfiguration::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToTileConfiguration::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToTileConfiguration::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::ConversionFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AxioVisionV4ToTileConfiguration::getHumanLabel() const
{ 
  return "ITK::Convert AxioVision To Tile Configuration file";
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid AxioVisionV4ToTileConfiguration::getUuid()
{
  return QUuid("{30687f44-9c10-5617-bcb8-4109cbd6e55e}");
}

