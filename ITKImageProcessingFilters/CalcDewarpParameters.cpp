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

#include "CalcDewarpParameters.h"

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include "tbb/queuing_mutex.h"
using MutexType = tbb::queuing_mutex;
#endif

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
#include "ITKImageProcessing/ITKImageProcessingFilters/util/FFTAmoebaOptimizer.h"
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

double calcDelta(double maxDelta, double mMax)
{
  return maxDelta / mMax;
}
} // namespace

uint CalcDewarpParameters::GetIterationsFromStopDescription(const QString& stopDescription) const
{
  if(GetConvergenceFromStopDescription(stopDescription))
  {
    const QString startStr = "have been met in ";
    const int startIdx = stopDescription.indexOf(startStr) + startStr.length();
    return stopDescription.midRef(startIdx, stopDescription.indexOf(" iterations") - startIdx).toUInt();
  }
  return m_MaxIterations;
}

bool CalcDewarpParameters::GetConvergenceFromStopDescription(const QString& stopDescription) const
{
  return stopDescription.contains("have been met");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalcDewarpParameters::CalcDewarpParameters()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalcDewarpParameters::~CalcDewarpParameters() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_MONTAGE_STRUCTURE_SELECTION_FP("Montage Name", MontageName, FilterParameter::Category::Parameter, CalcDewarpParameters));

  parameters.push_back(SeparatorFilterParameter::New("Amoeba Optimizer", FilterParameter::Parameter));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Max Iterations", MaxIterations, FilterParameter::Category::Parameter, CalcDewarpParameters));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Delta", Delta, FilterParameter::Category::Parameter, CalcDewarpParameters));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Fractional Tolerance", FractionalTolerance, FilterParameter::Category::Parameter, CalcDewarpParameters));

  QStringList linkedSpecifySimplexProps{"XFactors", "YFactors"};
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Specify Initial Simplex", SpecifyInitialSimplex, FilterParameter::Parameter, CalcDewarpParameters, linkedSpecifySimplexProps));
  parameters.push_back(SIMPL_NEW_EBSDWARPPOLYNOMIAL_FP("X Factors", XFactors, FilterParameter::Parameter, CalcDewarpParameters));
  parameters.push_back(SIMPL_NEW_EBSDWARPPOLYNOMIAL_FP("Y Factors", YFactors, FilterParameter::Parameter, CalcDewarpParameters));

  parameters.push_back(SeparatorFilterParameter::New("Montage Data Path", FilterParameter::Parameter));
  parameters.push_back(SIMPL_NEW_STRING_FP("Attribute Matrix Name", AttributeMatrixName, FilterParameter::Category::Parameter, CalcDewarpParameters));
  parameters.push_back(SIMPL_NEW_STRING_FP("IPF Colors Array Name", IPFColorsArrayName, FilterParameter::Category::Parameter, CalcDewarpParameters));

  // SIMPL_NEW_LINKED_BOOL_FP
  parameters.push_back(SeparatorFilterParameter::New("Transformation Arrays", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Transform Container", TransformDCName, FilterParameter::Category::CreatedArray, CalcDewarpParameters));
  parameters.push_back(SIMPL_NEW_STRING_FP("Transform Matrix", TransformMatrixName, FilterParameter::Category::CreatedArray, CalcDewarpParameters));
  parameters.push_back(SIMPL_NEW_STRING_FP("Transform Array", TransformArrayName, FilterParameter::Category::CreatedArray, CalcDewarpParameters));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::dataCheck()
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

  {
    // Create a new data container to hold the output of this filter
    DataContainerShPtr CalcDewarpParametersDC = getDataContainerArray()->createNonPrereqDataContainer(this, DataArrayPath(m_TransformDCName, "", ""));
    AttributeMatrixShPtr CalcDewarpParametersAM = CalcDewarpParametersDC->createNonPrereqAttributeMatrix(this, m_TransformMatrixName, {1}, AttributeMatrix::Type::Generic);
    CalcDewarpParametersAM->createNonPrereqArray<DoubleArrayType>(this, m_TransformArrayName, 0, {FFTHelper::getReqParameterSize()});
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::preflight()
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
void CalcDewarpParameters::execute()
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

#if 0
  // Print out step sizes
  QString stepSizeStr = "Step Sizes: [ ";
  for(auto param : stepSizes)
  {
    stepSizeStr += QString::number(param) + "; ";
  }
  stepSizeStr += " ]";
  notifyStatusMessage(stepSizeStr);
#endif

  itk::FFTAmoebaOptimizer::Pointer optimizer = itk::FFTAmoebaOptimizer::New();
  optimizer->SetMaximumNumberOfIterations(m_MaxIterations);
  optimizer->SetFractionalTolerance(m_FractionalTolerance);
  optimizer->SetInitialPosition(initialParams);
  optimizer->SetInitialSimplexDelta(stepSizes);
  //optimizer->SetOptimizeWithRestarts(true);

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

#if 0
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
#endif

  notifyStatusMessage(QString::fromStdString(optimizer->GetStopConditionDescription()));
  std::list<double> transform = ::convertParams2List(transformParams);

  // ...otherwise, set the appropriate values for the filter's output data array
  AttributeMatrixShPtr transformAM = getDataContainerArray()->getDataContainer(m_TransformDCName)->getAttributeMatrix(m_TransformMatrixName);
  transformAM->getAttributeArrayAs<DoubleArrayType>(m_TransformArrayName)->setArray(transform);

  // Remove internal arrays
  deleteGrayscaleIPF();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool CalcDewarpParameters::checkMontageRequirements()
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
size_t CalcDewarpParameters::getSingleParamCount() const
{
  return FFTHelper::getReqPartialParameterSize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> CalcDewarpParameters::getStepSizes(const std::vector<double>& params, size_t imgDimX, size_t imgDimY) const
{
  const size_t count = FFTHelper::getReqParameterSize();
  std::vector<double> stepSizes(count);

  const size_t xMax = (imgDimX / 2); // *(imgDimX / 2);
  const size_t yMax = (imgDimY / 2); // *(imgDimY / 2);

  // Px
  stepSizes[0] = calcDelta(m_StepDelta, xMax);
  stepSizes[1] = calcDelta(m_StepDelta, yMax);
  stepSizes[2] = calcDelta(m_StepDelta, xMax * xMax);
  stepSizes[3] = calcDelta(m_StepDelta, yMax * yMax);
  stepSizes[4] = calcDelta(m_StepDelta, xMax * yMax);
  stepSizes[5] = calcDelta(m_StepDelta, xMax * xMax * yMax);
  stepSizes[6] = calcDelta(m_StepDelta, xMax * yMax * yMax);

  // Py
  stepSizes[7] = calcDelta(m_StepDelta, xMax);
  stepSizes[8] = calcDelta(m_StepDelta, yMax);
  stepSizes[9] = calcDelta(m_StepDelta, xMax * xMax);
  stepSizes[10] = calcDelta(m_StepDelta, yMax * yMax);
  stepSizes[11] = calcDelta(m_StepDelta, xMax * yMax);
  stepSizes[12] = calcDelta(m_StepDelta, xMax * xMax * yMax);
  stepSizes[13] = calcDelta(m_StepDelta, xMax * yMax * yMax);

  return stepSizes;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> CalcDewarpParameters::getPxyVec() const
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
std::vector<double> CalcDewarpParameters::getDefaultPx() const
{
  size_t len = getSingleParamCount();
  std::vector<double> px(len, 0);
  px[0] = 1;
  return px;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::vector<double> CalcDewarpParameters::getDefaultPy() const
{
  size_t len = getSingleParamCount();
  std::vector<double> py(len, 0);
  py[1] = 1;
  return py;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getGrayscaleArrayName() const
{
  return ::InternalGrayscalePrefex + m_IPFColorsArrayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::generateGrayscaleIPF()
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
void CalcDewarpParameters::deleteGrayscaleIPF()
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CalcDewarpParameters::newFilterInstance(bool copyFilterParameters) const
{
  CalcDewarpParameters::Pointer filter = CalcDewarpParameters::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return std::move(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getSubGroupName() const
{
  return "Processing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getHumanLabel() const
{
  return "Calculate Dewarp Parameters";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid CalcDewarpParameters::getUuid() const
{
  return QUuid("{5394f60c-b0f0-5f98-85da-3b5730109060}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalcDewarpParameters::Pointer CalcDewarpParameters::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalcDewarpParameters::Pointer CalcDewarpParameters::New()
{
  struct make_shared_enabler : public CalcDewarpParameters
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getNameOfClass() const
{
  return "CalcDewarpParameters";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::ClassName()
{
  return "CalcDewarpParameters";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getMontageName() const
{
  return m_MontageName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setMontageName(const QString& value)
{
  m_MontageName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint CalcDewarpParameters::getMaxIterations() const
{
  return m_MaxIterations;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setMaxIterations(uint value)
{
  m_MaxIterations = value;
}

#if 0
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IntVec2Type CalcDewarpParameters::getOverlapAmt() const
{
  return m_OverlapAmt;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setOverlapAmt(const IntVec2Type& value)
{
  m_OverlapAmt = value;
}
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double CalcDewarpParameters::getFractionalTolerance() const
{
  return m_FractionalTolerance;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setFractionalTolerance(double value)
{
  m_FractionalTolerance = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int CalcDewarpParameters::getDelta() const
{
  return m_StepDelta;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setDelta(int value)
{
  m_StepDelta = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool CalcDewarpParameters::getSpecifyInitialSimplex() const
{
  return m_SpecifyInitialSimplex;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setSpecifyInitialSimplex(bool value)
{
  m_SpecifyInitialSimplex = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getAttributeMatrixName() const
{
  return m_AttributeMatrixName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setAttributeMatrixName(const QString& value)
{
  m_AttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getIPFColorsArrayName() const
{
  return m_IPFColorsArrayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setIPFColorsArrayName(const QString& value)
{
  m_IPFColorsArrayName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getTransformMatrixName() const
{
  return m_TransformMatrixName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setTransformMatrixName(const QString& value)
{
  m_TransformMatrixName = value;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getTransformArrayName() const
{
  return m_TransformArrayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setTransformArrayName(const QString& value)
{
  m_TransformArrayName = value;
}

// -----------------------------------------------------------------------------
FloatVec7Type CalcDewarpParameters::getXFactors() const
{
  return m_XFactors;
}
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setXFactors(const FloatVec7Type& value)
{
  m_XFactors = value;
}
// -----------------------------------------------------------------------------
FloatVec7Type CalcDewarpParameters::getYFactors() const
{
  return m_YFactors;
}
// -----------------------------------------------------------------------------
void CalcDewarpParameters::setYFactors(const FloatVec7Type& value)
{
  m_YFactors = value;
}

// -----------------------------------------------------------------------------
QString CalcDewarpParameters::getTransformDCName() const
{
  return m_TransformDCName;
}

// -----------------------------------------------------------------------------
void CalcDewarpParameters::setTransformDCName(const QString& value)
{
  m_TransformDCName = value;
}
