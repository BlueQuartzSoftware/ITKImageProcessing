/*
 * Your License or Copyright Information can go here
 */

#include "ITKImportRoboMetMontage.h"

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
ITKImportRoboMetMontage::ITKImportRoboMetMontage()
: m_DataContainerPrefix(SIMPL::Defaults::ImageDataContainerName + "_")
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_RegistrationFile("")
, m_AttributeArrayName("ImageTile")
{
  m_Origin.x = 0.0;
  m_Origin.y = 0.0;
  m_Origin.z = 0.0;

  m_Resolution.x = 1.0;
  m_Resolution.y = 1.0;
  m_Resolution.z = 1.0;

  m_NumImages = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportRoboMetMontage::~ITKImportRoboMetMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);

  m_NumImages = 0;
  m_RegisteredFilePaths.clear();
  m_Coords.clear();
  m_RowColId.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setupFilterParameters()
{
  QVector<FilterParameter::Pointer> parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Registration File", RegistrationFile, FilterParameter::Parameter, ITKImportRoboMetMontage, "", "*.txt"));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Resolution", Resolution, FilterParameter::Parameter, ITKImportRoboMetMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container Prefix", DataContainerPrefix, FilterParameter::CreatedArray, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Array Name", AttributeArrayName, FilterParameter::CreatedArray, ITKImportRoboMetMontage));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  initialize();

  QString ss;

  QFileInfo fi(m_RegistrationFile);
  if(!fi.exists())
  {
    QString ss = QObject::tr("The registration file does not exist");
    setErrorCondition(-2001);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if(m_RegistrationFile.isEmpty())
  {
    QString ss = QObject::tr("The registration file must be set").arg(getHumanLabel());
    setErrorCondition(-2002);
    notifyErrorMessage(getHumanLabel(), ss, -1);
    return;
  }

  int32_t err = 0;
  if (fi.completeSuffix() != "csv")
  {
    QString ss = QObject::tr("Config file is not in RoboMet format (*.csv).");
    setErrorCondition(-2003);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  // Parse Fiji Config File
  err = parseRoboMetConfigFile(getRegistrationFile());
  if(err < 0)
  {
    return;
  }

  QVector<size_t> cDims(1, 1);

  for(int i = 0; i < m_RegisteredFilePaths.size(); i++)
  {
    if(getCancel())
    {
      return;
    }

    QString registeredFilePath = m_RegisteredFilePaths[i];

    QFileInfo fi(registeredFilePath);
    if (fi.isRelative())
    {
      QString configFilePath = getRegistrationFile();
      QFileInfo configFi(configFilePath);
      fi.setFile(configFi.path() + QDir::separator() + registeredFilePath);
    }

    if(!fi.exists())
    {
      continue;
    }

    QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(m_RowColId[i]);

    DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, dcName);
    if(getErrorCondition() < 0)
    {
      return;
    }
    ImageGeom::Pointer geom = ImageGeom::New();
    geom->initializeWithZeros();
    m->setGeometry(geom);

    setFileName(fi.filePath());
    DataArrayPath dap(dcName, getCellAttributeMatrixName(), getAttributeArrayName());
    readImage(dap, getInPreflight());
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ITKImportRoboMetMontage::parseRoboMetConfigFile(const QString &filePath)
{
  int32_t err = 0;



  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::preflight()
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
void ITKImportRoboMetMontage::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  int fileCount = m_RegisteredFilePaths.size();
  for(int i = 0; i < fileCount; i++)
  {
    QString imageFName = m_RegisteredFilePaths[i];
    QFileInfo fi(imageFName);
    if(!fi.exists())
    {
      continue;
    }

    QString dcName = getDataContainerPrefix() + QString::number(i);

    DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, dcName);
    if(getErrorCondition() < 0)
    {
      return;
    }

    {
      ImageGeom::Pointer imageGeom = m->getGeometryAs<ImageGeom>();
      size_t dims[3] = {0, 0, 0};
      std::tie(dims[0], dims[1], dims[2]) = imageGeom->getDimensions();
      dims[2] = fileCount;
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
const QString ITKImportRoboMetMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKImportRoboMetMontage::getUuid()
{
  return QUuid("{cdb130af-3616-57b1-be59-fe18113b2621}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getHumanLabel() const
{
  return "Import RoboMet Montage";
}

#define ITK_IMAGE_READER_CLASS_NAME ITKImportRoboMetMontage

#include "SIMPLib/ITK/itkGetComponentsDimensions.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

#include "SIMPLib/ITK/itkImageReaderHelper.cpp"
