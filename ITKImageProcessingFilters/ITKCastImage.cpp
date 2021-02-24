/*
 * Your License or Copyright can go here
 */
#include "ITKCastImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCastImage::ITKCastImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCastImage::~ITKCastImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCastImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Casting Type");
    parameter->setPropertyName("CastingType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKCastImage, this, CastingType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKCastImage, this, CastingType));

    std::vector<QString> choices;
    choices.push_back("unsigned char");
    choices.push_back("char");
    choices.push_back("unsigned short");
    choices.push_back("short");
    choices.push_back("unsigned int");
    choices.push_back("int");
    choices.push_back("unsigned long");
    choices.push_back("long");
    choices.push_back("long long");
    choices.push_back("unsigned long long");
    choices.push_back("float");
    choices.push_back("double");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Category::Parameter);
    parameters.push_back(parameter);
  }

  std::vector<QString> linkedProps;
  linkedProps.push_back("NewCellArrayName");
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKCastImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKCastImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCastImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setCastingType(reader->readValue("CastingType", getCastingType()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKCastImage::dataCheckImpl()
{
  // Check consistency of parameters
  clearErrorCode();
  clearWarningCode();
  typedef typename itk::NumericTraits<InputPixelType>::ValueType InputValueType;
  typedef typename itk::NumericTraits<OutputPixelType>::ValueType OutputValueType;
  if(static_cast<double>(std::numeric_limits<InputValueType>::max()) > static_cast<double>(std::numeric_limits<OutputValueType>::max()) ||
     static_cast<double>(std::numeric_limits<InputValueType>::lowest()) < static_cast<double>(std::numeric_limits<OutputValueType>::lowest()))
  {
    setErrorCondition(-5, "Boundaries values of output component type inside boundaries of input component type. Use ITK::Rescale Intensity Image Filter instead.");
    return;
  }
  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCastImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  using CastingType = itk::ITK_IOCOMPONENT_CLASS::ITK_IOCOMPONENT_TYPE;
  CastingType castingType = static_cast<CastingType>(m_CastingType + 1);
  Dream3DArraySwitchOutputComponentMacro(this->dataCheckImpl, castingType, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKCastImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::CastImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCastImage::filterInternal()
{
  using CastingType = itk::ITK_IOCOMPONENT_CLASS::ITK_IOCOMPONENT_TYPE;
  CastingType castingType = static_cast<CastingType>(m_CastingType + 1);
  Dream3DArraySwitchOutputComponentMacro(this->filter, castingType, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKCastImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKCastImage::Pointer filter = ITKCastImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKCastImage::getHumanLabel() const
{
  return "ITK::Cast Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKCastImage::getUuid() const
{
  return QUuid("{3ad55a9b-98ec-549a-a6a8-c8967366260b}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKCastImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}

// -----------------------------------------------------------------------------
ITKCastImage::Pointer ITKCastImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKCastImage> ITKCastImage::New()
{
  struct make_shared_enabler : public ITKCastImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKCastImage::getNameOfClass() const
{
  return QString("ITKCastImage");
}

// -----------------------------------------------------------------------------
QString ITKCastImage::ClassName()
{
  return QString("ITKCastImage");
}

// -----------------------------------------------------------------------------
void ITKCastImage::setCastingType(int value)
{
  m_CastingType = value;
}

// -----------------------------------------------------------------------------
int ITKCastImage::getCastingType() const
{
  return m_CastingType;
}
