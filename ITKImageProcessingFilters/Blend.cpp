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
#include "SIMPLib/Common/SIMPLRange.h"

#include "SIMPLib/CoreFilters/ConvertColorToGrayScale.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/FilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/MontageSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/MontageStructureSelectionFilterParameter.h"
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
, m_Degree(1)
, m_OverlapPercentage(0.0f)
, m_LowTolerance(1E-2)
, m_HighTolerance(1E-2)
, m_InitialSimplexGuess("0.1;0.1;0.1;0.1;0.1;0.1;0.1;0.1")
, m_BlendDCName("Blend Data")
, m_TransformMatrixName("Transform Matrix")
, m_TransformArrayName("Transform")
//, m_NumIterationsArrayName("Iterations")
, m_ResidualArrayName("Residual")
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
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Max Iterations", MaxIterations, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Degree", Degree, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Overlap Percentage", OverlapPercentage, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Low Tolerance", LowTolerance, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("High Tolerance", HighTolerance, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Initial Simplex Guess", InitialSimplexGuess, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Attribute Matrix Name", AttributeMatrixName, FilterParameter::Category::Parameter, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("IPF Colors Array Name", IPFColorsArrayName, FilterParameter::Category::Parameter, Blend));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container", BlendDCName, FilterParameter::Category::CreatedArray, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Transform Matrix", TransformMatrixName, FilterParameter::Category::CreatedArray, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Transform Array", TransformArrayName, FilterParameter::Category::CreatedArray, Blend));
  //parameters.push_back(SIMPL_NEW_STRING_FP("Number of Iterations", NumIterationsArrayName, FilterParameter::Category::CreatedArray, Blend));
  parameters.push_back(SIMPL_NEW_STRING_FP("Residual Values", ResidualArrayName, FilterParameter::Category::CreatedArray, Blend));

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

  AbstractMontageShPtr montage = getDataContainerArray()->getMontage(getMontageName());
  if(nullptr == montage)
  {
    setErrorCondition(-66750, QString("Montage: %1 required").arg(getMontageName()));
    return;
  }

  // All of the types in the chosen data container's image data arrays should be the same
  for(const auto& dc : montage->getDataContainers())
  {

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

  // Create a new data container to hold the output of this filter
  DataContainerShPtr blendDC = getDataContainerArray()->createNonPrereqDataContainer(this, DataArrayPath(m_BlendDCName, "", ""));
  AttributeMatrixShPtr blendAM = blendDC->createNonPrereqAttributeMatrix(this, m_TransformMatrixName, { 1 }, AttributeMatrix::Type::Generic);

  // blendAM->createAndAddAttributeArray<UInt64ArrayType>(this, m_IterationsAAName, 0, {1});
  blendAM->createNonPrereqArray<DoubleArrayType>(this, m_TransformArrayName, 0, { m_InitialGuess.size() });
  blendAM->createNonPrereqArray<DoubleArrayType>(this, m_ResidualArrayName, 0, { 1 });
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


  QStringList dcNames;
  if(getDataContainerArray()->getMontage(getMontageName()))
  {
    dcNames = getDataContainerArray()->getMontage(getMontageName())->getDataContainerNames();
  }

  //  using CostFunctionType = MultiParamCostFunction;
  using CostFunctionType = FFTConvolutionCostFunction;
  CostFunctionType implementation;
  implementation.Initialize(
      // The line below is used for testing the MultiParamCostFunction
      //    std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}
      dcNames, "r", "c", m_Degree, m_OverlapPercentage, getDataContainerArray(), m_AttributeMatrixName, grayscaleArrayName);
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
  AttributeMatrixShPtr transformAM = getDataContainerArray()->getDataContainer(m_BlendDCName)->getAttributeMatrix(m_TransformMatrixName);
  //transformAM->getAttributeArrayAs<UInt64ArrayType>(m_IterationsAAName)->push_back(numIterations);
  transformAM->getAttributeArrayAs<DoubleArrayType>(m_ResidualArrayName)->push_back(value);
  transformAM->getAttributeArrayAs<DoubleArrayType>(m_TransformArrayName)->setArray(transform);

  // Remove internal arrays
  deleteGrayscaleIPF();

  // Apply transform to dewarp data
  std::vector<double> transformVector{ std::begin(transform), std::end(transform) };
  warpDataContainers(transformVector);
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
  AbstractMontage::Pointer montage =  getDataContainerArray()->getMontage(getMontageName());
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
itk::Image<double, 2>::Pointer generateKernelImage(const std::vector<double> & transformVector)
{
  using KernelType = itk::Image<double, 2>;
  KernelType::Pointer kernelImage = KernelType::New();
  KernelType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  KernelType::SizeType size;
  size[0] = 3;
  size[1] = 3;
  size[2] = 0;
  KernelType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  kernelImage->SetRegions(region);
  kernelImage->Allocate();

  KernelType::IndexType index;
  index[0] = 0;
  index[1] = 0;
  kernelImage->SetPixel(index, transformVector[0]);

  index[0] = 0;
  index[1] = 1;
  kernelImage->SetPixel(index, transformVector[1]);

  index[0] = 0;
  index[1] = 2;
  kernelImage->SetPixel(index, transformVector[2]);

  index[0] = 1;
  index[1] = 0;
  kernelImage->SetPixel(index, transformVector[3]);

  index[0] = 1;
  index[1] = 1;
  kernelImage->SetPixel(index, 1);

  index[0] = 1;
  index[1] = 2;
  kernelImage->SetPixel(index, transformVector[4]);

  index[0] = 2;
  index[1] = 0;
  kernelImage->SetPixel(index, transformVector[5]);

  index[0] = 2;
  index[1] = 1;
  kernelImage->SetPixel(index, transformVector[6]);

  index[0] = 2;
  index[1] = 2;
  kernelImage->SetPixel(index, transformVector[7]);

  return kernelImage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
void transformDataArray(const itk::Image<double, 2>::Pointer& kernelImage, const SizeVec3Type& dimensions, const DataArray<T>::Pointer& da)
{
  using PixelType = T;
  using ImageType = itk::Image<PixelType, 2>;
  using KernelType = itk::Image<double, 2>;
  using ConvolutionFilterType = itk::FFTConvolutionImageFilter<ImageType, KernelType>;
  using PixelContainer = ImageType::PixelContainer;
  
  ImageType::Pointer inputImage = ImageType::New();
  ImageType::IndexType start;
  ImageType::SizeType size;
  for(size_t i = 0; i < 3; i++)
  {
    start[i] = 0;
    size[i] = dimensions[i];
  }
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  inputImage->SetRegions(region);
  inputImage->Allocate();

  PixelContainer* inputPixelContainer = inputImage->GetPixelContainer();
  inputPixelContainer->SetImportPointer(da->getPointer(0), da->getNumberOfTuples(), false);
  inputImage->SetPixelContainer(inputPixelContainer);
  
  ConvolutionFilterType::Pointer convolutionFilter = ConvolutionFilterType::New();
  convolutionFilter->SetInputImage(inputImage);
  convolutionFilter->SetKernelImage(kernelImage);
  convolutionFilter->Update();
  
  ImageType::Pointer outputImage = convolutionFilter->GetOutput();
  PixelType* pixelArray = outputImage->GetPixelContainer()->GetImportPointer();
  PixelType* arrayPtr = da->getPointer(0);
  std::copy(std::begin(pixelArray), std::begin(pixelArray) + da->getNumberOfTuples(), std::begin(arrayPtr));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void transformIDataArray(const itk::Image<double, 2>::Pointer& kernelImage, const SizeVec3Type& dimensions, const IDataArray::Pointer& da)
{
  if(std::dynamic_pointer_cast<Int8ArrayType>(da))
  {
    Int8ArrayType::Pointer array = std::dynamic_pointer_cast<Int8ArrayType>(da);
    transformDataArray<int8_t>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<UInt8ArrayType>(da))
  {
    UInt8ArrayType::Pointer array = std::dynamic_pointer_cast<UInt8ArrayType>(da);
    transformDataArray<uint8_t>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<Int16ArrayType>(da))
  {
    Int16ArrayType::Pointer array = std::dynamic_pointer_cast<Int16ArrayType>(da);
    transformDataArray<int16_t>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<UInt16ArrayType>(da))
  {
    UInt16ArrayType::Pointer array = std::dynamic_pointer_cast<UInt16ArrayType>(da);
    transformDataArray<uint16_t>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<Int32ArrayType>(da))
  {
    Int32ArrayType::Pointer array = std::dynamic_pointer_cast<Int32ArrayType>(da);
    transformDataArray<int32_t>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<UInt32ArrayType>(da))
  {
    UInt32ArrayType::Pointer array = std::dynamic_pointer_cast<UInt32ArrayType>(da);
    transformDataArray<uint32_t>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<Int64ArrayType>(da))
  {
    Int64ArrayType::Pointer array = std::dynamic_pointer_cast<Int64ArrayType>(da);
    transformDataArray<int64_t>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<UInt64ArrayType>(da))
  {
    UInt64ArrayType::Pointer array = std::dynamic_pointer_cast<UInt64ArrayType>(da);
    transformDataArray<uint64_t>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<BoolArrayType>(da))
  {
    BoolArrayType::Pointer array = std::dynamic_pointer_cast<BoolArrayType>(da);
    transformDataArray<bool>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<FloatArrayType>(da))
  {
    FloatArrayType::Pointer array = std::dynamic_pointer_cast<FloatArrayType>(da);
    transformDataArray<float>(kernelImage, dimensions, array);
  }
  else if(std::dynamic_pointer_cast<DoubleArrayType>(da))
  {
    DoubleArrayType::Pointer array = std::dynamic_pointer_cast<DoubleArrayType>(da);
    transformDataArray<double>(kernelImage, dimensions, array);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::warpDataContainers(const std::vector<double>& transformVector)
{
  // Duplicate the DataContainers used and Warp them based on the transform kernel generated.

  // TODO: Warp DataContainers
  constexpr unsigned int Dimension = 2;
  using VectorComponentType = float;
  using VectorPixelType = itk::Vector<VectorComponentType, Dimension>;
  using PixelType = unsigned char;
  using ImageType = itk::Image<PixelType, Dimension>;
  using DisplacementFieldType = itk::Image<VectorPixelType, Dimension>;
  using KernelType = itk::Image<double, 2>;

  // Create the kernel image
  KernelType::Pointer kernelImage = generateKernelImage(transformVector);

  AbstractMontage::Pointer montage = getDataContainerArray()->getMontage(m_MontageName);
  for(const auto& dc : *montage)
  {
    ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
    SizeVec3Type dimensions = imageGeom->getDimensions();

    DataContainer::Pointer dCopy = dc->deepCopy();
    for(const auto& am : *dCopy)
    {
      for(const auto& da : *am)
      {
        transformIDataArray(kernelImage, dimensions, da);
      }
    }
  }
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
