// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKThresholdImage.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdImage::ITKThresholdImage()
{
  m_Lower = StaticCastScalar<double, double, double>(0.0);
  m_Upper = StaticCastScalar<double, double, double>(1.0);
  m_OutsideValue = StaticCastScalar<double, double, double>(0.0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdImage::~ITKThresholdImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Lower", Lower, FilterParameter::Parameter, ITKThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Upper", Upper, FilterParameter::Parameter, ITKThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKThresholdImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKThresholdImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKThresholdImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setLower(reader->readValue("Lower", getLower()));
  setUpper(reader->readValue("Upper", getUpper()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKThresholdImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKThresholdImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  // typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::ThresholdImageFilter<InputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetLower(static_cast<double>(m_Lower));
  filter->SetUpper(static_cast<double>(m_Upper));
  filter->SetOutsideValue(static_cast<double>(m_OutsideValue));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKThresholdImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKThresholdImage::Pointer filter = ITKThresholdImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKThresholdImage::getHumanLabel() const
{
  return "ITK::Threshold Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKThresholdImage::getUuid() const
{
  return QUuid("{5845ee06-5c8a-5a74-80fb-c820bd8dfb75}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKThresholdImage::getSubGroupName() const
{
  return "ITK Thresholding";
}

// -----------------------------------------------------------------------------
ITKThresholdImage::Pointer ITKThresholdImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKThresholdImage> ITKThresholdImage::New()
{
  struct make_shared_enabler : public ITKThresholdImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKThresholdImage::getNameOfClass() const
{
  return QString("ITKThresholdImage");
}

// -----------------------------------------------------------------------------
QString ITKThresholdImage::ClassName()
{
  return QString("ITKThresholdImage");
}

// -----------------------------------------------------------------------------
void ITKThresholdImage::setLower(double value)
{
  m_Lower = value;
}

// -----------------------------------------------------------------------------
double ITKThresholdImage::getLower() const
{
  return m_Lower;
}

// -----------------------------------------------------------------------------
void ITKThresholdImage::setUpper(double value)
{
  m_Upper = value;
}

// -----------------------------------------------------------------------------
double ITKThresholdImage::getUpper() const
{
  return m_Upper;
}

// -----------------------------------------------------------------------------
void ITKThresholdImage::setOutsideValue(double value)
{
  m_OutsideValue = value;
}

// -----------------------------------------------------------------------------
double ITKThresholdImage::getOutsideValue() const
{
  return m_OutsideValue;
}
