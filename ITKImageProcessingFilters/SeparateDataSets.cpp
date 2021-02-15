/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "SeparateDataSets.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

enum createdPathID : RenameDataPath::DataID_t
{
  AttributeMatrixID21 = 21,

  DataContainerID = 1
};

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
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SeparateDataSets::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    AttributeMatrixSelectionFilterParameter::RequirementType req = AttributeMatrixSelectionFilterParameter::CreateRequirement(AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Dataset Attribute Matrix", DatasetAMPath, FilterParameter::Category::RequiredArray, SeparateDataSets, req));
  }

  {
    AttributeMatrixSelectionFilterParameter::RequirementType req = AttributeMatrixSelectionFilterParameter::CreateRequirement(AttributeMatrix::Type::Generic, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Metadata Attribute Matrix", MetadataAMPath, FilterParameter::Category::RequiredArray, SeparateDataSets, req));
  }

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SeparateDataSets::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SeparateDataSets::preflight()
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
void SeparateDataSets::execute()
{
  initialize();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  AttributeMatrix::Pointer origDataSetAM = getDataContainerArray()->getPrereqAttributeMatrixFromPath(this, getDatasetAMPath(), -9000);
  if(getErrorCode() < 0)
  {
    return;
  }

  AttributeMatrix::Pointer origMetaDataAM = getDataContainerArray()->getPrereqAttributeMatrixFromPath(this, getMetadataAMPath(), -9001);
  if(getErrorCode() < 0)
  {
    return;
  }

  int err = 0;
  std::vector<size_t> cDims = {1};
  StringDataArray::Pointer attrArrayNamesPtr = origMetaDataAM->getPrereqArray<StringDataArray>(this, "AttributeArrayNames", err, cDims);
  if(getErrorCode() < 0)
  {
    return;
  }

  StringDataArray::Pointer stagePositionXPtr = origMetaDataAM->getPrereqArray<StringDataArray>(this, "StagePositionX", err, cDims);
  if(getErrorCode() < 0)
  {
    return;
  }

  StringDataArray::Pointer stagePositionYPtr = origMetaDataAM->getPrereqArray<StringDataArray>(this, "StagePositionY", err, cDims);
  if(getErrorCode() < 0)
  {
    return;
  }

  StringDataArray::Pointer scaleFactorForXPtr = origMetaDataAM->getPrereqArray<StringDataArray>(this, "ScaleFactorForX", err, cDims);
  if(getErrorCode() < 0)
  {
    return;
  }

  StringDataArray::Pointer scaleFactorForYPtr = origMetaDataAM->getPrereqArray<StringDataArray>(this, "ScaleFactorForY", err, cDims);
  if(getErrorCode() < 0)
  {
    return;
  }

  for(int i = 0; i < attrArrayNamesPtr->getNumberOfTuples(); i++)
  {
    QString dataSetName = attrArrayNamesPtr->getValue(i);
    IDataArray::Pointer origDataSetPtr = origDataSetAM->getAttributeArray(dataSetName);

    bool ok = false;
    float stagePositionX = stagePositionXPtr->getValue(i).toFloat(&ok);
    if(!ok)
    {
      QString ss = QObject::tr("The filter could not convert the string values in array '%1' to floating point values.").arg(stagePositionXPtr->getName());
      setErrorCondition(-90002, ss);
      return;
    }

    float stagePositionY = stagePositionYPtr->getValue(i).toFloat(&ok);
    if(!ok)
    {
      QString ss = QObject::tr("The filter could not convert the string values in array '%1' to floating point values.").arg(stagePositionYPtr->getName());
      setErrorCondition(-90003, ss);
      return;
    }

    float scaleFactorForX = scaleFactorForXPtr->getValue(i).toFloat(&ok);
    if(!ok)
    {
      QString ss = QObject::tr("The filter could not convert the string values in array '%1' to floating point values.").arg(scaleFactorForXPtr->getName());
      setErrorCondition(-90004, ss);
      return;
    }

    float scaleFactorForY = scaleFactorForYPtr->getValue(i).toFloat(&ok);
    if(!ok)
    {
      QString ss = QObject::tr("The filter could not convert the string values in array '%1' to floating point values.").arg(scaleFactorForYPtr->getName());
      setErrorCondition(-90005, ss);
      return;
    }

    // Create the new data container for this data set
    DataContainerShPtr origDCPtr = getDataContainerArray()->getDataContainer(getDatasetAMPath());
    DataContainerShPtr newDCPtr = getDataContainerArray()->createNonPrereqDataContainer(this, dataSetName, DataContainerID);

    ImageGeom::Pointer originalGeom = origDCPtr->getGeometryAs<ImageGeom>();
    ImageGeom::Pointer newGeom = ImageGeom::New();
    SizeVec3Type dims = originalGeom->getDimensions();
    newGeom->setDimensions(dims);
    // FloatVec3Type spacing = originalGeom->getSpacing();
    newGeom->setSpacing(FloatVec3Type(scaleFactorForX, scaleFactorForY, 0.0f));
    FloatVec3Type origin = originalGeom->getOrigin();
    newGeom->setOrigin(FloatVec3Type(stagePositionX, stagePositionY, origin[2]));
    newGeom->setName(originalGeom->getName());
    newDCPtr->setGeometry(newGeom);

    // Create the new attribute matrix for this data set
    DataArrayPath newDataSetAMPath = DataArrayPath(newDCPtr->getName(), origDataSetAM->getName(), "");
    AttributeMatrix::Pointer newDataSetAM = newDCPtr->createNonPrereqAttributeMatrix(this, newDataSetAMPath, origDataSetAM->getTupleDimensions(), origDataSetAM->getType(), AttributeMatrixID21);

    // Move the data set to the new attribute matrix
    IDataArray::Pointer newDataSetPtr = origDataSetAM->removeAttributeArray(origDataSetPtr->getName());
    if(nullptr != newDataSetPtr.get())
    {
      newDataSetAM->insertOrAssign(newDataSetPtr);
    }

    // Copy the meta data attribute matrix
    AttributeMatrix::Pointer newMetaDataAM = origMetaDataAM->deepCopy(getInPreflight());

    // Loop through the meta data arrays in the new meta data attribute matrix.  Make the tuple that refers to the current data set as the only tuple.
    QStringList newMetaDataArrayNames = newMetaDataAM->getAttributeArrayNames();
    for(int j = 0; j < newMetaDataArrayNames.size(); j++)
    {
      QString metaDataArrayName = newMetaDataArrayNames[j];
      IDataArray::Pointer newMetaDataPtr = newMetaDataAM->getAttributeArray(metaDataArrayName);

      newMetaDataPtr->copyTuple(i, 0);
      newMetaDataPtr->resizeTuples(1);
    }
    newMetaDataAM->setTupleDimensions(std::vector<size_t>(1, 1));

    newDCPtr->addOrReplaceAttributeMatrix(newMetaDataAM);
    newDCPtr->addOrReplaceAttributeMatrix(newDataSetAM);
  }

  getDataContainerArray()->removeDataContainer(getDatasetAMPath().getDataContainerName());

  //  if (getCancel() == true) { return; }

  //  if (getWarningCode() < 0)
  //  {
  //    QString ss = QObject::tr("Some warning message");
  //    setWarningCondition(-88888888, ss);
  //  }

  //  if (getErrorCode() < 0)
  //  {
  //    QString ss = QObject::tr("Some error message");
  //    setErrorCondition(-99999999, ss);
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
QString SeparateDataSets::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SeparateDataSets::getBrandingString() const
{
  return ITKImageProcessingConstants::ZeissImportPluginDisplayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SeparateDataSets::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SeparateDataSets::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SeparateDataSets::getSubGroupName() const
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SeparateDataSets::getHumanLabel() const
{
  return "Separate Datasets";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid SeparateDataSets::getUuid() const
{
  return QUuid("{e9a9fd4b-ba9d-5c9a-bf38-5f040723602c}");
}

// -----------------------------------------------------------------------------
SeparateDataSets::Pointer SeparateDataSets::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<SeparateDataSets> SeparateDataSets::New()
{
  struct make_shared_enabler : public SeparateDataSets
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString SeparateDataSets::getNameOfClass() const
{
  return QString("SeparateDataSets");
}

// -----------------------------------------------------------------------------
QString SeparateDataSets::ClassName()
{
  return QString("SeparateDataSets");
}

// -----------------------------------------------------------------------------
void SeparateDataSets::setDatasetAMPath(const DataArrayPath& value)
{
  m_DatasetAMPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath SeparateDataSets::getDatasetAMPath() const
{
  return m_DatasetAMPath;
}

// -----------------------------------------------------------------------------
void SeparateDataSets::setMetadataAMPath(const DataArrayPath& value)
{
  m_MetadataAMPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath SeparateDataSets::getMetadataAMPath() const
{
  return m_MetadataAMPath;
}
