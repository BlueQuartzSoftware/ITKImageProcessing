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
#endif

#include "itkAmoebaOptimizer.h"
#include <itkFFTConvolutionImageFilter.h>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/Common/SIMPLRange.hpp"

#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/FilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Utilities/ParallelData2DAlgorithm.h"
#include "SIMPLib/Utilities/ParallelTaskAlgorithm.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/FFTConvolutionCostFunction.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

using Grayscale_T = uint8_t;
using PixelValue_T = double;

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
, m_Degree(1)
, m_OverlapPercentage(0.0f)
, m_LowTolerance(1E-2)
, m_HighTolerance(1E-2)
, m_InitialSimplexGuess("0.1;0.1;0.1;0.1;0.1;0.1;0.1;0.1")
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

  {
    MultiDataContainerSelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_MDC_SELECTION_FP("Chosen Data Containers", ChosenDataContainers, FilterParameter::Category::RequiredArray, Blend, req));
  }

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Max Iterations", MaxIterations, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Degree", Degree, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Overlap Percentage", OverlapPercentage, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Low Tolerance", LowTolerance, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("High Tolerance", HighTolerance, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Initial Simplex Guess", InitialSimplexGuess, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Attribute Matrix Name", AttributeMatrixName, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Data Attribute Array Name", DataAttributeArrayName, FilterParameter::Category::Parameter, Blend));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  m_InitialGuess.clear();
  // Need to make sure that the filter parameter for the initial guess
  // can be cast into actual numeric data
  for(const auto& coeff : m_InitialSimplexGuess.split(";"))
  {
    bool coerced = false;
    m_InitialGuess.push_back(coeff.toDouble(&coerced));
    if(!coerced)
    {
      setErrorCondition(-66500, "A coefficient could not be translated into a floating-point precision number");
    }
  }

  // This step would not be necessary if using Dave's strict polynomial array
  // Otherwise, there is a direct correlation between the degree of the transform polynomial
  // and how many coefficients should reside in the initial guess
  size_t len = static_cast<size_t>(2 * m_Degree * m_Degree + 4 * m_Degree + 2);
  if(len != m_InitialGuess.size())
  {
    setErrorCondition(-66400, "Number of coefficients in initial guess is not compatible with degree number");
  }

  // Overlap percentages below 0% and above 100% don't make any sense
  if(m_OverlapPercentage < 0.0f || m_OverlapPercentage >= 1.00f)
  {
    setErrorCondition(-66600, "Overlap Percentage should be a floating-point precision number between 0.0 and 1.0");
  }

  // Requirements to avoid crashing
  if(getDataContainerArray()->getDataContainers().size() <= 0)
  {
    setErrorCondition(-66710, "At least one DataContainer required");
    return;
  }

  // All of the types in the chosen data container's image data arrays should be the same
  for(const auto& dc : getDataContainerArray()->getDataContainers())
  {
    if(!m_ChosenDataContainers.contains(dc->getName()))
    {
      continue;
    }

    AttributeMatrix::Pointer am = dc->getAttributeMatrix(m_AttributeMatrixName);
    if(nullptr == am)
    {
      setErrorCondition(-66720, QString("AttributeMatrix: %1 required").arg(m_AttributeMatrixName));
      return;
    }
    
    DataArray<Grayscale_T>::Pointer da = am->getAttributeArrayAs<DataArray<Grayscale_T>>(m_DataAttributeArrayName);
    if(nullptr == da)
    {
      setErrorCondition(-66730, QString("DataArray: %1 required").arg(m_DataAttributeArrayName));
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

    // DataArray values cannot be accessed in preflight when the array is empty
    if(!getInPreflight())
    {
      // If any component of each pixel is not equal to all the others then the image is not grayscaled
      // A warning should be thrown because the filter will then effectively only blend the red component
      size_t numComps = da->getNumberOfComponents();
      for(size_t pixelIdx = 0; pixelIdx < da->getNumberOfTuples(); ++pixelIdx)
      {
        Grayscale_T testPixel = da->getValue(pixelIdx);
        for(size_t compIdx = 1; compIdx < numComps; ++compIdx)
        {
          Grayscale_T actualPixel = da->getValue(pixelIdx + compIdx);
          if(actualPixel != testPixel)
          {
            setWarningCondition(-66900, "Not all components of the pixels are the same value. Images should be grayscaled before being filtered.");
            return;
          }
        }
      }
    }
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

  // Create a new data container to hold the output of this filter
  DataContainerShPtr blendDC = getDataContainerArray()->createNonPrereqDataContainer(this, DataArrayPath(m_BlendDCName, m_TransformAMName, ""));
  AttributeMatrixShPtr blendAM = blendDC->createAndAddAttributeMatrix({1}, m_TransformAMName, AttributeMatrix::Type::Generic);

  // The data container holds a single output attribute matrix with 3 data arrays
  // One for the number of iterations taken
  // One for the transform array
  // One for the output of the transform (the optimized value that is the
  // minimization of the square root of the sum for each overlap
  // of the max of a convolution of two overlapping FFT'd images
  blendAM->createAndAddAttributeArray<UInt64ArrayType>(this, m_IterationsAAName, 0, {1});
  blendAM->createAndAddAttributeArray<DoubleArrayType>(this, m_TransformAAName, 0, {m_InitialGuess.size()});
  blendAM->createAndAddAttributeArray<DoubleArrayType>(this, m_ValueAAName, 0, {1});
  getDataContainerArray()->addOrReplaceDataContainer(blendDC);

  // The optimizer needs an initial guess; this is supplied through a filter parameter
  itk::AmoebaOptimizer::ParametersType initialParams(m_InitialGuess.size());
  for(size_t idx = 0; idx < m_InitialGuess.size(); ++idx)
  {
    initialParams[idx] = m_InitialGuess[idx];
  }

  itk::AmoebaOptimizer::Pointer optimizer = itk::AmoebaOptimizer::New();
  optimizer->SetMaximumNumberOfIterations(m_MaxIterations);
  optimizer->SetFunctionConvergenceTolerance(m_LowTolerance);
  optimizer->SetParametersConvergenceTolerance(m_HighTolerance);
  optimizer->SetInitialPosition(initialParams);

  //  using CostFunctionType = MultiParamCostFunction;
  using CostFunctionType = FFTConvolutionCostFunction;
  CostFunctionType implementation;
  implementation.Initialize(
      // The line below is used for testing the MultiParamCostFunction
      //    std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}
      m_ChosenDataContainers, "r", "c", m_Degree, m_OverlapPercentage, getDataContainerArray(), m_AttributeMatrixName, m_DataAttributeArrayName);
  optimizer->SetCostFunction(&implementation);
  optimizer->StartOptimization();

  // Newer versions of the optimizer allow for easier methods of output information
  // to be obtained, but until then, we have to do some string parsing from the
  // optimizer's stop description
  QString stopReason = QString::fromStdString(optimizer->GetStopConditionDescription());
  std::list<double> transform;
  for(const auto& coeff : optimizer->GetCurrentPosition())
  {
    transform.push_back(coeff);
  }

  // cache value
  auto value = optimizer->GetValue();
  auto numIterations = GetIterationsFromStopDescription(stopReason);

  // Can get rid of these qDebug lines after debugging is done for filter
  qDebug() << "Initial Position: [ " << m_InitialGuess << " ]";
  qDebug() << "Final Position: [ " << transform << " ]";
  qDebug() << "Number of Iterations: " << numIterations;
  qDebug() << "Value: " << value;

  // If the optimization didn't converge, set an error...
  if(!GetConvergenceFromStopDescription(stopReason))
  {
    setErrorCondition(-66850, stopReason);
    notifyStatusMessage(stopReason);
    // Write the stop reason to the console
    qDebug() << "Stop Reason: " << stopReason;
    return;
  }

  // ...otherwise, set the appropriate values of the filter's output data arrays
  AttributeMatrixShPtr transformAM = getDataContainerArray()->getDataContainer(m_BlendDCName)->getAttributeMatrix(m_TransformAMName);
  transformAM->getAttributeArrayAs<UInt64ArrayType>(m_IterationsAAName)->push_back(numIterations);
  transformAM->getAttributeArrayAs<DoubleArrayType>(m_ValueAAName)->push_back(value);
  transformAM->getAttributeArrayAs<DoubleArrayType>(m_TransformAAName)->setArray(transform);
}

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
