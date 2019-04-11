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
#include "CalculateBackground.h"

#include <cstring>
#include <set>
#include <type_traits>

#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QTextStream>

#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AttributeMatrixCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArrayCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Geometry/RectGridGeom.h"

#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissImportVersion.h"
#include "ZeissImport/ZeissXml/ZeissTagMapping.h"


#include <Eigen/Dense>

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  DataContainerID10 = 10,
  AttributeMatrixID20 = 20,
  DataArrayID30 = 30,
};

#define ZIF_PRINT_DBG_MSGS 0

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::CalculateBackground()
: m_DataContainers("")
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_ImageDataArrayName("")
, m_OutputDataContainerPath("Background")
, m_OutputCellAttributeMatrixPath("Background", "Background Data", "")
, m_OutputImageArrayPath("Background", "Background Data", "Background Image")
, m_lowThresh(0)
, m_highThresh(255)
, m_SubtractBackground(false)
, m_DivideBackground(false)
, m_GaussianStdVariation(2)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::~CalculateBackground() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  MultiDataContainerSelectionFilterParameter::RequirementType req;
  req.dcGeometryTypes.push_back(IGeometry::Type::Image);
  req.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  parameters.push_back(SIMPL_NEW_MDC_SELECTION_FP("Select Image Data Containers", DataContainers, FilterParameter::Parameter, CalculateBackground, req));
  parameters.push_back(SIMPL_NEW_STRING_FP("Input Attribute Matrix Name", CellAttributeMatrixName, FilterParameter::RequiredArray, CalculateBackground));
  parameters.push_back(SIMPL_NEW_STRING_FP("Input Image Array Name", ImageDataArrayName, FilterParameter::RequiredArray, CalculateBackground));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("Created Data Container", OutputDataContainerPath, FilterParameter::CreatedArray, CalculateBackground));
  AttributeMatrixCreationFilterParameter::RequirementType cellAmReq;
  cellAmReq.dcGeometryTypes.push_back(IGeometry::Type::Image);
  cellAmReq.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  parameters.push_back(SIMPL_NEW_AM_CREATION_FP("Created Background Attribute Matrix", OutputCellAttributeMatrixPath, FilterParameter::CreatedArray, CalculateBackground, cellAmReq));

  DataArrayCreationFilterParameter::RequirementType imageReq;
  imageReq.dcGeometryTypes.push_back(IGeometry::Type::Image);
  imageReq.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  imageReq.amTypes.push_back(AttributeMatrix::Type::Cell);
  parameters.push_back(SIMPL_NEW_DA_CREATION_FP("Created Background Image Array Name", OutputImageArrayPath, FilterParameter::CreatedArray, CalculateBackground, imageReq));

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Lowest allowed Image value (Image Value)", lowThresh, FilterParameter::Parameter, CalculateBackground));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Highest allowed Image value (Image Value)", highThresh, FilterParameter::Parameter, CalculateBackground));

  // Only allow the Gaussian Blur property if the required filter is available
  FilterManager* filtManager = FilterManager::Instance();
  IFilterFactory::Pointer factory = filtManager->getFactoryFromClassName("ItkDiscreteGaussianBlur");
  if(nullptr != factory.get())
  {
    QStringList linkedProps;
    linkedProps.clear();
    linkedProps << "GaussianStdVariation";

    parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Use Gaussian Blur", GaussianBlur, FilterParameter::Parameter, CalculateBackground, linkedProps));
    parameters.push_back(SIMPL_NEW_FLOAT_FP("Gaussian: Std Deviations", GaussianStdVariation, FilterParameter::Parameter, CalculateBackground));
  }

  parameters.push_back(SIMPL_NEW_BOOL_FP("Subtract Background from Current Images", SubtractBackground, FilterParameter::Parameter, CalculateBackground));
  parameters.push_back(SIMPL_NEW_BOOL_FP("Divide Background from Current Images", DivideBackground, FilterParameter::Parameter, CalculateBackground));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::ArrayType CalculateBackground::getArrayType()
{
  DataContainerArray::Pointer dca = getDataContainerArray();
  const QVector<size_t> cDims = { 1 };

  for(const auto& dcName : m_DataContainers)
  {
    DataArrayPath imageArrayPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);
    AttributeMatrix::Pointer am = dca->getAttributeMatrix(imageArrayPath);
    IDataArray::Pointer da = am->getChildByName(m_ImageDataArrayName);
    if(da->getComponentDimensions() != cDims)
    {
      QString msg;
      QTextStream out(&msg);
      out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not single-component (Grayscale) data. Please select a pattern of AttributeArray Paths that are gray scale images";
      setErrorCondition(-53000, msg);
      return ArrayType::Error;
    }
    QString typeString = da->getTypeAsString();
    if("uint8_t" == typeString)
    {
      return ArrayType::UInt8;
    }
    if("uint16_t" == typeString)
    {
      return ArrayType::UInt16;
    }
    if("float" == typeString)
    {
      return ArrayType::Float32;
    }

    QString msg;
    QTextStream out(&msg);
    out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not of the appropriate type. Please select a pattern of AttributeArray Paths that are gray scale images";
    setErrorCondition(-53000, msg);
    return ArrayType::Error;
  }
  return ArrayType::Error;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::GeomType CalculateBackground::getGeomType()
{
  DataContainerArray::Pointer dca = getDataContainerArray();
  for(const auto& dcName : m_DataContainers)
  {
    IGeometryGrid::Pointer gridGeom = dca->getDataContainer(dcName)->getGeometryAs<IGeometryGrid>();
    if(nullptr == gridGeom)
    {
      QString msg;
      QTextStream out(&msg);
      out << "DataContainer: " << dcName << " needs to have an IGeometryGrid assigned. There is either no geometry assign to the Data Container or the Geometry is not of type IGeometryGrid.";
      setErrorCondition(-53001, msg);
      return GeomType::Error;
    }
    switch(gridGeom->getGeometryType())
    {
    case IGeometry::Type::Image:
      return GeomType::ImageGeom;
    case IGeometry::Type::RectGrid:
      return GeomType::RectGridGeom;
    default:
      setErrorCondition(-53002, "Invalid Geometry type detected.  An ImageGeometry or RectGridGeometry is required for incoming data.");
      return GeomType::Error;
    }
  }
  return GeomType::Error;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  // Check for empty list. If list is empty then the OutputGeometry was never formed and it wont help to go on..
  if(m_DataContainers.isEmpty())
  {
    setErrorCondition(-53006, "At least one DataContainer must be selected.");
    return;
  }

  DataContainerArray::Pointer dca = getDataContainerArray();
  QVector<size_t> cDims = {1};

  // CheckInputArrays() templated on array and geometry types.
  ArrayType arrayType = getArrayType();
  GeomType geomType = getGeomType();
  IGeometryGrid::Pointer outputGridGeom = checkInputArrays(arrayType, geomType);

  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainer::Pointer outputDc = dca->createNonPrereqDataContainer(this, getOutputDataContainerPath(), DataContainerID10);
  if(getErrorCode() < 0)
  {
    return;
  }

  outputDc->setGeometry(outputGridGeom);

  QVector<size_t> tDims = {0, 0, 0};
  std::tie(tDims[0], tDims[1], tDims[2]) = outputGridGeom->getDimensions();
  AttributeMatrix::Pointer outputAttrMat = outputDc->createNonPrereqAttributeMatrix(this, getOutputCellAttributeMatrixPath(), tDims, AttributeMatrix::Type::Cell, AttributeMatrixID20);

  switch(arrayType)
  {
  case ArrayType::UInt8:
    outputAttrMat->createNonPrereqArray<UInt8ArrayType>(this, m_OutputImageArrayPath.getDataArrayName(), 0, cDims, DataArrayID30);
    break;
  case ArrayType::UInt16:
    outputAttrMat->createNonPrereqArray<UInt16ArrayType>(this, m_OutputImageArrayPath.getDataArrayName(), 0, cDims, DataArrayID30);
    break;
  case ArrayType::Float32:
    outputAttrMat->createNonPrereqArray<FloatArrayType>(this, m_OutputImageArrayPath.getDataArrayName(), 0, cDims, DataArrayID30);
    break;
  default:
    setErrorCondition(-53006, "A valid Attribute Array type (UInt8, UInt16, or Float) is required for this filter.");
    break;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::preflight()
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
void CalculateBackground::execute()
{
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCode() < 0)
  {
    return;
  }

  ArrayType arrayType = getArrayType();
  GeomType geomType = getGeomType();
  calculateOutputValues(arrayType, geomType);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IGeometryGrid::Pointer CalculateBackground::checkInputArrays(ArrayType arrayType, GeomType geomType)
{
  switch(arrayType)
  {
  case ArrayType::UInt8:
    switch(geomType)
    {
    case GeomType::ImageGeom:
      return checkInputArrays<UInt8ArrayType, ImageGeom>();
    case GeomType::RectGridGeom:
      return checkInputArrays<UInt8ArrayType, RectGridGeom>();
    default:
      setErrorCondition(-53005, "A valid geometry type (ImageGeom or RectGridGeom) is required for this filter.");
    }
    break;
  case ArrayType::UInt16:
    switch(geomType)
    {
    case GeomType::ImageGeom:
      return checkInputArrays<UInt16ArrayType, ImageGeom>();
    case GeomType::RectGridGeom:
      return checkInputArrays<UInt16ArrayType, RectGridGeom>();
    default:
      setErrorCondition(-53005, "A valid geometry type (ImageGeom or RectGridGeom) is required for this filter.");
    }
    break;
  case ArrayType::Float32:
    switch(geomType)
    {
    case GeomType::ImageGeom:
      return checkInputArrays<FloatArrayType, ImageGeom>();
    case GeomType::RectGridGeom:
      return checkInputArrays<FloatArrayType, RectGridGeom>();
    default:
      setErrorCondition(-53005, "A valid geometry type (ImageGeom or RectGridGeom) is required for this filter.");
    }
    break;
  default:
    setErrorCondition(-53006, "A valid Attribute Array type (UInt8, UInt16, or Float) is required for this filter.");
  }

  return IGeometryGrid::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::calculateOutputValues(ArrayType arrayType, GeomType geomType)
{
  switch(arrayType)
  {
  case ArrayType::UInt8:
    switch(geomType)
    {
    case GeomType::ImageGeom:
      calculateOutputValues<uint8_t, ImageGeom, uint64_t>();
      break;
    case GeomType::RectGridGeom:
      calculateOutputValues<uint8_t, RectGridGeom, uint64_t>();
      break;
    case GeomType::Error:
      break;
    }
    break;
  case ArrayType::UInt16:
    switch(geomType)
    {
    case GeomType::ImageGeom:
      calculateOutputValues<uint16_t, ImageGeom, uint64_t>();
      break;
    case GeomType::RectGridGeom:
      calculateOutputValues<uint16_t, RectGridGeom, uint64_t>();
      break;
    case GeomType::Error:
      break;
    }
    break;
  case ArrayType::Float32:
    switch(geomType)
    {
    case GeomType::ImageGeom:
      calculateOutputValues<float, ImageGeom, double>();
      break;
    case GeomType::RectGridGeom:
      calculateOutputValues<float, RectGridGeom, double>();
      break;
    case GeomType::Error:
      break;
    }
    break;
  case ArrayType::Error:
    break;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getCompiledLibraryName() const
{
  return ZeissImportConstants::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getBrandingString() const
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ZeissImport::Version::Major() << "." << ZeissImport::Version::Minor() << "." << ZeissImport::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getHumanLabel() const
{
  return "Calculate Background";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid CalculateBackground::getUuid()
{
  return QUuid("{a48f7a51-0ca9-584f-a0ca-4bfebdc41d7c}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::MiscFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CalculateBackground::newFilterInstance(bool copyFilterParameters) const
{
  /*
  * write code to optionally copy the filter parameters from the current filter into the new instance
  */
  CalculateBackground::Pointer filter = CalculateBackground::New();
  if(copyFilterParameters)
  {
    /* If the filter uses all the standard Filter Parameter Widgets you can probabaly get
     * away with using this method to copy the filter parameters from the current instance
     * into the new instance
     */
    copyFilterParameterInstanceVariables(filter.get());
    /* If your filter is using a lot of custom FilterParameterWidgets @see ReadH5Ebsd then you
     * may need to copy each filter parameter explicitly plus any other instance variables that
     * are needed into the new instance. Here is some example code from ReadH5Ebsd
     */
    //    SIMPL_COPY_INSTANCEVAR(OutputFile)
    //    SIMPL_COPY_INSTANCEVAR(ZStartIndex)
    //    SIMPL_COPY_INSTANCEVAR(ZEndIndex)
    //    SIMPL_COPY_INSTANCEVAR(ZResolution)
  }
  return filter;
}

