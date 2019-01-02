/*
 * Your License or Copyright Information can go here
 */

#include "ITKImportFijiMontage.h"

#include <QtCore/QDir>
#include <QtCore/QDateTime>

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
  QString m_FijiConfigFilePathCache;
  QDateTime m_LastRead;
  ITKImportFijiMontage::ReaderMap m_ReaderCache;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportFijiMontagePrivate::ITKImportFijiMontagePrivate(ITKImportFijiMontage* ptr)
: q_ptr(ptr)
, m_FijiConfigFilePathCache("")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportFijiMontage::ITKImportFijiMontage()
: m_DataContainerPrefix(SIMPL::Defaults::ImageDataContainerName + "_")
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_FijiConfigFilePath("")
, m_AttributeArrayName("ImageTile")
, d_ptr(new ITKImportFijiMontagePrivate(this))
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
SIMPL_PIMPL_PROPERTY_DEF(ITKImportFijiMontage, QString, FijiConfigFilePathCache)
SIMPL_PIMPL_PROPERTY_DEF(ITKImportFijiMontage, QDateTime, LastRead)
SIMPL_PIMPL_GET_PROPERTY_DEF(ITKImportFijiMontage, ITKImportFijiMontage::ReaderMap, ReaderCache)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setReaderCacheValue(const QString &filePath, const ITKImageReader::Pointer &reader)
{
  Q_D(ITKImportFijiMontage);
  d->m_ReaderCache.insert(filePath, reader);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::clearReaderCache()
{
  Q_D(ITKImportFijiMontage);
  d->m_ReaderCache.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setupFilterParameters()
{
  QVector<FilterParameter::Pointer> parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Fiji Configuration File", FijiConfigFilePath, FilterParameter::Parameter, ITKImportFijiMontage, "", "*.txt"));

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

  QDateTime lastModified(fi.lastModified());

  // Only parse the fiji config file again if the cache is outdated
  if (!getInPreflight() || getFijiConfigFilePath() != getFijiConfigFilePathCache() || !getLastRead().isValid() || lastModified.msecsTo(getLastRead()) < 0)
  {
    clearReaderCache();
    clearParsingCache();

    // Parse Fiji Config File
    int32_t err = parseFijiConfigFile(getFijiConfigFilePath());
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

      readImageFile(registeredFilePath);
    }

    // Set the new data into the cache
    setLastRead(QDateTime::currentDateTime());
    setFijiConfigFilePathCache(getFijiConfigFilePath());
  }
  else
  {
    ReaderMap readerCache = getReaderCache();
    for (ReaderMap::iterator iter = readerCache.begin(); iter != readerCache.end(); iter++)
    {
      QString filePath = iter.key();

      QString rowColIdString = m_RowColIdMap[filePath];
      QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(rowColIdString);

      ITKImageReader::Pointer reader = iter.value();
      reader->setDataContainerName(dcName);
      reader->setCellAttributeMatrixName(getCellAttributeMatrixName());
      reader->setImageDataArrayName(getAttributeArrayName());
      reader->setDataContainerArray(DataContainerArray::New());
      if (getInPreflight())
      {
        reader->preflight();
      }
      else
      {
        reader->execute();
      }

      DataContainerArray::Pointer filterDca = reader->getDataContainerArray();
      QList<DataContainer::Pointer> dcs = filterDca->getDataContainers();
      for (DataContainer::Pointer dc : dcs)
      {
        getDataContainerArray()->addDataContainer(dc);

        QPointF coords = m_CoordsMap[filePath];

        ImageGeom::Pointer geom = dc->getGeometryAs<ImageGeom>();

        geom->setOrigin(coords.x(), coords.y(), 1.0f);
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::readImageFile(const QString &filePath)
{
  QFileInfo fi(filePath);
  QString rowColIdString = m_RowColIdMap[filePath];
  QPointF coords = m_CoordsMap[filePath];

  QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(rowColIdString);

  ITKImageReader::Pointer reader = ITKImageReader::New();
  reader->setFileName(fi.filePath());
  reader->setDataContainerName(dcName);
  reader->setCellAttributeMatrixName(getCellAttributeMatrixName());
  reader->setImageDataArrayName(getAttributeArrayName());

  if (getInPreflight())
  {
    reader->preflight();
  }
  else
  {
    reader->execute();
  }

  DataContainerArray::Pointer filterDca = reader->getDataContainerArray();
  QList<DataContainer::Pointer> dcs = filterDca->getDataContainers();
  for (DataContainer::Pointer dc : dcs)
  {
    getDataContainerArray()->addDataContainer(dc);
  }

  DataContainer::Pointer m = getDataContainerArray()->getPrereqDataContainer<AbstractFilter>(this, dcName);
  if(getErrorCondition() < 0)
  {
    return;
  }

  ImageGeom::Pointer geom = m->getGeometryAs<ImageGeom>();
  geom->setOrigin(coords.x(), coords.y(), 1.0f);

  setReaderCacheValue(filePath, reader);
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

      QString filePath = QString::fromStdString(tile.FileName);
      QFileInfo fi(filePath);
      if (fi.isRelative())
      {
        QFileInfo configFi(getFijiConfigFilePath());
        filePath = configFi.path() + QDir::separator() + filePath;
      }

      m_RegisteredFilePaths.push_back(filePath);

      uint32_t coordsSize = tile.Position.GetPointDimension();
      if (coordsSize != 2)
      {
        QString ss = QObject::tr("The dimension size of all tiles in the fiji config file must be equal to 2.");
        setErrorCondition(-2004);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        m_RegisteredFilePaths.clear();
        m_CoordsMap.clear();
        m_RowColIdMap.clear();
        m_NumImages = 0;
        return -1;
      }

      m_CoordsMap.insert(filePath, QPointF(tile.Position[0], tile.Position[1]));
      m_RowColIdMap.insert(filePath, tr("r%1c%2").arg(i).arg(j));
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
void ITKImportFijiMontage::clearParsingCache()
{
  m_NumImages = 0;
  m_RegisteredFilePaths.clear();
  m_CoordsMap.clear();
  m_RowColIdMap.clear();
}
