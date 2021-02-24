// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKBinaryOpeningByReconstructionImage.h"
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
ITKBinaryOpeningByReconstructionImage::ITKBinaryOpeningByReconstructionImage()
{
  m_ForegroundValue = StaticCastScalar<double, double, double>(1.0);
  m_BackgroundValue = StaticCastScalar<double, double, double>(0.0);
  m_FullyConnected = StaticCastScalar<bool, bool, bool>(false);
  m_KernelRadius = CastStdToVec3<std::vector<unsigned int>, FloatVec3Type, float>(std::vector<unsigned int>(3, 1));
  m_KernelType = StaticCastScalar<int, int, int>(itk::simple::sitkBall);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryOpeningByReconstructionImage::~ITKBinaryOpeningByReconstructionImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  // Structuring element
  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Kernel Type");
    parameter->setPropertyName("KernelType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKBinaryOpeningByReconstructionImage, this, KernelType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKBinaryOpeningByReconstructionImage, this, KernelType));

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
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ForegroundValue", ForegroundValue, FilterParameter::Category::Parameter, ITKBinaryOpeningByReconstructionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("BackgroundValue", BackgroundValue, FilterParameter::Category::Parameter, ITKBinaryOpeningByReconstructionImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Category::Parameter, ITKBinaryOpeningByReconstructionImage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("KernelRadius", KernelRadius, FilterParameter::Category::Parameter, ITKBinaryOpeningByReconstructionImage));

  std::vector<QString> linkedProps;
  linkedProps.push_back("NewCellArrayName");
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKBinaryOpeningByReconstructionImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKBinaryOpeningByReconstructionImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setForegroundValue(reader->readValue("ForegroundValue", getForegroundValue()));
  setBackgroundValue(reader->readValue("BackgroundValue", getBackgroundValue()));
  setFullyConnected(reader->readValue("FullyConnected", getFullyConnected()));
  setKernelRadius(reader->readFloatVec3("KernelRadius", getKernelRadius()));
  setKernelType(reader->readValue("KernelType", getKernelType()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKBinaryOpeningByReconstructionImage::dataCheckImpl()
{
  // Check consistency of parameters
  this->CheckVectorEntry<unsigned int, FloatVec3Type>(m_KernelRadius, "KernelRadius", true);
  QVector<QString> supportedTypes;
  // All integer types
  supportedTypes << "uint8_t"
                 << "int8_t"
                 << "uint16_t"
                 << "int16_t"
                 << "uint_32_t"
                 << "int32_t"
                 << "uint64_t"
                 << "int64_t";
  checkImageType(supportedTypes, getSelectedCellArrayPath());

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKBinaryOpeningByReconstructionImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  // typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
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
  typedef itk::BinaryOpeningByReconstructionImageFilter<InputImageType, StructuringElementType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetForegroundValue(static_cast<double>(m_ForegroundValue));
  filter->SetBackgroundValue(static_cast<double>(m_BackgroundValue));
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  filter->SetKernel(structuringElement);
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKBinaryOpeningByReconstructionImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKBinaryOpeningByReconstructionImage::Pointer filter = ITKBinaryOpeningByReconstructionImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBinaryOpeningByReconstructionImage::getHumanLabel() const
{
  return "ITK::Binary Opening By Reconstruction Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKBinaryOpeningByReconstructionImage::getUuid() const
{
  return QUuid("{bd1c2353-0a39-52c0-902b-ee64721994c7}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBinaryOpeningByReconstructionImage::getSubGroupName() const
{
  return "ITK BinaryMathematicalMorphology";
}

// -----------------------------------------------------------------------------
ITKBinaryOpeningByReconstructionImage::Pointer ITKBinaryOpeningByReconstructionImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKBinaryOpeningByReconstructionImage> ITKBinaryOpeningByReconstructionImage::New()
{
  struct make_shared_enabler : public ITKBinaryOpeningByReconstructionImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKBinaryOpeningByReconstructionImage::getNameOfClass() const
{
  return QString("ITKBinaryOpeningByReconstructionImage");
}

// -----------------------------------------------------------------------------
QString ITKBinaryOpeningByReconstructionImage::ClassName()
{
  return QString("ITKBinaryOpeningByReconstructionImage");
}

// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::setForegroundValue(double value)
{
  m_ForegroundValue = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryOpeningByReconstructionImage::getForegroundValue() const
{
  return m_ForegroundValue;
}

// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::setBackgroundValue(double value)
{
  m_BackgroundValue = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryOpeningByReconstructionImage::getBackgroundValue() const
{
  return m_BackgroundValue;
}

// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::setFullyConnected(bool value)
{
  m_FullyConnected = value;
}

// -----------------------------------------------------------------------------
bool ITKBinaryOpeningByReconstructionImage::getFullyConnected() const
{
  return m_FullyConnected;
}

// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::setKernelRadius(const FloatVec3Type& value)
{
  m_KernelRadius = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKBinaryOpeningByReconstructionImage::getKernelRadius() const
{
  return m_KernelRadius;
}

// -----------------------------------------------------------------------------
void ITKBinaryOpeningByReconstructionImage::setKernelType(int value)
{
  m_KernelType = value;
}

// -----------------------------------------------------------------------------
int ITKBinaryOpeningByReconstructionImage::getKernelType() const
{
  return m_KernelType;
}
