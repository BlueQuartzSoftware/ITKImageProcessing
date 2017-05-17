// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKFFTNormalizedCorrelationImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include <itkCastImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"

// Include the MOC generated file for this class
#include "moc_ITKFFTNormalizedCorrelationImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKFFTNormalizedCorrelationImage::ITKFFTNormalizedCorrelationImage()
: ITKImageBase()
{
  m_RequiredNumberOfOverlappingPixels = StaticCastScalar<double, double, double>(0);
  m_RequiredFractionOfOverlappingPixels = StaticCastScalar<double, double, double>(0.0);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKFFTNormalizedCorrelationImage::~ITKFFTNormalizedCorrelationImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFFTNormalizedCorrelationImage::setupFilterParameters()
{
  FilterParameterVector parameters;

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
  setErrorCondition(0);

  // Check consistency of parameters
  this->CheckIntegerEntry<uint64_t, double>(m_RequiredNumberOfOverlappingPixels, "RequiredNumberOfOverlappingPixels", 1);

  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
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
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> IntermediateImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;

  typedef itk::FFTNormalizedCorrelationImageFilter<InputImageType, IntermediateImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();

  filter->SetRequiredNumberOfOverlappingPixels(static_cast<itk::SizeValueType>(m_RequiredNumberOfOverlappingPixels));
  filter->SetRequiredFractionOfOverlappingPixels(static_cast<double>(m_RequiredFractionOfOverlappingPixels));

  // Set reference image.
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
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
    setErrorCondition(-55558);
    QString errorMessage = "ITK exception was thrown while filtering input image: %1";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(err.GetDescription()), getErrorCondition());
    return;
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
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
AbstractFilter::Pointer ITKFFTNormalizedCorrelationImage::newFilterInstance(bool copyFilterParameters)
{
  ITKFFTNormalizedCorrelationImage::Pointer filter = ITKFFTNormalizedCorrelationImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKFFTNormalizedCorrelationImage::getHumanLabel()
{
  return "ITK::FFT Normalized Correlation Image";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKFFTNormalizedCorrelationImage::getSubGroupName()
{
  return "ITK Registration";
}
