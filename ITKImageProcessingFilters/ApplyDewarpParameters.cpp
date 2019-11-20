/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
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

#include "ApplyDewarpParameters.h"

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include "tbb/queuing_mutex.h"
using MutexType = tbb::queuing_mutex;
#endif

#include <itkAmoebaOptimizer.h>
#include <itkFFTConvolutionImageFilter.h>
#include <itkNumericTraits.h>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/Common/SIMPLRange.h"

#include "SIMPLib/CoreFilters/ConvertColorToGrayScale.h"
#include "SIMPLib/DataContainers/DataArrayPath.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/FilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/MontageSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/MontageStructureSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Montages/GridMontage.h"
#include "SIMPLib/Utilities/ParallelData2DAlgorithm.h"
#include "SIMPLib/Utilities/ParallelTaskAlgorithm.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/FFTConvolutionCostFunction.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/FilterParameters/EbsdWarpPolynomialFilterParameter.h"

using Grayscale_T = uint8_t;
using PixelValue_T = double;

namespace
{
const QString InternalGrayscalePrefex = "_INTERNAL_Grayscale_";

std::vector<double> convertParams2Vec(const FFTHelper::ParametersType& params)
{
  const size_t size = params.size();
  std::vector<double> vec(size);
  for(size_t i = 0; i < size; i++)
  {
    vec[i] = params[i];
  }
  return vec;
}

FFTHelper::ParametersType convertVec2Params(const std::vector<double>& vec)
{
  const size_t size = vec.size();
  FFTHelper::ParametersType params(size);
  for(size_t i = 0; i < size; i++)
  {
    params[i] = vec[i];
  }
  return params;
}

std::list<double> convertParams2List(const FFTHelper::ParametersType& params)
{
  std::list<double> list;
  for(const auto& param : params)
  {
    list.push_back(param);
  }
  return list;
}

FFTHelper::PixelTypei pixelTypeFromSizeVec(const SizeVec2Type& sizeVec)
{
  return FFTHelper::PixelTypei{static_cast<int64_t>(sizeVec[0]), static_cast<int64_t>(sizeVec[1])};
}
SizeVec2Type pixelTypeToSizeVec(const FFTHelper::PixelTypei& pixelType)
{
  return SizeVec2Type(static_cast<size_t>(pixelType[0]), static_cast<size_t>(pixelType[1]));
}
size_t flatten(const SizeVec2Type& xyPos, const SizeVec3Type& dimensions)
{
  const size_t x = xyPos[0];
  const size_t y = xyPos[1];

  const size_t width = dimensions[0];

  return x + y * width;
}
size_t flatten(const FFTHelper::PixelTypei& xyPos, const SizeVec3Type& dimensions)
{
  return flatten(pixelTypeToSizeVec(xyPos), dimensions);
}

template <typename T>
void clearTuple(const typename DataArray<T>::Pointer& da, size_t numComponents, size_t tupleId)
{
  T initValue = da->getInitValue();
  typename std::vector<T> blankTuple(numComponents, initValue);
  da->setTuple(tupleId, blankTuple);
}

template <typename T>
void transformDataPixel(double x_trans, double y_trans, const SizeVec2Type& newPixel, const FFTHelper::ParametersType& parameters, const SizeVec3Type& dimensions,
                        const typename DataArray<T>::Pointer& da, const typename DataArray<T>::Pointer& tempDACopy)
{
  FFTHelper::PixelTypei offset = FFTHelper::pixelType(x_trans, y_trans);
  FFTHelper::PixelTypei oldPixelIndex = FFTHelper::getOldIndex(FFTHelper::pixelType(newPixel[0], newPixel[1]), offset, parameters);

  auto compDims = tempDACopy->getComponentDimensions();
  size_t numComponents = std::accumulate(compDims.begin(), compDims.end(), 0);
  size_t newIndex = flatten(newPixel, dimensions);

  // Cannot flatten invalid { X,Y } positions
  if(oldPixelIndex[0] < 0 || oldPixelIndex[1] < 0 || oldPixelIndex[0] >= dimensions[0] || oldPixelIndex[1] >= dimensions[1])
  {
    clearTuple<T>(tempDACopy, numComponents, newIndex);
    return;
  }

  size_t oldIndex = flatten(oldPixelIndex, dimensions);
  if((oldIndex >= da->getNumberOfTuples()) || (newIndex >= da->getNumberOfTuples()))
  {
    return;
  }

  T* oldTuplePtr = tempDACopy->getTuplePointer(oldIndex);
  da->setTuple(newIndex, oldTuplePtr);
}

template <typename T>
void transformDataArray(const FFTHelper::ParametersType& parameters, const SizeVec3Type& dimensions, double x_trans, double y_trans, const typename DataArray<T>::Pointer& da)
{
  // Do not resize items that do not match the geometry.
  size_t flattenedDims = std::accumulate(dimensions.begin(), dimensions.end(), 1, std::multiplies<double>());
  size_t numComps = da->getNumberOfComponents();
  size_t totalItems = da->getNumberOfTuples() * numComps;
  if(totalItems / numComps != flattenedDims)
  {
    return;
  }

  typename DataArray<T>::Pointer daCopy = std::dynamic_pointer_cast<DataArray<T>>(da->deepCopy());

  const size_t width = dimensions[0];
  const size_t height = dimensions[1];

  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  // Loop over pixels
  for(size_t x = 0; x < width; x++)
  {
    for(size_t y = 0; y < height; y++)
    {
      SizeVec2Type newPixel{x, y};

      fn = std::bind(&transformDataPixel<T>, x_trans, y_trans, newPixel, parameters, dimensions, da, daCopy);
      taskAlg.execute(fn);
    }
  }
  taskAlg.wait();
}

void transformIDataArray(const FFTHelper::ParametersType& parameters, const SizeVec3Type& dimensions, double x_trans, double y_trans, const IDataArray::Pointer& da)
{
  if(std::dynamic_pointer_cast<Int8ArrayType>(da))
  {
    Int8ArrayType::Pointer array = std::dynamic_pointer_cast<Int8ArrayType>(da);
    transformDataArray<int8_t>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<UInt8ArrayType>(da))
  {
    UInt8ArrayType::Pointer array = std::dynamic_pointer_cast<UInt8ArrayType>(da);
    transformDataArray<uint8_t>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<Int16ArrayType>(da))
  {
    Int16ArrayType::Pointer array = std::dynamic_pointer_cast<Int16ArrayType>(da);
    transformDataArray<int16_t>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<UInt16ArrayType>(da))
  {
    UInt16ArrayType::Pointer array = std::dynamic_pointer_cast<UInt16ArrayType>(da);
    transformDataArray<uint16_t>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<Int32ArrayType>(da))
  {
    Int32ArrayType::Pointer array = std::dynamic_pointer_cast<Int32ArrayType>(da);
    transformDataArray<int32_t>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<UInt32ArrayType>(da))
  {
    UInt32ArrayType::Pointer array = std::dynamic_pointer_cast<UInt32ArrayType>(da);
    transformDataArray<uint32_t>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<Int64ArrayType>(da))
  {
    Int64ArrayType::Pointer array = std::dynamic_pointer_cast<Int64ArrayType>(da);
    transformDataArray<int64_t>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<UInt64ArrayType>(da))
  {
    UInt64ArrayType::Pointer array = std::dynamic_pointer_cast<UInt64ArrayType>(da);
    transformDataArray<uint64_t>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<FloatArrayType>(da))
  {
    FloatArrayType::Pointer array = std::dynamic_pointer_cast<FloatArrayType>(da);
    transformDataArray<float>(parameters, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<DoubleArrayType>(da))
  {
    DoubleArrayType::Pointer array = std::dynamic_pointer_cast<DoubleArrayType>(da);
    transformDataArray<double>(parameters, dimensions, x_trans, y_trans, array);
  }
}
} // namespace

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ApplyDewarpParameters::ApplyDewarpParameters()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ApplyDewarpParameters::~ApplyDewarpParameters() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_MONTAGE_STRUCTURE_SELECTION_FP("Montage Name", MontageName, FilterParameter::Category::Parameter, ApplyDewarpParameters));

  // parameters.push_back(SIMPL_NEW_EBSDWARPPOLYNOMIAL_FP("X Factors", XFactors, FilterParameter::Parameter, ApplyDewarpParameters));
  // parameters.push_back(SIMPL_NEW_EBSDWARPPOLYNOMIAL_FP("Y Factors", YFactors, FilterParameter::Parameter, ApplyDewarpParameters));

  parameters.push_back(SIMPL_NEW_STRING_FP("Attribute Matrix Name", AttributeMatrixName, FilterParameter::Category::Parameter, ApplyDewarpParameters));
  DataArraySelectionFilterParameter::RequirementType req;
  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Transform Array", TransformPath, FilterParameter::Category::Parameter, ApplyDewarpParameters, req));

  parameters.push_back(SIMPL_NEW_STRING_FP("Transformed Data Container Prefix", TransformPrefix, FilterParameter::Category::CreatedArray, ApplyDewarpParameters));
  parameters.push_back(SIMPL_NEW_STRING_FP("Mask Array Name", MaskName, FilterParameter::Category::CreatedArray, ApplyDewarpParameters));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  if(!checkMontageRequirements())
  {
    return;
  }

  DataContainerArray::Pointer dca = getDataContainerArray();
  GridMontage::Pointer oldMontage = std::dynamic_pointer_cast<GridMontage>(dca->getPrereqMontage(this, m_MontageName));
  if(nullptr == oldMontage)
  {
    QString ss = tr("Unable to find the required GridMontage, %1").arg(m_MontageName);
    setErrorCondition(-6600, ss);
    return;
  }

  const QString newMontageName = m_TransformPrefix + m_MontageName;
  GridMontage::Pointer newMontage = dca->createNonPrereqGridMontage(this, newMontageName, oldMontage->getGridSize());

  // The data container holds a single output attribute matrix with 3 data arrays
  // One for the number of iterations taken
  // One for the transform array
  // One for the output of the transform (the optimized value that is the
  // minimization of the square root of the sum for each overlap
  // of the max of a convolution of two overlapping FFT'd images

  const size_t numRows = newMontage->getRowCount();
  const size_t numCols = newMontage->getColumnCount();
  for(size_t row = 0; row < numRows; row++)
  {
    for(size_t col = 0; col < numCols; col++)
    {
      GridTileIndex index = newMontage->getTileIndex(row, col);
      DataContainerShPtr dc = oldMontage->getDataContainer(index);

      const QString newName = m_TransformPrefix + dc->getName();
      DataContainerShPtr dcCopy = DataContainer::New(newName);
      dcCopy->setGeometry(dc->getGeometry()->deepCopy());
      AttributeMatrix::Pointer dewarpAM = dc->getAttributeMatrix(m_AttributeMatrixName)->deepCopy();
      dcCopy->insertOrAssign(dewarpAM);

      // Create Mask array
      dewarpAM->createNonPrereqArray<UInt8ArrayType>(this, m_MaskName, 255, {1});

      getDataContainerArray()->insertOrAssign(dcCopy);
      newMontage->setDataContainer(index, dcCopy);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::preflight()
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
void ApplyDewarpParameters::execute()
{
  initialize();
  dataCheck();

  if(getErrorCode() < 0 || getCancel())
  {
    return;
  }

  std::vector<double> pxy = getPxyVec();

  double imageDimX;
  double imageDimY;
  std::tie(imageDimX, imageDimY) = getImageDims();
  // Apply transform to dewarp data
  warpDataContainers(::convertVec2Params(pxy), imageDimX, imageDimY);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool ApplyDewarpParameters::checkMontageRequirements()
{
  // Requirements to avoid crashing
  if(getDataContainerArray()->getDataContainers().empty())
  {
    setErrorCondition(-66710, "At least one DataContainer required");
    return false;
  }

  AbstractMontageShPtr montage = getDataContainerArray()->getMontage(getMontageName());
  if(nullptr == montage)
  {
    setErrorCondition(-66750, QString("Montage: %1 required").arg(getMontageName()));
    return false;
  }

  // All of the types in the chosen data container's image data arrays should be the same
  for(const auto& dc : montage->getDataContainers())
  {
    if(nullptr == dc)
    {
      setErrorCondition(-66715, QString("Montage tiles must be empty"));
      return false;
    }

    AttributeMatrix::Pointer am = dc->getAttributeMatrix(m_AttributeMatrixName);
    if(nullptr == am)
    {
      setErrorCondition(-66720, QString("AttributeMatrix: %1 / %2 required").arg(dc->getName()).arg(m_AttributeMatrixName));
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
size_t ApplyDewarpParameters::getSingleParamCount() const
{
  return FFTHelper::getReqPartialParameterSize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> ApplyDewarpParameters::getPxyVec()
{
  const size_t halfSize = getSingleParamCount();
  const size_t fullSize = 2 * halfSize;
  std::vector<double> pxy(fullSize, 0);

  // if(m_UseDataArray)
  {
    DoubleArrayType::Pointer arr = getDataContainerArray()->getPrereqArrayFromPath<DoubleArrayType>(this, m_TransformPath, {fullSize});
    double* data = arr->data();
    for(size_t i = 0; i < pxy.size(); i++)
    {
      pxy[i] = data[i];
    }
  }
#if 0
  else
  {
    for(size_t i = 0; i < halfSize; i++)
    {
      pxy[i] = m_XFactors[i];
      pxy[i + halfSize] = m_YFactors[i];
    }
  }
#endif

  return pxy;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::warpDataContainers(const FFTHelper::ParametersType& parameters, double imageDimX, double imageDimY)
{
  // Duplicate the DataContainers used and Warp them based on the transformVector generated.
  AbstractMontage::Pointer montage = getDataContainerArray()->getMontage(m_MontageName);
  for(const auto& dcOrig : *montage)
  {
    DataContainerShPtr dc = getDataContainerArray()->getDataContainer(m_TransformPrefix + dcOrig->getName());
    ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
    SizeVec3Type dimensions = imageGeom->getDimensions();

    double xDif = std::max<double>(dimensions[0] - imageDimX, 0);
    double yDif = std::max<double>(dimensions[1] - imageDimY, 0);

    double x_trans = (imageDimX - 1) / 2.0 + xDif;
    double y_trans = (imageDimY - 1) / 2.0 + yDif;

    AttributeMatrix::Pointer am = dc->getAttributeMatrix(m_AttributeMatrixName);
    {
      // Mask Array is initialized with true values.  Use false when warping.
      UInt8ArrayType::Pointer maskArray = am->getAttributeArrayAs<UInt8ArrayType>(m_MaskName);
      maskArray->setInitValue(0);

      for(const auto& da : *am)
      {
        transformIDataArray(parameters, dimensions, x_trans, y_trans, da);
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::pair<double, double> ApplyDewarpParameters::getImageDims() const
{
  AbstractMontage::Pointer absMontage = getDataContainerArray()->getMontage(m_MontageName);
  GridMontage::Pointer gridMontage = std::dynamic_pointer_cast<GridMontage>(absMontage);
  if(nullptr == gridMontage)
  {
    return {0, 0};
  }

  size_t targetColumn = gridMontage->getColumnCount() > 2 ? 1 : 0;
  size_t targetRow = gridMontage->getRowCount() > 2 ? 1 : 0;

  GridTileIndex tileX = gridMontage->getTileIndex(0, targetColumn);
  ImageGeom::Pointer imgGeomX = gridMontage->getDataContainer(tileX)->getGeometryAs<ImageGeom>();
  if(nullptr == imgGeomX)
  {
    return {0, 0};
  }

  double imageDimX = imgGeomX->getDimensions()[0];

  GridTileIndex tileY = gridMontage->getTileIndex(targetRow, 0);
  ImageGeom::Pointer imgGeomY = gridMontage->getDataContainer(tileY)->getGeometryAs<ImageGeom>();
  double imageDimY = imgGeomY->getDimensions()[1];
  if(nullptr == imgGeomY)
  {
    return {0, 0};
  }

  return {imageDimX, imageDimY};
}
//#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ApplyDewarpParameters::newFilterInstance(bool copyFilterParameters) const
{
  ApplyDewarpParameters::Pointer filter = ApplyDewarpParameters::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return std::move(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getSubGroupName() const
{
  return "Processing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getHumanLabel() const
{
  return "Apply Dewarp Parameters";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ApplyDewarpParameters::getUuid() const
{
  return QUuid("{7c2a7ce-4582-52a6-2de-16b62d43fbf}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ApplyDewarpParameters::Pointer ApplyDewarpParameters::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ApplyDewarpParameters::Pointer ApplyDewarpParameters::New()
{
  struct make_shared_enabler : public ApplyDewarpParameters
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getNameOfClass() const
{
  return "ApplyDewarpParameters";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::ClassName()
{
  return "ApplyDewarpParameters";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getMontageName() const
{
  return m_MontageName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setMontageName(const QString& value)
{
  m_MontageName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getAttributeMatrixName() const
{
  return m_AttributeMatrixName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setAttributeMatrixName(const QString& value)
{
  m_AttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getMaskName() const
{
  return m_MaskName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setMaskName(const QString& value)
{
  m_MaskName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DataArrayPath ApplyDewarpParameters::getTransformPath() const
{
  return m_TransformPath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setTransformPath(const DataArrayPath& value)
{
  m_TransformPath = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ApplyDewarpParameters::getTransformPrefix() const
{
  return m_TransformPrefix;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setTransformPrefix(const QString& value)
{
  m_TransformPrefix = value;
}

// -----------------------------------------------------------------------------
bool ApplyDewarpParameters::getUseDataArray() const
{
  return m_UseDataArray;
}

// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setUseDataArray(bool value)
{
  m_UseDataArray = value;
}

#if 0
// -----------------------------------------------------------------------------
FloatVec7Type ApplyDewarpParameters::getXFactors() const
{
  return m_XFactors;
}
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setXFactors(const FloatVec7Type& value)
{
  m_XFactors = value;
}
// -----------------------------------------------------------------------------
FloatVec7Type ApplyDewarpParameters::getYFactors() const
{
  return m_YFactors;
}
// -----------------------------------------------------------------------------
void ApplyDewarpParameters::setYFactors(const FloatVec7Type& value)
{
  m_YFactors = value;
}
#endif
