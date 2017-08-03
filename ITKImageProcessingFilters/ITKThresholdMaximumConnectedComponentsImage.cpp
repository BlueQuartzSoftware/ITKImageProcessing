// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKThresholdMaximumConnectedComponentsImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"

// Include the MOC generated file for this class
#include "moc_ITKThresholdMaximumConnectedComponentsImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdMaximumConnectedComponentsImage::ITKThresholdMaximumConnectedComponentsImage()
: ITKImageBase()
{
  m_MinimumObjectSizeInPixels = StaticCastScalar<double, double, double>(0u);
  m_UpperBoundary = StaticCastScalar<double, double, double>(std::numeric_limits<double>::max());
  m_InsideValue = StaticCastScalar<int, int, int>(1u);
  m_OutsideValue = StaticCastScalar<int, int, int>(0u);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdMaximumConnectedComponentsImage::~ITKThresholdMaximumConnectedComponentsImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MinimumObjectSizeInPixels", MinimumObjectSizeInPixels, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("UpperBoundary", UpperBoundary, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("InsideValue", InsideValue, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKThresholdMaximumConnectedComponentsImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKThresholdMaximumConnectedComponentsImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setMinimumObjectSizeInPixels(reader->readValue("MinimumObjectSizeInPixels", getMinimumObjectSizeInPixels()));
  setUpperBoundary(reader->readValue("UpperBoundary", getUpperBoundary()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKThresholdMaximumConnectedComponentsImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_MinimumObjectSizeInPixels, "MinimumObjectSizeInPixels", 1);
  this->CheckIntegerEntry<uint8_t, int>(m_InsideValue, "InsideValue", 1);
  this->CheckIntegerEntry<uint8_t, int>(m_OutsideValue, "OutsideValue", 1);

  setErrorCondition(0);
  setWarningCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4, uint8_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKThresholdMaximumConnectedComponentsImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::ThresholdMaximumConnectedComponentsImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetMinimumObjectSizeInPixels(static_cast<uint32_t>(m_MinimumObjectSizeInPixels));
  filter->SetUpperBoundary(static_cast<typename InputImageType::PixelType>(std::min<double>(this->m_UpperBoundary, itk::NumericTraits<typename InputImageType::PixelType>::max())));
  filter->SetInsideValue(static_cast<uint8_t>(m_InsideValue));
  filter->SetOutsideValue(static_cast<uint8_t>(m_OutsideValue));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, uint8_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKThresholdMaximumConnectedComponentsImage::newFilterInstance(bool copyFilterParameters)
{
  ITKThresholdMaximumConnectedComponentsImage::Pointer filter = ITKThresholdMaximumConnectedComponentsImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKThresholdMaximumConnectedComponentsImage::getHumanLabel()
{
  return "ITK::Threshold Maximum Connected Components Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKThresholdMaximumConnectedComponentsImage::getSubGroupName()
{
  return "ITK Thresholding";
}
