/*
 * Your License or Copyright can go here
 */

#include "ImportZenInfoMontage.h"

#include <algorithm>
#include <iostream>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QVector>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/CoreFilters/ConvertColorToGrayScale.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/PreflightUpdatedValueFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "MetaXmlUtils.h"
#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissImportVersion.h"

static const QString k_AttributeArrayNames("AttributeArrayNames");
static const QString k_DataContaineNameDefaultName("Zeiss Zen Montage");
static const QString k_TileAttributeMatrixDefaultName("Tile Data");
static const QString k_GrayScaleTempArrayName("gray_scale_temp");
static const QString k_AxioVisionMetaData("Zen MetaData");

namespace
{
const QString k_DCName("ZenInfo");
const QString k_AMName("CellAM");
const QString k_AAName("ImageData");
const QString k_AAGrayScaleName("GrayImageData");
} // namespace

enum createdPathID : RenameDataPath::DataID_t
{
  //  DataContainerID = 1
};

// -----------------------------------------------------------------------------
bool compareBound(BoundsType& b0, BoundsType& b1)
{
  if(b0.StartY > b1.StartY)
  {
    return false;
  }
  if(b0.StartY < b1.StartY)
  {
    return true;
  }
  return (b0.StartX < b1.StartX);
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
  QDateTime m_TimeStamp_Cache;
  QString m_MontageInformation;
  std::vector<BoundsType> m_BoundsCache;
  bool m_ChangeOrigin = false;
  bool m_ChangeSpacing = false;
  bool m_ConvertToGrayScale = false;
  FloatVec3Type m_Origin;
  FloatVec3Type m_Spacing;
  FloatVec3Type m_ColorWeights;
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
, m_DataContainerName(k_DataContaineNameDefaultName)
, m_CellAttributeMatrixName(k_TileAttributeMatrixDefaultName)
, m_ImageDataArrayName(SIMPL::CellData::ImageData)
, m_ConvertToGrayScale(false)
, m_ImportAllMetaData(false)
, m_FileWasRead(false)
, m_ChangeOrigin(false)
, m_ChangeSpacing(false)
, d_ptr(new ImportZenInfoMontagePrivate(this))
{
  m_ColorWeights[0] = 0.2125f;
  m_ColorWeights[1] = 0.7154f;
  m_ColorWeights[2] = 0.0721f;

  m_Origin = FloatVec3Type(0.0f, 0.0f, 0.0f);
  m_Spacing = FloatVec3Type(1.0f, 1.0f, 1.0f);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportZenInfoMontage::~ImportZenInfoMontage() = default;

SIMPL_PIMPL_PROPERTY_DEF(ImportZenInfoMontage, QDomElement, Root)
SIMPL_PIMPL_PROPERTY_DEF(ImportZenInfoMontage, QString, InputFile_Cache)
SIMPL_PIMPL_PROPERTY_DEF(ImportZenInfoMontage, QDateTime, TimeStamp_Cache)
SIMPL_PIMPL_PROPERTY_DEF(ImportZenInfoMontage, std::vector<BoundsType>, BoundsCache)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Zen Export File (*_info.xml)", InputFile, FilterParameter::Parameter, ImportZenInfoMontage, "*[0]ml"));

  PreflightUpdatedValueFilterParameter::Pointer param = SIMPL_NEW_PREFLIGHTUPDATEDVALUE_FP("Montage Information", MontageInformation, FilterParameter::Parameter, ImportZenInfoMontage);
  param->setReadOnly(true);
  parameters.push_back(param);

  // parameters.push_back(SIMPL_NEW_BOOL_FP("Import All MetaData", ImportAllMetaData, FilterParameter::Parameter, ImportZenInfoMontage));

  QStringList linkedProps("Origin");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Origin", ChangeOrigin, FilterParameter::Parameter, ImportZenInfoMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ImportZenInfoMontage));

  linkedProps.clear();
  linkedProps << "Spacing";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Change Spacing", ChangeSpacing, FilterParameter::Parameter, ImportZenInfoMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ImportZenInfoMontage));

  linkedProps.clear();
  linkedProps << "ColorWeights";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Convert To GrayScale", ConvertToGrayScale, FilterParameter::Parameter, ImportZenInfoMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Color Weighting", ColorWeights, FilterParameter::Parameter, ImportZenInfoMontage));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("DataContainer Prefix", DataContainerName, FilterParameter::CreatedArray, ImportZenInfoMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix Name", CellAttributeMatrixName, FilterParameter::CreatedArray, ImportZenInfoMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image DataArray Name", ImageDataArrayName, FilterParameter::CreatedArray, ImportZenInfoMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::dataCheck()
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

  if(getDataContainerName().isEmpty())
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

  QString filtName = ZeissImportConstants::ImageProcessingFilters::k_ReadImageFilterClassName;
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
  if(nullptr != filterFactory.get())
  {
    // If we get this far, the Factory is good so creating the filter should not fail unless something has
    // horribly gone wrong in which case the system is going to come down quickly after this.
    AbstractFilter::Pointer filter = filterFactory->create();
    if(nullptr == filter.get())
    {
      ss = QObject::tr("The '%1' filter is not Available, did the ITKImageProcessing Plugin Load.").arg(filtName);
      setErrorCondition(-391, ss);
    }
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
  int errorLine;
  int errorColumn;

  QDateTime timeStamp(fi.lastModified());

  QDomDocument domDocument;
  QDomElement exportDocument;

  QString inputFile = getInputFile();
  QString cacheInputFile = d_ptr->m_InputFile_Cache;
  QDateTime cacheTimeStamp = d_ptr->m_TimeStamp_Cache;

  if(inputFile == cacheInputFile && cacheTimeStamp.isValid() && cacheTimeStamp == timeStamp && m_ChangeOrigin == d_ptr->m_ChangeOrigin && m_ChangeSpacing == d_ptr->m_ChangeSpacing &&
     m_ConvertToGrayScale == d_ptr->m_ConvertToGrayScale && m_Origin == d_ptr->m_Origin && m_Spacing == d_ptr->m_Spacing && m_ColorWeights == d_ptr->m_ColorWeights)
  {
    // We are reading from the cache, so set the FileWasRead flag to false
    m_FileWasRead = false;
    exportDocument = getRoot();
  }
  else
  {
    flushCache();
    // We are reading from the file, so set the FileWasRead flag to true
    m_FileWasRead = true;

    if(!domDocument.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn))
    {
      QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
      setErrorCondition(-70000, ss);
      return;
    }

    exportDocument = domDocument.documentElement();

    if(exportDocument.tagName() != Zeiss::ZenXml::ExportDocument)
    {
      QString ss = QObject::tr("Could not find the <ExportDocument> element. Abort Parsing. Is the file a Zeiss Zen Export XML file");
      setErrorCondition(-70001, ss);
      return;
    }
    setRoot(exportDocument);
    setInputFile_Cache(getInputFile());
    setTimeStamp_Cache(timeStamp);
    d_ptr->m_ChangeOrigin = m_ChangeOrigin;
    d_ptr->m_ChangeSpacing = m_ChangeSpacing;
    d_ptr->m_ConvertToGrayScale = m_ConvertToGrayScale;
    d_ptr->m_Origin = m_Origin;
    d_ptr->m_Spacing = m_Spacing;
    d_ptr->m_ColorWeights = m_ColorWeights;

    generateCache(exportDocument);
  }

  generateDataStructure();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::preflight()
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
  return d_ptr->m_MontageInformation;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportZenInfoMontage::flushCache()
{
  setInputFile_Cache("");
  setTimeStamp_Cache(QDateTime());
  setRoot(QDomElement());
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
  std::vector<ImageGeom::Pointer> geometries;

  bool ok = false;
  for(int32_t i = 0; i < imageList.count(); i++)
  {
    QDomElement imageElement = imageList.at(i).toElement();

    QDomElement filenameElement = imageElement.firstChildElement(Zeiss::ZenXml::Filename).toElement();
    QString fileName = filenameElement.text();
    QDomElement boundsElement = imageElement.firstChildElement(Zeiss::ZenXml::Bounds).toElement();

    BoundsType bound;
    bound.Filename = fileName;
    bound.StartX = boundsElement.attribute(Zeiss::ZenXml::StartX).toInt(&ok);
    xValuesSet.insert(bound.StartX);
    bound.SizeX = boundsElement.attribute(Zeiss::ZenXml::SizeX).toInt(&ok);

    bound.StartY = boundsElement.attribute(Zeiss::ZenXml::StartY).toInt(&ok);
    yValuesSet.insert(bound.StartY);
    bound.SizeY = boundsElement.attribute(Zeiss::ZenXml::SizeY).toInt(&ok);

    bound.StartC = boundsElement.attribute(Zeiss::ZenXml::StartC).toInt(&ok);
    bound.StartS = boundsElement.attribute(Zeiss::ZenXml::StartS).toInt(&ok);
    bound.StartB = boundsElement.attribute(Zeiss::ZenXml::StartB).toInt(&ok);
    bound.StartM = boundsElement.attribute(Zeiss::ZenXml::StartM).toInt(&ok);
    bounds[i] = bound;
  }

  // Sort the bounds so that they are in raster order with X moving fastest.
  std::sort(bounds.begin(), bounds.end(), compareBound);

  // Figure out the Row/Col index for each bound object
  int32_t curCol = 0;
  int32_t curRow = 0;

  m_ColumnCount = xValuesSet.size();
  m_RowCount = yValuesSet.size();

  // Set the Row and Column values into each BoundType object
  for(auto& bound : bounds)
  {
    bound.Col = curCol;
    bound.Row = curRow;
    curCol++;
    if(curCol == m_ColumnCount)
    {
      curCol = 0;
      curRow++;
    }

    // Get the meta information from disk
    DataArrayPath dap(::k_DCName, ::k_AMName, ::k_AAName);
    AbstractFilter::Pointer imageImportFilter = createImageImportFiler(bound.Filename, dap);
    if(nullptr == imageImportFilter.get())
    {
      continue;
    }
    imageImportFilter->preflight();
    if(imageImportFilter->getErrorCode() < 0)
    {
      setErrorCondition(imageImportFilter->getErrorCode(), "Error Preflighting Image Import Filter.");
      continue;
    }

    DataContainerArray::Pointer importImageDca = imageImportFilter->getDataContainerArray();
    {
      DataContainer::Pointer fromDca = importImageDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer fromCellAttrMat = fromDca->getAttributeMatrix(::k_AMName);
      IDataArray::Pointer fromImageData = fromCellAttrMat->getAttributeArray(::k_AAName);

      // Get the spacing from the geometry
      ImageGeom::Pointer fromImageGeom = fromDca->getGeometryAs<ImageGeom>();
      FloatVec3Type spacing;
      fromImageGeom->getSpacing(spacing);

      bound.SpacingX = spacing[0];
      bound.SpacingY = spacing[1];
      bound.ImageDataProxy = fromImageData;
      minSpacing[0] = spacing[0];
      minSpacing[1] = spacing[1];
      minSpacing[2] = spacing[2];

      FloatVec3Type origin;
      fromImageGeom->getOrigin(origin);
      minCoord[0] = std::min(origin[0], minCoord[0]);
      minCoord[1] = std::min(origin[1], minCoord[1]);
      minCoord[2] = 0.0f;

      geometries.emplace_back(fromImageGeom);
    }

    if(getConvertToGrayScale())
    {
      DataArrayPath daPath(::k_DCName, ::k_AMName, k_AAName);
      AbstractFilter::Pointer grayScaleFilter = createColorToGrayScaleFilter(daPath);
      grayScaleFilter->setDataContainerArray(importImageDca);
      grayScaleFilter->preflight();
      if(grayScaleFilter->getErrorCode() < 0)
      {
        setErrorCondition(grayScaleFilter->getErrorCode(), "Error Preflighting Color to GrayScale filter");
        continue;
      }

      DataContainerArray::Pointer colorToGrayDca = grayScaleFilter->getDataContainerArray();
      DataContainer::Pointer fromDca = colorToGrayDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer fromCellAttrMat = fromDca->getAttributeMatrix(::k_AMName);
      // Remove the RGB Attribute Array so we can rename the gray scale AttributeArray
      IDataArray::Pointer rgbImageArray = fromCellAttrMat->removeAttributeArray(::k_AAName);
      QString grayScaleArrayName = k_GrayScaleTempArrayName + ::k_AAName;
      IDataArray::Pointer fromGrayScaleData = fromCellAttrMat->removeAttributeArray(grayScaleArrayName);
      fromGrayScaleData->setName(getImageDataArrayName());
      bound.ImageDataProxy = fromGrayScaleData;
    }
  }

  QString montageInfo;
  QTextStream ss(&montageInfo);
  ss << "Columns=" << m_ColumnCount << "  Rows=" << m_RowCount << "  Num. Images=" << bounds.size();

  FloatVec3Type overrideOrigin = minCoord;
  FloatVec3Type overrideSpacing = minSpacing;

  // Now adjust the origin/spacing if needed
  if(getChangeOrigin() || getChangeSpacing())
  {

    if(getChangeOrigin())
    {
      overrideOrigin = {m_Origin[0], m_Origin[1], m_Origin[2]};
    }
    if(getChangeSpacing())
    {
      overrideSpacing = {m_Spacing[0], m_Spacing[1], m_Spacing[2]};
    }

    for(const auto& image : geometries)
    {
      FloatVec3Type currentOrigin;
      image->getOrigin(currentOrigin);

      FloatVec3Type currentSpacing;
      image->getSpacing(currentSpacing);

      for(size_t i = 0; i < 3; i++)
      {
        float delta = currentOrigin[i] - minCoord[i];
        // Convert to Pixel Coords
        delta = delta / currentSpacing[i];
        // Convert to the override origin
        delta = delta * overrideSpacing[i];
        currentOrigin[i] = overrideOrigin[i] + delta;
      }
      image->setOrigin(currentOrigin.data());
      image->setSpacing(overrideSpacing.data());
    }
  }
  ss << "\nOrigin: " << overrideOrigin[0] << ", " << overrideOrigin[1] << ", " << overrideOrigin[2];
  ss << "  Spacing: " << overrideSpacing[0] << ", " << overrideSpacing[1] << ", " << overrideSpacing[2];
  d_ptr->m_MontageInformation = montageInfo;

  setBoundsCache(bounds);
}

// -----------------------------------------------------------------------------
void ImportZenInfoMontage::generateDataStructure()
{
  std::vector<BoundsType>& bounds = d_ptr->m_BoundsCache;

  DataContainerArray::Pointer dca = getDataContainerArray();

  // int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(bounds.size());
  int32_t rowCountPadding = MetaXmlUtils::CalculatePaddingDigits(m_RowCount);
  int32_t colCountPadding = MetaXmlUtils::CalculatePaddingDigits(m_ColumnCount);
  int charPaddingCount = std::max(rowCountPadding, colCountPadding);

  for(const auto& bound : bounds)
  {
    // Create our DataContainer Name using a Prefix and a rXXcYY format.
    QString dcName = getDataContainerName().getDataContainerName();
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
    SizeVec3Type dims(bound.SizeX, bound.SizeY, 1);
    image->setDimensions(dims);
    FloatVec3Type origin(bound.StartX, bound.StartY, 0.0f);
    image->setOrigin(origin);
    FloatVec3Type spacing(bound.SpacingX, bound.SpacingY, 1.0);
    image->setSpacing(spacing);

    dc->setGeometry(image);

    std::vector<size_t> tDims = {dims[0], dims[1], dims[2]};
    // Create the Cell Attribute Matrix into which the image data would be read
    AttributeMatrix::Pointer cellAttrMat = AttributeMatrix::Create(tDims, getCellAttributeMatrixName(), AttributeMatrix::Type::Cell);
    dc->addOrReplaceAttributeMatrix(cellAttrMat);
    cellAttrMat->addOrReplaceAttributeArray(bound.ImageDataProxy);
  }
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
    QString msg;
    QTextStream out(&msg);
    out << "Importing " << bound.Filename;
    notifyStatusMessage(msg);
    // Create our DataContainer Name using a Prefix and a rXXcYY format.
    QString dcName = getDataContainerName().getDataContainerName();
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
    SizeVec3Type dims;
    image->getDimensions(dims);
    FloatVec3Type origin;
    image->getOrigin(origin);
    FloatVec3Type spacing;
    image->getSpacing(spacing);

    std::vector<size_t> tDims = {dims[0], dims[1], dims[2]};
    // The Cell AttributeMatrix is also already created at this point
    AttributeMatrix::Pointer cellAttrMat = dc->getAttributeMatrix(getCellAttributeMatrixName());
    // Instantiate the Image Import Filter to actually read the image into a data array
    DataArrayPath dap(::k_DCName, ::k_AMName, getImageDataArrayName()); // This is just a temp path for the subfilter to use
    AbstractFilter::Pointer imageImportFilter = createImageImportFiler(bound.Filename, dap);
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
    {
      DataContainer::Pointer fromDca = importImageDca->getDataContainer(::k_DCName);
      AttributeMatrix::Pointer fromCellAttrMat = fromDca->getAttributeMatrix(::k_AMName);
      IDataArray::Pointer fromImageData = fromCellAttrMat->getAttributeArray(getImageDataArrayName());

      if(getConvertToGrayScale())
      {
        DataArrayPath daPath(::k_DCName, k_AMName, getImageDataArrayName());
        AbstractFilter::Pointer grayScaleFilter = createColorToGrayScaleFilter(daPath);
        grayScaleFilter->setDataContainerArray(importImageDca); // Use the Data COntainer array that was use for the import. It is setup and ready to go
        grayScaleFilter->execute();
        if(grayScaleFilter->getErrorCode() < 0)
        {
          setErrorCondition(grayScaleFilter->getErrorCode(), "Error Executing Color to GrayScale filter");
          continue;
        }

        DataContainer::Pointer fromDca = importImageDca->getDataContainer(::k_DCName);
        AttributeMatrix::Pointer fromCellAttrMat = fromDca->getAttributeMatrix(::k_AMName);

        QString grayScaleArrayName = k_GrayScaleTempArrayName + ::k_AAName;
        IDataArray::Pointer fromGrayScaleData = fromCellAttrMat->getAttributeArray(grayScaleArrayName);

        IDataArray::Pointer rgbImageArray = fromCellAttrMat->removeAttributeArray(::k_AAName);
        IDataArray::Pointer gray = fromCellAttrMat->removeAttributeArray(k_GrayScaleTempArrayName + getImageDataArrayName());
        gray->setName(getImageDataArrayName());
        dc->getAttributeMatrix(getCellAttributeMatrixName())->addOrReplaceAttributeArray(gray);
      }
      else
      {
        // Copy the IDataArray (which contains the image data) from the temp data container array into our persistent data structure
        dc->getAttributeMatrix(getCellAttributeMatrixName())->addOrReplaceAttributeArray(fromImageData);
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ImportZenInfoMontage::createImageImportFiler(const QString& imageFileName, const DataArrayPath& daPath)
{

  QFileInfo fi = QFileInfo(getInputFile());
  QString imagePath = fi.absoluteDir().path() + "/" + imageFileName;

  QString filtName = ZeissImportConstants::ImageProcessingFilters::k_ReadImageFilterClassName;
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
  if(nullptr == filterFactory.get())
  {
    QString ss = QObject::tr("Error trying to instantiate the '%1' filter which is typically included in the 'ITKImageProcessing' plugin.")
                     .arg(ZeissImportConstants::ImageProcessingFilters::k_ReadImageFilterClassName);
    setErrorCondition(-70019, ss);
    return AbstractFilter::NullPointer();
  }

  // If we get this far, the Factory is good so creating the filter should not fail unless something has
  // horribly gone wrong in which case the system is going to come down quickly after this.
  AbstractFilter::Pointer filter = filterFactory->create();

  // Connect up the Error/Warning/Progress object so the filter can report those things
  connect(filter.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), this, SIGNAL(messageGenerated(const AbstractMessage::Pointer&)));
  DataContainerArrayShPtr dca = DataContainerArray::New();

  filter->setDataContainerArray(dca); // AbstractFilter implements this so no problem

  // Add to filename list
  m_FilenameList.push_back(imagePath);

  bool propWasSet = filter->setProperty("FileName", imagePath);
  if(!propWasSet)
  {
    QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a "
                             "Q_PROPERTY macro. Please notify the developers.")
                     .arg("InputFileName", filtName, getHumanLabel());
    setErrorCondition(-70015, ss);
  }
  QVariant var;
  var.setValue(daPath);
  propWasSet = filter->setProperty("DataContainerName", var);
  if(!propWasSet)
  {
    QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a "
                             "Q_PROPERTY macro. Please notify the developers.")
                     .arg("DataContainerName", filtName, getHumanLabel());
    setErrorCondition(-70016, ss);
  }

  propWasSet = filter->setProperty("CellAttributeMatrixName", daPath.getAttributeMatrixName());
  if(!propWasSet)
  {
    QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a "
                             "Q_PROPERTY macro. Please notify the developers.")
                     .arg("CellAttributeMatrixName", filtName, getHumanLabel());
    setErrorCondition(-70017, ss);
  }

  propWasSet = filter->setProperty("ImageDataArrayName", daPath.getDataArrayName());
  if(!propWasSet)
  {
    QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a "
                             "Q_PROPERTY macro. Please notify the developers.")
                     .arg("ImageDataArrayName", filtName, getHumanLabel());
    setErrorCondition(-70018, ss);
  }

  return filter;
}

