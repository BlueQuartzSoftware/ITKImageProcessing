/*
 * Your License or Copyright can go here
 */

#include <memory>

#include "ITKPatchBasedDenoisingImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include <itkGaussianRandomSpatialNeighborSubsampler.h>

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKPatchBasedDenoisingImage::ITKPatchBasedDenoisingImage()
{
  m_KernelBandwidthSigma = StaticCastScalar<double, double, double>(400.0);
  m_PatchRadius = StaticCastScalar<double, double, double>(4u);
  m_NumberOfIterations = StaticCastScalar<double, double, double>(1u);
  m_NumberOfSamplePatches = StaticCastScalar<double, double, double>(200u);
  m_SampleVariance = StaticCastScalar<double, double, double>(400.0);
  m_NoiseSigma = StaticCastScalar<double, double, double>(0.0);
  m_NoiseModelFidelityWeight = StaticCastScalar<double, double, double>(0.0);
  m_AlwaysTreatComponentsAsEuclidean = StaticCastScalar<bool, bool, bool>(false);
  m_KernelBandwidthEstimation = StaticCastScalar<bool, bool, bool>(false);
  m_KernelBandwidthMultiplicationFactor = StaticCastScalar<double, double, double>(1.0);
  m_KernelBandwidthUpdateFrequency = StaticCastScalar<double, double, double>(3u);
  m_KernelBandwidthFractionPixelsForEstimation = StaticCastScalar<double, double, double>(0.2);
  m_NoiseModel = 0; //  NOMODEL
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKPatchBasedDenoisingImage::~ITKPatchBasedDenoisingImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Noise Model");
    parameter->setPropertyName("NoiseModel");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKPatchBasedDenoisingImage, this, NoiseModel));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKPatchBasedDenoisingImage, this, NoiseModel));

    QVector<QString> choices;
    choices.push_back("No Model");
    choices.push_back("Gaussian");
    choices.push_back("Rician");
    choices.push_back("Poisson");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Parameter);
    parameters.push_back(parameter);
  }

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("KernelBandwidthSigma", KernelBandwidthSigma, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("PatchRadius", PatchRadius, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfSamplePatches", NumberOfSamplePatches, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("SampleVariance", SampleVariance, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NoiseSigma", NoiseSigma, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NoiseModelFidelityWeight", NoiseModelFidelityWeight, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("AlwaysTreatComponentsAsEuclidean", AlwaysTreatComponentsAsEuclidean, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("KernelBandwidthEstimation", KernelBandwidthEstimation, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("KernelBandwidthMultiplicationFactor", KernelBandwidthMultiplicationFactor, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("KernelBandwidthUpdateFrequency", KernelBandwidthUpdateFrequency, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("KernelBandwidthFractionPixelsForEstimation", KernelBandwidthFractionPixelsForEstimation, FilterParameter::Parameter, ITKPatchBasedDenoisingImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKPatchBasedDenoisingImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKPatchBasedDenoisingImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKPatchBasedDenoisingImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setKernelBandwidthSigma(reader->readValue("KernelBandwidthSigma", getKernelBandwidthSigma()));
  setPatchRadius(reader->readValue("PatchRadius", getPatchRadius()));
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));
  setNumberOfSamplePatches(reader->readValue("NumberOfSamplePatches", getNumberOfSamplePatches()));
  setSampleVariance(reader->readValue("SampleVariance", getSampleVariance()));
  setNoiseSigma(reader->readValue("NoiseSigma", getNoiseSigma()));
  setNoiseModelFidelityWeight(reader->readValue("NoiseModelFidelityWeight", getNoiseModelFidelityWeight()));
  setAlwaysTreatComponentsAsEuclidean(reader->readValue("AlwaysTreatComponentsAsEuclidean", getAlwaysTreatComponentsAsEuclidean()));
  setKernelBandwidthEstimation(reader->readValue("KernelBandwidthEstimation", getKernelBandwidthEstimation()));
  setKernelBandwidthMultiplicationFactor(reader->readValue("KernelBandwidthMultiplicationFactor", getKernelBandwidthMultiplicationFactor()));
  setKernelBandwidthUpdateFrequency(reader->readValue("KernelBandwidthUpdateFrequency", getKernelBandwidthUpdateFrequency()));
  setKernelBandwidthFractionPixelsForEstimation(reader->readValue("KernelBandwidthFractionPixelsForEstimation", getKernelBandwidthFractionPixelsForEstimation()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKPatchBasedDenoisingImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_PatchRadius, "PatchRadius", true);
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfIterations, "NumberOfIterations", true);
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfSamplePatches, "NumberOfSamplePatches", true);
  this->CheckIntegerEntry<uint32_t, double>(m_KernelBandwidthUpdateFrequency, "KernelBandwidthUpdateFrequency", true);

  clearErrorCode();
  clearWarningCode();
  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4, double, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKPatchBasedDenoisingImage::filter()
{
  typedef itk::Image<OutputPixelType, Dimension> RealImageType;
  // define filter
  typedef itk::PatchBasedDenoisingImageFilter<RealImageType, RealImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  typedef itk::InPlaceDream3DDataToImageFilter<InputPixelType, Dimension> toITKType;
  DataArrayPath dap = getSelectedCellArrayPath();
  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());
  typename toITKType::Pointer toITK = toITKType::New();
  toITK->SetInput(dc);
  toITK->SetInPlace(true);
  toITK->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
  toITK->SetDataArrayName(getSelectedCellArrayPath().getDataArrayName().toStdString());
  typename FilterType::RealArrayType a(toITK->GetOutput()->GetNumberOfComponentsPerPixel());
  a.Fill(m_KernelBandwidthSigma);
  filter->SetKernelBandwidthSigma(a);
  filter->SetPatchRadius(static_cast<uint32_t>(m_PatchRadius));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  if(this->m_NoiseSigma != 0.0)
  {
    filter->SetNoiseSigma(this->m_NoiseSigma);
  }
  filter->SetNoiseModelFidelityWeight(static_cast<double>(m_NoiseModelFidelityWeight));
  filter->SetAlwaysTreatComponentsAsEuclidean(static_cast<bool>(m_AlwaysTreatComponentsAsEuclidean));
  filter->SetKernelBandwidthEstimation(static_cast<bool>(m_KernelBandwidthEstimation));
  filter->SetKernelBandwidthMultiplicationFactor(static_cast<double>(m_KernelBandwidthMultiplicationFactor));
  filter->SetKernelBandwidthUpdateFrequency(static_cast<uint32_t>(m_KernelBandwidthUpdateFrequency));
  filter->SetKernelBandwidthFractionPixelsForEstimation(static_cast<double>(m_KernelBandwidthFractionPixelsForEstimation));
