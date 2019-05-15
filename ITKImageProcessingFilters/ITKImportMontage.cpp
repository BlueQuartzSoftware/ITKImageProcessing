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

#include "ITKImportMontage.h"

#include <QtCore/QFileInfo>

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

namespace
{

const QString k_AttributeArrayNames("AttributeArrayNames");
const QString k_DataContaineNameDefaultName("Mosaic");
const QString k_TileAttributeMatrixDefaultName("Tile Data");
const QString k_TileDataArrayDefaultName("Image Data");
const QString k_GrayScaleTempArrayName("gray_scale_temp");
const QString k_AxioVisionMetaData("RoboMet MetaData");

} // namespace

/* ############## Start Private Implementation ############################### */
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImportMontagePrivate
{
  Q_DISABLE_COPY(ITKImportMontagePrivate)
  Q_DECLARE_PUBLIC(ITKImportMontage)
  ITKImportMontage* const q_ptr;
  ITKImportMontagePrivate(ITKImportMontage* ptr);
  ITKImportMontage::MontageCacheVector m_MontageCacheVector;
  FloatVec3Type montageMinCoord;
  FloatVec3Type montageSpacing;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportMontagePrivate::ITKImportMontagePrivate(ITKImportMontage* ptr)
: q_ptr(ptr)
{
  montageMinCoord = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportMontage::ITKImportMontage()
: m_DataContainerPrefix(::k_DataContaineNameDefaultName)
, m_CellAttributeMatrixName(::k_TileAttributeMatrixDefaultName)
, m_AttributeArrayName(::k_TileDataArrayDefaultName)
, m_ChangeOrigin(false)
, m_UsePixelCoordinates(false)
, m_ChangeSpacing(false)
, m_LengthUnit(static_cast<int32_t>(IGeometry::LengthUnit::Unspecified))
, d_ptr(new ITKImportMontagePrivate(this))
{
  // m_ColorWeights = FloatVec3Type(0.2125f, 0.7154f, 0.0721f);
  m_Origin = FloatVec3Type(0.0f, 0.0f, 0.0f);
  m_Spacing = FloatVec3Type(1.0f, 1.0f, 1.0f);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportMontage::~ITKImportMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SIMPL_PIMPL_PROPERTY_DEF(ITKImportMontage, ITKImportMontage::MontageCacheVector, MontageCacheVector)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportMontage::appendToCache(const ITKImageReader::Pointer &reader, const QString &filePath, QPointF coords, int row, int col, FloatVec3Type spacing)
{
  Q_D(ITKImportMontage);

  ITKMontageCache cache;
  cache.imageReader = reader;
  cache.filePath = filePath;
  cache.coords = coords;
  cache.row = row;
  cache.col = col;

  d->montageMinCoord[0] = std::min<float>(coords.x(), d->montageMinCoord[0]);
  d->montageMinCoord[1] = std::min<float>(coords.y(), d->montageMinCoord[1]);
  d->montageMinCoord[2] = 1.0f;

  d->montageSpacing = spacing;

  d->m_MontageCacheVector.push_back(cache);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportMontage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportMontage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportMontage::readImageFile(const QString &filePath, QPointF coords, int row, int col)
{
  QFileInfo fi(filePath);
  if(!fi.exists())
  {
    return;
  }
  QString rowColIdString = tr("r%1c%2").arg(row).arg(col);

  QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(rowColIdString);

  ITKImageReader::Pointer reader = ITKImageReader::New();
  reader->setFileName(fi.filePath());
  reader->setDataContainerName(DataArrayPath(dcName, "", ""));
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
  DataContainerArray::Container dcs = filterDca->getDataContainers();
  for (DataContainer::Pointer dc : dcs)
  {
    getDataContainerArray()->addOrReplaceDataContainer(dc);
  }

  DataContainer::Pointer m = getDataContainerArray()->getPrereqDataContainer<AbstractFilter>(this, dcName);
  if(getErrorCode() < 0)
  {
    return;
  }

  ImageGeom::Pointer geom = m->getGeometryAs<ImageGeom>();
  geom->setOrigin(coords.x(), coords.y(), 1.0f);

  geom->setUnits(static_cast<IGeometry::LengthUnit>(m_LengthUnit));

  FloatVec3Type spacing = geom->getSpacing();

  appendToCache(reader, filePath, coords, row, col, spacing);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportMontage::readImagesFromCache()
{
  MontageCacheVector montageCacheVector = getMontageCacheVector();
  for(const auto& montageCache : montageCacheVector)
  {
    QString rowColIdString = tr("r%1c%2").arg(montageCache.row).arg(montageCache.col);
    QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(rowColIdString);

    ITKImageReader::Pointer reader = montageCache.imageReader;
    reader->setDataContainerName(DataArrayPath(dcName, "", ""));
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
    DataContainerArray::Container dcs = filterDca->getDataContainers();
    for(const DataContainer::Pointer& dc : dcs)
    {
      getDataContainerArray()->addOrReplaceDataContainer(dc);

      QPointF coords = montageCache.coords;

      ImageGeom::Pointer geom = dc->getGeometryAs<ImageGeom>();

      geom->setOrigin(coords.x(), coords.y(), 1.0f);

      geom->setUnits(static_cast<IGeometry::LengthUnit>(m_LengthUnit));
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportMontage::adjustOriginAndSpacing()
{
  Q_D(ITKImportMontage);

  FloatVec3Type overrideOrigin = d->montageMinCoord;
  FloatVec3Type overrideSpacing = d->montageSpacing;

  if(getChangeOrigin())
  {
    overrideOrigin = {m_Origin[0], m_Origin[1], m_Origin[2]};
  }
  if(getChangeSpacing())
  {
    overrideSpacing = {m_Spacing[0], m_Spacing[1], m_Spacing[2]};
  }

  MontageCacheVector montageCacheVector = getMontageCacheVector();
  for(const auto& montageCache : montageCacheVector)
  {
    QString rowColIdString = tr("r%1c%2").arg(montageCache.row).arg(montageCache.col);
    QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(rowColIdString);

    DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dcName);
    ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();

    FloatVec3Type currentOrigin = imageGeom->getOrigin();

    FloatVec3Type currentSpacing = imageGeom->getSpacing();

    for(size_t i = 0; i < 3; i++)
    {
      float delta = currentOrigin[i] - d->montageMinCoord[i];
      if (m_UsePixelCoordinates)
      {
        // Convert to Pixel Coords
        delta = delta / currentSpacing[i];
      }
//      // Convert to the override origin
//      delta = delta * overrideSpacing[i];
      currentOrigin[i] = overrideOrigin[i] + delta;
      if (m_UsePixelCoordinates)
      {
        // Convert back to physical coords
        currentOrigin[i] = currentOrigin[i] * currentSpacing[i];
      }
    }
    imageGeom->setOrigin(currentOrigin.data());
    imageGeom->setSpacing(overrideSpacing.data());
  }

  QString montageInfo;
  QTextStream ss(&montageInfo);
  ss << "Columns=" << m_ColumnCount << "  Rows=" << m_RowCount << "  Num. Images=" << montageCacheVector.size();

  ss << "\nOrigin: " << overrideOrigin[0] << ", " << overrideOrigin[1] << ", " << overrideOrigin[2];
  ss << "  Spacing: " << overrideSpacing[0] << ", " << overrideSpacing[1] << ", " << overrideSpacing[2];
  m_MontageInformation = montageInfo;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportMontage::preflight()
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
void ITKImportMontage::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage("Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKImportMontage::getUuid()
{
  return QUuid("{5808733b-cc12-5486-ac5f-ff0107807e74}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportMontage::getHumanLabel() const
{
  return "Import Montage";
}
