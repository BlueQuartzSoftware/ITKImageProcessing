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

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

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
: ITKImportMontage()
, m_FijiConfigFilePath("")
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Fiji Configuration File", FijiConfigFilePath, FilterParameter::Parameter, ITKImportFijiMontage, "", "*.txt"));

  QStringList linkedProps("Origin");
  linkedProps.push_back("UsePixelCoordinates");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Override Origin", ChangeOrigin, FilterParameter::Parameter, ITKImportMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ITKImportMontage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("Pixel Coordinates", UsePixelCoordinates, FilterParameter::Parameter, ITKImportMontage));

  linkedProps.clear();
  linkedProps << "Spacing";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Override Spacing", ChangeSpacing, FilterParameter::Parameter, ITKImportMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ITKImportMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container Prefix", DataContainerPrefix, FilterParameter::CreatedArray, ITKImportMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImportMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Array Name", AttributeArrayName, FilterParameter::CreatedArray, ITKImportMontage));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportFijiMontage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  QString ss;

  if(getFijiConfigFilePath().isEmpty())
  {
    QString ss = QObject::tr("The fiji configuration file must be set").arg(getHumanLabel());
    setErrorCondition(-2000, ss);
    return;
  }

  QFileInfo fi(getFijiConfigFilePath());
  if (fi.suffix() != "txt")
  {
    QString ss = QObject::tr("Configuration file is not in Fiji format (*.txt).");
    setErrorCondition(-2001, ss);
    return;
  }

  if(!fi.exists())
  {
    QString ss = QObject::tr("The fiji configuration file does not exist");
    setErrorCondition(-2002, ss);
    return;
  }

  QDateTime lastModified(fi.lastModified());

  // Only parse the fiji config file again if the cache is outdated
  if (!getInPreflight() || getFijiConfigFilePath() != getFijiConfigFilePathCache() || !getLastRead().isValid() || lastModified.msecsTo(getLastRead()) < 0)
  {
    setMontageCacheVector(MontageCacheVector());

    // Parse Fiji Config File
    itk::FijiConfigurationFileReader fijiFileReader;
    itk::FijiFileData fijiFileData = fijiFileReader.parseFijiConfigFile(getFijiConfigFilePath());
    if(fijiFileReader.getErrorCode() < 0)
    {
      QString ss = fijiFileReader.getErrorMessage();
      setErrorCondition(fijiFileReader.getErrorCode(), ss);
      return;
    }

    if (fijiFileData.size() <= 0)
    {
      QString ss = "Fiji file does not contain at least 1 tile row.  Please choose a Fiji file that contains at least 1 tile row.";
      setErrorCondition(-2003, ss);
      return;
    }

    if (fijiFileData[0].size() <= 0)
    {
      QString ss = "Fiji file does not contain at least 1 tile column.  Please choose a Fiji file that contains at least 1 tile column.";
      setErrorCondition(-2004, ss);
      return;
    }

    setRowCount(fijiFileData.size());
    setColumnCount(fijiFileData[0].size());

    QVector<size_t> cDims(1, 1);

    size_t totalImageCount = fijiFileData.size() * fijiFileData[0].size();
    int currentImageCount = 1;
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

        if (!getInPreflight())
        {
          QString imagePath = fijiImageData.filePath;
          QFileInfo imageFi(imagePath);
          notifyStatusMessage(tr("[%1/%2]: Reading image '%3'").arg(currentImageCount).arg(totalImageCount).arg(imageFi.fileName()));
        }

        readImageFile(fijiImageData.filePath, fijiImageData.coords, fijiImageData.row, fijiImageData.col);
        currentImageCount++;
      }
    }

    // Set the new data into the cache
    setLastRead(QDateTime::currentDateTime());
    setFijiConfigFilePathCache(getFijiConfigFilePath());
  }
  else
  {
    readImagesFromCache();
  }

  if(getChangeOrigin() || getChangeSpacing())
  {
    adjustOriginAndSpacing();
  }
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
