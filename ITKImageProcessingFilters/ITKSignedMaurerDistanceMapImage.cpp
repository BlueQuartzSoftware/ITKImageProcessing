// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKSignedMaurerDistanceMapImage.h"
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
ITKSignedMaurerDistanceMapImage::ITKSignedMaurerDistanceMapImage()
{
  m_InsideIsPositive = StaticCastScalar<bool, bool, bool>(false);
  m_SquaredDistance = StaticCastScalar<bool, bool, bool>(true);
  m_UseImageSpacing = StaticCastScalar<bool, bool, bool>(false);
  m_BackgroundValue = StaticCastScalar<double, double, double>(0.0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSignedMaurerDistanceMapImage::~ITKSignedMaurerDistanceMapImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_BOOL_FP("InsideIsPositive", InsideIsPositive, FilterParameter::Category::Parameter, ITKSignedMaurerDistanceMapImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("SquaredDistance", SquaredDistance, FilterParameter::Category::Parameter, ITKSignedMaurerDistanceMapImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("UseImageSpacing", UseImageSpacing, FilterParameter::Category::Parameter, ITKSignedMaurerDistanceMapImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("BackgroundValue", BackgroundValue, FilterParameter::Category::Parameter, ITKSignedMaurerDistanceMapImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKSignedMaurerDistanceMapImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKSignedMaurerDistanceMapImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setInsideIsPositive(reader->readValue("InsideIsPositive", getInsideIsPositive()));
  setSquaredDistance(reader->readValue("SquaredDistance", getSquaredDistance()));
  setUseImageSpacing(reader->readValue("UseImageSpacing", getUseImageSpacing()));
  setBackgroundValue(reader->readValue("BackgroundValue", getBackgroundValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSignedMaurerDistanceMapImage::dataCheckImpl()
{
  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSignedMaurerDistanceMapImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::SignedMaurerDistanceMapImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInsideIsPositive(static_cast<bool>(m_InsideIsPositive));
  filter->SetSquaredDistance(static_cast<bool>(m_SquaredDistance));
  filter->SetUseImageSpacing(static_cast<bool>(m_UseImageSpacing));
  filter->SetBackgroundValue(static_cast<double>(m_BackgroundValue));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKSignedMaurerDistanceMapImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKSignedMaurerDistanceMapImage::Pointer filter = ITKSignedMaurerDistanceMapImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSignedMaurerDistanceMapImage::getHumanLabel() const
{
  return "ITK::Signed Maurer Distance Map Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKSignedMaurerDistanceMapImage::getUuid() const
{
  return QUuid("{bb15d42a-3077-582a-be1a-76b2bae172e9}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSignedMaurerDistanceMapImage::getSubGroupName() const
{
  return "ITK DistanceMap";
}

// -----------------------------------------------------------------------------
ITKSignedMaurerDistanceMapImage::Pointer ITKSignedMaurerDistanceMapImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKSignedMaurerDistanceMapImage> ITKSignedMaurerDistanceMapImage::New()
{
  struct make_shared_enabler : public ITKSignedMaurerDistanceMapImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKSignedMaurerDistanceMapImage::getNameOfClass() const
{
  return QString("ITKSignedMaurerDistanceMapImage");
}

// -----------------------------------------------------------------------------
QString ITKSignedMaurerDistanceMapImage::ClassName()
{
  return QString("ITKSignedMaurerDistanceMapImage");
}

// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::setInsideIsPositive(bool value)
{
  m_InsideIsPositive = value;
}

// -----------------------------------------------------------------------------
bool ITKSignedMaurerDistanceMapImage::getInsideIsPositive() const
{
  return m_InsideIsPositive;
}

// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::setSquaredDistance(bool value)
{
  m_SquaredDistance = value;
}

// -----------------------------------------------------------------------------
bool ITKSignedMaurerDistanceMapImage::getSquaredDistance() const
{
  return m_SquaredDistance;
}

// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::setUseImageSpacing(bool value)
{
  m_UseImageSpacing = value;
}

// -----------------------------------------------------------------------------
bool ITKSignedMaurerDistanceMapImage::getUseImageSpacing() const
{
  return m_UseImageSpacing;
}

// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::setBackgroundValue(double value)
{
  m_BackgroundValue = value;
}

// -----------------------------------------------------------------------------
double ITKSignedMaurerDistanceMapImage::getBackgroundValue() const
{
  return m_BackgroundValue;
}
