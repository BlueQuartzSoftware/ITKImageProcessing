// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKBinaryDilateImage.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

#include <itkFlatStructuringElement.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryDilateImage::ITKBinaryDilateImage()
{
  m_BackgroundValue = StaticCastScalar<double, double, double>(0.0);
  m_ForegroundValue = StaticCastScalar<double, double, double>(1.0);
  m_BoundaryToForeground = StaticCastScalar<bool, bool, bool>(false);
  m_KernelRadius = CastStdToVec3<std::vector<unsigned int>, FloatVec3Type, float>(std::vector<unsigned int>(3, 1));
  m_KernelType = StaticCastScalar<int, int, int>(itk::simple::sitkBall);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryDilateImage::~ITKBinaryDilateImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryDilateImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  // Structuring element
  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Kernel Type");
    parameter->setPropertyName("KernelType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKBinaryDilateImage, this, KernelType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKBinaryDilateImage, this, KernelType));

    QVector<QString> choices;
    choices.push_back("Annulus");
    choices.push_back("Ball");
    choices.push_back("Box");
    choices.push_back("Cross");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Parameter);
    parameters.push_back(parameter);
  }
  // Other parameters
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("BackgroundValue", BackgroundValue, FilterParameter::Parameter, ITKBinaryDilateImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ForegroundValue", ForegroundValue, FilterParameter::Parameter, ITKBinaryDilateImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("BoundaryToForeground", BoundaryToForeground, FilterParameter::Parameter, ITKBinaryDilateImage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("KernelRadius", KernelRadius, FilterParameter::Parameter, ITKBinaryDilateImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKBinaryDilateImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKBinaryDilateImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKBinaryDilateImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryDilateImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setBackgroundValue(reader->readValue("BackgroundValue", getBackgroundValue()));
  setForegroundValue(reader->readValue("ForegroundValue", getForegroundValue()));
  setBoundaryToForeground(reader->readValue("BoundaryToForeground", getBoundaryToForeground()));
  setKernelRadius(reader->readFloatVec3("KernelRadius", getKernelRadius()));
  setKernelType(reader->readValue("KernelType", getKernelType()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKBinaryDilateImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

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
void ITKBinaryDilateImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKBinaryDilateImage::filter()
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
  typedef itk::BinaryDilateImageFilter<InputImageType, OutputImageType, StructuringElementType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetBackgroundValue(static_cast<double>(m_BackgroundValue));
  filter->SetForegroundValue(static_cast<double>(m_ForegroundValue));
  filter->SetBoundaryToForeground(static_cast<bool>(m_BoundaryToForeground));
  filter->SetKernel(structuringElement);
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryDilateImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKBinaryDilateImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKBinaryDilateImage::Pointer filter = ITKBinaryDilateImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBinaryDilateImage::getHumanLabel() const
{
  return "ITK::Binary Dilate Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKBinaryDilateImage::getUuid() const
{
  return QUuid("{f86167ad-a1a1-557b-97ea-92a3618baa8f}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBinaryDilateImage::getSubGroupName() const
{
  return "ITK BinaryMathematicalMorphology";
}

// -----------------------------------------------------------------------------
ITKBinaryDilateImage::Pointer ITKBinaryDilateImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKBinaryDilateImage> ITKBinaryDilateImage::New()
{
  struct make_shared_enabler : public ITKBinaryDilateImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKBinaryDilateImage::getNameOfClass() const
{
  return QString("ITKBinaryDilateImage");
}

// -----------------------------------------------------------------------------
QString ITKBinaryDilateImage::ClassName()
{
  return QString("ITKBinaryDilateImage");
}

// -----------------------------------------------------------------------------
void ITKBinaryDilateImage::setBackgroundValue(double value)
{
  m_BackgroundValue = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryDilateImage::getBackgroundValue() const
{
  return m_BackgroundValue;
}

// -----------------------------------------------------------------------------
void ITKBinaryDilateImage::setForegroundValue(double value)
{
  m_ForegroundValue = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryDilateImage::getForegroundValue() const
{
  return m_ForegroundValue;
}

// -----------------------------------------------------------------------------
void ITKBinaryDilateImage::setBoundaryToForeground(bool value)
{
  m_BoundaryToForeground = value;
}

// -----------------------------------------------------------------------------
bool ITKBinaryDilateImage::getBoundaryToForeground() const
{
  return m_BoundaryToForeground;
}

// -----------------------------------------------------------------------------
void ITKBinaryDilateImage::setKernelRadius(const FloatVec3Type& value)
{
  m_KernelRadius = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKBinaryDilateImage::getKernelRadius() const
{
  return m_KernelRadius;
}

// -----------------------------------------------------------------------------
void ITKBinaryDilateImage::setKernelType(int value)
{
  m_KernelType = value;
}

// -----------------------------------------------------------------------------
int ITKBinaryDilateImage::getKernelType() const
{
  return m_KernelType;
}
