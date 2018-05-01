/*
 * Your License or Copyright Information can go here
 */

#include "ImportImageMontage.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

// Include the factory
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/Common/TemplateHelpers.hpp"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportImageMontage::ImportImageMontage()
: m_DataContainerName(SIMPL::Defaults::ImageDataContainerName)
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_MetaDataAttributeMatrixName("MetaDataAttributeMatrix")
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
ImportImageMontage::~ImportImageMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::setupFilterParameters()
{
  QVector<FilterParameter::Pointer> parameters;
  parameters.push_back(SIMPL_NEW_FILELISTINFO_FP("Input File List", InputFileListInfo, FilterParameter::Parameter, ImportImageMontage));

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ImportImageMontage));

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Resolution", Resolution, FilterParameter::Parameter, ImportImageMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container", DataContainerName, FilterParameter::CreatedArray, ImportImageMontage));

  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));

  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ImportImageMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Meta Data Attribute Matrix", MetaDataAttributeMatrixName, FilterParameter::CreatedArray, ImportImageMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setDataContainerName(reader->readString("DataContainerName", getDataContainerName()));
  setCellAttributeMatrixName(reader->readString("CellAttributeMatrixName", getCellAttributeMatrixName()));
  setMetaDataAttributeMatrixName(reader->readString("MetaDataAttributeMatrixName", getMetaDataAttributeMatrixName()));
  setInputFileListInfo(reader->readFileListInfo("InputFileListInfo", getInputFileListInfo()));
  setOrigin(reader->readFloatVec3("Origin", getOrigin()));
  setResolution(reader->readFloatVec3("Resolution", getResolution()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::initialize()
{
  m_AttributeArrayNamesPtr = StringDataArray::NullPointer();
  if(m_InStream.isOpen())
    m_InStream.close();
  m_NumImages = 0;
  m_ArrayNames.clear();
  m_Coords.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportImageMontage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  initialize();

  QString ss;

  if(m_InputFileListInfo.InputPath.isEmpty() == true)
  {
    ss = QObject::tr("The input directory must be set");
    setErrorCondition(-13);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, getDataContainerName());
  if(getErrorCondition() < 0)
  {
    return;
  }

  ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
  m->setGeometry(image);

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
  if(fileList.size() == 0)
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
    mdAttrMat->setTupleDimensions(QVector<size_t>(1, availableFileCount));

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
void ImportImageMontage::preflight()
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
void ImportImageMontage::execute()
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
  if(fileList.size() == 0)
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
AbstractFilter::Pointer ImportImageMontage::newFilterInstance(bool copyFilterParameters) const
{
  ImportImageMontage::Pointer filter = ImportImageMontage::New();
  if(true == copyFilterParameters)
  {
    filter->setFilterParameters(getFilterParameters());
    // We are going to hand copy all of the parameters because the other way of copying the parameters are going to
    // miss some of them because we are not enumerating all of them.
    SIMPL_COPY_INSTANCEVAR(DataContainerName)
    SIMPL_COPY_INSTANCEVAR(CellAttributeMatrixName)
    SIMPL_COPY_INSTANCEVAR(Resolution)
    SIMPL_COPY_INSTANCEVAR(Origin)
    SIMPL_COPY_INSTANCEVAR(InputFileListInfo)
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportImageMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportImageMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportImageMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportImageMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ImportImageMontage::getUuid()
{
  return QUuid("{40a1e653-67ed-5106-b45c-179c29e422bb}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportImageMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportImageMontage::getHumanLabel() const
{
  return "Import Image Montage";
}

#define ITK_IMAGE_READER_CLASS_NAME ImportImageMontage

#include "SIMPLib/ITK/itkGetComponentsDimensions.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

#include "SIMPLib/ITK/itkImageReaderHelper.cpp"
