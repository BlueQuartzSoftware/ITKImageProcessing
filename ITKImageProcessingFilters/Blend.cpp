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

#include "SIMPLib/Common/Constants.h"

#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/FilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
//#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"

/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkAffineTransform.h"
#include "itkAmoebaOptimizer.h"
#include "itkCastImageFilter.h"
#include "itkCenteredTransformInitializer.h"
#include "itkFileOutputWindow.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegistrationMethod.h"
#include "itkMatchCardinalityImageToImageMetric.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include "itkSquaredDifferenceImageFilter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Blend::Blend()
: m_OverlapMethod(0)
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
  setErrorCondition(0, "");
  setWarningCondition(0, "");
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  //  MultiDataContainerSelectionFilterParameter::Pointer dcs{
  //    MultiDataContainerSelectionFilterParameter::New(
  //      "Chosen Data Containers", "ChosenDataContainers",
  //      getChosenDataContainers(), FilterParameter::Category::RequiredArray,
  //      std::bind(&Blend::setChosenDataContainers, this, std::placeholders::_1),
  //      std::bind(&Blend::getChosenDataContainers, this), {}
  //  )};

  IntFilterParameter::Pointer maxIterations{IntFilterParameter::New("Max Iterations", "MaxIterations", 1000, FilterParameter::Category::Parameter,
                                                                    std::bind(&Blend::setMaxIterations, this, std::placeholders::_1), std::bind(&Blend::getMaxIterations, this))};

  IntFilterParameter::Pointer degree{
      IntFilterParameter::New("Degree", "Degree", 1, FilterParameter::Category::Parameter, std::bind(&Blend::setDegree, this, std::placeholders::_1), std::bind(&Blend::getDegree, this))};

  LinkedChoicesFilterParameter::Pointer overlapMethod{LinkedChoicesFilterParameter::New("Overlap Method", "OverlapMethod", 0, FilterParameter::Parameter,
                                                                                        std::bind(&Blend::setOverlapMethod, this, std::placeholders::_1), std::bind(&Blend::getOverlapMethod, this),
                                                                                        {"Percent", "Origins"}, {"OverlapPercentage"})};

  FloatFilterParameter::Pointer overlapPercentage{FloatFilterParameter::New("Overlap Percentage", "OverlapPercentage", 0.0f, FilterParameter::Category::Parameter,
                                                                            std::bind(&Blend::setOverlapPercentage, this, std::placeholders::_1), std::bind(&Blend::getOverlapPercentage, this), 0)};

  StringFilterParameter::Pointer amName{StringFilterParameter::New("Attribute Matrix Name", "AttributeMatrixName", {}, FilterParameter::Category::Parameter,
                                                                   std::bind(&Blend::setAttributeMatrixName, this, std::placeholders::_1), std::bind(&Blend::getAttributeMatrixName, this), {})};

  StringFilterParameter::Pointer xAAName{StringFilterParameter::New("Attribute Array Name", "AttributeArrayName", {}, FilterParameter::Category::Parameter,
                                                                    std::bind(&Blend::setXAttributeArrayName, this, std::placeholders::_1), std::bind(&Blend::getXAttributeArrayName, this), {})};

  StringFilterParameter::Pointer yAAName{StringFilterParameter::New("Attribute Array Name", "AttributeArrayName", {}, FilterParameter::Category::Parameter,
                                                                    std::bind(&Blend::setYAttributeArrayName, this, std::placeholders::_1), std::bind(&Blend::getYAttributeArrayName, this), {})};

  StringFilterParameter::Pointer dataAAName{StringFilterParameter::New("Attribute Array Name", "AttributeArrayName", {}, FilterParameter::Category::Parameter,
                                                                       std::bind(&Blend::setDataAttributeArrayName, this, std::placeholders::_1), std::bind(&Blend::getDataAttributeArrayName, this),
                                                                       {})};

  //  parameters.push_back(dcs);
  parameters.push_back(maxIterations);
  parameters.push_back(degree);
  parameters.push_back(overlapMethod);
  parameters.push_back(overlapPercentage);
  parameters.push_back(amName);
  parameters.push_back(xAAName);
  parameters.push_back(yAAName);
  parameters.push_back(dataAAName);

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::dataCheck()
{
  setErrorCondition(0, "");
  setWarningCondition(0, "");
  //  OverlapMethod overlapMethod = static_cast<OverlapMethod>(m_OverlapMethod);
  // Check bounds of m_overlapPercent
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
  if(getCancel())
  {
    return;
  }

  if(getWarningCode() < 0)
  {
    QString ss = QObject::tr("Some warning message");
    setWarningCondition(-88888888, ss);
    notifyStatusMessage(ss);
  }

  if(getErrorCode() < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999, ss);
    notifyStatusMessage(ss);
    return;
  }

  // The types of each one of the components in the registration methods should
  // be instantiated. First, we select the image dimension and the type for
  // representing image pixels.
  //
  constexpr uint8_t Dimension = 2;
  using PixelType = float;
  //  The types of the input images are instantiated by the following lines.
  //
  using FixedImageType = itk::Image<PixelType, Dimension>;
  using MovingImageType = itk::Image<PixelType, Dimension>;
  using OptimizerType = itk::AmoebaOptimizer;

  // NOTE This will likely have to change
  using TransformType = itk::AffineTransform<double, Dimension>;

  // NOTE This will likely have to change
  // NOTE Change to a metric that will behave like the ITK FFTConvolution
  using MetricType = itk::MatchCardinalityImageToImageMetric<FixedImageType, MovingImageType>;

  //  Finally, the type of the interpolator is declared. The
  //  interpolator will evaluate the moving image at non-grid
  //  positions.
  using InterpolatorType = itk::NearestNeighborInterpolateImageFunction<MovingImageType, double>;

  //  The registration method type is instantiated using the types of the
  //  fixed and moving images. This class is responsible for interconnecting
  //  all the components we have described so far.
  // NOTE This will likely have to change
  using RegistrationType = itk::ImageRegistrationMethod<FixedImageType, MovingImageType>;
  //  Each one of the registration components is created using its
  //  \code{New()} method and is assigned to its respective
  //  \doxygen{SmartPointer}.
  //
  MetricType::Pointer metric = MetricType::New();
  TransformType::Pointer transform = TransformType::New();
  OptimizerType::Pointer optimizer = OptimizerType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  RegistrationType::Pointer registration = RegistrationType::New();

  metric->MeasureMatchesOff();
  registration->SetMetric(metric);
  registration->SetOptimizer(optimizer);
  registration->SetTransform(transform);
  registration->SetInterpolator(interpolator);

  //  In this example, the fixed and moving images are read from files. This
  //  requires the \doxygen{ImageRegistrationMethod} to acquire its inputs to
  //  the output of the readers.
  //
  itk::ImageFileReader<FixedImageType>::Pointer fixedImageReader = itk::ImageFileReader<FixedImageType>::New();
  itk::ImageFileReader<MovingImageType>::Pointer movingImageReader = itk::ImageFileReader<MovingImageType>::New();
  fixedImageReader->SetFileName(argv[1]);
  movingImageReader->SetFileName(argv[2]);
  registration->SetFixedImage(fixedImageReader->GetOutput());
  registration->SetMovingImage(movingImageReader->GetOutput());

  //  The registration can be restricted to consider only a particular region
  //  of the fixed image as input to the metric computation. This region is
  //  defined by the \code{SetFixedImageRegion()} method.  You could use this
  //  feature to reduce the computational time of the registration or to avoid
  //  unwanted objects present in the image affecting the registration outcome.
  //  In this example we use the full available content of the image. This
  //  region is identified by the \code{BufferedRegion} of the fixed image.
  //  Note that for this region to be valid the reader must first invoke its
  //  \code{Update()} method.
  //
  fixedImageReader->Update();
  movingImageReader->Update();
  registration->SetFixedImageRegion(fixedImageReader->GetOutput()->GetBufferedRegion());

  //
  // Here we initialize the transform to make sure that the center of
  // rotation is set to the center of mass of the object in the fixed image.
  //
  using TransformInitializerType = itk::CenteredTransformInitializer<TransformType, FixedImageType, MovingImageType>;
  TransformInitializerType::Pointer initializer = TransformInitializerType::New();
  initializer->SetTransform(transform);
  initializer->SetFixedImage(fixedImageReader->GetOutput());
  initializer->SetMovingImage(movingImageReader->GetOutput());
  initializer->MomentsOn();
  initializer->InitializeTransform();

  //  The parameters of the transform are initialized by passing them in an
  //  array. This can be used to setup an initial known correction of the
  //  misalignment. In this particular case, a translation transform is
  //  being used for the registration. The array of parameters for this
  //  transform is simply composed of the rotation matrix and the translation
  //  values along each dimension.
  //
  //  \index{itk::AffineTransform!GetNumberOfParameters()}
  //  \index{itk::RegistrationMethod!SetInitialTransformParameters()}
  //
  using ParametersType = RegistrationType::ParametersType;
  ParametersType initialParameters = transform->GetParameters();
  double tx = 0.0;
  double ty = 0.0;
  if(argc > 6)
  {
    tx = std::stod(argv[5]);
    ty = std::stod(argv[6]);
  }
  initialParameters[4] = tx; // Initial offset in mm along X
  initialParameters[5] = ty; // Initial offset in mm along Y
  registration->SetInitialTransformParameters(initialParameters);
  //  At this point the registration method is ready for execution. The
  //  optimizer is the component that drives the execution of the
  //  registration.  However, the ImageRegistrationMethod class
  //  orchestrates the ensemble to make sure that everything is in place
  //  before control is passed to the optimizer.
  //
  const unsigned int numberOfParameters = transform->GetNumberOfParameters();
  OptimizerType::ParametersType simplexDelta(numberOfParameters);
  // This parameter is tightly coupled to the translationScale below
  constexpr double stepInParametricSpace = 0.01;
  simplexDelta.Fill(stepInParametricSpace);
  optimizer->AutomaticInitialSimplexOff();
  optimizer->SetInitialSimplexDelta(simplexDelta);
  optimizer->SetParametersConvergenceTolerance(1e-4); // about 0.005 degrees
  optimizer->SetFunctionConvergenceTolerance(1e-6);   // variation in metric value
  optimizer->SetMaximumNumberOfIterations(200);
  // This parameter is tightly coupled to the stepInParametricSpace above.
  double translationScale = 1.0 / 1000.0;
  using OptimizerScalesType = OptimizerType::ScalesType;
  OptimizerScalesType optimizerScales(numberOfParameters);
  optimizerScales[0] = 1.0;
  optimizerScales[1] = 1.0;
  optimizerScales[2] = 1.0;
  optimizerScales[3] = 1.0;
  optimizerScales[4] = translationScale;
  optimizerScales[5] = translationScale;
  optimizer->SetScales(optimizerScales);

  //  The registration process is triggered by an invocation of the
  //  \code{Update()} method. If something goes wrong during the
  //  initialization or execution of the registration an exception will be
  //  thrown. We should therefore place the \code{Update()} method
  //  in a \code{try/catch} block as illustrated in the following lines.
  //
  try
  {
    // print out the initial metric value.  need to initialize the
    // registration method to force all the connections to be established.
    registration->Initialize();
    std::cout << "Initial Metric value  = " << metric->GetValue(initialParameters) << std::endl;
    // run the registration
    registration->Update();
    std::cout << "Optimizer stop condition = " << registration->GetOptimizer()->GetStopConditionDescription() << std::endl;
  } catch(itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    // TODO Set an eror for the filter
  }
  // In a real application, you may attempt to recover from the error in the
  // catch block. Here we are simply printing out a message and then
  // terminating the execution of the program.
  //
  //
  //  The result of the registration process is an array of parameters that
  //  defines the spatial transformation in an unique way. This final result is
  //  obtained using the \code{GetLastTransformParameters()} method.
  //
  //  \index{itk::RegistrationMethod!GetLastTransformParameters()}
  //
  ParametersType finalParameters = registration->GetLastTransformParameters();
  //  In the case of the \doxygen{AffineTransform}, there is a straightforward
  //  interpretation of the parameters.  The last two elements of the array
  //  corresponds to a translation along one spatial dimension.
  //
  const double TranslationAlongX = finalParameters[4];
  const double TranslationAlongY = finalParameters[5];
  //  The optimizer can be queried for the actual number of iterations
  //  performed to reach convergence.
  //
  const unsigned int numberOfIterations = optimizer->GetOptimizer()->get_num_evaluations();
  //  The value of the image metric corresponding to the last set of parameters
  //  can be obtained with the \code{GetValue()} method of the optimizer. Since
  //  the AmoebaOptimizer does not yet support a call to GetValue(), we will
  //  simply re-evaluate the metric at the final parameters.
  //
  const double bestValue = metric->GetValue(finalParameters);
  // Print out results
  //
  std::cout << "Result = " << std::endl;
  std::cout << " Translation X = " << TranslationAlongX << std::endl;
  std::cout << " Translation Y = " << TranslationAlongY << std::endl;
  std::cout << " Iterations    = " << numberOfIterations << std::endl;
  std::cout << " Metric value  = " << bestValue << std::endl;
  //  It is common, as the last step of a registration task, to use the
  //  resulting transform to map the moving image into the fixed image space.
  //  This is easily done with the \doxygen{ResampleImageFilter}. Please
  //  refer to Section~\ref{sec:ResampleImageFilter} for details on the use
  //  of this filter.  First, a ResampleImageFilter type is instantiated
  //  using the image types. It is convenient to use the fixed image type as
  //  the output type since it is likely that the transformed moving image
  //  will be compared with the fixed image.
  //
  using ResampleFilterType = itk::ResampleImageFilter<MovingImageType, FixedImageType>;
  //  A transform of the same type used in the registration process should be
  //  created and initialized with the parameters resulting from the
  //  registration process.
  //
  //  \index{itk::ImageRegistrationMethod!Resampling image}
  //
  TransformType::Pointer finalTransform = TransformType::New();
  finalTransform->SetParameters(finalParameters);
  finalTransform->SetFixedParameters(transform->GetFixedParameters());
  std::cout << "Final Transform " << std::endl;
  finalTransform->Print(std::cout);
  //  Then a resampling filter is created and the corresponding transform and
  //  moving image connected as inputs.
  //
  ResampleFilterType::Pointer resample = ResampleFilterType::New();
  resample->SetTransform(finalTransform);
  resample->SetInput(movingImageReader->GetOutput());
  //  As described in Section \ref{sec:ResampleImageFilter}, the
  //  ResampleImageFilter requires additional parameters to be
  //  specified, in particular, the spacing, origin and size of the output
  //  image. The default pixel value is also set to the standard label
  //  for "unknown" or background. Finally, we need to set the
  //  interpolator to be the same type of interpolator as the
  //  registration method used (nearest neighbor).
  //
  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
  resample->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
  resample->SetOutputOrigin(fixedImage->GetOrigin());
  resample->SetOutputSpacing(fixedImage->GetSpacing());
  resample->SetOutputDirection(fixedImage->GetDirection());
  resample->SetDefaultPixelValue(0);
  resample->SetInterpolator(interpolator);
  //  The output of the filter is passed to a writer that will store the
  //  image in a file. An \doxygen{CastImageFilter} is used to convert the
  //  pixel type of the resampled image to the final type used by the
  //  writer. The cast and writer filters are instantiated below.
  //
  using OutputPixelType = unsigned short;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;
  using CastFilterType = itk::CastImageFilter<FixedImageType, OutputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;
  //  The filters are created by invoking their \code{New()}
  //  method.
  //
  WriterType::Pointer writer = WriterType::New();
  CastFilterType::Pointer caster = CastFilterType::New();
  writer->SetFileName(argv[3]);
  //  The \code{Update()} method of the writer is invoked in order to trigger
  //  the execution of the pipeline.
  //
  caster->SetInput(resample->GetOutput());
  writer->SetInput(caster->GetOutput());
  writer->Update();
  //
  //  The fixed image and the transformed moving image can easily be compared
  //  using the \code{SquaredDifferenceImageFilter}. This pixel-wise
  //  filter computes the squared value of the difference between homologous
  //  pixels of its input images.
  //
  using DifferenceFilterType = itk::SquaredDifferenceImageFilter<FixedImageType, FixedImageType, OutputImageType>;
  DifferenceFilterType::Pointer difference = DifferenceFilterType::New();
  difference->SetInput1(fixedImageReader->GetOutput());
  difference->SetInput2(resample->GetOutput());

  //  Its output can be passed to another writer.
  //
  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(difference->GetOutput());
  if(argc > 4)
  {
    writer2->SetFileName(argv[4]);
    writer2->Update();
  }

  // TODO
  // Set up new Attribute matrix with results of filter
  // Should contain the transformation matrix (a)
  // And a data array with a single element for the iterations run
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
