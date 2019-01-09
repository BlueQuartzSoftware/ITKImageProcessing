/* ============================================================================
 * Copyright (c) 2018 BlueQuartz Software, LLC
 * All rights reserved.
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "SeparateDataSets.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissImportVersion.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SeparateDataSets::SeparateDataSets()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SeparateDataSets::~SeparateDataSets() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SeparateDataSets::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SeparateDataSets::setupFilterParameters()
{
  FilterParameterVector parameters;

  {
    AttributeMatrixSelectionFilterParameter::RequirementType req =
        AttributeMatrixSelectionFilterParameter::CreateRequirement(AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Dataset Attribute Matrix", DatasetAMPath, FilterParameter::RequiredArray, SeparateDataSets, req));
  }

  {
    AttributeMatrixSelectionFilterParameter::RequirementType req =
        AttributeMatrixSelectionFilterParameter::CreateRequirement(AttributeMatrix::Type::Generic, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Metadata Attribute Matrix", MetadataAMPath, FilterParameter::RequiredArray, SeparateDataSets, req));
  }

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SeparateDataSets::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SeparateDataSets::preflight()
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
void SeparateDataSets::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  AttributeMatrix::Pointer origDataSetAM = getDataContainerArray()->getPrereqAttributeMatrixFromPath<AbstractFilter>(this, getDatasetAMPath(), -9000);
  if(getErrorCondition() < 0)
  {
    return;
  }

  AttributeMatrix::Pointer origMetaDataAM = getDataContainerArray()->getPrereqAttributeMatrixFromPath<AbstractFilter>(this, getMetadataAMPath(), -9001);
  if(getErrorCondition() < 0)
  {
    return;
  }

  int err = 0;
  QVector<size_t> cDims = { 1 };
  StringDataArray::Pointer attrArrayNamesPtr = origMetaDataAM->getPrereqArray<StringDataArray, AbstractFilter>(this, "AttributeArrayNames", err, cDims);
  if(getErrorCondition() < 0)
  {
    return;
  }

  StringDataArray::Pointer stagePositionXPtr = origMetaDataAM->getPrereqArray<StringDataArray, AbstractFilter>(this, "StagePositionX", err, cDims);
  if(getErrorCondition() < 0)
  {
    return;
  }

  StringDataArray::Pointer stagePositionYPtr = origMetaDataAM->getPrereqArray<StringDataArray, AbstractFilter>(this, "StagePositionY", err, cDims);
  if(getErrorCondition() < 0)
  {
    return;
  }

  StringDataArray::Pointer scaleFactorForXPtr = origMetaDataAM->getPrereqArray<StringDataArray, AbstractFilter>(this, "ScaleFactorForX", err, cDims);
  if(getErrorCondition() < 0)
  {
    return;
  }

  StringDataArray::Pointer scaleFactorForYPtr = origMetaDataAM->getPrereqArray<StringDataArray, AbstractFilter>(this, "ScaleFactorForY", err, cDims);
  if(getErrorCondition() < 0)
  {
    return;
  }

  for (int i = 0; i < attrArrayNamesPtr->getNumberOfTuples(); i++)
  {
    QString dataSetName = attrArrayNamesPtr->getValue(i);
    IDataArray::Pointer origDataSetPtr = origDataSetAM->getAttributeArray(dataSetName);

    bool ok = false;
    float stagePositionX = stagePositionXPtr->getValue(i).toFloat(&ok);
    if(!ok)
    {
      QString ss = QObject::tr("The filter could not convert the string values in array '%1' to floating point values.").arg(stagePositionXPtr->getName());
      setErrorCondition(-90002);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    float stagePositionY = stagePositionYPtr->getValue(i).toFloat(&ok);
    if(!ok)
    {
      QString ss = QObject::tr("The filter could not convert the string values in array '%1' to floating point values.").arg(stagePositionYPtr->getName());
      setErrorCondition(-90003);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    float scaleFactorForX = scaleFactorForXPtr->getValue(i).toFloat(&ok);
    if(!ok)
    {
      QString ss = QObject::tr("The filter could not convert the string values in array '%1' to floating point values.").arg(scaleFactorForXPtr->getName());
      setErrorCondition(-90004);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    float scaleFactorForY = scaleFactorForYPtr->getValue(i).toFloat(&ok);
    if(!ok)
    {
      QString ss = QObject::tr("The filter could not convert the string values in array '%1' to floating point values.").arg(scaleFactorForYPtr->getName());
      setErrorCondition(-90005);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    // Create the new data container for this data set
    DataContainerShPtr origDCPtr = getDataContainerArray()->getDataContainer(getDatasetAMPath());
    DataContainerShPtr newDCPtr = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, dataSetName);

    ImageGeom::Pointer originalGeom = origDCPtr->getGeometryAs<ImageGeom>();
    ImageGeom::Pointer newGeom = ImageGeom::New();
    size_t dims[3] = {0, 0, 0};
    std::tie(dims[0], dims[1], dims[2]) = originalGeom->getDimensions();
    newGeom->setDimensions(dims);
    float res[3] = {0.0f, 0.0f, 0.0f};
    originalGeom->getResolution(res);
    newGeom->setResolution(scaleFactorForX, scaleFactorForY, 0.0f);
    float origin[3] = {0.0f, 0.0f, 0.0f};
    originalGeom->getOrigin(origin);
    newGeom->setOrigin(stagePositionX, stagePositionY, origin[2]);
    newGeom->setName(originalGeom->getName());
    newDCPtr->setGeometry(newGeom);

    // Create the new attribute matrix for this data set
    DataArrayPath newDataSetAMPath = DataArrayPath(newDCPtr->getName(), origDataSetAM->getName(), "");
    AttributeMatrix::Pointer newDataSetAM = newDCPtr->createNonPrereqAttributeMatrix(this, newDataSetAMPath, origDataSetAM->getTupleDimensions(), origDataSetAM->getType());

    // Move the data set to the new attribute matrix
    IDataArray::Pointer newDataSetPtr = origDataSetAM->removeAttributeArray(origDataSetPtr->getName());
    if (nullptr != newDataSetPtr.get())
    {
      newDataSetAM->addAttributeArray(newDataSetPtr->getName(), newDataSetPtr);
    }

    // Copy the meta data attribute matrix
    AttributeMatrix::Pointer newMetaDataAM = origMetaDataAM->deepCopy(getInPreflight());

    // Loop through the meta data arrays in the new meta data attribute matrix.  Make the tuple that refers to the current data set as the only tuple.
    QStringList newMetaDataArrayNames = newMetaDataAM->getAttributeArrayNames();
    for (int j = 0; j < newMetaDataArrayNames.size(); j++)
    {
      QString metaDataArrayName = newMetaDataArrayNames[j];
      IDataArray::Pointer newMetaDataPtr = newMetaDataAM->getAttributeArray(metaDataArrayName);

      newMetaDataPtr->copyTuple(i, 0);
      newMetaDataPtr->resize(1);
    }
    newMetaDataAM->setTupleDimensions(QVector<size_t>(1, 1));

    newDCPtr->addAttributeMatrix(newMetaDataAM->getName(), newMetaDataAM);
    newDCPtr->addAttributeMatrix(newDataSetAM->getName(), newDataSetAM);
  }

  getDataContainerArray()->removeDataContainer(getDatasetAMPath().getDataContainerName());

//  if (getCancel() == true) { return; }

//  if (getWarningCondition() < 0)
//  {
//    QString ss = QObject::tr("Some warning message");
//    setWarningCondition(-88888888);
//    notifyWarningMessage(getHumanLabel(), ss, getWarningCondition());
//  }

//  if (getErrorCondition() < 0)
//  {
//    QString ss = QObject::tr("Some error message");
//    setErrorCondition(-99999999);
//    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
//    return;
//  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer SeparateDataSets::newFilterInstance(bool copyFilterParameters) const
{
  SeparateDataSets::Pointer filter = SeparateDataSets::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SeparateDataSets::getCompiledLibraryName() const
{ 
  return ZeissImportConstants::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SeparateDataSets::getBrandingString() const
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SeparateDataSets::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ZeissImport::Version::Major() << "." << ZeissImport::Version::Minor() << "." << ZeissImport::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SeparateDataSets::getGroupName() const
{ 
  return SIMPL::FilterGroups::Unsupported; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SeparateDataSets::getSubGroupName() const
{ 
  return "ZeissImport"; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SeparateDataSets::getHumanLabel() const
{ 
  return "Separate Datasets";
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid SeparateDataSets::getUuid()
{
  return QUuid("{e9a9fd4b-ba9d-5c9a-bf38-5f040723602c}");
}