// -----------------------------------------------------------------------------
AbstractFilter::Pointer ImportZenInfoMontage::createColorToGrayScaleFilter(const DataArrayPath& daPath)
{
  ConvertColorToGrayScale::Pointer filter = ConvertColorToGrayScale::New();

  // Connect up the Error/Warning/Progress object so the filter can report those things
  connect(filter.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), this, SIGNAL(messageGenerated(const AbstractMessage::Pointer&)));

  filter->setDataContainerArray(getDataContainerArray());
  filter->setConversionAlgorithm(0);
  filter->setColorWeights(getColorWeights());
  QVector<DataArrayPath> inputDataArrayVector = {daPath};
  filter->setInputDataArrayVector(inputDataArrayVector);
  filter->setCreateNewAttributeMatrix(false);
  filter->setOutputAttributeMatrixName(getCellAttributeMatrixName());
  filter->setOutputArrayPrefix(k_GrayScaleTempArrayName);

  return filter;
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
const QString ImportZenInfoMontage::getCompiledLibraryName() const
{
  return ZeissImportConstants::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportZenInfoMontage::getBrandingString() const
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportZenInfoMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ZeissImport::Version::Major() << "." << ZeissImport::Version::Minor() << "." << ZeissImport::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportZenInfoMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportZenInfoMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::ImportFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportZenInfoMontage::getHumanLabel() const
{
  return "Zeiss Zen Info Import";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ImportZenInfoMontage::getUuid()
{
  return QUuid("{774ec947-eed6-5eb2-a01b-ee6470e61521}");
}
