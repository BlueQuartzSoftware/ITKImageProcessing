// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKApproximateSignedDistanceMapImage.h"
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
ITKApproximateSignedDistanceMapImage::ITKApproximateSignedDistanceMapImage()
{
  m_InsideValue = StaticCastScalar<double, double, double>(1u);
  m_OutsideValue = StaticCastScalar<double, double, double>(0u);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKApproximateSignedDistanceMapImage::~ITKApproximateSignedDistanceMapImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKApproximateSignedDistanceMapImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("InsideValue", InsideValue, FilterParameter::Parameter, ITKApproximateSignedDistanceMapImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKApproximateSignedDistanceMapImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKApproximateSignedDistanceMapImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKApproximateSignedDistanceMapImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKApproximateSignedDistanceMapImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKApproximateSignedDistanceMapImage::dataCheckImpl()
{
  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKApproximateSignedDistanceMapImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKApproximateSignedDistanceMapImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::ApproximateSignedDistanceMapImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInsideValue(static_cast<double>(m_InsideValue));
  filter->SetOutsideValue(static_cast<double>(m_OutsideValue));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKApproximateSignedDistanceMapImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKApproximateSignedDistanceMapImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKApproximateSignedDistanceMapImage::Pointer filter = ITKApproximateSignedDistanceMapImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKApproximateSignedDistanceMapImage::getHumanLabel() const
{
  return "ITK::Approximate Signed Distance Map Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKApproximateSignedDistanceMapImage::getUuid() const
{
  return QUuid("{066712e3-0378-566e-8236-1796c88d5e02}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKApproximateSignedDistanceMapImage::getSubGroupName() const
{
  return "ITK DistanceMap";
}

// -----------------------------------------------------------------------------
ITKApproximateSignedDistanceMapImage::Pointer ITKApproximateSignedDistanceMapImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKApproximateSignedDistanceMapImage> ITKApproximateSignedDistanceMapImage::New()
{
  struct make_shared_enabler : public ITKApproximateSignedDistanceMapImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKApproximateSignedDistanceMapImage::getNameOfClass() const
{
  return QString("ITKApproximateSignedDistanceMapImage");
}

// -----------------------------------------------------------------------------
QString ITKApproximateSignedDistanceMapImage::ClassName()
{
  return QString("ITKApproximateSignedDistanceMapImage");
}

// -----------------------------------------------------------------------------
void ITKApproximateSignedDistanceMapImage::setInsideValue(double value)
{
  m_InsideValue = value;
}

// -----------------------------------------------------------------------------
double ITKApproximateSignedDistanceMapImage::getInsideValue() const
{
  return m_InsideValue;
}

// -----------------------------------------------------------------------------
void ITKApproximateSignedDistanceMapImage::setOutsideValue(double value)
{
  m_OutsideValue = value;
}

// -----------------------------------------------------------------------------
double ITKApproximateSignedDistanceMapImage::getOutsideValue() const
{
  return m_OutsideValue;
}
