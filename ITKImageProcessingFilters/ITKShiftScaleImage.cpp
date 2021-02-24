// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKShiftScaleImage.h"
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
ITKShiftScaleImage::ITKShiftScaleImage()
{
  m_Shift = StaticCastScalar<double, double, double>(0);
  m_Scale = StaticCastScalar<double, double, double>(1.0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKShiftScaleImage::~ITKShiftScaleImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKShiftScaleImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Shift", Shift, FilterParameter::Category::Parameter, ITKShiftScaleImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Scale", Scale, FilterParameter::Category::Parameter, ITKShiftScaleImage));

  std::vector<QString> linkedProps;
  linkedProps.push_back("NewCellArrayName");
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKShiftScaleImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKShiftScaleImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKShiftScaleImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setShift(reader->readValue("Shift", getShift()));
  setScale(reader->readValue("Scale", getScale()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKShiftScaleImage::dataCheckImpl()
{
  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKShiftScaleImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKShiftScaleImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::ShiftScaleImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetShift(static_cast<double>(m_Shift));
  filter->SetScale(static_cast<double>(m_Scale));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKShiftScaleImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKShiftScaleImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKShiftScaleImage::Pointer filter = ITKShiftScaleImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKShiftScaleImage::getHumanLabel() const
{
  return "ITK::Shift Scale Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKShiftScaleImage::getUuid() const
{
  return QUuid("{31d325fa-e605-5415-85ab-ab93e8cbf32f}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKShiftScaleImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}

// -----------------------------------------------------------------------------
ITKShiftScaleImage::Pointer ITKShiftScaleImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKShiftScaleImage> ITKShiftScaleImage::New()
{
  struct make_shared_enabler : public ITKShiftScaleImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKShiftScaleImage::getNameOfClass() const
{
  return QString("ITKShiftScaleImage");
}

// -----------------------------------------------------------------------------
QString ITKShiftScaleImage::ClassName()
{
  return QString("ITKShiftScaleImage");
}

// -----------------------------------------------------------------------------
void ITKShiftScaleImage::setShift(double value)
{
  m_Shift = value;
}

// -----------------------------------------------------------------------------
double ITKShiftScaleImage::getShift() const
{
  return m_Shift;
}

// -----------------------------------------------------------------------------
void ITKShiftScaleImage::setScale(double value)
{
  m_Scale = value;
}

// -----------------------------------------------------------------------------
double ITKShiftScaleImage::getScale() const
{
  return m_Scale;
}
