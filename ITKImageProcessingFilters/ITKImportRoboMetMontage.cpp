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

#include "ITKImportRoboMetMontage.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "SIMPLib/CoreFilters/ConvertColorToGrayScale.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/PreflightUpdatedValueFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Utilities/StringOperations.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/MontageImportHelper.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "MetaXmlUtils.h"

namespace
{
const QString k_DCName("RoboMetInfo");
const QString k_MetaDataName("RoboMet MetaData");
} // namespace

enum createdPathID : RenameDataPath::DataID_t
{
  // DataContainerID = 1
};

/* ############## Start Private Implementation ############################### */
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImportRoboMetMontagePrivate
{
  Q_DISABLE_COPY(ITKImportRoboMetMontagePrivate)
  Q_DECLARE_PUBLIC(ITKImportRoboMetMontage)
  ITKImportRoboMetMontage* const q_ptr;
  ITKImportRoboMetMontagePrivate(ITKImportRoboMetMontage* ptr);

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
  int32_t m_SliceNumber = -1;
  QString m_ImageFilePrefix;
  QString m_ImageFileExtension;
  int32_t m_MaxRow = 0;
  int32_t m_MaxCol = 0;
  std::vector<ITKImportRoboMetMontage::BoundsType> m_BoundsCache;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportRoboMetMontagePrivate::ITKImportRoboMetMontagePrivate(ITKImportRoboMetMontage* ptr)
: q_ptr(ptr)
, m_InputFile_Cache("")
, m_TimeStamp_Cache(QDateTime())
{
}
/* ############## Start Public Implementation ############################### */

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportRoboMetMontage::ITKImportRoboMetMontage()
: m_InputFile("")
, m_DataContainerPath(ITKImageProcessing::Montage::k_DataContaineNameDefaultName)
, m_CellAttributeMatrixName(ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName)
, m_ImageDataArrayName(ITKImageProcessing::Montage::k_TileDataArrayDefaultName)
, m_LengthUnit(-1)
, d_ptr(new ITKImportRoboMetMontagePrivate(this))
{
  m_NumImages = 0;
  m_ColorWeights = FloatVec3Type(0.2125f, 0.7154f, 0.0721f);
  m_Origin = FloatVec3Type(0.0f, 0.0f, 0.0f);
  m_Spacing = FloatVec3Type(1.0f, 1.0f, 1.0f);
  m_MontageStart = IntVec2Type(0, 0);
  m_MontageEnd = IntVec2Type(0, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportRoboMetMontage::~ITKImportRoboMetMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setInputFile_Cache(const QString& value)
{
  Q_D(ITKImportRoboMetMontage);
  d->m_InputFile_Cache = value;
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getInputFile_Cache() const
{
  Q_D(const ITKImportRoboMetMontage);
  return d->m_InputFile_Cache;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setTimeStamp_Cache(const QDateTime& value)
{
  Q_D(ITKImportRoboMetMontage);
  d->m_TimeStamp_Cache = value;
}

// -----------------------------------------------------------------------------
QDateTime ITKImportRoboMetMontage::getTimeStamp_Cache() const
{
  Q_D(const ITKImportRoboMetMontage);
  return d->m_TimeStamp_Cache;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setBoundsCache(const std::vector<ITKImportRoboMetMontage::BoundsType>& value)
{
  Q_D(ITKImportRoboMetMontage);
  d->m_BoundsCache = value;
}

// -----------------------------------------------------------------------------
std::vector<ITKImportRoboMetMontage::BoundsType> ITKImportRoboMetMontage::getBoundsCache() const
{
  Q_D(const ITKImportRoboMetMontage);
  return d->m_BoundsCache;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Registration File (Mosaic Details)", InputFile, FilterParameter::Parameter, ITKImportRoboMetMontage, "", "*.txt"));

  PreflightUpdatedValueFilterParameter::Pointer param = SIMPL_NEW_PREFLIGHTUPDATEDVALUE_FP("Montage Information", MontageInformation, FilterParameter::Parameter, ITKImportRoboMetMontage);
  param->setReadOnly(true);
  parameters.push_back(param);

  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Start (Col, Row) [Inclusive, Zero Based]", MontageStart, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage End (Col, Row) [Inclusive, Zero Based]", MontageEnd, FilterParameter::Parameter, ITKImportRoboMetMontage));

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Slice Number", SliceNumber, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image File Prefix", ImageFilePrefix, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image File Extension", ImageFileExtension, FilterParameter::Parameter, ITKImportRoboMetMontage));

  QStringList linkedProps("Origin");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Origin", ChangeOrigin, FilterParameter::Parameter, ITKImportRoboMetMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ITKImportRoboMetMontage));

  linkedProps.clear();
  linkedProps << "Spacing";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Spacing", ChangeSpacing, FilterParameter::Parameter, ITKImportRoboMetMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ITKImportRoboMetMontage));

  linkedProps.clear();
  linkedProps << "ColorWeights";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Convert To GrayScale", ConvertToGrayScale, FilterParameter::Parameter, ITKImportRoboMetMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Color Weighting", ColorWeights, FilterParameter::Parameter, ITKImportRoboMetMontage));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("DataContainer Prefix", DataContainerPath, FilterParameter::CreatedArray, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix Name", CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image DataArray Name", ImageDataArrayName, FilterParameter::CreatedArray, ITKImportRoboMetMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  QString ss;

  if(m_InputFile.isEmpty())
  {
    QString ss = QObject::tr("The registration file must be set");
    setErrorCondition(-2002, ss);
    return;
  }

  QFileInfo fi(m_InputFile);
  if(!fi.exists())
  {
    QString ss = QObject::tr("The registration file does not exist");
    setErrorCondition(-2001, ss);
    return;
  }

  if(fi.completeSuffix().toLower() != "csv")
  {
    QString ss = QObject::tr("Config file is not in RoboMet format (*.csv).");
    setErrorCondition(-2003, ss);
    return;
  }

  if(m_ImageFilePrefix.isEmpty())
  {
    QString ss = QObject::tr("The image file prefix must be set");
    setErrorCondition(-2004, ss);
    return;
  }

  if(m_ImageFileExtension.isEmpty())
  {
    QString ss = QObject::tr("The image file extension must be set");
    setErrorCondition(-2005, ss);
    return;
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
    && timeStamp == d_ptr->m_TimeStamp_Cache
    && d_ptr->m_TimeStamp_Cache.isValid()
    && m_SliceNumber == d_ptr->m_SliceNumber
    && m_ImageFilePrefix == d_ptr->m_ImageFilePrefix
    && m_ImageFileExtension == d_ptr->m_ImageFileExtension
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
    QString ss = QObject::tr("Montage Start Row (%1) and Montage End Row(%2) must be <= %3").arg(m_MontageStart[1]).arg(m_MontageEnd[1]).arg(d_ptr->m_MaxRow);
    setErrorCondition(-401, ss);
    return;
  }

  generateDataStructure();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::execute()
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
    QString ss = QObject::tr("Error Importing RoboMet Montage.");
    setErrorCondition(-91000, ss);
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::generateCache()
{
  QByteArray buf;
  QFile in(getInputFile());
  if(!in.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString msg = QString("Csv file could not be opened: ") + getInputFile();
    setErrorCondition(-100, msg);
    return;
  }

  int sliceNumberIndex = -1;
  int rowIndex = -1;
  int colIndex = -1;
  int xPosIndex = -1;
  int yPosIndex = -1;

  // Read the Header Line
  buf = in.readLine().trimmed();
  QList<QByteArray> line = buf.split(',');
  for(const auto& columnName : line)
  {
    if(QString::compare(columnName.trimmed(), "row#", Qt::CaseInsensitive) == 0)
    {
      rowIndex = line.indexOf(columnName);
    }
    else if(QString::compare(columnName.trimmed(), "col#", Qt::CaseInsensitive) == 0)
    {
      colIndex = line.indexOf(columnName);
    }
    else if(QString::compare(columnName.trimmed(), "imageNumber", Qt::CaseInsensitive) == 0)
    {
      sliceNumberIndex = line.indexOf(columnName);
    }
    else if(QString::compare(columnName.trimmed(), "Xposition", Qt::CaseInsensitive) == 0)
    {
      xPosIndex = line.indexOf(columnName);
    }
    else if(QString::compare(columnName.trimmed(), "Yposition", Qt::CaseInsensitive) == 0)
    {
      yPosIndex = line.indexOf(columnName);
    }
  }
  // Sanity check the column indices
  if(sliceNumberIndex < 0 || rowIndex < 0 || colIndex < 0 || xPosIndex < 0 || yPosIndex < 0)
  {
    setErrorCondition(-101, "Required columns missing in CSV file.");
    return;
  }

  std::vector<BoundsType> bounds;
  m_ColumnCount = 0;
  m_RowCount = 0;

  while(!in.atEnd())
  {
    buf = in.readLine().trimmed();
    line = buf.split(',');

    int sliceNumber = line[sliceNumberIndex].toInt();
    if(sliceNumber != getSliceNumber())
    {
      continue; // Skip slice numbers not requested by the user
    }
    int row = line[rowIndex].toInt();
    m_RowCount = std::max(row, m_RowCount);

    int col = line[colIndex].toInt();
    m_ColumnCount = std::max(col, m_ColumnCount);

    double xPos = line[xPosIndex].trimmed().toDouble();
    double yPos = line[yPosIndex].trimmed().toDouble();

    QString imageFilePath = getImageFilePath(getInputFile(), sliceNumber, row, col);
    QFileInfo fi(imageFilePath);
    if(!fi.exists())
    {
      setErrorCondition(-103, QString("Montage Tile File does not exist.'%1'").arg(imageFilePath));
      // qDebug() << "Image File '" << imageFilePath << "' does not exist";
      continue;
    }

    BoundsType bound;
    bound.Filename = imageFilePath;
    bound.Origin = FloatVec3Type(xPos, yPos, 0.0f);
    bound.Spacing = FloatVec3Type(1.0f, 1.0f, 1.0f);
    if(m_ChangeSpacing)
    {
      bound.Spacing = m_Spacing;
    }
    bound.Col = col;
    bound.Row = row;

    d_ptr->m_MaxCol = std::max(bound.Col, d_ptr->m_MaxCol);
    d_ptr->m_MaxRow = std::max(bound.Row, d_ptr->m_MaxRow);

    bounds.push_back(bound);
  }
  // We use Zero based indexing so add 1 to get the counts
  m_ColumnCount++;
  m_RowCount++;
  m_NumImages = bounds.size();

  FloatVec3Type minCoord = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
  FloatVec3Type minSpacing = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
  std::vector<ImageGeom::Pointer> geometries;

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
      DataContainer::Pointer fromDc = importImageDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer fromCellAttrMat = fromDc->getAttributeMatrix(ITKImageProcessing::Montage::k_AMName);
      IDataArray::Pointer fromImageData = fromCellAttrMat->getAttributeArray(ITKImageProcessing::Montage::k_AAName);
      fromImageData->setName(getImageDataArrayName());
      bound.ImageDataProxy = fromImageData;

      // Set the Origin based on the CSV values
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
  }

  QString montageInfo;
  QTextStream ss(&montageInfo);
  ss << "Max Column: " << m_ColumnCount - 1 << "  Max Row: " << m_RowCount - 1 << "  Image Count: " << m_NumImages;
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
void ITKImportRoboMetMontage::generateDataStructure()
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
    DataContainer::Pointer dc = dca->createNonPrereqDataContainer(this, dcName);
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
void ITKImportRoboMetMontage::readImages()
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
      connect(grayScaleFilter.get(), SIGNAL(messageGenerated(AbstractMessage::Pointer)), this, SIGNAL(messageGenerated(AbstractMessage::Pointer)));
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
QString ITKImportRoboMetMontage::getImageFilePath(const QString& filePath, int imageNumber, int row, int col)
{
  QFileInfo fi(filePath);
  QString sixDigitImageNumber = StringOperations::GeneratePaddedString(imageNumber, 6, '0');

  QString twoDigitRowNumber = StringOperations::GeneratePaddedString(row, 2, '0');

  QString twoDigitColNumber = StringOperations::GeneratePaddedString(col, 2, '0');

  QString imageDir = getImageFilePrefix() + sixDigitImageNumber;
  QString imageFile = getImageFilePrefix() + sixDigitImageNumber + '_' + twoDigitColNumber + '_' + twoDigitRowNumber + '.' + getImageFileExtension();
  QString imageFilePath = fi.path() + QDir::separator() + imageDir + QDir::separator() + imageFile;

  return imageFilePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::flushCache()
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
  d_ptr->m_BoundsCache.clear();
  d_ptr->m_SliceNumber = -1;
  d_ptr->m_ImageFilePrefix.clear();
  d_ptr->m_ImageFileExtension.clear();
  d_ptr->m_MaxCol = 0;
  d_ptr->m_MaxRow = 0;
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getMontageInformation()
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
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImportRoboMetMontage::newFilterInstance(bool copyFilterParameters) const
{
  ITKImportRoboMetMontage::Pointer filter = ITKImportRoboMetMontage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKImportRoboMetMontage::getUuid() const
{
  return QUuid("{cdb130af-3616-57b1-be59-fe18113b2621}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getHumanLabel() const
{
  return "ITK::Import RoboMet Montage";
}

// -----------------------------------------------------------------------------
ITKImportRoboMetMontage::Pointer ITKImportRoboMetMontage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKImportRoboMetMontage> ITKImportRoboMetMontage::New()
{
  struct make_shared_enabler : public ITKImportRoboMetMontage  
  {

  private:

  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getNameOfClass() const
{
  return QString("ITKImportRoboMetMontage");
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::ClassName()
{
  return QString("ITKImportRoboMetMontage");
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setInputFile(const QString& value)
{
  m_InputFile = value;
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getInputFile() const
{
  return m_InputFile;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setMontageStart(const IntVec2Type& value)
{
  m_MontageStart = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ITKImportRoboMetMontage::getMontageStart() const
{
  return m_MontageStart;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setMontageEnd(const IntVec2Type& value)
{
  m_MontageEnd = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ITKImportRoboMetMontage::getMontageEnd() const
{
  return m_MontageEnd;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setDataContainerPath(const DataArrayPath& value)
{
  m_DataContainerPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ITKImportRoboMetMontage::getDataContainerPath() const
{
  return m_DataContainerPath;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setCellAttributeMatrixName(const QString& value)
{
  m_CellAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getCellAttributeMatrixName() const
{
  return m_CellAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setImageDataArrayName(const QString& value)
{
  m_ImageDataArrayName = value;
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getImageDataArrayName() const
{
  return m_ImageDataArrayName;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setConvertToGrayScale(bool value)
{
  m_ConvertToGrayScale = value;
}

// -----------------------------------------------------------------------------
bool ITKImportRoboMetMontage::getConvertToGrayScale() const
{
  return m_ConvertToGrayScale;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setColorWeights(const FloatVec3Type& value)
{
  m_ColorWeights = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKImportRoboMetMontage::getColorWeights() const
{
  return m_ColorWeights;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setChangeOrigin(bool value)
{
  m_ChangeOrigin = value;
}

// -----------------------------------------------------------------------------
bool ITKImportRoboMetMontage::getChangeOrigin() const
{
  return m_ChangeOrigin;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setOrigin(const FloatVec3Type& value)
{
  m_Origin = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKImportRoboMetMontage::getOrigin() const
{
  return m_Origin;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setChangeSpacing(bool value)
{
  m_ChangeSpacing = value;
}

// -----------------------------------------------------------------------------
bool ITKImportRoboMetMontage::getChangeSpacing() const
{
  return m_ChangeSpacing;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setSpacing(const FloatVec3Type& value)
{
  m_Spacing = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKImportRoboMetMontage::getSpacing() const
{
  return m_Spacing;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setSliceNumber(int value)
{
  m_SliceNumber = value;
}

// -----------------------------------------------------------------------------
int ITKImportRoboMetMontage::getSliceNumber() const
{
  return m_SliceNumber;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setImageFilePrefix(const QString& value)
{
  m_ImageFilePrefix = value;
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getImageFilePrefix() const
{
  return m_ImageFilePrefix;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setImageFileExtension(const QString& value)
{
  m_ImageFileExtension = value;
}

// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getImageFileExtension() const
{
  return m_ImageFileExtension;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setFileWasRead(bool value)
{
  m_FileWasRead = value;
}

// -----------------------------------------------------------------------------
bool ITKImportRoboMetMontage::getFileWasRead() const
{
  return m_FileWasRead;
}

// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setLengthUnit(int32_t value)
{
  m_LengthUnit = value;
}

// -----------------------------------------------------------------------------
int32_t ITKImportRoboMetMontage::getLengthUnit() const
{
  return m_LengthUnit;
}


