/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
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
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

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
  ITKImportFijiMontage::ImageReaderVector m_ImageReaderCache;
  ITKImportFijiMontage::TileDataVector m_TileDataCache;
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
SIMPL_PIMPL_PROPERTY_DEF(ITKImportFijiMontage, ITKImportFijiMontage::ImageReaderVector, ImageReaderCache)
SIMPL_PIMPL_PROPERTY_DEF(ITKImportFijiMontage, ITKImportFijiMontage::TileDataVector, TileDataCache)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::appendImageReaderToCache(const ITKImageReader::Pointer &reader)
{
  Q_D(ITKImportFijiMontage);
  d_ptr->m_ImageReaderCache.push_back(reader);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::appendImageTileToCache(const itk::FijiImageTileData &tileData)
{
  Q_D(ITKImportFijiMontage);
  d_ptr->m_TileDataCache.push_back(tileData);
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
    setImageReaderCache(ImageReaderVector());
    setTileDataCache(TileDataVector());

    // Parse Fiji Config File
    itk::FijiConfigurationFileReader fijiFileReader;
    itk::FijiFileData fijiFileData = fijiFileReader.parseFijiConfigFile(getFijiConfigFilePath());
    if(fijiFileReader.getErrorCode() < 0)
    {
      QString ss = fijiFileReader.getErrorMessage();
      int code = fijiFileReader.getErrorCode();
      notifyErrorMessage(getHumanLabel(), ss, code);
      return;
    }

    QVector<size_t> cDims(1, 1);

    for (int row = 0; row < fijiFileData.size(); row++)
    {
      itk::FijiRowData fijiRowData = fijiFileData[row];
      for (int col = 0; col < fijiRowData.size(); col++)
      {
        itk::FijiImageTileData fijiImageData = fijiRowData[col];

        if(getCancel())
        {
          return;
        }

        readImageFile(fijiImageData);
      }
    }

    // Set the new data into the cache
    setLastRead(QDateTime::currentDateTime());
    setFijiConfigFilePathCache(getFijiConfigFilePath());
  }
  else
  {
    ImageReaderVector imageReaderCache = getImageReaderCache();
    TileDataVector tileDataCache = getTileDataCache();
    for (int i = 0; i < imageReaderCache.size(); i++)
    {
      itk::FijiImageTileData imageTileData = tileDataCache[i];

      QString rowColIdString = tr("r%1c%2").arg(imageTileData.row).arg(imageTileData.col);
      QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(rowColIdString);

      ITKImageReader::Pointer reader = imageReaderCache[i];
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

        QPointF coords = imageTileData.coords;

        ImageGeom::Pointer geom = dc->getGeometryAs<ImageGeom>();

        geom->setOrigin(coords.x(), coords.y(), 1.0f);
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::readImageFile(itk::FijiImageTileData imageTileData)
{
  QString imageFilePath = imageTileData.filePath;
  QFileInfo fi(imageFilePath);
  QString rowColIdString = tr("r%1c%2").arg(imageTileData.row).arg(imageTileData.col);
  QPointF coords = imageTileData.coords;

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

  appendImageReaderToCache(reader);
  appendImageTileToCache(imageTileData);
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