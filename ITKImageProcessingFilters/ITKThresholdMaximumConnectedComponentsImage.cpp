// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKThresholdMaximumConnectedComponentsImage.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdMaximumConnectedComponentsImage::ITKThresholdMaximumConnectedComponentsImage()
{
  m_MinimumObjectSizeInPixels = StaticCastScalar<double, double, double>(0u);
  m_UpperBoundary = StaticCastScalar<double, double, double>(std::numeric_limits<double>::max());
  m_InsideValue = StaticCastScalar<int, int, int>(1u);
  m_OutsideValue = StaticCastScalar<int, int, int>(0u);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdMaximumConnectedComponentsImage::~ITKThresholdMaximumConnectedComponentsImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MinimumObjectSizeInPixels", MinimumObjectSizeInPixels, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("UpperBoundary", UpperBoundary, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("InsideValue", InsideValue, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
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
  setMinimumObjectSizeInPixels(reader->readValue("MinimumObjectSizeInPixels", getMinimumObjectSizeInPixels()));
  setUpperBoundary(reader->readValue("UpperBoundary", getUpperBoundary()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKThresholdMaximumConnectedComponentsImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_MinimumObjectSizeInPixels, "MinimumObjectSizeInPixels", true);
  this->CheckIntegerEntry<uint8_t, int>(m_InsideValue, "InsideValue", true);
  this->CheckIntegerEntry<uint8_t, int>(m_OutsideValue, "OutsideValue", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::dataCheck()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, uint8_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKThresholdMaximumConnectedComponentsImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::ThresholdMaximumConnectedComponentsImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetMinimumObjectSizeInPixels(static_cast<uint32_t>(m_MinimumObjectSizeInPixels));
  filter->SetUpperBoundary(static_cast<typename InputImageType::PixelType>(std::min<double>(this->m_UpperBoundary, itk::NumericTraits<typename InputImageType::PixelType>::max())));
  filter->SetInsideValue(static_cast<uint8_t>(m_InsideValue));
  filter->SetOutsideValue(static_cast<uint8_t>(m_OutsideValue));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
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
AbstractFilter::Pointer ITKThresholdMaximumConnectedComponentsImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKThresholdMaximumConnectedComponentsImage::Pointer filter = ITKThresholdMaximumConnectedComponentsImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKThresholdMaximumConnectedComponentsImage::getHumanLabel() const
{
  return "ITK::Threshold Maximum Connected Components Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKThresholdMaximumConnectedComponentsImage::getUuid() const
{
  return QUuid("{2a531add-79fd-5c07-98a6-f875d2ecef4e}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKThresholdMaximumConnectedComponentsImage::getSubGroupName() const
{
  return "ITK SegmentationPostProcessing";
}

// -----------------------------------------------------------------------------
ITKThresholdMaximumConnectedComponentsImage::Pointer ITKThresholdMaximumConnectedComponentsImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKThresholdMaximumConnectedComponentsImage> ITKThresholdMaximumConnectedComponentsImage::New()
{
  struct make_shared_enabler : public ITKThresholdMaximumConnectedComponentsImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKThresholdMaximumConnectedComponentsImage::getNameOfClass() const
{
  return QString("ITKThresholdMaximumConnectedComponentsImage");
}

// -----------------------------------------------------------------------------
QString ITKThresholdMaximumConnectedComponentsImage::ClassName()
{
  return QString("ITKThresholdMaximumConnectedComponentsImage");
}

// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::setMinimumObjectSizeInPixels(double value)
{
  m_MinimumObjectSizeInPixels = value;
}

// -----------------------------------------------------------------------------
double ITKThresholdMaximumConnectedComponentsImage::getMinimumObjectSizeInPixels() const
{
  return m_MinimumObjectSizeInPixels;
}

// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::setUpperBoundary(double value)
{
  m_UpperBoundary = value;
}

// -----------------------------------------------------------------------------
double ITKThresholdMaximumConnectedComponentsImage::getUpperBoundary() const
{
  return m_UpperBoundary;
}

// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::setInsideValue(int value)
{
  m_InsideValue = value;
}

// -----------------------------------------------------------------------------
int ITKThresholdMaximumConnectedComponentsImage::getInsideValue() const
{
  return m_InsideValue;
}

// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::setOutsideValue(int value)
{
  m_OutsideValue = value;
}

// -----------------------------------------------------------------------------
int ITKThresholdMaximumConnectedComponentsImage::getOutsideValue() const
{
  return m_OutsideValue;
}
