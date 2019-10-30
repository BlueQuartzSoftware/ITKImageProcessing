// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include <memory>

#include "ITKFFTNormalizedCorrelationImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include <itkCastImageFilter.h>

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKFFTNormalizedCorrelationImage::ITKFFTNormalizedCorrelationImage()
{
  m_RequiredNumberOfOverlappingPixels = StaticCastScalar<double, double, double>(0);
  m_RequiredFractionOfOverlappingPixels = StaticCastScalar<double, double, double>(0.0);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKFFTNormalizedCorrelationImage::~ITKFFTNormalizedCorrelationImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFFTNormalizedCorrelationImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("RequiredNumberOfOverlappingPixels", RequiredNumberOfOverlappingPixels, FilterParameter::Parameter, ITKFFTNormalizedCorrelationImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("RequiredFractionOfOverlappingPixels", RequiredFractionOfOverlappingPixels, FilterParameter::Parameter, ITKFFTNormalizedCorrelationImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Fixed Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKFFTNormalizedCorrelationImage, req));
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Moving Attribute Array to filter", MovingCellArrayPath, FilterParameter::RequiredArray, ITKFFTNormalizedCorrelationImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKFFTNormalizedCorrelationImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFFTNormalizedCorrelationImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setSelectedCellArrayPath(reader->readDataArrayPath("MovingCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setRequiredNumberOfOverlappingPixels(reader->readValue("RequiredNumberOfOverlappingPixels", getRequiredNumberOfOverlappingPixels()));
  setRequiredFractionOfOverlappingPixels(reader->readValue("RequiredFractionOfOverlappingPixels", getRequiredFractionOfOverlappingPixels()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKFFTNormalizedCorrelationImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<uint64_t, double>(m_RequiredNumberOfOverlappingPixels, "RequiredNumberOfOverlappingPixels", true);

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFFTNormalizedCorrelationImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKFFTNormalizedCorrelationImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> IntermediateImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;

  typedef itk::FFTNormalizedCorrelationImageFilter<InputImageType, IntermediateImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();

  filter->SetRequiredNumberOfOverlappingPixels(static_cast<itk::SizeValueType>(m_RequiredNumberOfOverlappingPixels));
  filter->SetRequiredFractionOfOverlappingPixels(static_cast<double>(m_RequiredFractionOfOverlappingPixels));

  // Set reference image.
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::InPlaceDream3DDataToImageFilter<InputPixelType, Dimension> toITKType;
  DataArrayPath dapMoving = getMovingCellArrayPath();
  DataContainer::Pointer dcMoving = getDataContainerArray()->getDataContainer(dapMoving.getDataContainerName());
  // Create a bridge to wrap an existing DREAM.3D array with an ItkImage container
  typename toITKType::Pointer toITKMoving = toITKType::New();
  toITKMoving->SetInput(dcMoving);
  toITKMoving->SetInPlace(true);
  toITKMoving->SetAttributeMatrixArrayName(getMovingCellArrayPath().getAttributeMatrixName().toStdString());
  toITKMoving->SetDataArrayName(getMovingCellArrayPath().getDataArrayName().toStdString());
  filter->SetMovingImage(toITKMoving->GetOutput());

  try
  {
    DataArrayPath dap = getSelectedCellArrayPath();
    DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());

    // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container
    typename toITKType::Pointer toITK = toITKType::New();
    toITK->SetInput(dc);
    toITK->SetInPlace(true);
    toITK->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(getSelectedCellArrayPath().getDataArrayName().toStdString());

    itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
    interruption->SetFilter(this);

    // Set up filter
    filter->SetFixedImage(toITK->GetOutput());
    filter->AddObserver(itk::ProgressEvent(), interruption);

    typedef itk::CastImageFilter<IntermediateImageType, OutputImageType> CasterType;
    typename CasterType::Pointer caster = CasterType::New();
    caster->SetInput(filter->GetOutput());
    caster->Update();

    typename OutputImageType::Pointer image = OutputImageType::New();
    image = caster->GetOutput();
    image->DisconnectPipeline();
    std::string outputArrayName(getNewCellArrayName().toStdString());

    typedef itk::InPlaceImageToDream3DDataFilter<OutputPixelType, Dimension> toDream3DType;
    typename toDream3DType::Pointer toDream3DFilter = toDream3DType::New();
    toDream3DFilter->SetInput(image);
    toDream3DFilter->SetInPlace(true);
    toDream3DFilter->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
    toDream3DFilter->SetDataArrayName(outputArrayName);
    toDream3DFilter->SetDataContainer(dc);
    toDream3DFilter->Update();
  } catch(itk::ExceptionObject& err)
  {
    QString errorMessage = "ITK exception was thrown while filtering input image: %1";
    setErrorCondition(-55558, errorMessage.arg(err.GetDescription()));
    return;
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFFTNormalizedCorrelationImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKFFTNormalizedCorrelationImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKFFTNormalizedCorrelationImage::Pointer filter = ITKFFTNormalizedCorrelationImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKFFTNormalizedCorrelationImage::getHumanLabel() const
{
  return "ITK::FFT Normalized Correlation Image";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKFFTNormalizedCorrelationImage::getUuid() const
{
  return QUuid("{a0d962b7-9d5c-5abc-a078-1fe795df4663}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKFFTNormalizedCorrelationImage::getSubGroupName() const
{
  return "ITK Registration";
}

// -----------------------------------------------------------------------------
ITKFFTNormalizedCorrelationImage::Pointer ITKFFTNormalizedCorrelationImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKFFTNormalizedCorrelationImage> ITKFFTNormalizedCorrelationImage::New()
{
  struct make_shared_enabler : public ITKFFTNormalizedCorrelationImage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKFFTNormalizedCorrelationImage::getNameOfClass() const
{
  return QString("ITKFFTNormalizedCorrelationImage");
}

// -----------------------------------------------------------------------------
QString ITKFFTNormalizedCorrelationImage::ClassName()
{
  return QString("ITKFFTNormalizedCorrelationImage");
}

// -----------------------------------------------------------------------------
void ITKFFTNormalizedCorrelationImage::setMovingCellArrayPath(const DataArrayPath& value)
{
  m_MovingCellArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ITKFFTNormalizedCorrelationImage::getMovingCellArrayPath() const
{
  return m_MovingCellArrayPath;
}

// -----------------------------------------------------------------------------
void ITKFFTNormalizedCorrelationImage::setRequiredNumberOfOverlappingPixels(double value)
{
  m_RequiredNumberOfOverlappingPixels = value;
}

// -----------------------------------------------------------------------------
double ITKFFTNormalizedCorrelationImage::getRequiredNumberOfOverlappingPixels() const
{
  return m_RequiredNumberOfOverlappingPixels;
}

// -----------------------------------------------------------------------------
void ITKFFTNormalizedCorrelationImage::setRequiredFractionOfOverlappingPixels(double value)
{
  m_RequiredFractionOfOverlappingPixels = value;
}

// -----------------------------------------------------------------------------
double ITKFFTNormalizedCorrelationImage::getRequiredFractionOfOverlappingPixels() const
{
  return m_RequiredFractionOfOverlappingPixels;
}


