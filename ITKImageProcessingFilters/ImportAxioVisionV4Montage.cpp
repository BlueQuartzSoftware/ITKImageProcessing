/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "ImportAxioVisionV4Montage.h"

#include <array>
#include <cstring>
#include <limits>
#include <set>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtCore/QTextStream>

#include "SIMPLib/CoreFilters/ConvertColorToGrayScale.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/PreflightUpdatedValueFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/MontageImportHelper.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagMapping.h"
#include "MetaXmlUtils.h"

#define ZIF_PRINT_DBG_MSGS 0

namespace
{
const QString k_DCName("AxioVisionInfo");
const QString k_MetaDataName("AxioVision MetaData");
} // namespace

enum createdPathID : RenameDataPath::DataID_t
{
  // DataContainerID = 1
};

/* ############## Start Private Implementation ############################### */
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ImportAxioVisionV4MontagePrivate
{
  Q_DISABLE_COPY(ImportAxioVisionV4MontagePrivate)
  Q_DECLARE_PUBLIC(ImportAxioVisionV4Montage)
  ImportAxioVisionV4Montage* const q_ptr;
  ImportAxioVisionV4MontagePrivate(ImportAxioVisionV4Montage* ptr);

  ZeissTagsXmlSection::Pointer m_RootTagsSection;

  QDomElement m_Root;
  QString m_InputFile_Cache;
  DataArrayPath m_DataContainerPath;
  QString m_CellAttributeMatrixName;
  QString m_ImageDataArrayName;
  bool m_ChangeOrigin = false;
  bool m_ChangeSpacing = false;
  bool m_ConvertToGrayScale = false;
  FloatVec3Type m_Origin;
  FloatVec3Type m_Spacing;
  FloatVec3Type m_ColorWeights;
  QDateTime m_TimeStamp_Cache;
  QString m_MontageInformation;
  bool m_ImportAllMetaData = false;
  QString m_MetaDataAttributeMatrixName;
  int32_t m_MaxRow = 0;
  int32_t m_MaxCol = 0;
  std::vector<ImportAxioVisionV4Montage::BoundsType> m_BoundsCache;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportAxioVisionV4MontagePrivate::ImportAxioVisionV4MontagePrivate(ImportAxioVisionV4Montage* ptr)
: q_ptr(ptr)
, m_Root(QDomElement())
, m_InputFile_Cache("")
, m_TimeStamp_Cache(QDateTime())
{
}

/* ############## Start Public Implementation ############################### */

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportAxioVisionV4Montage::ImportAxioVisionV4Montage()
: m_InputFile("")
, m_DataContainerPath(ITKImageProcessing::Montage::k_DataContaineNameDefaultName)
, m_CellAttributeMatrixName(ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName)
, m_ImageDataArrayName(ITKImageProcessing::Montage::k_TileDataArrayDefaultName)
, m_MetaDataAttributeMatrixName(::k_MetaDataName)
, m_ConvertToGrayScale(false)
, m_ImportAllMetaData(false)
, m_ChangeOrigin(false)
, m_ChangeSpacing(false)
, d_ptr(new ImportAxioVisionV4MontagePrivate(this))
{
  m_ColorWeights = FloatVec3Type(0.2125f, 0.7154f, 0.0721f);
  m_Origin = FloatVec3Type(0.0f, 0.0f, 0.0f);
  m_Spacing = FloatVec3Type(1.0f, 1.0f, 1.0f);
  m_MontageStart = IntVec2Type(0, 0);
  m_MontageEnd = IntVec2Type(0, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportAxioVisionV4Montage::~ImportAxioVisionV4Montage() = default;

SIMPL_PIMPL_PROPERTY_DEF(ImportAxioVisionV4Montage, QDomElement, Root)
SIMPL_PIMPL_PROPERTY_DEF(ImportAxioVisionV4Montage, QString, InputFile_Cache)
SIMPL_PIMPL_PROPERTY_DEF(ImportAxioVisionV4Montage, QDateTime, TimeStamp_Cache)
SIMPL_PIMPL_PROPERTY_DEF(ImportAxioVisionV4Montage, std::vector<ImportAxioVisionV4Montage::BoundsType>, BoundsCache)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("AxioVision XML File (_meta.xml)", InputFile, FilterParameter::Parameter, ImportAxioVisionV4Montage, "*.xml"));

  PreflightUpdatedValueFilterParameter::Pointer param = SIMPL_NEW_PREFLIGHTUPDATEDVALUE_FP("Montage Information", MontageInformation, FilterParameter::Parameter, ImportAxioVisionV4Montage);
  param->setReadOnly(true);
  parameters.push_back(param);

  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Start (Col, Row) [Inclusive, Zero Based]", MontageStart, FilterParameter::Parameter, ImportAxioVisionV4Montage));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage End (Col, Row) [Inclusive, Zero Based]", MontageEnd, FilterParameter::Parameter, ImportAxioVisionV4Montage));

  parameters.push_back(SIMPL_NEW_BOOL_FP("Import All MetaData", ImportAllMetaData, FilterParameter::Parameter, ImportAxioVisionV4Montage));

  QStringList linkedProps("Origin");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Origin", ChangeOrigin, FilterParameter::Parameter, ImportAxioVisionV4Montage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ImportAxioVisionV4Montage));

  linkedProps.clear();
  linkedProps << "Spacing";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Spacing", ChangeSpacing, FilterParameter::Parameter, ImportAxioVisionV4Montage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ImportAxioVisionV4Montage));

  linkedProps.clear();
  linkedProps << "ColorWeights";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Convert To GrayScale", ConvertToGrayScale, FilterParameter::Parameter, ImportAxioVisionV4Montage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Color Weighting", ColorWeights, FilterParameter::Parameter, ImportAxioVisionV4Montage));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("DataContainer Prefix", DataContainerPath, FilterParameter::CreatedArray, ImportAxioVisionV4Montage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix Name", CellAttributeMatrixName, FilterParameter::CreatedArray, ImportAxioVisionV4Montage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image DataArray Name", ImageDataArrayName, FilterParameter::CreatedArray, ImportAxioVisionV4Montage));
  parameters.push_back(SIMPL_NEW_STRING_FP("MetaData AttributeMatrix Name", MetaDataAttributeMatrixName, FilterParameter::CreatedArray, ImportAxioVisionV4Montage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  QString ss;
  QFileInfo fi(getInputFile());
  if(getInputFile().isEmpty())
  {
    ss = QObject::tr("%1 needs the Input File Set and it was not.").arg(ClassName());
    setErrorCondition(-387, ss);
  }
  else if(!fi.exists())
  {
    ss = QObject::tr("The input file '%1' does not exist.").arg(getInputFile());
    setErrorCondition(-388, ss);
  }

  if(fi.isDir())
  {
    ss = QObject::tr("The input path '%1' is a directory. Please select an XML file.").arg(getInputFile());
    setErrorCondition(-395, ss);
  }

  if(getDataContainerPath().isEmpty())
  {
    ss = QObject::tr("The Data Container Name cannot be empty.");
    setErrorCondition(-392, ss);
  }
  if(getCellAttributeMatrixName().isEmpty())
  {
    ss = QObject::tr("The Attribute Matrix Name cannot be empty.");
    setErrorCondition(-393, ss);
  }
  if(getImageDataArrayName().isEmpty())
  {
    ss = QObject::tr("The Image Data Array Name cannot be empty.");
    setErrorCondition(-394, ss);
  }

  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainerArray::Pointer dca = getDataContainerArray();
  if(nullptr == dca.get())
  {
    ss = QObject::tr("%1 needs a valid DataContainerArray").arg(ClassName());
    setErrorCondition(-390, ss);
    return;
  }

  // Parse the XML file to get all the meta-data information and create all the
  // data structure that is needed.
  QFile xmlFile(getInputFile());

  QString errorStr;
  int errorLine = -1;
  int errorColumn = -1;

  QDateTime timeStamp(fi.lastModified());

  QDomDocument domDocument;
  QDomElement root;

  // clang-format off
  if(m_InputFile ==  d_ptr->m_InputFile_Cache
    && m_DataContainerPath == d_ptr->m_DataContainerPath
    && m_CellAttributeMatrixName == d_ptr->m_CellAttributeMatrixName
    && m_ImageDataArrayName == d_ptr->m_ImageDataArrayName
    && m_ChangeOrigin == d_ptr->m_ChangeOrigin
    && m_ChangeSpacing == d_ptr->m_ChangeSpacing 
    && m_ConvertToGrayScale == d_ptr->m_ConvertToGrayScale
    && m_Origin == d_ptr->m_Origin
    && m_Spacing == d_ptr->m_Spacing
    && m_ColorWeights == d_ptr->m_ColorWeights
    && d_ptr->m_TimeStamp_Cache.isValid()
    && timeStamp == d_ptr->m_TimeStamp_Cache
    && d_ptr->m_ImportAllMetaData == m_ImportAllMetaData
    && d_ptr->m_MetaDataAttributeMatrixName == m_MetaDataAttributeMatrixName
  )
  // clang-format on
  {
    // We are reading from the cache, so set the FileWasRead flag to false
    m_FileWasRead = false;
    //    root = getRoot();
  }
  else
  {
    flushCache();
    // We are reading from the file, so set the FileWasRead flag to true
    m_FileWasRead = true;

    if(!domDocument.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn))
    {
      QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
      setErrorCondition(-71000, ss);
      return;
    }

    root = domDocument.documentElement();
    setRoot(root);
    QDomElement tags = root.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
    if(tags.isNull())
    {
      QString ss = QObject::tr("Could not find the <ROOT><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file");
      setErrorCondition(-71001, ss);
      return;
    }

    d_ptr->m_Root = root;
    d_ptr->m_InputFile_Cache = m_InputFile;
    d_ptr->m_DataContainerPath = m_DataContainerPath;
    d_ptr->m_CellAttributeMatrixName = m_CellAttributeMatrixName;
    d_ptr->m_ImageDataArrayName = m_ImageDataArrayName;
    d_ptr->m_ChangeOrigin = m_ChangeOrigin;
    d_ptr->m_ChangeSpacing = m_ChangeSpacing;
    d_ptr->m_ConvertToGrayScale = m_ConvertToGrayScale;
    d_ptr->m_Origin = m_Origin;
    d_ptr->m_Spacing = m_Spacing;
    d_ptr->m_ColorWeights = m_ColorWeights;
    d_ptr->m_TimeStamp_Cache = timeStamp;
    d_ptr->m_ImportAllMetaData = m_ImportAllMetaData;
    d_ptr->m_MetaDataAttributeMatrixName = m_MetaDataAttributeMatrixName;

    setTimeStamp_Cache(timeStamp);

    generateCache(root);
  }

  if(m_MontageStart[0] > m_MontageEnd[0])
  {
    QString ss = QObject::tr("Montage Start Column (%1) must be equal or less than Montage End Column(%2)").arg(m_MontageStart[0]).arg(m_MontageEnd[0]);
    setErrorCondition(-396, ss);
    return;
  }
  if(m_MontageStart[1] > m_MontageEnd[1])
  {
    QString ss = QObject::tr("Montage Start Row (%1) must be equal or less than Montage End Row(%2)").arg(m_MontageStart[1]).arg(m_MontageEnd[1]);
    setErrorCondition(-397, ss);
    return;
  }
  if(m_MontageStart[0] < 0 || m_MontageEnd[0] < 0)
  {
    QString ss = QObject::tr("Montage Start Column (%1) and Montage End Column(%2) must be greater than Zero (0)").arg(m_MontageStart[0]).arg(m_MontageEnd[0]);
    setErrorCondition(-398, ss);
    return;
  }
  if(m_MontageStart[1] < 0 || m_MontageEnd[1] < 0)
  {
    QString ss = QObject::tr("Montage Start Row (%1) and Montage End Row(%2) must be greater than Zero (0)").arg(m_MontageStart[1]).arg(m_MontageEnd[1]);
    setErrorCondition(-399, ss);
    return;
  }
  if(m_MontageStart[0] > d_ptr->m_MaxCol || m_MontageEnd[0] > d_ptr->m_MaxCol)
  {
    QString ss = QObject::tr("Montage Start Column (%1) and Montage End Column(%2) must be <= %3").arg(m_MontageStart[0]).arg(m_MontageEnd[0]).arg(d_ptr->m_MaxCol);
    setErrorCondition(-400, ss);
    return;
  }
  if(m_MontageStart[1] > d_ptr->m_MaxRow || m_MontageEnd[1] > d_ptr->m_MaxRow)
  {
    QString ss = QObject::tr("Montage Start Row (%1) and Montage End Row(%2) must be <=").arg(m_MontageStart[1]).arg(m_MontageEnd[1]).arg(d_ptr->m_MaxRow);
    setErrorCondition(-401, ss);
    return;
  }

  generateDataStructure();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::preflight()
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
void ImportAxioVisionV4Montage::execute()
{
  int err = 0;
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCode() < 0)
  {
    return;
  }
  clearErrorCode();
  clearWarningCode();

  readImages();

  /* If some error occurs this code snippet can report the error up the call chain*/
  if(err < 0)
  {
    QString ss = QObject::tr("Error Importing a Zeiss AxioVision file set.");
    setErrorCondition(-90000, ss);
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::flushCache()
{
  m_GeneratedFileList.clear();
  setTimeStamp_Cache(QDateTime());
  setRoot(QDomElement());

  d_ptr->m_InputFile_Cache = "";
  d_ptr->m_DataContainerPath = DataArrayPath();
  d_ptr->m_CellAttributeMatrixName = "";
  d_ptr->m_ImageDataArrayName = "";
  d_ptr->m_ChangeOrigin = false;
  d_ptr->m_ChangeSpacing = false;
  d_ptr->m_ConvertToGrayScale = false;
  d_ptr->m_Origin = FloatVec3Type(0.0f, 0.0f, 0.0f);
  d_ptr->m_Spacing = FloatVec3Type(1.0f, 1.0f, 1.0f);
  d_ptr->m_ColorWeights = FloatVec3Type(0.2125f, 0.7154f, 0.0721f);
  d_ptr->m_BoundsCache.clear();
  d_ptr->m_ImportAllMetaData = false;
  d_ptr->m_MetaDataAttributeMatrixName = "";
  d_ptr->m_MaxCol = 0;
  d_ptr->m_MaxRow = 0;
}

// -----------------------------------------------------------------------------
QString ImportAxioVisionV4Montage::getMontageInformation()
{
  return d_ptr->m_MontageInformation;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::generateCache(QDomElement& root)
{
  // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
  QDomElement tags = root.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
  ZeissTagsXmlSection::Pointer rootTagsSection = MetaXmlUtils::ParseTagsSection(this, tags);
  if(nullptr == rootTagsSection.get())
  {
    return;
  }

  int32_t imageCount = MetaXmlUtils::GetInt32Entry(this, rootTagsSection.get(), Zeiss::MetaXML::ImageCountRawId);
  if(getErrorCode() < 0)
  {
    return;
  }

  m_RowCount = MetaXmlUtils::GetInt32Entry(this, rootTagsSection.get(), Zeiss::MetaXML::ImageCountVId);
  if(getErrorCode() < 0)
  {
    return;
  }

  m_ColumnCount = MetaXmlUtils::GetInt32Entry(this, rootTagsSection.get(), Zeiss::MetaXML::ImageCountUId);
  if(getErrorCode() < 0)
  {
    return;
  }

  AbstractZeissMetaData::Pointer fileNamePtr = rootTagsSection->getEntry(Zeiss::MetaXML::FilenameId);
  StringZeissMetaEntry::Pointer imageNamePtr = std::dynamic_pointer_cast<StringZeissMetaEntry>(fileNamePtr);
  QString imageName = imageNamePtr->getValue();

  // Figure out the max padding digits for both the imageCount (we need that to generate the proper xml tag) and
  // the row/col values because we need to have a consistent numbering format for later filters.
  int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(imageCount);
  // int32_t rowCountPadding = MetaXmlUtils::CalculatePaddingDigits(m_RowCount);
  // int32_t colCountPadding = MetaXmlUtils::CalculatePaddingDigits(m_ColumnCount);
  // int charPaddingCount = std::max(rowCountPadding, colCountPadding);
  FloatVec3Type minCoord = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
  FloatVec3Type minSpacing = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

  std::vector<ImageGeom::Pointer> geometries(imageCount);
  std::vector<BoundsType> bounds(imageCount);
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
    QString msg = QString("%1: Caching meta data for file %2 of %3").arg(getHumanLabel()).arg(p).arg(imageCount);
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
    // Create the Image Geometry
    ImageGeom::Pointer image = initializeImageGeom(root, photoTagsSection);
    geometries[p] = image;

    minSpacing = image->getSpacing();

    FloatVec3Type origin = image->getOrigin();
    minCoord[0] = std::min(origin[0], minCoord[0]);
    minCoord[1] = std::min(origin[1], minCoord[1]);
    minCoord[2] = 0.0f;

    QFileInfo fi(imageName);
    QString imagePath = fi.completeBaseName() + "_" + pTag + "." + fi.suffix();
    fi = QFileInfo(getInputFile());
    imagePath = fi.absoluteDir().path() + "/" + imagePath;
    fi = QFileInfo(imagePath);
    if(!fi.exists())
    {
      setErrorCondition(-224, QString("Montage Tile File does not exist.'%1'").arg(imagePath));
    }
    m_GeneratedFileList.push_back(imagePath);

    BoundsType bound;
    bound.Filename = imagePath;
    bound.Dims = image->getDimensions();
    bound.Origin = image->getOrigin();
    bound.Spacing = image->getSpacing();
    bound.Col = colIndex;
    bound.Row = rowIndex;

    if(getImportAllMetaData())
    {
      std::vector<size_t> dims = {1};
      AttributeMatrix::Pointer metaAm = AttributeMatrix::New(dims, getMetaDataAttributeMatrixName(), AttributeMatrix::Type::Generic);
      ZeissTagsXmlSection::MetaDataType tagMap = photoTagsSection->getMetaDataMap();
      for(const auto& value : tagMap)
      {
        IDataArray::Pointer dataArray = value->createDataArray(!getInPreflight());
        metaAm->insertOrAssign(dataArray);
      }
      bound.MetaData = metaAm;
    }
    d_ptr->m_MaxCol = std::max(bound.Col, d_ptr->m_MaxCol);
    d_ptr->m_MaxRow = std::max(bound.Row, d_ptr->m_MaxRow);
    bounds[p] = bound;
  }

  // Get the meta information from disk for each image
  for(auto& bound : bounds)
  {
    DataArrayPath dap(::k_DCName, ITKImageProcessing::Montage::k_AMName, ITKImageProcessing::Montage::k_AAName);
    AbstractFilter::Pointer imageImportFilter = MontageImportHelper::CreateImageImportFilter(this, bound.Filename, dap);
    imageImportFilter->preflight();
    if(imageImportFilter->getErrorCode() < 0)
    {
      setErrorCondition(imageImportFilter->getErrorCode(), "Error Preflighting Image Import Filter.");
      continue;
    }

    DataContainerArray::Pointer importImageDca = imageImportFilter->getDataContainerArray();
    {
      DataContainer::Pointer fromDca = importImageDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer fromCellAttrMat = fromDca->getAttributeMatrix(ITKImageProcessing::Montage::k_AMName);
      IDataArray::Pointer fromImageData = fromCellAttrMat->getAttributeArray(ITKImageProcessing::Montage::k_AAName);
      fromImageData->setName(getImageDataArrayName());
      bound.ImageDataProxy = fromImageData;
    }

    if(getConvertToGrayScale())
    {
      DataArrayPath daPath(::k_DCName, ITKImageProcessing::Montage::k_AMName, ITKImageProcessing::Montage::k_AAName);
      AbstractFilter::Pointer grayScaleFilter = MontageImportHelper::CreateColorToGrayScaleFilter(this, daPath, getColorWeights(), ITKImageProcessing::Montage::k_GrayScaleTempArrayName);
      grayScaleFilter->setDataContainerArray(importImageDca);
      grayScaleFilter->preflight();
      if(grayScaleFilter->getErrorCode() < 0)
      {
        setErrorCondition(grayScaleFilter->getErrorCode(), "Error Preflighting Color to GrayScale filter");
        continue;
      }

      DataContainerArray::Pointer colorToGrayDca = grayScaleFilter->getDataContainerArray();
      DataContainer::Pointer fromDca = colorToGrayDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer fromCellAttrMat = fromDca->getAttributeMatrix(ITKImageProcessing::Montage::k_AMName);
      // Remove the RGB Attribute Array so we can rename the gray scale AttributeArray
      IDataArray::Pointer rgbImageArray = fromCellAttrMat->removeAttributeArray(ITKImageProcessing::Montage::k_AAName);
      QString grayScaleArrayName = ITKImageProcessing::Montage::k_GrayScaleTempArrayName + ITKImageProcessing::Montage::k_AAName;
      IDataArray::Pointer fromGrayScaleData = fromCellAttrMat->removeAttributeArray(grayScaleArrayName);
      fromGrayScaleData->setName(getImageDataArrayName());
      bound.ImageDataProxy = fromGrayScaleData;
    }
  }

  QString montageInfo;
  QTextStream ss(&montageInfo);
  ss << "Max Column: " << m_ColumnCount - 1 << "  Max Row: " << m_RowCount - 1 << "  Image Count: " << imageCount;

  int32_t importedCols = m_MontageEnd[0] - m_MontageStart[0] + 1;
  int32_t importedRows = m_MontageEnd[1] - m_MontageStart[1] + 1;
  ss << "\nImported Columns: " << importedCols << "  Imported Rows: " << importedRows << "  Imported Image Count: " << (importedCols * importedRows);

  FloatVec3Type overrideOrigin = minCoord;
  FloatVec3Type overrideSpacing = minSpacing;

  // Now adjust the origin/spacing if needed
  if(getChangeOrigin() || getChangeSpacing())
  {
    if(getChangeOrigin())
    {
      overrideOrigin = m_Origin;
    }
    if(getChangeSpacing())
    {
      overrideSpacing = m_Spacing;
    }
    for(auto& bound : bounds)
    {
      //  FloatVec3Type currentOrigin = bound.Origin;
      // FloatVec3Type currentSpacing = bound.Spacing;

      for(size_t i = 0; i < 3; i++)
      {
        float delta = bound.Origin[i] - minCoord[i];
        // Convert to Pixel Coords
        delta = delta / bound.Spacing[i];
        // Convert to the override origin
        delta = delta * overrideSpacing[i];
        bound.Origin[i] = overrideOrigin[i] + delta;
      }
      //  bound.Origin = currentOrigin;
      bound.Spacing = overrideSpacing;
    }
  }
  ss << "\nOrigin: " << overrideOrigin[0] << ", " << overrideOrigin[1] << ", " << overrideOrigin[2];
  ss << "\nSpacing: " << overrideSpacing[0] << ", " << overrideSpacing[1] << ", " << overrideSpacing[2];

  d_ptr->m_MontageInformation = montageInfo;
  setBoundsCache(bounds);
}

// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::generateDataStructure()
{
  std::vector<BoundsType>& bounds = d_ptr->m_BoundsCache;

  DataContainerArray::Pointer dca = getDataContainerArray();

  // int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(bounds.size());
  int32_t rowCountPadding = MetaXmlUtils::CalculatePaddingDigits(m_RowCount);
  int32_t colCountPadding = MetaXmlUtils::CalculatePaddingDigits(m_ColumnCount);
  int charPaddingCount = std::max(rowCountPadding, colCountPadding);

  for(const auto& bound : bounds)
  {
    if(bound.Row < m_MontageStart[1] || bound.Row > m_MontageEnd[1] || bound.Col < m_MontageStart[0] || bound.Col > m_MontageEnd[0])
    {
      continue;
    }

    // Create our DataContainer Name using a Prefix and a rXXcYY format.
    QString dcName = getDataContainerPath().getDataContainerName();
    QTextStream dcNameStream(&dcName);
    dcNameStream << "_r";
    dcNameStream.setFieldWidth(charPaddingCount);
    dcNameStream.setFieldAlignment(QTextStream::AlignRight);
    dcNameStream.setPadChar('0');
    dcNameStream << bound.Row;
    dcNameStream.setFieldWidth(0);
    dcNameStream << "c";
    dcNameStream.setFieldWidth(charPaddingCount);
    dcNameStream << bound.Col;

    // Create the DataContainer with a name based on the ROW & COLUMN indices
    DataContainer::Pointer dc = dca->createNonPrereqDataContainer<AbstractFilter>(this, dcName);

    // Create the Image Geometry
    ImageGeom::Pointer image = ImageGeom::CreateGeometry(dcName);
    image->setDimensions(bound.Dims);
    image->setOrigin(bound.Origin);
    image->setSpacing(bound.Spacing);

    dc->setGeometry(image);

    using StdVecSizeType = std::vector<size_t>;
    // Create the Cell Attribute Matrix into which the image data would be read
    AttributeMatrix::Pointer cellAttrMat = AttributeMatrix::New(bound.Dims.toContainer<StdVecSizeType>(), getCellAttributeMatrixName(), AttributeMatrix::Type::Cell);
    dc->addOrReplaceAttributeMatrix(cellAttrMat);
    cellAttrMat->addOrReplaceAttributeArray(bound.ImageDataProxy);
    if(getImportAllMetaData())
    {
      dc->addOrReplaceAttributeMatrix(bound.MetaData);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::addMetaData(const AttributeMatrix::Pointer& metaAm, const ZeissTagsXmlSection::Pointer& photoTagsSection, int index)
{
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();

  ZeissTagsXmlSection::MetaDataType tags = photoTagsSection->getMetaDataMap();

  QMapIterator<int, AbstractZeissMetaData::Pointer> iter(tags);
  while(iter.hasNext())
  {
    iter.next();
    StringZeissMetaEntry::Pointer zStrVal = std::dynamic_pointer_cast<StringZeissMetaEntry>(iter.value());

    QString tagName = tagMap->nameForId(iter.key());

    IDataArray::Pointer iDataArray = metaAm->getAttributeArray(tagName);
    StringDataArray::Pointer strArray = std::dynamic_pointer_cast<StringDataArray>(iDataArray);
    strArray->setValue(index, zStrVal->getValue());
    // IDataArray::Pointer dataArray = iter.value()->createDataArray(!getInPreflight());
    // metaData->insertOrAssign(dataArray);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::addRootMetaData(const AttributeMatrix::Pointer& metaAm, const ZeissTagsXmlSection::Pointer& rootTagsSection, int index)
{
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();
  ZeissTagsXmlSection::MetaDataType tags = rootTagsSection->getMetaDataMap();

  QMapIterator<int, AbstractZeissMetaData::Pointer> iter(tags);
  while(iter.hasNext())
  {
    iter.next();
    StringZeissMetaEntry::Pointer zStrVal = std::dynamic_pointer_cast<StringZeissMetaEntry>(iter.value());

    QString tagName = tagMap->nameForId(iter.key());
    if(tagName == "ScaleFactorForX" || tagName == "ScaleFactorForY")
    {
      IDataArray::Pointer iDataArray = metaAm->getAttributeArray(tagName);
      StringDataArray::Pointer strArray = std::dynamic_pointer_cast<StringDataArray>(iDataArray);
      strArray->setValue(index, zStrVal->getValue());
      // IDataArray::Pointer dataArray = iter.value()->createDataArray(!getInPreflight());
      // metaData->insertOrAssign(dataArray);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportAxioVisionV4Montage::readImages()
{
  std::vector<BoundsType>& bounds = d_ptr->m_BoundsCache;
  // Import Each Image
  DataContainerArray::Pointer dca = getDataContainerArray();

  //  int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(bounds.size());
  int32_t rowCountPadding = MetaXmlUtils::CalculatePaddingDigits(m_RowCount);
  int32_t colCountPadding = MetaXmlUtils::CalculatePaddingDigits(m_ColumnCount);
  int charPaddingCount = std::max(rowCountPadding, colCountPadding);

  for(const auto& bound : bounds)
  {
    if(bound.Row < m_MontageStart[1] || bound.Row > m_MontageEnd[1] || bound.Col < m_MontageStart[0] || bound.Col > m_MontageEnd[0])
    {
      continue;
    }

    QString msg;
    QTextStream out(&msg);
    out << "Importing " << bound.Filename;
    notifyStatusMessage(msg);
    // Create our DataContainer Name using a Prefix and a rXXcYY format.
    QString dcName = getDataContainerPath().getDataContainerName();
    QTextStream dcNameStream(&dcName);
    dcNameStream << "_r";
    dcNameStream.setFieldWidth(charPaddingCount);
    dcNameStream.setFieldAlignment(QTextStream::AlignRight);
    dcNameStream.setPadChar('0');
    dcNameStream << bound.Row;
    dcNameStream.setFieldWidth(0);
    dcNameStream << "c";
    dcNameStream.setFieldWidth(charPaddingCount);
    dcNameStream << bound.Col;

    // The DataContainer with a name based on the ROW & COLUMN indices is already created in the preflight
    DataContainer::Pointer dc = dca->getDataContainer(dcName);
    // So is the Geometry
    ImageGeom::Pointer image = dc->getGeometryAs<ImageGeom>();

    // Create the Image Geometry
    SizeVec3Type dims = image->getDimensions();
    // FloatVec3Type origin = image->getOrigin();
    // FloatVec3Type spacing = image->getSpacing();

    std::vector<size_t> tDims = {dims[0], dims[1], dims[2]};
    // The Cell AttributeMatrix is also already created at this point
    AttributeMatrix::Pointer cellAttrMat = dc->getAttributeMatrix(getCellAttributeMatrixName());
    // Instantiate the Image Import Filter to actually read the image into a data array
    DataArrayPath dap(::k_DCName, ITKImageProcessing::Montage::k_AMName, getImageDataArrayName()); // This is just a temp path for the subfilter to use
    AbstractFilter::Pointer imageImportFilter = MontageImportHelper::CreateImageImportFilter(this, bound.Filename, dap);
    if(nullptr == imageImportFilter.get())
    {
      continue;
    }
    // This same filter was used to preflight so as long as nothing changes on disk this really should work....
    imageImportFilter->execute();
    if(imageImportFilter->getErrorCode() < 0)
    {
      setErrorCondition(imageImportFilter->getErrorCode(), "Error Executing Image Import Filter.");
      continue;
    }
    // Now transfer the image data from the actual image data read from disk into our existing Attribute Matrix
    DataContainerArray::Pointer importImageDca = imageImportFilter->getDataContainerArray();
    DataContainer::Pointer fromDc = importImageDca->getDataContainer(::k_DCName);
    AttributeMatrix::Pointer fromCellAttrMat = fromDc->getAttributeMatrix(ITKImageProcessing::Montage::k_AMName);
    // IDataArray::Pointer fromImageData = fromCellAttrMat->getAttributeArray(getImageDataArrayName());

    if(getConvertToGrayScale())
    {
      AbstractFilter::Pointer grayScaleFilter = MontageImportHelper::CreateColorToGrayScaleFilter(this, dap, getColorWeights(), ITKImageProcessing::Montage::k_GrayScaleTempArrayName);
      grayScaleFilter->setDataContainerArray(importImageDca); // Use the Data COntainer array that was use for the import. It is setup and ready to go
      connect(grayScaleFilter.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), this, SIGNAL(messageGenerated(const AbstractMessage::Pointer&)));
      grayScaleFilter->execute();
      if(grayScaleFilter->getErrorCode() < 0)
      {
        setErrorCondition(grayScaleFilter->getErrorCode(), "Error Executing Color to GrayScale filter");
        continue;
      }

      DataContainerArray::Pointer c2gDca = grayScaleFilter->getDataContainerArray();
      DataContainer::Pointer c2gDc = c2gDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer c2gAttrMat = c2gDc->getAttributeMatrix(ITKImageProcessing::Montage::k_AMName);

      QString grayScaleArrayName = ITKImageProcessing::Montage::k_GrayScaleTempArrayName + getImageDataArrayName();
      // IDataArray::Pointer fromGrayScaleData = fromCellAttrMat->getAttributeArray(grayScaleArrayName);

      IDataArray::Pointer rgbImageArray = c2gAttrMat->removeAttributeArray(ITKImageProcessing::Montage::k_AAName);
      IDataArray::Pointer gray = c2gAttrMat->removeAttributeArray(grayScaleArrayName);
      gray->setName(getImageDataArrayName());
      cellAttrMat->addOrReplaceAttributeArray(gray);
    }
    else
    {
      // Copy the IDataArray (which contains the image data) from the temp data container array into our persistent data structure
      IDataArray::Pointer gray = fromCellAttrMat->removeAttributeArray(getImageDataArrayName());
      cellAttrMat->addOrReplaceAttributeArray(gray);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImageGeom::Pointer ImportAxioVisionV4Montage::initializeImageGeom(const QDomElement& root, const ZeissTagsXmlSection::Pointer& photoTagsSection)
{

  ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);

  //#######################################################################
  // Parse out the Pixel Dimensions of the image.
  SizeVec3Type dims;
  AbstractZeissMetaData::Pointer ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageWidthPixelId);
  Int32ZeissMetaEntry::Pointer int32Entry = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
  Q_ASSERT_X(int32Entry.get() != nullptr, "Could not Cast to Int32ZeissMetaEntry", "");

  dims[0] = int32Entry->getValue();

  ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageHeightPixelId);
  int32Entry = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
  Q_ASSERT_X(int32Entry.get() != nullptr, "Could not Cast to Int32ZeissMetaEntry", "");

  dims[1] = int32Entry->getValue();
  dims[2] = 1;
  image->setDimensions(dims);

  //#######################################################################
  // Initialize the Origin to the Stage Positions
  float stageXPos = MetaXmlUtils::GetFloatEntry(this, photoTagsSection.get(), Zeiss::MetaXML::StagePositionXId);
  float stageYPos = MetaXmlUtils::GetFloatEntry(this, photoTagsSection.get(), Zeiss::MetaXML::StagePositionYId);
  FloatVec3Type origin = {stageXPos, stageYPos, 0.0f};
  image->setOrigin(origin);

  //#######################################################################
  // Initialize the Spacing of the geometry
  bool ok = false;
  FloatVec3Type scaling = {1.0f, 1.0f, 1.0f};
  QDomElement scalingDom = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling).firstChildElement("Factor_0");
  scaling[0] = scalingDom.text().toFloat(&ok);
  scalingDom = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling).firstChildElement("Factor_1");
  scaling[1] = scalingDom.text().toFloat(&ok);
  image->setSpacing(scaling);

  //#######################################################################
  // Initialize the Length Units of the geometry
  QDomElement unitsDom = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling).firstChildElement("Type_0");
  int xUnits = unitsDom.text().toInt(&ok);
  // We are going to assume that the units in both the X and Y are the same. Why would they be different?
  IGeometry::LengthUnit lengthUnit = ZeissUnitMapping::Instance()->convertToIGeometryLengthUnit(xUnits);
  image->setUnits(lengthUnit);
  image->setName("AxioVision V4 Geometry");

  return image;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ImportAxioVisionV4Montage::newFilterInstance(bool copyFilterParameters) const
{
  ImportAxioVisionV4Montage::Pointer filter = ImportAxioVisionV4Montage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportAxioVisionV4Montage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportAxioVisionV4Montage::getBrandingString() const
{
  return ITKImageProcessingConstants::ITKImageProcessingPluginDisplayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportAxioVisionV4Montage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportAxioVisionV4Montage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportAxioVisionV4Montage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::ImportFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportAxioVisionV4Montage::getHumanLabel() const
{
  return "ITK::Zeiss AxioVision Import (V4)";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ImportAxioVisionV4Montage::getUuid()
{
  return QUuid("{411b008c-006f-51b2-ba05-99e51a01af3c}");
}
