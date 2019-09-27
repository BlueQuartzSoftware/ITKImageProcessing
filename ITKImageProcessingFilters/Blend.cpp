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

using Grayscale_T = uint8_t;
using PixelValue_T = double;

namespace
{
const QString InternalGrayscalePrefex = "_INTERNAL_Grayscale_";
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
: m_MaxIterations(1000)
, m_Degree(2)
, m_OverlapAmt({ 10, 10 })
, m_LowTolerance(1E-2)
, m_HighTolerance(1E-2)
, m_UseAmoebaOptimizer(false)
, m_SpecifyInitialSimplex(true)
, m_PxStr("0.0;0.0;0.0;1.0;0.0;0.0;0.0")
, m_PyStr("0.0;1.0;0.0;0.0;0.0;0.0;0.0")
, m_BlendDCName("Blend Data")
, m_TransformMatrixName("Transform Matrix")
, m_TransformArrayName("Transform")
//, m_NumIterationsArrayName("Iterations")
, m_ResidualArrayName("Residual")
, m_TransformPrefix("Transformed_")
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
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Degree", Degree, FilterParameter::Category::Parameter, Blend));

  QStringList linkedAmoebaProps{"MaxIterations", "OverlapAmt", "LowTolerance", "HighTolerance"};
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Use Amoeba Optimizer", UseAmoebaOptimizer, FilterParameter::Parameter, Blend, linkedAmoebaProps));

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Max Iterations", MaxIterations, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Overlap Amount (Width, Height)", OverlapAmt, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Function Convergence Tolerance", LowTolerance, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Parameter Convergence Tolerance", HighTolerance, FilterParameter::Category::Parameter, Blend));

  QStringList linkedSpecifySimplexProps{"PxStr", "PyStr"};
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Specify Initial Simplex", SpecifyInitialSimplex, FilterParameter::Parameter, Blend, linkedSpecifySimplexProps));
  parameters.push_back(SIMPL_NEW_STRING_FP("Px", PxStr, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Py", PyStr, FilterParameter::Category::Parameter, Blend));

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

  if(m_Degree < 0)
  {
    setErrorCondition(-66509, "The degree must be non-negative");
  }

  if(m_SpecifyInitialSimplex)
  {
    // Need to make sure that the filter parameter for the initial guess
    // can be cast into actual numeric data
    m_PxVec.clear();
    for(const auto& coeff : m_PxStr.split(";"))
    {
      bool coerced = false;
      m_PxVec.push_back(coeff.toDouble(&coerced));
      if(!coerced)
      {
        QString str = QString("A Px coefficient (%1) could not be translated into a floating-point precision number").arg(coeff);
        setErrorCondition(-66500, str);
      }
    }

    // Need to make sure that the filter parameter for the initial guess
    // can be cast into actual numeric data
    m_PyVec.clear();
    for(const auto& coeff : m_PyStr.split(";"))
    {
      bool coerced = false;
      m_PyVec.push_back(coeff.toDouble(&coerced));
      if(!coerced)
      {
        QString str = QString("A Py coefficient (%1) could not be translated into a floating-point precision number").arg(coeff);
        setErrorCondition(-66500, str);
      }
    }
  }
  else
  {
    size_t len = static_cast<size_t>(m_Degree * m_Degree + 2 * m_Degree + 1);
    m_PxVec.resize(len);
    m_PyVec.resize(len);
    for(size_t i = 0; i < len; i++)
    {
      m_PxVec[i] = 0;
      m_PyVec[i] = 0;
    }

    // Default value
    m_PxVec[m_Degree + 1] = 1;
    m_PyVec[1] = 1;
  }

  // This step would not be necessary if using Dave's strict polynomial array
  // Otherwise, there is a direct correlation between the degree of the transform polynomial
  // and how many coefficients should reside in the initial guess
  size_t len = static_cast<size_t>(m_Degree * m_Degree + 2 * m_Degree + 1);
  if(len != m_PxVec.size())
  {
    QString str = QString("Number of coefficients in Px (%1) is not compatible with degree number (req: %2)").arg(m_PxVec.size()).arg(len);
    setErrorCondition(-66400, str);
  }
  if(len != m_PyVec.size())
  {
    QString str = QString("Number of coefficients in Py (%1) is not compatible with degree number (req: %2)").arg(m_PyVec.size()).arg(len);
    setErrorCondition(-66400, str);
  }

  // Some overlap required
  if(m_OverlapAmt[0] <= 0 || m_OverlapAmt[1] <= 0)
  {
    setErrorCondition(-66600, "Overlap amount should be a greater than 0 in both dimensions");
  }

  // Requirements to avoid crashing
  if(getDataContainerArray()->getDataContainers().size() <= 0)
  {
    setErrorCondition(-66710, "At least one DataContainer required");
    return;
  }

  AbstractMontageShPtr montage = getDataContainerArray()->getMontage(getMontageName());
  if(nullptr == montage)
  {
    setErrorCondition(-66750, QString("Montage: %1 required").arg(getMontageName()));
    return;
  }

  // All of the types in the chosen data container's image data arrays should be the same
  for(const auto& dc : montage->getDataContainers())
  {
    if(nullptr == dc)
    {
      setErrorCondition(-66715, QString("Montage cannot contain null DataContainers"));
      return;
    }

    AttributeMatrix::Pointer am = dc->getAttributeMatrix(m_AttributeMatrixName);
    if(nullptr == am)
    {
      setErrorCondition(-66720, QString("AttributeMatrix: %1 required").arg(m_AttributeMatrixName));
      return;
    }

    DataArray<Grayscale_T>::Pointer da = am->getAttributeArrayAs<DataArray<Grayscale_T>>(m_IPFColorsArrayName);
    if(nullptr == da)
    {
      setErrorCondition(-66730, QString("DataArray: %1 required").arg(m_IPFColorsArrayName));
      return;
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
    blendAM->createNonPrereqArray<DoubleArrayType>(this, m_TransformArrayName, 0, {m_PxVec.size() + m_PyVec.size()});
    blendAM->createNonPrereqArray<DoubleArrayType>(this, m_ResidualArrayName, 0, {1});
  }

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
  const QString grayscaleArrayName = getGrayscaleArrayName();
  std::vector<double> transformVector;
  double imageDimX;
  double imageDimY;

  std::vector<double> xyParameters(m_PxVec.begin(), m_PxVec.end());
  xyParameters.insert(xyParameters.end(), m_PyVec.begin(), m_PyVec.end());

  if(m_UseAmoebaOptimizer)
  {
    // The optimizer needs an initial guess; this is supplied through a filter parameter
    itk::AmoebaOptimizer::ParametersType initialParams(xyParameters.size());
    itk::AmoebaOptimizer::ScalesType scales(xyParameters.size());
    for(size_t idx = 0; idx < xyParameters.size(); ++idx)
    {
      initialParams[idx] = xyParameters[idx];
      scales[idx] = 0.001;
    }
#if 0
    constexpr double epsilon = itk::NumericTraits<double>::epsilon() + 1e-15;
    for(size_t x = 0; x < m_Degree + 1; x++)
    {
      double xScale = x == 0 ? 1.0 : std::pow(0.1, x - 1);
      for(size_t y = 0; y < m_Degree + 1; y++)
      {
        double yScale = y == 0 ? 1.0 : std::pow(0.1, y - 1);
        size_t id = x * (m_Degree + 1) + y;
        double minScale = std::min(xScale, yScale);
        scales[id] = std::max(minScale, epsilon);
      }
    }
    scales[0] = epsilon;
    //scales[m_PxVec.size() - 1] = epsilon;
    //scales[m_PxVec.size()] = epsilon;
    scales[xyParameters.size() - 1] = epsilon;

    //scales[m_Degree + 2] = 1;
    //scales[m_PxVec.size() + 1] = 1;
#endif

    itk::AmoebaOptimizer::Pointer optimizer = itk::AmoebaOptimizer::New();
    optimizer->SetMaximumNumberOfIterations(m_MaxIterations);
    optimizer->SetFunctionConvergenceTolerance(m_LowTolerance);
    optimizer->SetParametersConvergenceTolerance(m_HighTolerance);
    optimizer->SetInitialPosition(initialParams);
    //optimizer->SetScales(scales);
    optimizer->SetOptimizeWithRestarts(true);

    GridMontageShPtr gridMontage = std::dynamic_pointer_cast<GridMontage>(getDataContainerArray()->getMontage(getMontageName()));

    using CostFunctionType = FFTConvolutionCostFunction;
    CostFunctionType implementation;
    implementation.Initialize(gridMontage, m_Degree, m_OverlapAmt, getDataContainerArray(), m_AttributeMatrixName, grayscaleArrayName);
    optimizer->SetCostFunction(&implementation);
    optimizer->MaximizeOn(); // Search for the greatest value
    optimizer->StartOptimization();

    // Newer versions of the optimizer allow for easier methods of output information
    // to be obtained, but until then, we have to do some string parsing from the
    // optimizer's stop description
    QString stopReason = QString::fromStdString(optimizer->GetStopConditionDescription());
    std::list<double> transform;
    itk::AmoebaOptimizer::ParametersType finalParams = optimizer->GetCurrentPosition();
    for(const auto& coeff : finalParams)
    {
      transform.push_back(coeff);
    }

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
    for(auto param : finalParams)
    {
      finalParamStr += QString::number(param) + "; ";
    }
    finalParamStr += " ]";
    notifyStatusMessage(finalParamStr);

    notifyStatusMessage(QString::fromStdString(optimizer->GetStopConditionDescription()));

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

    transformVector = {std::begin(transform), std::end(transform)};
  }
  else
  {
    // std::tie(imageDimX, imageDimY) = getImageDims();
    transformVector = {std::begin(xyParameters), std::end(xyParameters)};

    if(m_CreateTransformContainer)
    {
      AttributeMatrixShPtr transformAM = getDataContainerArray()->getDataContainer(m_BlendDCName)->getAttributeMatrix(m_TransformMatrixName);
      transformAM->getAttributeArrayAs<DoubleArrayType>(m_TransformArrayName)->setArray({transformVector.begin(), transformVector.end()});
    }
  }

  // Remove internal arrays
  deleteGrayscaleIPF();

  std::tie(imageDimX, imageDimY) = getImageDims();
  // Apply transform to dewarp data
  warpDataContainers(transformVector, imageDimX, imageDimY);
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
size_t flatten(const SizeVec2Type& xyPos, const SizeVec3Type& dimensions)
{
  const size_t x = xyPos[0];
  const size_t y = xyPos[1];

  const size_t width = dimensions[0];
  const size_t height = dimensions[1];

  return x + y * width;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double pow(double base, size_t pow)
{
  double val = 1;
  for(size_t i = 0; i < pow; i++)
  {
    val *= base;
  }

  return val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename T>
void transformDataPixel(int degree, double x_trans, double y_trans, const SizeVec2Type& newPixel, const std::vector<double>& transformVector, const SizeVec3Type& dimensions,
                        typename const DataArray<T>::Pointer& da, typename const DataArray<T>::Pointer& tempDACopy)
{
  using PixelTyped = std::array<double, 2>;

  const std::array<double, 2> newPrime = {newPixel[0] - x_trans, newPixel[1] - y_trans};
  const double newXPrime = newPrime[0];
  const double newYPrime = newPrime[1];

  // old' = Ei(Ej(aij * x^i * y^j)
  std::array<double, 2> oldPrime{0.0, 0.0};
  const size_t yInit = (degree * degree) + (2 * degree + 1);
  for(size_t i = 0; i <= degree; i++)
  {
    const double xVal = pow(newXPrime, i);
    for(size_t j = 0; j <= degree; j++)
    {
      const double yVal = pow(newYPrime, j);
      const size_t xOffset = i * (degree + 1) + j;
      const size_t yOffset = xOffset + yInit;
      oldPrime[0] += transformVector[xOffset] * xVal * yVal;
      oldPrime[1] += transformVector[yOffset] * xVal * yVal;
    }
  }

  const int64_t oldXUnbound = static_cast<int64_t>(round(oldPrime[0] + x_trans));
  const int64_t oldYUnbound = static_cast<int64_t>(round(oldPrime[1] + y_trans));

  auto compDims = tempDACopy->getComponentDimensions();
  size_t numComponents = std::accumulate(compDims.begin(), compDims.end(), 0);
  size_t newIndex = flatten(newPixel, dimensions);

  // Cannot flatten invalid { X,Y } positions
  if(oldXUnbound < 0 || oldYUnbound < 0 || oldXUnbound >= dimensions[0] || oldYUnbound >= dimensions[1])
  {
    typename std::vector<T> blankTuple(numComponents, 0);
    da->setTuple(newIndex, blankTuple);
    return;
  }

  SizeVec2Type oldPixel{static_cast<size_t>(oldXUnbound), static_cast<size_t>(oldYUnbound)};
  size_t oldIndex = flatten(oldPixel, dimensions);
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
void transformDataArray(int degree, const std::vector<double>& transformVector, const SizeVec3Type& dimensions, double x_trans, double y_trans, typename const DataArray<T>::Pointer& da)
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

      fn = std::bind(&transformDataPixel<T>, degree, x_trans, y_trans, newPixel, transformVector, dimensions, da, daCopy);
      taskAlg.execute(fn);
    }
  }
  taskAlg.wait();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void transformIDataArray(int degree, const std::vector<double>& transformVector, const SizeVec3Type& dimensions, double x_trans, double y_trans, const IDataArray::Pointer& da)
{
  if(std::dynamic_pointer_cast<Int8ArrayType>(da))
  {
    Int8ArrayType::Pointer array = std::dynamic_pointer_cast<Int8ArrayType>(da);
    transformDataArray<int8_t>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<UInt8ArrayType>(da))
  {
    UInt8ArrayType::Pointer array = std::dynamic_pointer_cast<UInt8ArrayType>(da);
    transformDataArray<uint8_t>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<Int16ArrayType>(da))
  {
    Int16ArrayType::Pointer array = std::dynamic_pointer_cast<Int16ArrayType>(da);
    transformDataArray<int16_t>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<UInt16ArrayType>(da))
  {
    UInt16ArrayType::Pointer array = std::dynamic_pointer_cast<UInt16ArrayType>(da);
    transformDataArray<uint16_t>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<Int32ArrayType>(da))
  {
    Int32ArrayType::Pointer array = std::dynamic_pointer_cast<Int32ArrayType>(da);
    transformDataArray<int32_t>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<UInt32ArrayType>(da))
  {
    UInt32ArrayType::Pointer array = std::dynamic_pointer_cast<UInt32ArrayType>(da);
    transformDataArray<uint32_t>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<Int64ArrayType>(da))
  {
    Int64ArrayType::Pointer array = std::dynamic_pointer_cast<Int64ArrayType>(da);
    transformDataArray<int64_t>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<UInt64ArrayType>(da))
  {
    UInt64ArrayType::Pointer array = std::dynamic_pointer_cast<UInt64ArrayType>(da);
    transformDataArray<uint64_t>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<FloatArrayType>(da))
  {
    FloatArrayType::Pointer array = std::dynamic_pointer_cast<FloatArrayType>(da);
    transformDataArray<float>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
  else if(std::dynamic_pointer_cast<DoubleArrayType>(da))
  {
    DoubleArrayType::Pointer array = std::dynamic_pointer_cast<DoubleArrayType>(da);
    transformDataArray<double>(degree, transformVector, dimensions, x_trans, y_trans, array);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::warpDataContainers(const std::vector<double>& transformVector, double imageDimX, double imageDimY)
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
        transformIDataArray(m_Degree, transformVector, dimensions, x_trans, y_trans, da);
      }
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

  size_t targetColumn = gridMontage->getColumnCount() > 1 ? 1 : 0;
  size_t targetRow = gridMontage->getRowCount() > 1 ? 1 : 0;

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
const QString Blend::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getSubGroupName() const
{
  return "Processing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getHumanLabel() const
{
  return "Blend";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid Blend::getUuid()
{
  return QUuid("{f2c81aa6-8edf-51d8-8c30-81ad983815e9}");
}
