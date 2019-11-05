/* ============================================================================
 * Copyright (c) 2019-2019 BlueQuartz Software, LLC
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
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
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
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-15-D-5231
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <memory>

#include "ITKImportFijiMontage.h"

#include <vector>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/CoreFilters/ConvertColorToGrayScale.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/PreflightUpdatedValueFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/MontageImportHelper.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "MetaXmlUtils.h"

namespace
{
const QString k_DCName("ZenInfo");
const QString k_MetaDataName("Zen MetaData");
} // namespace

enum createdPathID : RenameDataPath::DataID_t
{
  //  DataContainerID = 1
};

/* ############## Start Private Implementation ############################### */
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImportFijiMontagePrivate
{
  Q_DISABLE_COPY(ITKImportFijiMontagePrivate)
  Q_DECLARE_PUBLIC(ITKImportFijiMontage)
  ITKImportFijiMontage* const q_ptr;
  ITKImportFijiMontagePrivate(ITKImportFijiMontage* ptr);

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
  int32_t m_MaxRow = 0;
  int32_t m_MaxCol = 0;
  std::vector<ITKImportFijiMontage::BoundsType> m_BoundsCache;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportFijiMontagePrivate::ITKImportFijiMontagePrivate(ITKImportFijiMontage* ptr)
: q_ptr(ptr)
, m_InputFile_Cache("")
, m_TimeStamp_Cache(QDateTime())
{
}

/* ############## Start Public Implementation ############################### */
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportFijiMontage::ITKImportFijiMontage()
: m_InputFile("")
, m_DataContainerPath(ITKImageProcessing::Montage::k_DataContaineNameDefaultName)
, m_CellAttributeMatrixName(ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName)
, m_ImageDataArrayName(ITKImageProcessing::Montage::k_TileDataArrayDefaultName)
, m_ConvertToGrayScale(false)
, m_ChangeOrigin(false)
, m_ChangeSpacing(false)
, m_LengthUnit(-1)
, d_ptr(new ITKImportFijiMontagePrivate(this))
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
ITKImportFijiMontage::~ITKImportFijiMontage() = default;

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setInputFile_Cache(const QString& value)
{
  Q_D(ITKImportFijiMontage);
  d->m_InputFile_Cache = value;
}

// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getInputFile_Cache() const
{
  Q_D(const ITKImportFijiMontage);
  return d->m_InputFile_Cache;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setTimeStamp_Cache(const QDateTime& value)
{
  Q_D(ITKImportFijiMontage);
  d->m_TimeStamp_Cache = value;
}

// -----------------------------------------------------------------------------
QDateTime ITKImportFijiMontage::getTimeStamp_Cache() const
{
  Q_D(const ITKImportFijiMontage);
  return d->m_TimeStamp_Cache;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setBoundsCache(const std::vector<ITKImportFijiMontage::BoundsType>& value)
{
  Q_D(ITKImportFijiMontage);
  d->m_BoundsCache = value;
}

// -----------------------------------------------------------------------------
std::vector<ITKImportFijiMontage::BoundsType> ITKImportFijiMontage::getBoundsCache() const
{
  Q_D(const ITKImportFijiMontage);
  return d->m_BoundsCache;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Fiji Configuration File", InputFile, FilterParameter::Parameter, ITKImportFijiMontage, "*.txt"));

  PreflightUpdatedValueFilterParameter::Pointer param = SIMPL_NEW_PREFLIGHTUPDATEDVALUE_FP("Montage Information", MontageInformation, FilterParameter::Parameter, ITKImportFijiMontage);
  param->setReadOnly(true);
  parameters.push_back(param);

  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Start (Col, Row) [Inclusive, Zero Based]", MontageStart, FilterParameter::Parameter, ITKImportFijiMontage));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage End (Col, Row) [Inclusive, Zero Based]", MontageEnd, FilterParameter::Parameter, ITKImportFijiMontage));

  QStringList linkedProps("Origin");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Origin", ChangeOrigin, FilterParameter::Parameter, ITKImportFijiMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ITKImportFijiMontage));

  // Changing the Spacing makes NO sense because the origins in the XML file are in Pixel Coordinates so the spacing MUST be 1.0
  //  linkedProps.clear();
  //  linkedProps << "Spacing";
  //  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Spacing", ChangeSpacing, FilterParameter::Parameter, ITKImportFijiMontage, linkedProps));
  //  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ITKImportFijiMontage));

  linkedProps.clear();
  linkedProps << "ColorWeights";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Convert To GrayScale", ConvertToGrayScale, FilterParameter::Parameter, ITKImportFijiMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Color Weighting", ColorWeights, FilterParameter::Parameter, ITKImportFijiMontage));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("DataContainer Prefix", DataContainerPath, FilterParameter::CreatedArray, ITKImportFijiMontage));
  parameters.push_back(SIMPL_NEW_AM_WITH_LINKED_DC_FP("Cell Attribute Matrix Name", CellAttributeMatrixName, DataContainerPath, FilterParameter::CreatedArray, ITKImportFijiMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image DataArray Name", ImageDataArrayName, FilterParameter::CreatedArray, ITKImportFijiMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::dataCheck()
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

  QDateTime timeStamp(fi.lastModified());

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
  )
  // clang-format on
  {
    // We are reading from the cache, so set the FileWasRead flag to false
    m_FileWasRead = false;
    // exportDocument = getRoot();
  }
  else
  {
    flushCache();
    // We are reading from the file, so set the FileWasRead flag to true
    m_FileWasRead = true;

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
    setTimeStamp_Cache(timeStamp);

    generateCache();
  }

  if(m_MontageStart[0] > m_MontageEnd[0])
  {
    QString ss = QObject::tr("Montage Start Column (%1) must be equal or less than Montage End Column (%2)").arg(m_MontageStart[0]).arg(m_MontageEnd[0]);
    setErrorCondition(-396, ss);
    return;
  }
  if(m_MontageStart[1] > m_MontageEnd[1])
  {
    QString ss = QObject::tr("Montage Start Row (%1) must be equal or less than Montage End Row (%2)").arg(m_MontageStart[1]).arg(m_MontageEnd[1]);
    setErrorCondition(-397, ss);
    return;
  }
  if(m_MontageStart[0] < 0 || m_MontageEnd[0] < 0)
  {
    QString ss = QObject::tr("Montage Start Column (%1) and Montage End Column (%2) must be greater than Zero (0)").arg(m_MontageStart[0]).arg(m_MontageEnd[0]);
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
    QString ss = QObject::tr("Montage Start Column (%1) and Montage End Column (%2) must be <= %3").arg(m_MontageStart[0]).arg(m_MontageEnd[0]).arg(d_ptr->m_MaxCol);
    setErrorCondition(-400, ss);
    return;
  }
  if(m_MontageStart[1] > d_ptr->m_MaxRow || m_MontageEnd[1] > d_ptr->m_MaxRow)
  {
    QString ss = QObject::tr("Montage Start Row (%1) and Montage End Row (%2) must be <= %3").arg(m_MontageStart[1]).arg(m_MontageEnd[1]).arg(d_ptr->m_MaxRow);
    setErrorCondition(-401, ss);
    return;
  }

  generateDataStructure();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::execute()
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
    QString ss = QObject::tr("Error Importing Fiji Montage.");
    setErrorCondition(-91000, ss);
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::generateCache()
{
  // This next function will set the FileName (Partial), Row, Col for each "bound" object
  std::vector<BoundsType> bounds = parseConfigFile(getInputFile());

  FloatVec3Type minCoord = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
  FloatVec3Type minSpacing = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
  std::vector<ImageGeom::Pointer> geometries;

  // Get the meta information from disk for each image
  for(auto& bound : bounds)
  {
    // This will update the FileName to the absolutePath
    QFileInfo fi(getInputFile());
    QString absolutePath = fi.absolutePath() + QDir::separator() + bound.Filename;
    bound.Filename = absolutePath;

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
      DataContainer::Pointer fromDc = importImageDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer fromCellAttrMat = fromDc->getAttributeMatrix(ITKImageProcessing::Montage::k_AMName);
      IDataArray::Pointer fromImageData = fromCellAttrMat->getAttributeArray(ITKImageProcessing::Montage::k_AAName);
      fromImageData->setName(getImageDataArrayName());
      bound.ImageDataProxy = fromImageData;

      // Set the Origin based on the values from the Fiji Config File
      // Set the spacing to the default 1,1,1
      ImageGeom::Pointer imageGeom = fromDc->getGeometryAs<ImageGeom>();
      bound.Dims = imageGeom->getDimensions();
      imageGeom->setSpacing(bound.Spacing);
      imageGeom->setOrigin(bound.Origin);
      minSpacing = imageGeom->getSpacing();
      //
      minCoord[0] = std::min(bound.Origin[0], minCoord[0]);
      minCoord[1] = std::min(bound.Origin[1], minCoord[1]);
      minCoord[2] = 0.0f;
      geometries.push_back(imageGeom);
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
      DataContainer::Pointer fromDc = colorToGrayDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer fromCellAttrMat = fromDc->getAttributeMatrix(ITKImageProcessing::Montage::k_AMName);
      // Remove the RGB Attribute Array so we can rename the gray scale AttributeArray
      IDataArray::Pointer rgbImageArray = fromCellAttrMat->removeAttributeArray(ITKImageProcessing::Montage::k_AAName);
      QString grayScaleArrayName = ITKImageProcessing::Montage::k_GrayScaleTempArrayName + ITKImageProcessing::Montage::k_AAName;
      IDataArray::Pointer fromGrayScaleData = fromCellAttrMat->removeAttributeArray(grayScaleArrayName);
      fromGrayScaleData->setName(getImageDataArrayName());
      bound.ImageDataProxy = fromGrayScaleData;
    }

    d_ptr->m_MaxCol = std::max(bound.Col, d_ptr->m_MaxCol);
    d_ptr->m_MaxRow = std::max(bound.Row, d_ptr->m_MaxRow);
  }

  QString montageInfo;
  QTextStream ss(&montageInfo);
  ss << "Max Column: " << m_ColumnCount - 1 << "  Max Row: " << m_RowCount - 1 << "  Image Count: " << m_ColumnCount * m_RowCount;
  ss << ITKImageProcessing::Montage::k_MontageInfoReplaceKeyword;

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
    FloatVec3Type delta = {minCoord[0] - overrideOrigin[0], minCoord[1] - overrideOrigin[1], minCoord[2] - overrideOrigin[2]};
    for(size_t i = 0; i < geometries.size(); i++)
    {
      ImageGeom::Pointer imageGeom = geometries[i];
      BoundsType& bound = bounds.at(i);
      std::transform(bound.Origin.begin(), bound.Origin.end(), delta.begin(), bound.Origin.begin(), std::minus<>());
      imageGeom->setOrigin(bound.Origin); // Sync up the ImageGeom with the calculated values
      imageGeom->setSpacing(overrideSpacing);
    }
  }
  ss << "\nOrigin: " << overrideOrigin[0] << ", " << overrideOrigin[1] << ", " << overrideOrigin[2];
  ss << "\nSpacing: " << overrideSpacing[0] << ", " << overrideSpacing[1] << ", " << overrideSpacing[2];
  d_ptr->m_MontageInformation = montageInfo;
  setBoundsCache(bounds);
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::generateDataStructure()
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
    if(getErrorCode() < 0)
    {
      continue;
    }

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
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::readImages()
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

    image->setUnits(static_cast<IGeometry::LengthUnit>(m_LengthUnit));

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
void ITKImportFijiMontage::flushCache()
{
  setTimeStamp_Cache(QDateTime());

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
  d_ptr->m_MaxCol = 0;
  d_ptr->m_MaxRow = 0;
  d_ptr->m_BoundsCache.clear();
}

// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getMontageInformation()
{
  QString info = d_ptr->m_MontageInformation;
  QString montageInfo;
  QTextStream ss(&montageInfo);
  int32_t importedCols = m_MontageEnd[0] - m_MontageStart[0] + 1;
  int32_t importedRows = m_MontageEnd[1] - m_MontageStart[1] + 1;
  ss << "\nImported Columns: " << importedCols << "  Imported Rows: " << importedRows << "  Imported Image Count: " << (importedCols * importedRows);
  info = info.replace(ITKImageProcessing::Montage::k_MontageInfoReplaceKeyword, montageInfo);
  return info;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::findTileIndices(int32_t tolerance, std::vector<ITKImportFijiMontage::BoundsType>& bounds)
{
  std::vector<int32_t> xValues(bounds.size());
  std::vector<int32_t> yValues(bounds.size());

  for(size_t i = 0; i < bounds.size(); i++)
  {
    xValues[i] = bounds.at(i).Origin[0];
    yValues[i] = bounds.at(i).Origin[1];
  }

  std::map<int32_t, std::vector<size_t>> avg_indices = MontageImportHelper::Burn(tolerance, xValues);
  int32_t index = 0;
  for(auto& iter : avg_indices)
  {
    const std::vector<size_t>& indices = iter.second;
    for(const auto& i : indices)
    {
      bounds.at(i).Col = index;
    }
    index++;
  }
  m_ColumnCount = index;

  avg_indices = MontageImportHelper::Burn(tolerance, yValues);
  index = 0;
  for(auto& iter : avg_indices)
  {
    const std::vector<size_t>& indices = iter.second;
    for(const auto& i : indices)
    {
      bounds.at(i).Row = index;
    }
    index++;
  }
  m_RowCount = index;
}

// -----------------------------------------------------------------------------
std::vector<ITKImportFijiMontage::BoundsType> ITKImportFijiMontage::parseConfigFile(const QString& filePath)
{
  QString contents;

  // Read the Source File
  QFile source(filePath);
  source.open(QFile::ReadOnly);
  contents = source.readAll();
  source.close();

  QStringList list = contents.split(QRegExp("\\n"));
  QStringListIterator sourceLines(list);
  bool dimFound = false;
  bool dataFound = false;

  while(sourceLines.hasNext())
  {
    QString line = sourceLines.next().trimmed();

    if(line.startsWith("dim =")) // found the dimensions
    {
      dimFound = true;
      // Should check that the value = 2
    }
    if(line.startsWith("# Define the image coordinates"))
    {
      // Found the start of the data
      dataFound = true;
      break;
    }
    if(line.startsWith("#")) // comment line
    {
      continue;
    }
  }

  std::vector<ITKImportFijiMontage::BoundsType> bounds;
  if(!dimFound || !dataFound)
  {
    return bounds;
  }

  // slice_12.tif; ; (471.2965233276666, -0.522608066434236)
  while(sourceLines.hasNext())
  {
    QString line = sourceLines.next().trimmed();
    if(line.isEmpty())
    {
      continue;
    }
    QStringList tokens = line.split(";");
    if(tokens.count() != 3)
    {
      continue;
    }
    ITKImportFijiMontage::BoundsType bound;
    bound.Filename = tokens.at(0);

    QString coords = tokens.at(2).trimmed();
    coords = coords.replace("(", "");
    coords = coords.replace(")", "");
    tokens = coords.split(",");
    if(tokens.count() != 2)
    {
      continue;
    }
    float x = tokens.at(0).toFloat();
    float y = tokens.at(1).toFloat();
    bound.Origin = FloatVec3Type(x, y, 0.0f);
    bound.Spacing = FloatVec3Type(1.0f, 1.0f, 1.0f);
    bounds.push_back(bound);
  }

  findTileIndices(m_Tolerance, bounds);

  return bounds;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImportFijiMontage::newFilterInstance(bool copyFilterParameters) const
{
  ITKImportFijiMontage::Pointer filter = ITKImportFijiMontage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKImportFijiMontage::getUuid() const
{
  return QUuid("{5878723b-cc16-5486-ac5f-ff0107107e74}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getHumanLabel() const
{
  return "ITK::Import Fiji Montage";
}

// -----------------------------------------------------------------------------
ITKImportFijiMontage::Pointer ITKImportFijiMontage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKImportFijiMontage> ITKImportFijiMontage::New()
{
  struct make_shared_enabler : public ITKImportFijiMontage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getNameOfClass() const
{
  return QString("ITKImportFijiMontage");
}

// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::ClassName()
{
  return QString("ITKImportFijiMontage");
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setInputFile(const QString& value)
{
  m_InputFile = value;
}

// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getInputFile() const
{
  return m_InputFile;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setMontageStart(const IntVec2Type& value)
{
  m_MontageStart = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ITKImportFijiMontage::getMontageStart() const
{
  return m_MontageStart;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setMontageEnd(const IntVec2Type& value)
{
  m_MontageEnd = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ITKImportFijiMontage::getMontageEnd() const
{
  return m_MontageEnd;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setDataContainerPath(const DataArrayPath& value)
{
  m_DataContainerPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ITKImportFijiMontage::getDataContainerPath() const
{
  return m_DataContainerPath;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setCellAttributeMatrixName(const QString& value)
{
  m_CellAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getCellAttributeMatrixName() const
{
  return m_CellAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setImageDataArrayName(const QString& value)
{
  m_ImageDataArrayName = value;
}

// -----------------------------------------------------------------------------
QString ITKImportFijiMontage::getImageDataArrayName() const
{
  return m_ImageDataArrayName;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setConvertToGrayScale(bool value)
{
  m_ConvertToGrayScale = value;
}

// -----------------------------------------------------------------------------
bool ITKImportFijiMontage::getConvertToGrayScale() const
{
  return m_ConvertToGrayScale;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setColorWeights(const FloatVec3Type& value)
{
  m_ColorWeights = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKImportFijiMontage::getColorWeights() const
{
  return m_ColorWeights;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setChangeOrigin(bool value)
{
  m_ChangeOrigin = value;
}

// -----------------------------------------------------------------------------
bool ITKImportFijiMontage::getChangeOrigin() const
{
  return m_ChangeOrigin;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setOrigin(const FloatVec3Type& value)
{
  m_Origin = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKImportFijiMontage::getOrigin() const
{
  return m_Origin;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setChangeSpacing(bool value)
{
  m_ChangeSpacing = value;
}

// -----------------------------------------------------------------------------
bool ITKImportFijiMontage::getChangeSpacing() const
{
  return m_ChangeSpacing;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setSpacing(const FloatVec3Type& value)
{
  m_Spacing = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKImportFijiMontage::getSpacing() const
{
  return m_Spacing;
}

// -----------------------------------------------------------------------------
int32_t ITKImportFijiMontage::getRowCount() const
{
  return m_RowCount;
}

// -----------------------------------------------------------------------------
int32_t ITKImportFijiMontage::getColumnCount() const
{
  return m_ColumnCount;
}

// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setLengthUnit(int32_t value)
{
  m_LengthUnit = value;
}

// -----------------------------------------------------------------------------
int32_t ITKImportFijiMontage::getLengthUnit() const
{
  return m_LengthUnit;
}


