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
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
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

namespace
{
const QString k_MedianFilter("ITKMedianImage");

/**
 * @brief Calculates the output values using the templated output IDataArray output type
 */
template <typename OutArrayType, typename GeomType, typename AccumType>
void calculateOutputValues(CalculateBackground* filter)
{
  DataContainerArray::Pointer dca = filter->getDataContainerArray();

  DataContainer::Pointer outputDc = dca->getDataContainer(filter->getOutputDataContainerPath());
  AttributeMatrix::Pointer outputAttrMat = outputDc->getAttributeMatrix(filter->getOutputCellAttributeMatrixPath());
  typename DataArray<OutArrayType>::Pointer outputArrayPtr = outputAttrMat->getAttributeArrayAs<DataArray<OutArrayType>>(filter->getOutputImageArrayPath().getDataArrayName());
  DataArray<OutArrayType>& outputArray = *(outputArrayPtr);
  outputArray.initializeWithZeros();

  typename GeomType::Pointer outputGeom = outputDc->getGeometryAs<GeomType>();
  SizeVec3Type dims;
  outputGeom->getDimensions(dims);

  typename DataArray<AccumType>::Pointer accumulateArrayPtr = DataArray<AccumType>::CreateArray(outputArrayPtr->getNumberOfTuples(), "Accumulation Array", true);
  accumulateArrayPtr->initializeWithZeros();
  DataArray<AccumType>& accumArray = *accumulateArrayPtr;
  size_t numTuples = accumArray.getNumberOfTuples();

  typename SizeTArrayType::Pointer countArrayPtr = SizeTArrayType::CreateArray(outputArrayPtr->getNumberOfTuples(), "Count Array", true);
  countArrayPtr->initializeWithZeros();
  SizeTArrayType& counter = *(countArrayPtr);

  int32_t lowThresh = filter->getlowThresh();
  int32_t highThresh = filter->gethighThresh();

  for(const auto& dcName : filter->getDataContainers())
  {
    DataArrayPath imageArrayPath(dcName, filter->getCellAttributeMatrixName(), filter->getImageDataArrayName());
    DataArray<OutArrayType>& imageArray = *(dca->getAttributeMatrix(imageArrayPath)->getAttributeArrayAs<DataArray<OutArrayType>>(imageArrayPath.getDataArrayName()));

    for(size_t t = 0; t < numTuples; t++)
    {
      if(imageArray[t] >= lowThresh && imageArray[t] <= highThresh)
      {
        accumArray[t] += imageArray[t];
        counter[t]++;
      }
    }
  }

  // average the background values by the number of counts (counts will be the number of images unless the threshold
  // values do not include all the possible image values
  // (i.e. for an 8 bit image, if we only include values from 0 to 100, not every image value will be counted)
  for(int j = 0; j < numTuples; j++)
  {
    if(counter[j] > 0) // Guard against Divide by zero
    {
      accumArray[j] /= counter[j];
    }
  }

  // Assign output array values
  for(int i = 0; i < numTuples; ++i)
  {
    outputArray[i] = static_cast<OutArrayType>(accumArray[i]);
  }

  // Blur
  if(filter->getApplyMedianFilter())
  {
    FilterManager* filtManager = FilterManager::Instance();
    IFilterFactory::Pointer factory = filtManager->getFactoryFromClassName(::k_MedianFilter);
    if(nullptr != factory.get())
    {
      AbstractFilter::Pointer imageProcessingFilter = factory->create();
      if(nullptr != imageProcessingFilter.get())
      {
        QVariant var;

        imageProcessingFilter->setDataContainerArray(filter->getDataContainerArray());

        var.setValue(filter->getOutputImageArrayPath());
        bool propSet = imageProcessingFilter->setProperty("SelectedCellArrayPath", var);

        propSet = imageProcessingFilter->setProperty("SaveAsNewArray", false);

        var.setValue(filter->getRadius());
        propSet = imageProcessingFilter->setProperty("Radius", var);

        imageProcessingFilter->execute();
      }
    }
    else
    {
      filter->setErrorCondition(-53009, " filter not found.");
    }
  } // Blur

  // Edit the input data
  if(filter->getSubtractBackground())
  {
    for(const auto& dcName : filter->getDataContainers())
    {
      DataArrayPath imageDataPath(dcName, filter->getCellAttributeMatrixName(), filter->getImageDataArrayName());
      auto iDataArray = filter->getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<OutArrayType>, AbstractFilter>(filter, imageDataPath);
      auto imagePtr = std::dynamic_pointer_cast<DataArray<OutArrayType>>(iDataArray);
      size_t totalPoints = imagePtr->getNumberOfComponents();
      if(nullptr != imagePtr)
      {
        auto* image = imagePtr->getPointer(0);

        for(size_t t = 0; t < totalPoints; t++)
        {
          if((image[t] >= lowThresh) && (image[t] <= highThresh))
          {
            image[t] -= outputArray[t];

            if(image[t] < 0)
            {
              image[t] = 0;
            }
            if(image[t] > 255)
            {
              image[t] = 255;
            }
          }
        }
      }
    }
  } // Subtract Background

  if(filter->getDivideBackground())
  {
    for(const auto& dcName : filter->getDataContainers())
    {
      DataArrayPath imageDataPath(dcName, filter->getCellAttributeMatrixName(), filter->getImageDataArrayName());
      auto iDataArray = filter->getDataContainerArray()->getPrereqIDataArrayFromPath<DataArray<OutArrayType>, AbstractFilter>(filter, imageDataPath);
      auto imagePtr = std::dynamic_pointer_cast<DataArray<OutArrayType>>(iDataArray);
      size_t totalPoints = imagePtr->getNumberOfComponents();
      if(nullptr != imagePtr)
      {
        auto* image = imagePtr->getPointer(0);

        for(size_t t = 0; t < totalPoints; t++)
        {
          if((image[t] >= lowThresh) && (image[t] <= highThresh) && outputArray[t] != 0)
          {
            image[t] /= outputArray[t];
          }
        }
      }
    }
  } // Divide Background
}

} // namespace

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
, m_lowThresh(20000)
, m_highThresh(67000)
, m_SubtractBackground(false)
, m_DivideBackground(false)
{
  m_ApplyMedianFilter = true;
  m_Radius = {10.0f, 10.0f, 1.0f};
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

  // Only allow the Median Filter property if the required filter is available
  FilterManager* filtManager = FilterManager::Instance();
  IFilterFactory::Pointer factory = filtManager->getFactoryFromClassName(::k_MedianFilter);
  if(nullptr != factory.get())
  {
    parameters.push_back(SeparatorFilterParameter::New("Median Filter Parameters", FilterParameter::Parameter));
    QStringList linkedProps;
    linkedProps.clear();
    linkedProps << "Radius";

    parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Apply Median Filter", ApplyMedianFilter, FilterParameter::Parameter, CalculateBackground, linkedProps));
    parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Radius", Radius, FilterParameter::Parameter, CalculateBackground));
  }

  parameters.push_back(SeparatorFilterParameter::New("Process Input Images", FilterParameter::Parameter));
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
    if(!getDataContainerArray()->doesAttributeArrayExist(imageArrayPath))
    {
      continue;
    }
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
  if(geomType != GeomType::ImageGeom && geomType != GeomType::RectGridGeom)
  {
    setErrorCondition(-53005, "A valid geometry type (ImageGeom or RectGridGeom) is required for this filter.");
    return IGeometryGrid::NullPointer();
  }

  if(arrayType == ArrayType::UInt8 && geomType == GeomType::ImageGeom)
  {
    return checkInputArrays<UInt8ArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::UInt16 && geomType == GeomType::ImageGeom)
  {
    return checkInputArrays<UInt16ArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::Float32 && geomType == GeomType::ImageGeom)
  {
    return checkInputArrays<FloatArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::UInt8 && geomType == GeomType::RectGridGeom)
  {
    return checkInputArrays<UInt8ArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::UInt16 && geomType == GeomType::RectGridGeom)
  {
    return checkInputArrays<UInt16ArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::Float32 && geomType == GeomType::RectGridGeom)
  {
    return checkInputArrays<FloatArrayType, RectGridGeom>();
  }

  return IGeometryGrid::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::calculateOutputValues(ArrayType arrayType, GeomType geomType)
{

  if(geomType != GeomType::ImageGeom && geomType != GeomType::RectGridGeom)
  {
    setErrorCondition(-53005, "A valid geometry type (ImageGeom or RectGridGeom) is required for this filter.");
    return;
  }

  if(arrayType == ArrayType::UInt8 && geomType == GeomType::ImageGeom)
  {
    ::calculateOutputValues<uint8_t, ImageGeom, uint64_t>(this);
  }
  if(arrayType == ArrayType::UInt16 && geomType == GeomType::ImageGeom)
  {
    ::calculateOutputValues<uint16_t, ImageGeom, uint64_t>(this);
  }
  if(arrayType == ArrayType::Float32 && geomType == GeomType::ImageGeom)
  {
    ::calculateOutputValues<float, ImageGeom, double>(this);
  }
  if(arrayType == ArrayType::UInt8 && geomType == GeomType::RectGridGeom)
  {
    ::calculateOutputValues<uint8_t, RectGridGeom, uint64_t>(this);
  }
  if(arrayType == ArrayType::UInt16 && geomType == GeomType::RectGridGeom)
  {
    ::calculateOutputValues<uint16_t, RectGridGeom, uint64_t>(this);
  }
  if(arrayType == ArrayType::Float32 && geomType == GeomType::RectGridGeom)
  {
    ::calculateOutputValues<float, RectGridGeom, double>(this);
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
  return SIMPL::FilterGroups::Unsupported;
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
  return SIMPL::FilterSubGroups::ImageFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CalculateBackground::newFilterInstance(bool copyFilterParameters) const
{
  CalculateBackground::Pointer filter = CalculateBackground::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

#if 0
    // This block was previously disabled and divided on both sides of the for loop copying values into the output array.
    // The first part performs required work for polynomial operations.
    // The first part is required for SubtractBackground and DivideBackground operations.
    if(getPolynomial())
    {
      int xval = 0;
      int yval = 0;
      // Fit the background to a second order polynomial
      // p are the coefficients p[0] + p[1]*x + p[2]*y +p[3]*xy + p[4]*x^2 + p[5]*y^2
      Eigen::MatrixXd A(numTuples, ZeissImportConstants::PolynomialOrder::NumConsts2ndOrder);
      Eigen::VectorXd B(numTuples);

      for(size_t i = 0; i < numTuples; ++i)
      {
        xval = static_cast<int>(i / dims[0]);
        yval = static_cast<int>(i % dims[0]);
        B(i) = static_cast<double>(accumArray[i]);
        A(i, 0) = 1.0;
        A(i, 1) = static_cast<double>(xval);
        A(i, 2) = static_cast<double>(yval);
        A(i, 3) = static_cast<double>(xval * yval);
        A(i, 4) = static_cast<double>(xval * xval);
        A(i, 5) = static_cast<double>(yval * yval);
      }

      notifyStatusMessage("Fitting a polynomial to data. May take a while to solve if images are large");
      Eigen::VectorXd p = A.colPivHouseholderQr().solve(B);

      QVector<size_t> tDims(3);
      tDims[0] = dims[0];
      tDims[1] = dims[1];
      tDims[2] = dims[2];

      //  m->getAttributeMatrix(getOutputCellAttributeMatrixPath())->resizeAttributeArrays(tDims);
      //  if(nullptr != m_BackgroundImagePtr.lock())                          /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
      //  { m_BackgroundImage = m_BackgroundImagePtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */

      Eigen::VectorXd Bcalc(numTuples);
      double average = 0;

      Bcalc = A * p;
      average = Bcalc.mean();
      Bcalc = Bcalc - Eigen::VectorXd::Constant(numTuples, average);

      for(int i = 0; i < numTuples; ++i)
      {
        accumArray[i] = Bcalc(i);
      }
    } // Polynomial
#endif
