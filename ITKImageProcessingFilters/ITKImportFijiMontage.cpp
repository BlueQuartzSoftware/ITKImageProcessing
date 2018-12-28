/*
 * Your License or Copyright Information can go here
 */

#include "ITKImportFijiMontage.h"

#include <QtCore/QDir>

#include "itkParseTileConfiguration.h"

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
ITKImportFijiMontage::ITKImportFijiMontage()
: m_DataContainerPrefix(SIMPL::Defaults::ImageDataContainerName + "_")
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_FijiConfigFilePath("")
, m_AttributeArrayName("ImageTile")
{
  m_NumImages = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportFijiMontage::~ITKImportFijiMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::initialize()
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
void ITKImportFijiMontage::setupFilterParameters()
{
  QVector<FilterParameter::Pointer> parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Fiji Configuration File", FijiConfigFilePath, FilterParameter::Parameter, ITKImportFijiMontage, "", "*.txt"));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Resolution", Resolution, FilterParameter::Parameter, ITKImportFijiMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container Prefix", DataContainerPrefix, FilterParameter::CreatedArray, ITKImportFijiMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImportFijiMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Array Name", AttributeArrayName, FilterParameter::CreatedArray, ITKImportFijiMontage));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  initialize();

  QString ss;

  if(getFijiConfigFilePath().isEmpty())
  {
    QString ss = QObject::tr("The fiji configuration file must be set").arg(getHumanLabel());
    setErrorCondition(-2000);
    notifyErrorMessage(getHumanLabel(), ss, -1);
    return;
  }

  QFileInfo fi(getFijiConfigFilePath());
  if (fi.completeSuffix() != "txt")
  {
    QString ss = QObject::tr("Configuration file is not in Fiji format (*.txt).");
    setErrorCondition(-2001);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if(!fi.exists())
  {
    QString ss = QObject::tr("The fiji configuration file does not exist");
    setErrorCondition(-2002);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  int32_t err = 0;

  // Parse Fiji Config File
  err = parseFijiConfigFile(getFijiConfigFilePath());
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
      QFileInfo configFi(getFijiConfigFilePath());
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
    FloatVec3_t res = getResolution();
    geom->setResolution(res.x, res.y, res.z);
    geom->setOrigin(m_Coords[i].x(), m_Coords[i].y(), 0.0f);
//    geom->setDimensions()
    m->setGeometry(geom);

    setFileName(fi.filePath());
    DataArrayPath dap(dcName, getCellAttributeMatrixName(), getAttributeArrayName());
    readImage(dap, getInPreflight());
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ITKImportFijiMontage::parseFijiConfigFile(const QString &filePath)
{
  int32_t err = 0;

  itk::TileLayout2D stageTiles = itk::ParseTileConfiguration2D(filePath.toStdString());
  for (int i = 0; i < stageTiles.size(); i++)
  {
    auto tile2D = stageTiles[i];
    for (int j = 0; j < tile2D.size(); j++)
    {
      auto tile = tile2D[j];
      m_RegisteredFilePaths.push_back(QString::fromStdString(tile.FileName));

      uint32_t coordsSize = tile.Position.GetPointDimension();
      if (coordsSize != 2)
      {
        QString ss = QObject::tr("The dimension size of all tiles in the fiji config file must be equal to 2.");
        setErrorCondition(-2004);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        m_RegisteredFilePaths.clear();
        m_Coords.clear();
        m_RowColId.clear();
        m_NumImages = 0;
        return -1;
      }

      m_Coords.push_back(QPointF(tile.Position[0], tile.Position[1]));
      m_RowColId.push_back(tr("r%1c%2").arg(i).arg(j));
      m_NumImages++;
    }
  }

  return err;
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
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
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
const QString ITKImportFijiMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportFijiMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportFijiMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportFijiMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKImportFijiMontage::getUuid()
{
  return QUuid("{5878723b-cc16-5486-ac5f-ff0107107e74}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportFijiMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportFijiMontage::getHumanLabel() const
{
  return "Import Fiji Montage";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void getDimensions(const QString &filePath)
{
//  ReaderType::Pointer reader = ReaderType::New();
//    reader->SetFileName( argv[1] );
//    reader->Update();

//    ImageType::Pointer image = reader->GetOutput();

//    ImageType::RegionType region = image->GetLargestPossibleRegion();

//    ImageType::SizeType size = region.GetSize();
}

#define ITK_IMAGE_READER_CLASS_NAME ITKImportFijiMontage

#include "SIMPLib/ITK/itkGetComponentsDimensions.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

#include "SIMPLib/ITK/itkImageReaderHelper.cpp"
