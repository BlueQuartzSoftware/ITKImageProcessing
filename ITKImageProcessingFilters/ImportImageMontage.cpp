/*
 * Your License or Copyright Information can go here
 */

#include "ImportImageMontage.h"

#include <QtCore/QFileInfo>

// Include the factory
#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

enum createdPathID : RenameDataPath::DataID_t
{
  AttributeMatrixID21 = 21,

  DataArrayID31 = 31,

  DataContainerID = 1
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportImageMontage::ImportImageMontage()
: m_DataContainerName(SIMPL::Defaults::ImageDataContainerName)
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_MetaDataAttributeMatrixName("MetaDataAttributeMatrix")
{
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;
  m_Origin[2] = 0.0;

  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  m_Spacing[2] = 1.0;

  m_InputFileListInfo.FileExtension = QString("tif");
  m_InputFileListInfo.StartIndex = 0;
  m_InputFileListInfo.EndIndex = 0;
  m_InputFileListInfo.PaddingDigits = 0;

  m_NumImages = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportImageMontage::~ImportImageMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_FILELISTINFO_FP("Input File List", InputFileListInfo, FilterParameter::Parameter, ImportImageMontage));

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ImportImageMontage));

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ImportImageMontage));
  parameters.back()->setLegacyPropertyName("Resolution");

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("Data Container", DataContainerName, FilterParameter::CreatedArray, ImportImageMontage));

  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));

  parameters.push_back(SIMPL_NEW_AM_WITH_LINKED_DC_FP("Cell Attribute Matrix", CellAttributeMatrixName, DataContainerName, FilterParameter::CreatedArray, ImportImageMontage));
  parameters.push_back(SIMPL_NEW_AM_WITH_LINKED_DC_FP("Meta Data Attribute Matrix", MetaDataAttributeMatrixName, DataContainerName, FilterParameter::CreatedArray, ImportImageMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setDataContainerName(reader->readDataArrayPath("DataContainerName", getDataContainerName()));
  setCellAttributeMatrixName(reader->readString("CellAttributeMatrixName", getCellAttributeMatrixName()));
  setMetaDataAttributeMatrixName(reader->readString("MetaDataAttributeMatrixName", getMetaDataAttributeMatrixName()));
  setInputFileListInfo(reader->readFileListInfo("InputFileListInfo", getInputFileListInfo()));
  setOrigin(reader->readFloatVec3("Origin", getOrigin()));
  setSpacing(reader->readFloatVec3("Spacing", getSpacing()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::initialize()
{
  m_AttributeArrayNamesPtr = StringDataArray::NullPointer();
  if(m_InStream.isOpen())
  {
    m_InStream.close();
  }
  m_NumImages = 0;
  m_ArrayNames.clear();
  m_Coords.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  QString ss;

  ss = QObject::tr("This filter is NOT supported or working.");
  setErrorCondition(-1, ss);
  return;
#if 0
  if(m_InputFileListInfo.InputPath.isEmpty())
  {
    ss = QObject::tr("The input directory must be set");
    setErrorCondition(-13, ss);
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer(this, getDataContainerName(), DataContainerID);
  if(getErrorCode() < 0)
  {
    return;
  }

  ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
  m->setGeometry(image);

  if(getInPreflight())
  {
    m_InStream.close();
  }

  std::vector<size_t> tDims(1, m_NumImages);
  std::vector<size_t> cDims(1, 1);
  getDataContainerArray()
      ->getDataContainer(getDataContainerName())
      ->createNonPrereqAttributeMatrix(this, getMetaDataAttributeMatrixName(), tDims, AttributeMatrix::Type::MetaData, AttributeMatrixID21);
  if(getErrorCode() < 0)
  {
    return;
  }

  bool hasMissingFiles = false;
  bool orderAscending = false;

  if(m_InputFileListInfo.Ordering == 0)
  {
    orderAscending = true;
  }
  else if(m_InputFileListInfo.Ordering == 1)
  {
    orderAscending = false;
  }

  // Now generate all the file names the user is asking for and populate the table
  QVector<QString> fileList = FilePathGenerator::GenerateFileList(m_InputFileListInfo.StartIndex, m_InputFileListInfo.EndIndex, m_InputFileListInfo.IncrementIndex, hasMissingFiles, orderAscending,
                                                                  m_InputFileListInfo.InputPath, m_InputFileListInfo.FilePrefix, m_InputFileListInfo.FileSuffix, m_InputFileListInfo.FileExtension,
                                                                  m_InputFileListInfo.PaddingDigits);
  if(fileList.empty())
  {
    QString ss = QObject::tr("No files have been selected for import. Have you set the input directory?");
    setErrorCondition(-11, ss);
  }
  else
  {
    setFileName(fileList[0]);
    QFileInfo fi(fileList[0]);
    DataArrayPath dap(getDataContainerName().getDataContainerName(), getCellAttributeMatrixName(), fi.baseName());
    readImage(dap, true);
    // The previous call will add an attribute array that we don't need at this point
    // so just remove it.
    //    AttributeMatrix::Pointer am = m->getAttributeMatrix(getCellAttributeMatrixName());
    //    am->removeAttributeArray(fi.baseName());

    AttributeMatrix::Pointer mdAttrMat = getDataContainerArray()->getDataContainer(getDataContainerName())->getAttributeMatrix(getMetaDataAttributeMatrixName());
    size_t availableFileCount = 0;
    for(QVector<QString>::iterator filepath = fileList.begin(); filepath != fileList.end(); ++filepath)
    {
      QString imageFName = *filepath;
      QFileInfo fi(imageFName);
      if(fi.exists())
      {
        availableFileCount++;
      }
    }
    mdAttrMat->setTupleDimensions(std::vector<size_t>(1, availableFileCount));

    std::vector<size_t> cDims(1, 1);

    for(QVector<QString>::iterator filepath = fileList.begin(); filepath != fileList.end(); ++filepath)
    {
      QString imageFName = *filepath;
      QFileInfo fi(imageFName);
      if(!fi.exists())
      {
        continue;
      }
      QStringList splitFilePaths = imageFName.split('/');
      QString fileName = splitFilePaths[splitFilePaths.size() - 1];
      splitFilePaths = fileName.split('.');
      DataArrayPath path(getDataContainerName().getDataContainerName(), getCellAttributeMatrixName(), splitFilePaths[0]);
      getDataContainerArray()->createNonPrereqArrayFromPath<UInt8ArrayType, AbstractFilter, uint8_t>(this, path, 0, cDims, "", DataArrayID31);
      if(getErrorCode() < 0)
      {
        return;
      }
    }
  }
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(getDataContainerName());
  AttributeMatrix::Pointer attrMat = m->getAttributeMatrix(getCellAttributeMatrixName());

  bool hasMissingFiles = false;
  bool orderAscending = false;

  if(m_InputFileListInfo.Ordering == 0)
  {
    orderAscending = true;
  }
  else if(m_InputFileListInfo.Ordering == 1)
  {
    orderAscending = false;
  }

  // Now generate all the file names the user is asking for and populate the table
  QVector<QString> fileList = FilePathGenerator::GenerateFileList(m_InputFileListInfo.StartIndex, m_InputFileListInfo.EndIndex, m_InputFileListInfo.IncrementIndex, hasMissingFiles, orderAscending,
                                                                  m_InputFileListInfo.InputPath, m_InputFileListInfo.FilePrefix, m_InputFileListInfo.FileSuffix, m_InputFileListInfo.FileExtension,
                                                                  m_InputFileListInfo.PaddingDigits);
  if(fileList.empty())
  {
    QString ss = QObject::tr("No files have been selected for import. Have you set the input directory?");
    setErrorCondition(-11, ss);
  }

  for(QVector<QString>::iterator filepath = fileList.begin(); filepath != fileList.end(); ++filepath)
  {
    QString imageFName = *filepath;
    QFileInfo fi(imageFName);
    if(!fi.exists())
    {
      continue;
    }
    QStringList splitFilePaths = imageFName.split('/');
    QString fileName = splitFilePaths[splitFilePaths.size() - 1];
    splitFilePaths = fileName.split('.');
    QString ss = QObject::tr("Importing file %1").arg(imageFName);
    notifyStatusMessage(ss);

    setFileName(imageFName);
    DataArrayPath dap(getDataContainerName().getDataContainerName(), getCellAttributeMatrixName(), splitFilePaths[0]);
    readImage(dap, false);
    if(getErrorCode() < 0)
    {
      setErrorCondition(-14000, "Failed to load image file");
      break;
    }

    if(getCancel())
    {
      return;
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ImportImageMontage::newFilterInstance(bool copyFilterParameters) const
{
  ImportImageMontage::Pointer filter = ImportImageMontage::New();
  if(copyFilterParameters)
  {
    filter->setFilterParameters(getFilterParameters());
    // We are going to hand copy all of the parameters because the other way of copying the parameters are going to
    // miss some of them because we are not enumerating all of them.
    filter->setDataContainerName(getDataContainerName());
    filter->setCellAttributeMatrixName(getCellAttributeMatrixName());
    filter->setSpacing(getSpacing());
    filter->setOrigin(getOrigin());
    filter->setInputFileListInfo(getInputFileListInfo());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportImageMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportImageMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportImageMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportImageMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ImportImageMontage::getUuid() const
{
  return QUuid("{40a1e653-67ed-5106-b45c-179c29e422bb}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportImageMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ImportImageMontage::getHumanLabel() const
{
  return "ITK::Import Image Montage";
}

#define ITK_IMAGE_READER_CLASS_NAME ImportImageMontage

#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/ITK/itkGetComponentsDimensions.h"
#include "SIMPLib/ITK/itkImageReaderHelper.cpp"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

// -----------------------------------------------------------------------------
ImportImageMontage::Pointer ImportImageMontage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ImportImageMontage> ImportImageMontage::New()
{
  struct make_shared_enabler : public ImportImageMontage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ImportImageMontage::getNameOfClass() const
{
  return QString("ImportImageMontage");
}

// -----------------------------------------------------------------------------
QString ImportImageMontage::ClassName()
{
  return QString("ImportImageMontage");
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setDataContainerName(const DataArrayPath& value)
{
  m_DataContainerName = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ImportImageMontage::getDataContainerName() const
{
  return m_DataContainerName;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setCellAttributeMatrixName(const QString& value)
{
  m_CellAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ImportImageMontage::getCellAttributeMatrixName() const
{
  return m_CellAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setMetaDataAttributeMatrixName(const QString& value)
{
  m_MetaDataAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ImportImageMontage::getMetaDataAttributeMatrixName() const
{
  return m_MetaDataAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setOrigin(const FloatVec3Type& value)
{
  m_Origin = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ImportImageMontage::getOrigin() const
{
  return m_Origin;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setSpacing(const FloatVec3Type& value)
{
  m_Spacing = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ImportImageMontage::getSpacing() const
{
  return m_Spacing;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setInputFileListInfo(const FileListInfo_t& value)
{
  m_InputFileListInfo = value;
}

// -----------------------------------------------------------------------------
FileListInfo_t ImportImageMontage::getInputFileListInfo() const
{
  return m_InputFileListInfo;
}

// -----------------------------------------------------------------------------
ImportImageMontage::Pointer ImportImageMontage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ImportImageMontage> ImportImageMontage::New()
{
  struct make_shared_enabler : public ImportImageMontage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ImportImageMontage::getNameOfClass() const
{
  return QString("ImportImageMontage");
}

// -----------------------------------------------------------------------------
QString ImportImageMontage::ClassName()
{
  return QString("ImportImageMontage");
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setDataContainerName(const DataArrayPath& value)
{
  m_DataContainerName = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ImportImageMontage::getDataContainerName() const
{
  return m_DataContainerName;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setCellAttributeMatrixName(const QString& value)
{
  m_CellAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ImportImageMontage::getCellAttributeMatrixName() const
{
  return m_CellAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setMetaDataAttributeMatrixName(const QString& value)
{
  m_MetaDataAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ImportImageMontage::getMetaDataAttributeMatrixName() const
{
  return m_MetaDataAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setOrigin(const FloatVec3Type& value)
{
  m_Origin = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ImportImageMontage::getOrigin() const
{
  return m_Origin;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setSpacing(const FloatVec3Type& value)
{
  m_Spacing = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ImportImageMontage::getSpacing() const
{
  return m_Spacing;
}

// -----------------------------------------------------------------------------
void ImportImageMontage::setInputFileListInfo(const FileListInfo_t& value)
{
  m_InputFileListInfo = value;
}

// -----------------------------------------------------------------------------
FileListInfo_t ImportImageMontage::getInputFileListInfo() const
{
  return m_InputFileListInfo;
}
