// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKErodeObjectMorphologyImage.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

#include <itkFlatStructuringElement.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKErodeObjectMorphologyImage::ITKErodeObjectMorphologyImage()
{
  m_ObjectValue = StaticCastScalar<double, double, double>(1);
  m_BackgroundValue = StaticCastScalar<double, double, double>(0);
  m_KernelRadius = CastStdToVec3<std::vector<unsigned int>, FloatVec3Type, float>(std::vector<unsigned int>(3, 1));
  m_KernelType = StaticCastScalar<int, int, int>(itk::simple::sitkBall);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKErodeObjectMorphologyImage::~ITKErodeObjectMorphologyImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKErodeObjectMorphologyImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  // Structuring element
  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Kernel Type");
    parameter->setPropertyName("KernelType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKErodeObjectMorphologyImage, this, KernelType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKErodeObjectMorphologyImage, this, KernelType));

    std::vector<QString> choices;
    choices.push_back("Annulus");
    choices.push_back("Ball");
    choices.push_back("Box");
    choices.push_back("Cross");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Category::Parameter);
    parameters.push_back(parameter);
  }
  // Other parameters
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ObjectValue", ObjectValue, FilterParameter::Category::Parameter, ITKErodeObjectMorphologyImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("BackgroundValue", BackgroundValue, FilterParameter::Category::Parameter, ITKErodeObjectMorphologyImage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("KernelRadius", KernelRadius, FilterParameter::Category::Parameter, ITKErodeObjectMorphologyImage));

  std::vector<QString> linkedProps;
  linkedProps.push_back("NewCellArrayName");
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKErodeObjectMorphologyImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKErodeObjectMorphologyImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKErodeObjectMorphologyImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setObjectValue(reader->readValue("ObjectValue", getObjectValue()));
  setBackgroundValue(reader->readValue("BackgroundValue", getBackgroundValue()));
  setKernelRadius(reader->readFloatVec3("KernelRadius", getKernelRadius()));
  setKernelType(reader->readValue("KernelType", getKernelType()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKErodeObjectMorphologyImage::dataCheckImpl()
{
  // Check consistency of parameters
  this->CheckVectorEntry<unsigned int, FloatVec3Type>(m_KernelRadius, "KernelRadius", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKErodeObjectMorphologyImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKErodeObjectMorphologyImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  typedef itk::FlatStructuringElement<Dimension> StructuringElementType;
  typedef typename StructuringElementType::RadiusType RadiusType;
  RadiusType elementRadius = CastVec3ToITK<FloatVec3Type, RadiusType, typename RadiusType::SizeValueType>(m_KernelRadius, RadiusType::Dimension);
  StructuringElementType structuringElement;
  switch(getKernelType())
  {
  case 0:
    structuringElement = StructuringElementType::Annulus(elementRadius, false);
    break;
  case 1:
    structuringElement = StructuringElementType::Ball(elementRadius, false);
    break;
  case 2:
    structuringElement = StructuringElementType::Box(elementRadius);
    break;
  case 3:
    structuringElement = StructuringElementType::Cross(elementRadius);
    break;
  default:
    setErrorCondition(-20, "Unsupported structuring element");
    return;
  }
  // define filter
  typedef itk::ErodeObjectMorphologyImageFilter<InputImageType, OutputImageType, StructuringElementType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetObjectValue(static_cast<typename FilterType::PixelType>(this->getObjectValue()));
  filter->SetBackgroundValue(static_cast<double>(m_BackgroundValue));
  filter->SetKernel(structuringElement);
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKErodeObjectMorphologyImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKErodeObjectMorphologyImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKErodeObjectMorphologyImage::Pointer filter = ITKErodeObjectMorphologyImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKErodeObjectMorphologyImage::getHumanLabel() const
{
  return "ITK::Erode Object Morphology Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKErodeObjectMorphologyImage::getUuid() const
{
  return QUuid("{caea0698-4253-518b-ab3f-8ebc140d92ea}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKErodeObjectMorphologyImage::getSubGroupName() const
{
  return "ITK BinaryMathematicalMorphology";
}

// -----------------------------------------------------------------------------
ITKErodeObjectMorphologyImage::Pointer ITKErodeObjectMorphologyImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKErodeObjectMorphologyImage> ITKErodeObjectMorphologyImage::New()
{
  struct make_shared_enabler : public ITKErodeObjectMorphologyImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKErodeObjectMorphologyImage::getNameOfClass() const
{
  return QString("ITKErodeObjectMorphologyImage");
}

// -----------------------------------------------------------------------------
QString ITKErodeObjectMorphologyImage::ClassName()
{
  return QString("ITKErodeObjectMorphologyImage");
}

// -----------------------------------------------------------------------------
void ITKErodeObjectMorphologyImage::setObjectValue(double value)
{
  m_ObjectValue = value;
}

// -----------------------------------------------------------------------------
double ITKErodeObjectMorphologyImage::getObjectValue() const
{
  return m_ObjectValue;
}

// -----------------------------------------------------------------------------
void ITKErodeObjectMorphologyImage::setBackgroundValue(double value)
{
  m_BackgroundValue = value;
}

// -----------------------------------------------------------------------------
double ITKErodeObjectMorphologyImage::getBackgroundValue() const
{
  return m_BackgroundValue;
}

// -----------------------------------------------------------------------------
void ITKErodeObjectMorphologyImage::setKernelRadius(const FloatVec3Type& value)
{
  m_KernelRadius = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKErodeObjectMorphologyImage::getKernelRadius() const
{
  return m_KernelRadius;
}

// -----------------------------------------------------------------------------
void ITKErodeObjectMorphologyImage::setKernelType(int value)
{
  m_KernelType = value;
}

// -----------------------------------------------------------------------------
int ITKErodeObjectMorphologyImage::getKernelType() const
{
  return m_KernelType;
}
