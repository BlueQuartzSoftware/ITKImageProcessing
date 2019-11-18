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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
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

#include "Blend.h"

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
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/FilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/MontageSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/MontageStructureSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
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
}

uint Blend::GetIterationsFromStopDescription(const QString& stopDescription) const
{
  if(GetConvergenceFromStopDescription(stopDescription))
  {
    const QString startStr = "have been met in ";
    const int startIdx = stopDescription.indexOf(startStr) + startStr.length();
    return stopDescription.midRef(startIdx, stopDescription.indexOf(" iterations") - startIdx).toUInt();
  }
  return m_MaxIterations;
}

bool Blend::GetConvergenceFromStopDescription(const QString& stopDescription) const
{
  return stopDescription.contains("have been met");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Blend::Blend()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Blend::~Blend() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_MONTAGE_STRUCTURE_SELECTION_FP("Montage Name", MontageName, FilterParameter::Category::Parameter, Blend));

  QStringList linkedAmoebaProps{"MaxIterations", "OverlapAmt", "LowTolerance", "HighTolerance"};
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Use Amoeba Optimizer", UseAmoebaOptimizer, FilterParameter::Parameter, Blend, linkedAmoebaProps));

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Max Iterations", MaxIterations, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Function Convergence Tolerance", LowTolerance, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Parameter Convergence Tolerance", HighTolerance, FilterParameter::Category::Parameter, Blend));

  QStringList linkedSpecifySimplexProps{"XFactors", "YFactors"};
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Specify Initial Simplex", SpecifyInitialSimplex, FilterParameter::Parameter, Blend, linkedSpecifySimplexProps));

  parameters.push_back(SIMPL_NEW_EBSDWARPPOLYNOMIAL_FP("X Factors", XFactors, FilterParameter::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_EBSDWARPPOLYNOMIAL_FP("Y Factors", YFactors, FilterParameter::Parameter, Blend));

  parameters.push_back(SIMPL_NEW_STRING_FP("Attribute Matrix Name", AttributeMatrixName, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("IPF Colors Array Name", IPFColorsArrayName, FilterParameter::Category::Parameter, Blend));

  // SIMPL_NEW_LINKED_BOOL_FP
  parameters.push_back(SeparatorFilterParameter::New("Transformation Arrays", FilterParameter::CreatedArray));
  QStringList linkedProps{"BlendDCName", "TransformMatrixName", "TransformArrayName", "ResidualArrayName"};
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Create Transformation Container", CreateTransformContainer, FilterParameter::CreatedArray, Blend, linkedProps));

  parameters.push_back(SIMPL_NEW_STRING_FP("Transform Container", BlendDCName, FilterParameter::Category::CreatedArray, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Transform Matrix", TransformMatrixName, FilterParameter::Category::CreatedArray, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Transform Array", TransformArrayName, FilterParameter::Category::CreatedArray, Blend));
  // parameters.push_back(SIMPL_NEW_STRING_FP("Number of Iterations", NumIterationsArrayName, FilterParameter::Category::CreatedArray, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Residual Values", ResidualArrayName, FilterParameter::Category::CreatedArray, Blend));

  parameters.push_back(SeparatorFilterParameter::New("Created Transformation", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Transformed Data Container Prefix", TransformPrefix, FilterParameter::Category::CreatedArray, Blend));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  if(!checkMontageRequirements())
  {
    return;
  }

  // The data container holds a single output attribute matrix with 3 data arrays
  // One for the number of iterations taken
  // One for the transform array
  // One for the output of the transform (the optimized value that is the
  // minimization of the square root of the sum for each overlap
  // of the max of a convolution of two overlapping FFT'd images

  if(m_CreateTransformContainer)
  {
    // Create a new data container to hold the output of this filter
    DataContainerShPtr blendDC = getDataContainerArray()->createNonPrereqDataContainer(this, DataArrayPath(m_BlendDCName, "", ""));
    AttributeMatrixShPtr blendAM = blendDC->createNonPrereqAttributeMatrix(this, m_TransformMatrixName, {1}, AttributeMatrix::Type::Generic);

    // blendAM->createAndAddAttributeArray<UInt64ArrayType>(this, m_IterationsAAName, 0, {1});
    blendAM->createNonPrereqArray<DoubleArrayType>(this, m_TransformArrayName, 0, {getSingleParamCount()});
    blendAM->createNonPrereqArray<DoubleArrayType>(this, m_ResidualArrayName, 0, {1});
  }

  AbstractMontageShPtr montage = getDataContainerArray()->getMontage(getMontageName());
  QStringList dcNames = montage->getDataContainerNames();
  for(const QString& dcName : dcNames)
  {
    DataContainerShPtr dc = getDataContainerArray()->getDataContainer(dcName);
    DataContainerShPtr dcCopy = DataContainer::New(m_TransformPrefix + dc->getName());
    dcCopy->setGeometry(dc->getGeometry()->deepCopy());
    dcCopy->insertOrAssign(dc->getAttributeMatrix(m_AttributeMatrixName)->deepCopy());
    getDataContainerArray()->insertOrAssign(dcCopy);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::preflight()
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
void Blend::execute()
{
  initialize();
  dataCheck();

  if(getErrorCode() < 0 || getCancel())
  {
    return;
  }

  // Generate internal grayscale values
  generateGrayscaleIPF();
  std::vector<double> xyParameters = getPxyVec();
  FFTHelper::ParametersType transformParams = ::convertVec2Params(xyParameters);

  if(m_UseAmoebaOptimizer)
  {
    // The optimizer needs an initial guess; this is supplied through a filter parameter
    FFTHelper::ParametersType initialParams = ::convertVec2Params(xyParameters);

    using CostFunctionType = FFTConvolutionCostFunction;
    CostFunctionType implementation;
    GridMontageShPtr gridMontage = std::dynamic_pointer_cast<GridMontage>(getDataContainerArray()->getMontage(getMontageName()));
    implementation.Initialize(gridMontage, getDataContainerArray(), m_AttributeMatrixName, getGrayscaleArrayName());

    // Calculate parameter step sizes
    const double imgX = implementation.getImageDimX();
    const double imgY = implementation.getImageDimY();
    FFTHelper::ParametersType stepSizes = ::convertVec2Params(getStepSizes(xyParameters, imgX, imgY));

    // Print out step sizes
    QString stepSizeStr = "Step Sizes: [ ";
    for(auto param : stepSizes)
    {
      stepSizeStr += QString::number(param) + "; ";
    }
    stepSizeStr += " ]";
    notifyStatusMessage(stepSizeStr);

    itk::AmoebaOptimizer::Pointer optimizer = itk::AmoebaOptimizer::New();
    optimizer->SetMaximumNumberOfIterations(m_MaxIterations);
    optimizer->SetFunctionConvergenceTolerance(m_LowTolerance);
    optimizer->SetParametersConvergenceTolerance(m_HighTolerance);
    optimizer->SetInitialPosition(initialParams);
    optimizer->SetInitialSimplexDelta(stepSizes);
    optimizer->SetOptimizeWithRestarts(true);

    optimizer->SetCostFunction(&implementation);
    optimizer->MaximizeOn(); // Search for the greatest value
    optimizer->StartOptimization();

    // Newer versions of the optimizer allow for easier methods of output information
    // to be obtained, but until then, we have to do some string parsing from the
    // optimizer's stop description
    QString stopReason = QString::fromStdString(optimizer->GetStopConditionDescription());
    transformParams = optimizer->GetCurrentPosition();

    // cache value
    auto value = optimizer->GetValue();
    auto numIterations = GetIterationsFromStopDescription(stopReason);

    // Can get rid of these qDebug lines after debugging is done for filter
    QString initParamStr = "Initial Position: [ ";
    for(auto param : xyParameters)
    {
      initParamStr += QString::number(param) + "; ";
    }
    initParamStr += " ]";
    notifyStatusMessage(initParamStr);

    QString finalParamStr = "Final Position: [ ";
    for(auto param : transformParams)
    {
      finalParamStr += QString::number(param) + "; ";
    }
    finalParamStr += " ]";
    notifyStatusMessage(finalParamStr);

    notifyStatusMessage(QString::fromStdString(optimizer->GetStopConditionDescription()));
    std::list<double> transform = ::convertParams2List(transformParams);

    qDebug() << "Initial Position: [ " << xyParameters << " ]";
    qDebug() << "Final Position: [ " << transform << " ]";
    qDebug() << "Number of Iterations: " << numIterations;
    qDebug() << "Value: " << value;

#if 0
    // If the optimization didn't converge, set an error...
    if(!GetConvergenceFromStopDescription(stopReason))
    {
      setErrorCondition(-66850, stopReason);
      notifyStatusMessage(stopReason);
      // Write the stop reason to the console
      qDebug() << "Stop Reason: " << stopReason;
      return;
    }
#endif

    // ...otherwise, set the appropriate values of the filter's output data arrays
    if(m_CreateTransformContainer)
    {
      AttributeMatrixShPtr transformAM = getDataContainerArray()->getDataContainer(m_BlendDCName)->getAttributeMatrix(m_TransformMatrixName);
      // transformAM->getAttributeArrayAs<UInt64ArrayType>(m_IterationsAAName)->push_back(numIterations);
      transformAM->getAttributeArrayAs<DoubleArrayType>(m_ResidualArrayName)->push_back(value);
      transformAM->getAttributeArrayAs<DoubleArrayType>(m_TransformArrayName)->setArray(transform);
    }
  }
  else
  {
    if(m_CreateTransformContainer)
    {
      AttributeMatrixShPtr transformAM = getDataContainerArray()->getDataContainer(m_BlendDCName)->getAttributeMatrix(m_TransformMatrixName);
      transformAM->getAttributeArrayAs<DoubleArrayType>(m_TransformArrayName)->setArray({transformParams.begin(), transformParams.end()});
    }
  }

  // Remove internal arrays
  deleteGrayscaleIPF();

  double imageDimX;
  double imageDimY;
  std::tie(imageDimX, imageDimY) = getImageDims();
  // Apply transform to dewarp data
  warpDataContainers(transformParams, imageDimX, imageDimY);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool Blend::checkMontageRequirements()
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

    DataArray<Grayscale_T>::Pointer da = am->getAttributeArrayAs<DataArray<Grayscale_T>>(m_IPFColorsArrayName);
    if(nullptr == da)
    {
      setErrorCondition(-66730, QString("DataArray: %1 / %2 / %3 required").arg(dc->getName()).arg(m_AttributeMatrixName).arg(m_IPFColorsArrayName));
      return false;
    }

    QString typeName = da->getTypeAsString();
    if(da->getComponentDimensions().size() > 1)
    {
      setErrorCondition(-66700, "Data array has unexpected dimensions");
    }
    else if(da->getTypeAsString() != typeName)
    {
      setErrorCondition(-66800, "Not all data attribute arrays are the same type");
    }
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
size_t Blend::getSingleParamCount() const
{
  return FFTHelper::getReqPartialParameterSize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double calcDelta(double maxDelta, double param, double mMax)
{
  return maxDelta / mMax - param;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> Blend::getStepSizes(const std::vector<double>& params, size_t imgDimX, size_t imgDimY) const
{
  const size_t count = FFTHelper::getReqParameterSize();
  std::vector<double> stepSizes(count);
  
  const size_t xMax = (imgDimX / 2);// *(imgDimX / 2);
  const size_t yMax = (imgDimY / 2);// *(imgDimY / 2);

  // Px
  stepSizes[0] = 1 + calcDelta(m_Delta, params[0], xMax);
  stepSizes[1] = calcDelta(m_Delta, params[1], yMax);
  stepSizes[2] = calcDelta(m_Delta, params[2], xMax * xMax);
  stepSizes[3] = calcDelta(m_Delta, params[3], yMax * yMax);
  stepSizes[4] = calcDelta(m_Delta, params[4], xMax * yMax);
  stepSizes[5] = calcDelta(m_Delta, params[5], xMax * xMax * yMax);
  stepSizes[6] = calcDelta(m_Delta, params[6], xMax * yMax * yMax);

  // Py
  stepSizes[7] = calcDelta(m_Delta, params[7], xMax);
  stepSizes[8] = 1 + calcDelta(m_Delta, params[8], yMax);
  stepSizes[9] = calcDelta(m_Delta, params[9], xMax * xMax);
  stepSizes[10] = calcDelta(m_Delta, params[10], yMax * yMax);
  stepSizes[11] = calcDelta(m_Delta, params[11], xMax * yMax);
  stepSizes[12] = calcDelta(m_Delta, params[12], xMax * xMax * yMax);
  stepSizes[13] = calcDelta(m_Delta, params[13], xMax * yMax * yMax);

  return stepSizes;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> Blend::getPxyVec() const
{
  size_t halfSize = getSingleParamCount();
  std::vector<double> pxy(2 * halfSize, 0);

  if(m_SpecifyInitialSimplex)
  {
    for(size_t i = 0; i < halfSize; i++)
    {
      pxy[i] = m_XFactors[i];
      pxy[i + halfSize] = m_YFactors[i];
    }
  }
  else
  {
    std::vector<double> px = getDefaultPx();
    std::vector<double> py = getDefaultPy();
    for(size_t i = 0; i < halfSize; i++)
    {
      pxy[i] = px[i];
      pxy[i + halfSize] = py[i];
    }
  }

  return pxy;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> Blend::getDefaultPx() const
{
  size_t len = getSingleParamCount();
  std::vector<double> px(len, 0);
  px[0] = 1;
  return px;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> Blend::getDefaultPy() const
{
  size_t len = getSingleParamCount();
  std::vector<double> py(len, 0);
  py[1] = 1;
  return py;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getGrayscaleArrayName() const
{
  return ::InternalGrayscalePrefex + m_IPFColorsArrayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::generateGrayscaleIPF()
{
  ConvertColorToGrayScale::Pointer conversionFilter = ConvertColorToGrayScale::New();
  conversionFilter->setConversionAlgorithm(static_cast<int>(ConvertColorToGrayScale::ConversionType::Luminosity));
  conversionFilter->setOutputArrayPrefix(::InternalGrayscalePrefex);

  AbstractMontage::Pointer montage = getDataContainerArray()->getMontage(getMontageName());
  AbstractMontage::CollectionType dcs = montage->getDataContainers();
  for(const auto& dc : dcs)
  {
    // Find IPF Colors
    AttributeMatrix::Pointer am = dc->getAttributeMatrix(m_AttributeMatrixName);
    DataArray<Grayscale_T>::Pointer ipfArray = am->getAttributeArrayAs<DataArray<Grayscale_T>>(m_IPFColorsArrayName);

    // Create internal grayscale version
    conversionFilter->setDataContainerArray(getDataContainerArray());
    QVector<DataArrayPath> dataArrayPaths;
    dataArrayPaths.push_back(DataArrayPath(dc->getName(), m_AttributeMatrixName, m_IPFColorsArrayName));
    conversionFilter->setInputDataArrayVector(dataArrayPaths);

    conversionFilter->execute();
    int err = conversionFilter->getErrorCode();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::deleteGrayscaleIPF()
{
  AbstractMontage::Pointer montage = getDataContainerArray()->getMontage(getMontageName());
  AbstractMontage::CollectionType dcs = montage->getDataContainers();
  for(const auto& dc : dcs)
  {
    // Find internal IPF grayscale
    AttributeMatrix::Pointer am = dc->getAttributeMatrix(m_AttributeMatrixName);

    const QString internalArrayName = getGrayscaleArrayName();
    if(am->hasChildWithName(internalArrayName))
    {
      am->removeAttributeArray(internalArrayName);
    }
  }
}

#if 1
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTHelper::PixelTypei pixelTypeFromSizeVec(const SizeVec2Type& sizeVec)
{
  return FFTHelper::PixelTypei{ static_cast<int64_t>(sizeVec[0]), static_cast<int64_t>(sizeVec[1]) };
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
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
    T initValue = tempDACopy->getInitValue();
    typename std::vector<T> blankTuple(numComponents, initValue);
    da->setTuple(newIndex, blankTuple);
    return;
  }

  size_t oldIndex = flatten(oldPixelIndex, dimensions);
  if((oldIndex >= da->getNumberOfTuples()) || (newIndex >= da->getNumberOfTuples()))
  {
    return;
  }

  T* oldTuplePtr = tempDACopy->getTuplePointer(oldIndex);
  typename std::vector<T> oldTuple(oldTuplePtr, oldTuplePtr + numComponents);

  da->setTuple(newIndex, oldTuple);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::warpDataContainers(const FFTHelper::ParametersType& parameters, double imageDimX, double imageDimY)
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
      for(const auto& da : *am)
      {
        transformIDataArray(parameters, dimensions, x_trans, y_trans, da);
      }
      //DataArray<bool>::Pointer maskArray = createMaskArray(parameters, dimensions, x_trans, y_trans, m_MaskArrayName);
      //am->addOrReplaceAttributeArray(maskArray);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::pair<double, double> Blend::getImageDims() const
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
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer Blend::newFilterInstance(bool copyFilterParameters) const
{
  Blend::Pointer filter = Blend::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return std::move(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getSubGroupName() const
{
  return "Processing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getHumanLabel() const
{
  return "Blend";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid Blend::getUuid() const
{
  return QUuid("{f2c81aa6-8edf-51d8-8c30-81ad983815e9}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Blend::Pointer Blend::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Blend::Pointer Blend::New()
{
  struct make_shared_enabler : public Blend
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getNameOfClass() const
{
  return "Blend";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::ClassName()
{
  return "Blend";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getMontageName() const
{
  return m_MontageName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setMontageName(const QString& value)
{
  m_MontageName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint Blend::getMaxIterations() const
{
  return m_MaxIterations;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setMaxIterations(uint value)
{
  m_MaxIterations = value;
}

#if 0
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IntVec2Type Blend::getOverlapAmt() const
{
  return m_OverlapAmt;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setOverlapAmt(const IntVec2Type& value)
{
  m_OverlapAmt = value;
}
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double Blend::getLowTolerance() const
{
  return m_LowTolerance;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setLowTolerance(double value)
{
  m_LowTolerance = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double Blend::getHighTolerance() const
{
  return m_HighTolerance;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setHighTolerance(double value)
{
  m_HighTolerance = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int Blend::getDelta() const
{
  return m_Delta;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setDelta(int value)
{
  m_Delta = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool Blend::getUseAmoebaOptimizer() const
{
  return m_UseAmoebaOptimizer;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setUseAmoebaOptimizer(bool value)
{
  m_UseAmoebaOptimizer = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool Blend::getSpecifyInitialSimplex() const
{
  return m_SpecifyInitialSimplex;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setSpecifyInitialSimplex(bool value)
{
  m_SpecifyInitialSimplex = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getAttributeMatrixName() const
{
  return m_AttributeMatrixName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setAttributeMatrixName(const QString& value)
{
  m_AttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getIPFColorsArrayName() const
{
  return m_IPFColorsArrayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setIPFColorsArrayName(const QString& value)
{
  m_IPFColorsArrayName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool Blend::getCreateTransformContainer() const
{
  return m_CreateTransformContainer;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setCreateTransformContainer(bool value)
{
  m_CreateTransformContainer = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getBlendDCName() const
{
  return m_BlendDCName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setBlendDCName(const QString& value)
{
  m_BlendDCName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getTransformMatrixName() const
{
  return m_TransformMatrixName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setTransformMatrixName(const QString& value)
{
  m_TransformMatrixName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getTransformArrayName() const
{
  return m_TransformArrayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setTransformArrayName(const QString& value)
{
  m_TransformArrayName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getResidualArrayName() const
{
  return m_ResidualArrayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setResidualArrayName(const QString& value)
{
  m_ResidualArrayName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString Blend::getTransformPrefix() const
{
  return m_TransformPrefix;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setTransformPrefix(const QString& value)
{
  m_TransformPrefix = value;
}
// -----------------------------------------------------------------------------
FloatVec7Type Blend::getXFactors() const
{
  return m_XFactors;
}
// -----------------------------------------------------------------------------
void Blend::setXFactors(const FloatVec7Type& value)
{
  m_XFactors = value;
}
// -----------------------------------------------------------------------------
FloatVec7Type Blend::getYFactors() const
{
  return m_YFactors;
}
// -----------------------------------------------------------------------------
void Blend::setYFactors(const FloatVec7Type& value)
{
  m_YFactors = value;
}