#if ITK_VERSION_MAJOR >= 5
  filter->SetNumberOfWorkUnits(this->getNumberOfThreads());
#else
  filter->SetNumberOfThreads(this->getNumberOfThreads());
#endif
  typedef itk::Statistics::GaussianRandomSpatialNeighborSubsampler<typename FilterType::PatchSampleType, typename RealImageType::RegionType> SamplerType;
  typename SamplerType::Pointer sampler = SamplerType::New();
  sampler->SetVariance(m_SampleVariance);
  sampler->SetRadius(itk::Math::Floor<unsigned int>(std::sqrt(m_SampleVariance) * 2.5));
  sampler->SetNumberOfResultsRequested(m_NumberOfSamplePatches);
  filter->SetSampler(sampler);
  typedef typename itk::PatchBasedDenoisingBaseImageFilter<RealImageType, RealImageType>::NoiseModelType NoiseModelType;
  NoiseModelType noiseModel = static_cast<NoiseModelType>(m_NoiseModel);
  filter->SetNoiseModel(noiseModel);

  this->ITKImageProcessingBase::filterCastToFloat<InputPixelType, OutputPixelType, Dimension, FilterType, RealImageType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, double, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKPatchBasedDenoisingImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKPatchBasedDenoisingImage::Pointer filter = ITKPatchBasedDenoisingImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKPatchBasedDenoisingImage::getHumanLabel() const
{
  return "ITK::Patch Based Denoising Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKPatchBasedDenoisingImage::getUuid() const
{
  return QUuid("{ed61aebd-3a47-5ee1-8c9e-4ce205111b76}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKPatchBasedDenoisingImage::getSubGroupName() const
{
  return "ITK Denoising";
}

// -----------------------------------------------------------------------------
ITKPatchBasedDenoisingImage::Pointer ITKPatchBasedDenoisingImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKPatchBasedDenoisingImage> ITKPatchBasedDenoisingImage::New()
{
  struct make_shared_enabler : public ITKPatchBasedDenoisingImage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKPatchBasedDenoisingImage::getNameOfClass() const
{
  return QString("ITKPatchBasedDenoisingImage");
}

// -----------------------------------------------------------------------------
QString ITKPatchBasedDenoisingImage::ClassName()
{
  return QString("ITKPatchBasedDenoisingImage");
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setKernelBandwidthSigma(double value)
{
  m_KernelBandwidthSigma = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getKernelBandwidthSigma() const
{
  return m_KernelBandwidthSigma;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setPatchRadius(double value)
{
  m_PatchRadius = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getPatchRadius() const
{
  return m_PatchRadius;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setNumberOfIterations(double value)
{
  m_NumberOfIterations = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getNumberOfIterations() const
{
  return m_NumberOfIterations;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setNumberOfSamplePatches(double value)
{
  m_NumberOfSamplePatches = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getNumberOfSamplePatches() const
{
  return m_NumberOfSamplePatches;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setSampleVariance(double value)
{
  m_SampleVariance = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getSampleVariance() const
{
  return m_SampleVariance;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setNoiseSigma(double value)
{
  m_NoiseSigma = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getNoiseSigma() const
{
  return m_NoiseSigma;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setNoiseModelFidelityWeight(double value)
{
  m_NoiseModelFidelityWeight = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getNoiseModelFidelityWeight() const
{
  return m_NoiseModelFidelityWeight;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setAlwaysTreatComponentsAsEuclidean(bool value)
{
  m_AlwaysTreatComponentsAsEuclidean = value;
}

// -----------------------------------------------------------------------------
bool ITKPatchBasedDenoisingImage::getAlwaysTreatComponentsAsEuclidean() const
{
  return m_AlwaysTreatComponentsAsEuclidean;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setKernelBandwidthEstimation(bool value)
{
  m_KernelBandwidthEstimation = value;
}

// -----------------------------------------------------------------------------
bool ITKPatchBasedDenoisingImage::getKernelBandwidthEstimation() const
{
  return m_KernelBandwidthEstimation;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setKernelBandwidthMultiplicationFactor(double value)
{
  m_KernelBandwidthMultiplicationFactor = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getKernelBandwidthMultiplicationFactor() const
{
  return m_KernelBandwidthMultiplicationFactor;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setKernelBandwidthUpdateFrequency(double value)
{
  m_KernelBandwidthUpdateFrequency = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getKernelBandwidthUpdateFrequency() const
{
  return m_KernelBandwidthUpdateFrequency;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setKernelBandwidthFractionPixelsForEstimation(double value)
{
  m_KernelBandwidthFractionPixelsForEstimation = value;
}

// -----------------------------------------------------------------------------
double ITKPatchBasedDenoisingImage::getKernelBandwidthFractionPixelsForEstimation() const
{
  return m_KernelBandwidthFractionPixelsForEstimation;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setNoiseModel(int value)
{
  m_NoiseModel = value;
}

// -----------------------------------------------------------------------------
int ITKPatchBasedDenoisingImage::getNoiseModel() const
{
  return m_NoiseModel;
}

// -----------------------------------------------------------------------------
void ITKPatchBasedDenoisingImage::setNumberOfThreads(int value)
{
  m_NumberOfThreads = value;
}

// -----------------------------------------------------------------------------
int ITKPatchBasedDenoisingImage::getNumberOfThreads() const
{
  return m_NumberOfThreads;
}


