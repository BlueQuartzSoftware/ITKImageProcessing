/*
 * Your License or Copyright can go here
 */

#include "ImportZenInfoMontage.h"

#include <algorithm>
#include <iostream>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/CoreFilters/ConvertColorToGrayScale.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/PreflightUpdatedValueFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/MetaXmlUtils.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/MontageImportHelper.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

namespace
{
const QString k_DCName("ZenInfo");
const QString k_MetaDataName("Zen MetaData");
} // namespace

enum createdPathID : RenameDataPath::DataID_t
{
  //  DataContainerID = 1
};

// -----------------------------------------------------------------------------
bool compareBound(ImportZenInfoMontage::BoundsType& b0, ImportZenInfoMontage::BoundsType& b1)
{
  if(b0.Origin[1] > b1.Origin[1])
  {
    return false;
  }
  if(b0.Origin[1] < b1.Origin[1])
  {
    return true;
  }
  return (b0.Origin[0] < b1.Origin[0]);
}

/* ############## Start Private Implementation ############################### */
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ImportZenInfoMontagePrivate
{
  Q_DISABLE_COPY(ImportZenInfoMontagePrivate)
  Q_DECLARE_PUBLIC(ImportZenInfoMontage)
  ImportZenInfoMontage* const q_ptr;
  ImportZenInfoMontagePrivate(ImportZenInfoMontage* ptr);

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
  int32_t m_MaxRow = 0;
  int32_t m_MaxCol = 0;
  std::vector<ImportZenInfoMontage::BoundsType> m_BoundsCache;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportZenInfoMontagePrivate::ImportZenInfoMontagePrivate(ImportZenInfoMontage* ptr)
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
ImportZenInfoMontage::ImportZenInfoMontage()
: m_InputFile("")
, m_DataContainerPath(ITKImageProcessing::Montage::k_DataContaineNameDefaultName)
, m_CellAttributeMatrixName(ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName)
, m_ImageDataArrayName(ITKImageProcessing::Montage::k_TileDataArrayDefaultName)
, m_ChangeSpacing(true)
, d_ptr(new ImportZenInfoMontagePrivate(this))
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
ImportZenInfoMontage::~ImportZenInfoMontage() = default;

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setRoot(const QDomElement& value)
{
  Q_D(ImportZenInfoMontage);
  d->m_Root = value;
}

// -----------------------------------------------------------------------------
QDomElement ImportZenInfoMontage::getRoot() const
{
  Q_D(const ImportZenInfoMontage);
  return d->m_Root;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setInputFile_Cache(const QString& value)
{
  Q_D(ImportZenInfoMontage);
  d->m_InputFile_Cache = value;
}

// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getInputFile_Cache() const
{
  Q_D(const ImportZenInfoMontage);
  return d->m_InputFile_Cache;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setTimeStamp_Cache(const QDateTime& value)
{
  Q_D(ImportZenInfoMontage);
  d->m_TimeStamp_Cache = value;
}

// -----------------------------------------------------------------------------
QDateTime ImportZenInfoMontage::getTimeStamp_Cache() const
{
  Q_D(const ImportZenInfoMontage);
  return d->m_TimeStamp_Cache;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setBoundsCache(const std::vector<ImportZenInfoMontage::BoundsType>& value)
{
  Q_D(ImportZenInfoMontage);
  d->m_BoundsCache = value;
}

// -----------------------------------------------------------------------------
std::vector<ImportZenInfoMontage::BoundsType> ImportZenInfoMontage::getBoundsCache() const
{
  Q_D(const ImportZenInfoMontage);
  return d->m_BoundsCache;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
  m_MontageStart[0] = m_ColumnMontageLimits[0];
  m_MontageStart[1] = m_RowMontageLimits[0];

  m_MontageEnd[0] = m_ColumnMontageLimits[1];
  m_MontageEnd[1] = m_RowMontageLimits[1];
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Zen Export File (*_info.xml)", InputFile, FilterParameter::Category::Parameter, ImportZenInfoMontage, "*.xml"));

  parameters.push_back(SIMPL_NEW_STRING_FP("Name of Created Montage", MontageName, FilterParameter::Category::Parameter, ImportZenInfoMontage));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Column Start/End [Inclusive, Zero Based]", ColumnMontageLimits, FilterParameter::Category::Parameter, ImportZenInfoMontage));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Row Start/End [Inclusive, Zero Based]", RowMontageLimits, FilterParameter::Category::Parameter, ImportZenInfoMontage));
  QVector<QString> choices = IGeometry::GetAllLengthUnitStrings();
  parameters.push_back(SIMPL_NEW_CHOICE_FP("Length Unit", LengthUnit, FilterParameter::Category::Parameter, ImportZenInfoMontage, choices, false));

  PreflightUpdatedValueFilterParameter::Pointer param = SIMPL_NEW_PREFLIGHTUPDATEDVALUE_FP("Montage Information", MontageInformation, FilterParameter::Category::Parameter, ImportZenInfoMontage);
  param->setReadOnly(true);
  parameters.push_back(param);

  QStringList linkedProps("Origin");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Origin", ChangeOrigin, FilterParameter::Category::Parameter, ImportZenInfoMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Category::Parameter, ImportZenInfoMontage));

  // Changing the Spacing makes NO sense because the origins in the XML file are in Pixel Coordinates so the spacing MUST be 1.0
  //  linkedProps.clear();
  //  linkedProps << "Spacing";
  //  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Spacing", ChangeSpacing, FilterParameter::Category::Parameter, ImportZenInfoMontage, linkedProps));
  //  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Category::Parameter, ImportZenInfoMontage));

  linkedProps.clear();
  linkedProps << "ColorWeights";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Convert To GrayScale", ConvertToGrayScale, FilterParameter::Category::Parameter, ImportZenInfoMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Color Weighting", ColorWeights, FilterParameter::Category::Parameter, ImportZenInfoMontage));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("DataContainer Prefix", DataContainerPath, FilterParameter::Category::CreatedArray, ImportZenInfoMontage));
  parameters.push_back(SIMPL_NEW_AM_WITH_LINKED_DC_FP("Cell Attribute Matrix Name", CellAttributeMatrixName, DataContainerPath, FilterParameter::Category::CreatedArray, ImportZenInfoMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image DataArray Name", ImageDataArrayName, FilterParameter::Category::CreatedArray, ImportZenInfoMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

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
    QDomDocument domDocument;

    if(!domDocument.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn))
    {
      QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
      setErrorCondition(-70000, ss);
      return;
    }

    QDomElement exportDocument = domDocument.documentElement();

    if(exportDocument.tagName() != Zeiss::ZenXml::ExportDocument)
    {
      QString ss = QObject::tr("Could not find the <ExportDocument> element. Abort Parsing. Is the file a Zeiss Zen Export XML file");
      setErrorCondition(-70001, ss);
      return;
    }

    d_ptr->m_Root = exportDocument;
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

    generateCache(exportDocument);
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
void ImportZenInfoMontage::execute()
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
    QString ss = QObject::tr("Error Importing a Zeiss Zen Info file set.");
    setErrorCondition(-91000, ss);
    return;
  }
}

// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getMontageInformation()
{
  QString info = d_ptr->m_MontageInformation;
  QString montageInfo;
  QTextStream ss(&montageInfo);
  int32_t importedCols = m_MontageEnd[0] - m_MontageStart[0] + 1;
  int32_t importedRows = m_MontageEnd[1] - m_MontageStart[1] + 1;
  ss << "\n"
     << "Imported Columns: " << importedCols << "  Imported Rows: " << importedRows << "  Imported Image Count: " << (importedCols * importedRows);
  info = info.replace(ITKImageProcessing::Montage::k_MontageInfoReplaceKeyword, montageInfo);
  return info;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::flushCache()
{
  m_FilenameList.clear();
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
  d_ptr->m_MaxCol = 0;
  d_ptr->m_MaxRow = 0;
  d_ptr->m_BoundsCache.clear();
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::findTileIndices(int32_t tolerance, std::vector<BoundsType>& bounds)
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
void ImportZenInfoMontage::generateCache(QDomElement& exportDocument)
{
  QDomNodeList imageList = exportDocument.elementsByTagName(Zeiss::ZenXml::Image);

  if(imageList.isEmpty())
  {
    return;
  }

  std::vector<BoundsType> bounds(imageList.size());
  std::set<int32_t> xValuesSet;
  std::set<int32_t> yValuesSet;

  FloatVec3Type minCoord = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
  FloatVec3Type minSpacing = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

  bool ok = false;
  for(int32_t i = 0; i < imageList.count(); i++)
  {
    QDomElement imageElement = imageList.at(i).toElement();

    QDomElement filenameElement = imageElement.firstChildElement(Zeiss::ZenXml::Filename).toElement();
    QString imageFileName = filenameElement.text();
    QDomElement boundsElement = imageElement.firstChildElement(Zeiss::ZenXml::Bounds).toElement();

    QFileInfo fi = QFileInfo(getInputFile());
    QString imagePath = fi.absoluteDir().path() + "/" + imageFileName;
    fi = QFileInfo(imagePath);
    if(!fi.exists())
    {
      setErrorCondition(-222, QString("Montage Tile File does not exist.'%1'").arg(imagePath));
    }
    m_FilenameList.push_back(imagePath);

    BoundsType bound;
    bound.Filename = fi.absoluteFilePath();

    bound.Origin[0] = boundsElement.attribute(Zeiss::ZenXml::StartX).toInt(&ok);
    xValuesSet.insert(bound.Origin[0]);
    bound.Dims[0] = boundsElement.attribute(Zeiss::ZenXml::SizeX).toInt(&ok);

    bound.Origin[1] = boundsElement.attribute(Zeiss::ZenXml::StartY).toInt(&ok);
    yValuesSet.insert(bound.Origin[1]);
    bound.Dims[1] = boundsElement.attribute(Zeiss::ZenXml::SizeY).toInt(&ok);

    bound.Dims[2] = 0;
    bound.Origin[2] = 0.0f;
    bound.Spacing[2] = 1.0f;

    if(m_ChangeSpacing)
    {
      bound.Spacing = m_Spacing;
    }

    bound.LengthUnit = static_cast<IGeometry::LengthUnit>(getLengthUnit());

    bound.StartC = boundsElement.attribute(Zeiss::ZenXml::StartC).toInt(&ok);
    bound.StartS = boundsElement.attribute(Zeiss::ZenXml::StartS).toInt(&ok);
    bound.StartB = boundsElement.attribute(Zeiss::ZenXml::StartB).toInt(&ok);
    bound.StartM = boundsElement.attribute(Zeiss::ZenXml::StartM).toInt(&ok);

    // Do some accounting here for spacing/origin calculations later on...
    minSpacing = bound.Spacing;
    minCoord[0] = std::min(bound.Origin[0], minCoord[0]);
    minCoord[1] = std::min(bound.Origin[1], minCoord[1]);
    minCoord[2] = 0.0f;

    d_ptr->m_MaxCol = std::max(bound.Col, d_ptr->m_MaxCol);
    d_ptr->m_MaxRow = std::max(bound.Row, d_ptr->m_MaxRow);

    // Finally set the bound into the bounds vector
    bounds[i] = bound;
  }

  findTileIndices(m_Tolerance, bounds);

  // std::vector<ImageGeom::Pointer> geometries;
  d_ptr->m_MaxCol = 0;
  d_ptr->m_MaxRow = 0;
  // Get the meta information from disk for each image
  for(auto& bound : bounds)
  {
    d_ptr->m_MaxCol = std::max(bound.Col, d_ptr->m_MaxCol);
    d_ptr->m_MaxRow = std::max(bound.Row, d_ptr->m_MaxRow);

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
      AbstractFilter::Pointer grayScaleFilter = MontageImportHelper::CreateColorToGrayScaleFilter(this, dap, getColorWeights(), ITKImageProcessing::Montage::k_GrayScaleTempArrayName);
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
  ss << "Tile Column(s): " << m_ColumnCount - 1 << "  Tile Row(s): " << m_RowCount - 1 << "  Image Count: " << bounds.size();

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
    for(auto& bound : bounds)
    {
      // BoundsType& bound = bounds.at(i);
      std::transform(bound.Origin.begin(), bound.Origin.end(), delta.begin(), bound.Origin.begin(), std::minus<>());
    }
  }
  ss << "\nOrigin: " << overrideOrigin[0] << ", " << overrideOrigin[1] << ", " << overrideOrigin[2];
  ss << "\nSpacing: " << overrideSpacing[0] << ", " << overrideSpacing[1] << ", " << overrideSpacing[2];
  ss << ITKImageProcessing::Montage::k_MontageInfoReplaceKeyword;
  d_ptr->m_MontageInformation = montageInfo;
  setBoundsCache(bounds);
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::generateDataStructure()
{
  std::vector<BoundsType>& bounds = d_ptr->m_BoundsCache;

  DataContainerArray::Pointer dca = getDataContainerArray();

  GridMontage::Pointer gridMontage = GridMontage::New(getMontageName(), m_RowCount, m_ColumnCount);

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
    dcNameStream << "r";
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

    // Create the Image Geometry
    ImageGeom::Pointer image = ImageGeom::CreateGeometry(dcName);
    SizeVec3Type dims(bound.Dims[0], bound.Dims[1], 1);
    image->setDimensions(dims);
    FloatVec3Type origin(bound.Origin[0], bound.Origin[1], 0.0f);
    image->setOrigin(origin);
    FloatVec3Type spacing(bound.Spacing[0], bound.Spacing[1], 1.0);
    image->setSpacing(spacing);
    image->setUnits(bound.LengthUnit);

    dc->setGeometry(image);

    GridTileIndex gridIndex = gridMontage->getTileIndex(bound.Row, bound.Col);
    // Set the montage's DataContainer for the current index
    gridMontage->setDataContainer(gridIndex, dc);

    std::vector<size_t> tDims = {dims[0], dims[1], dims[2]};
    // Create the Cell Attribute Matrix into which the image data would be read
    AttributeMatrix::Pointer cellAttrMat = AttributeMatrix::New(tDims, getCellAttributeMatrixName(), AttributeMatrix::Type::Cell);
    dc->addOrReplaceAttributeMatrix(cellAttrMat);
    cellAttrMat->addOrReplaceAttributeArray(bound.ImageDataProxy);
  }
  getDataContainerArray()->addOrReplaceMontage(gridMontage);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::readImages()
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
    dcNameStream << "r";
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
AbstractFilter::Pointer ImportZenInfoMontage::newFilterInstance(bool copyFilterParameters) const
{
  ImportZenInfoMontage::Pointer filter = ImportZenInfoMontage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getBrandingString() const
{
  return ITKImageProcessingConstants::ITKImageProcessingPluginDisplayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::ImportFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getHumanLabel() const
{
  return "ITK::Zeiss Zen Import";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ImportZenInfoMontage::getUuid() const
{
  return QUuid("{774ec947-eed6-5eb2-a01b-ee6470e61521}");
}

// -----------------------------------------------------------------------------
ImportZenInfoMontage::Pointer ImportZenInfoMontage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ImportZenInfoMontage> ImportZenInfoMontage::New()
{
  struct make_shared_enabler : public ImportZenInfoMontage
  {

  private:
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getNameOfClass() const
{
  return QString("ImportZenInfoMontage");
}

// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::ClassName()
{
  return QString("ImportZenInfoMontage");
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setInputFile(const QString& value)
{
  m_InputFile = value;
}

// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getInputFile() const
{
  return m_InputFile;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setColumnMontageLimits(const IntVec2Type& value)
{
  m_ColumnMontageLimits = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ImportZenInfoMontage::getColumnMontageLimits() const
{
  return m_ColumnMontageLimits;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setRowMontageLimits(const IntVec2Type& value)
{
  m_RowMontageLimits = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ImportZenInfoMontage::getRowMontageLimits() const
{
  return m_RowMontageLimits;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setDataContainerPath(const DataArrayPath& value)
{
  m_DataContainerPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ImportZenInfoMontage::getDataContainerPath() const
{
  return m_DataContainerPath;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setCellAttributeMatrixName(const QString& value)
{
  m_CellAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getCellAttributeMatrixName() const
{
  return m_CellAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setImageDataArrayName(const QString& value)
{
  m_ImageDataArrayName = value;
}

// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getImageDataArrayName() const
{
  return m_ImageDataArrayName;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setConvertToGrayScale(bool value)
{
  m_ConvertToGrayScale = value;
}

// -----------------------------------------------------------------------------
bool ImportZenInfoMontage::getConvertToGrayScale() const
{
  return m_ConvertToGrayScale;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setColorWeights(const FloatVec3Type& value)
{
  m_ColorWeights = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ImportZenInfoMontage::getColorWeights() const
{
  return m_ColorWeights;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setChangeOrigin(bool value)
{
  m_ChangeOrigin = value;
}

// -----------------------------------------------------------------------------
bool ImportZenInfoMontage::getChangeOrigin() const
{
  return m_ChangeOrigin;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setOrigin(const FloatVec3Type& value)
{
  m_Origin = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ImportZenInfoMontage::getOrigin() const
{
  return m_Origin;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setChangeSpacing(bool value)
{
  m_ChangeSpacing = value;
}

// -----------------------------------------------------------------------------
bool ImportZenInfoMontage::getChangeSpacing() const
{
  return m_ChangeSpacing;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setSpacing(const FloatVec3Type& value)
{
  m_Spacing = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ImportZenInfoMontage::getSpacing() const
{
  return m_Spacing;
}

// -----------------------------------------------------------------------------
int32_t ImportZenInfoMontage::getRowCount() const
{
  return m_RowCount;
}

// -----------------------------------------------------------------------------
int32_t ImportZenInfoMontage::getColumnCount() const
{
  return m_ColumnCount;
}

// -----------------------------------------------------------------------------
QStringList ImportZenInfoMontage::getFilenameList() const
{
  return m_FilenameList;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setMontageName(const QString& value)
{
  m_MontageName = value;
}

// -----------------------------------------------------------------------------
QString ImportZenInfoMontage::getMontageName() const
{
  return m_MontageName;
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setLengthUnit(int32_t value)
{
  m_LengthUnit = value;
}

// -----------------------------------------------------------------------------
int32_t ImportZenInfoMontage::getLengthUnit() const
{
  return m_LengthUnit;
}
