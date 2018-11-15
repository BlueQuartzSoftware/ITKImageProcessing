/*
 * Your License or Copyright Information can go here
 */

#include "ImportRegisteredImageMontage.h"

#include <QtCore/QDir>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/itkReadImageImpl.hpp"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportRegisteredImageMontage::ImportRegisteredImageMontage()
: m_DataContainerName(SIMPL::Defaults::ImageDataContainerName)
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_MetaDataAttributeMatrixName("MetaDataAttributeMatrix")
, m_RegistrationFile("")
, m_RegistrationCoordinatesArrayName("RegistrationCoordinates")
, m_AttributeArrayNamesArrayName("AttributeArrayNames")
, m_RegistrationCoordinates(nullptr)
{
  m_Origin.x = 0.0;
  m_Origin.y = 0.0;
  m_Origin.z = 0.0;

  m_Resolution.x = 1.0;
  m_Resolution.y = 1.0;
  m_Resolution.z = 1.0;

  m_InputFileListInfo.FileExtension = QString("tif");
  m_InputFileListInfo.StartIndex = 0;
  m_InputFileListInfo.EndIndex = 0;
  m_InputFileListInfo.PaddingDigits = 0;

  m_NumImages = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportRegisteredImageMontage::~ImportRegisteredImageMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRegisteredImageMontage::setupFilterParameters()
{
  QVector<FilterParameter::Pointer> parameters;
  parameters.push_back(SIMPL_NEW_FILELISTINFO_FP("Input File List", InputFileListInfo, FilterParameter::Parameter, ImportRegisteredImageMontage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ImportRegisteredImageMontage));

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Resolution", Resolution, FilterParameter::Parameter, ImportRegisteredImageMontage));

  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Registration File", RegistrationFile, FilterParameter::Parameter, ImportRegisteredImageMontage, "", "*.txt"));
  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container", DataContainerName, FilterParameter::CreatedArray, ImportRegisteredImageMontage));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ImportRegisteredImageMontage));
  parameters.push_back(SeparatorFilterParameter::New("Meta Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Meta Data Attribute Matrix", MetaDataAttributeMatrixName, FilterParameter::CreatedArray, ImportRegisteredImageMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Registration Coordinates", RegistrationCoordinatesArrayName, FilterParameter::CreatedArray, ImportRegisteredImageMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Array Names", AttributeArrayNamesArrayName, FilterParameter::CreatedArray, ImportRegisteredImageMontage));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRegisteredImageMontage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setDataContainerName(reader->readString("DataContainerName", getDataContainerName()));
  setCellAttributeMatrixName(reader->readString("CellAttributeMatrixName", getCellAttributeMatrixName()));
  setMetaDataAttributeMatrixName(reader->readString("MetaDataAttributeMatrixName", getMetaDataAttributeMatrixName()));
  setRegistrationCoordinatesArrayName(reader->readString("RegistrationCoordinatesArrayName", getRegistrationCoordinatesArrayName()));
  setAttributeArrayNamesArrayName(reader->readString("AttributeArrayNamesArrayName", getAttributeArrayNamesArrayName()));
  setInputFileListInfo(reader->readFileListInfo("InputFileListInfo", getInputFileListInfo()));
  setOrigin(reader->readFloatVec3("Origin", getOrigin()));
  setResolution(reader->readFloatVec3("Resolution", getResolution()));
  setRegistrationFile(reader->readString("RegistrationFile", getRegistrationFile()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ImportRegisteredImageMontage::parseRegistrationFile(QFile& reader)
{
  reader.reset();
  int32_t err = 0;
  QByteArray buf;
  QList<QByteArray> tokens;
  QList<QByteArray> files;
  bool ok = false;
  QString word("");
  QString xCoord;
  QString yCoord;
  int32_t lineNum = 0;

  // TODO: modify this reader to use caching
  while(!reader.atEnd())
  {
    buf = reader.readLine();
    buf = buf.trimmed();
    buf = buf.simplified();
    tokens = buf.split(' ');

    // If the buffer is split by a ';' and ends in a ')', then
    // we are probable actually reading a file name and its coordinates
    if(tokens.size() == 4)
    {
      if(tokens[1][0] == ';' && tokens[tokens.size() - 1][tokens[tokens.size() - 1].size() - 1] == ')')
      {
        lineNum++;
      }

      files = tokens[0].split('.');
      word = files[0];
      m_ArrayNames.push_back(word);

      yCoord = tokens[tokens.size() - 1];
      xCoord = tokens[tokens.size() - 2];
      yCoord.remove(')');
      xCoord.remove(',');
      xCoord.remove('(');
      m_Coords.push_back(xCoord.toFloat(&ok));
      m_Coords.push_back(yCoord.toFloat(&ok));
    }
  }

  m_NumImages = lineNum;

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRegisteredImageMontage::initialize()
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
void ImportRegisteredImageMontage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  initialize();

  QString ss;
  int32_t err = 0;

  if(m_InputFileListInfo.InputPath.isEmpty())
  {
    ss = QObject::tr("The input directory must be set");
    setErrorCondition(-13);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());

    return;
  }

  QFileInfo fi(m_RegistrationFile);
  if(!fi.exists())
  {
    QString ss = QObject::tr("The registration file does not exist");
    setErrorCondition(-388);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if(m_RegistrationFile.isEmpty())
  {
    QString ss = QObject::tr("The registration file must be set").arg(getHumanLabel());
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), ss, -1);
    return;
  }

  m_InStream.setFileName(getRegistrationFile());
  if(!m_InStream.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString ss = QObject::tr("Registration file could not be opened");
    setErrorCondition(-100);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, getDataContainerName());
  if(getErrorCondition() < 0)
  {
    return;
  }

  err = parseRegistrationFile(m_InStream);
  if(err < 0)
  {
    return;
  }

  if(getInPreflight())
  {
    m_InStream.close();
  }

  QVector<size_t> tDims(1, m_NumImages);
  QVector<size_t> cDims(1, 1);
  getDataContainerArray()->getDataContainer(getDataContainerName())->createNonPrereqAttributeMatrix(this, getMetaDataAttributeMatrixName(), tDims, AttributeMatrix::Type::MetaData);
  if(getErrorCondition() < 0)
  {
    return;
  }
  DataArrayPath path(getDataContainerName(), getMetaDataAttributeMatrixName(), getAttributeArrayNamesArrayName());
  AttributeMatrix::Pointer metaDataAttrMat = getDataContainerArray()->getAttributeMatrix(path);
  StringDataArray::Pointer attributeArrayNames = StringDataArray::CreateArray(metaDataAttrMat->getNumberOfTuples(), getAttributeArrayNamesArrayName());
  metaDataAttrMat->addAttributeArray(getAttributeArrayNamesArrayName(), attributeArrayNames);
  m_AttributeArrayNamesPtr = attributeArrayNames;
  if(getErrorCondition() < 0)
  {
    return;
  }

  cDims[0] = 2;
  path.update(getDataContainerName(), getMetaDataAttributeMatrixName(), getRegistrationCoordinatesArrayName());
  m_RegistrationCoordinatesPtr = getDataContainerArray()->createNonPrereqArrayFromPath<FloatArrayType, AbstractFilter, float>(this, path, 0, cDims);
  if(getErrorCondition() < 0)
  {
    return;
  }
  if(nullptr != m_RegistrationCoordinatesPtr.lock())
  {
    m_RegistrationCoordinates = m_RegistrationCoordinatesPtr.lock()->getPointer(0);
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
    setErrorCondition(-11);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
  else
  {
    setFileName(fileList[0]);
    QFileInfo fi(fileList[0]);
    DataArrayPath dap(getDataContainerName(), getCellAttributeMatrixName(), fi.baseName());
    readImage(dap, true);
    // The previous call will add an attribute array that we don't need at this point
    // so just remove it.
    AttributeMatrix::Pointer am = m->getAttributeMatrix(getCellAttributeMatrixName());
    am->removeAttributeArray(fi.baseName());

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
    if(availableFileCount != mdAttrMat->getNumberOfTuples())
    {
      QString ss = QObject::tr("The number of files in selected folder (%1) does not match the number in the registration file (%2)").arg(fileList.size()).arg(mdAttrMat->getNumberOfTuples());
      setErrorCondition(-101);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    QVector<size_t> cDims(1, 1);

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
      DataArrayPath path(getDataContainerName(), getCellAttributeMatrixName(), splitFilePaths[0]);
      getDataContainerArray()->createNonPrereqArrayFromPath<UInt8ArrayType, AbstractFilter, uint8_t>(this, path, 0, cDims);
      if(getErrorCondition() < 0)
      {
        return;
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRegisteredImageMontage::preflight()
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
void ImportRegisteredImageMontage::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(getDataContainerName());
  AttributeMatrix::Pointer attrMat = m->getAttributeMatrix(getCellAttributeMatrixName());

  for(int32_t i = 0; i < m_NumImages; i++)
  {
    m_AttributeArrayNamesPtr.lock()->setValue(i, m_ArrayNames[i]);
    m_RegistrationCoordinates[2 * i] = m_Coords[2 * i];
    m_RegistrationCoordinates[2 * i + 1] = m_Coords[2 * i + 1];
  }

  //qint64 z = m_InputFileListInfo.StartIndex;

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
    setErrorCondition(-11);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
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
    notifyStatusMessage(getMessagePrefix(), getHumanLabel(), ss);

    setFileName(imageFName);
    DataArrayPath dap(getDataContainerName(), getCellAttributeMatrixName(), splitFilePaths[0]);
    readImage(dap, false);
    if(getErrorCondition() < 0)
    {
      setErrorCondition(-14000);
      notifyErrorMessage(getHumanLabel(), "Failed to load image file", getErrorCondition());
      break;
    }

    // This is the first image so we need to create our block of data to store the data
    // if(z == m_InputFileListInfo.StartIndex)
    {
      ImageGeom::Pointer imageGeom = m->getGeometryAs<ImageGeom>();
      size_t dims[3] = {0, 0, 0};
      std::tie(dims[0], dims[1], dims[2]) = imageGeom->getDimensions();
      dims[2] = fileList.size();
      imageGeom->setDimensions(dims);
    }
    if(getCancel())
    {
      return;
    }
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ImportRegisteredImageMontage::newFilterInstance(bool copyFilterParameters) const
{
  ImportRegisteredImageMontage::Pointer filter = ImportRegisteredImageMontage::New();
  if(copyFilterParameters)
  {
    filter->setFilterParameters(getFilterParameters());
    // We are going to hand copy all of the parameters because the other way of copying the parameters are going to
    // miss some of them because we are not enumerating all of them.
    SIMPL_COPY_INSTANCEVAR(DataContainerName)
    SIMPL_COPY_INSTANCEVAR(CellAttributeMatrixName)
    SIMPL_COPY_INSTANCEVAR(MetaDataAttributeMatrixName)
    SIMPL_COPY_INSTANCEVAR(RegistrationCoordinatesArrayName)
    SIMPL_COPY_INSTANCEVAR(AttributeArrayNamesArrayName)
    SIMPL_COPY_INSTANCEVAR(Resolution)
    SIMPL_COPY_INSTANCEVAR(Origin)
    SIMPL_COPY_INSTANCEVAR(InputFileListInfo)
    SIMPL_COPY_INSTANCEVAR(RegistrationFile)
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ImportRegisteredImageMontage::getUuid()
{
  return QUuid("{5878723b-cc16-5486-ac5f-ff0107107e74}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getHumanLabel() const
{
  return "Import Registered Image Montage";
}

#define ITK_IMAGE_READER_CLASS_NAME ImportRegisteredImageMontage

#include "SIMPLib/ITK/itkGetComponentsDimensions.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

#include "SIMPLib/ITK/itkImageReaderHelper.cpp"
