// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKDiscreteGaussianImage.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDiscreteGaussianImage::ITKDiscreteGaussianImage()
{
  m_Variance = CastStdToVec3<std::vector<double>, FloatVec3Type, float>(std::vector<double>(3, 1.0));
  m_MaximumKernelWidth = StaticCastScalar<double, double, double>(32u);
  m_MaximumError = CastStdToVec3<std::vector<double>, FloatVec3Type, float>(std::vector<double>(3, 0.01));
  m_UseImageSpacing = StaticCastScalar<bool, bool, bool>(true);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDiscreteGaussianImage::~ITKDiscreteGaussianImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Variance", Variance, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("MaximumKernelWidth", MaximumKernelWidth, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("MaximumError", MaximumError, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("UseImageSpacing", UseImageSpacing, FilterParameter::Parameter, ITKDiscreteGaussianImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKDiscreteGaussianImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKDiscreteGaussianImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setVariance(reader->readFloatVec3("Variance", getVariance()));
  setMaximumKernelWidth(reader->readValue("MaximumKernelWidth", getMaximumKernelWidth()));
  setMaximumError(reader->readFloatVec3("MaximumError", getMaximumError()));
  setUseImageSpacing(reader->readValue("UseImageSpacing", getUseImageSpacing()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKDiscreteGaussianImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckVectorEntry<float, FloatVec3Type>(m_Variance, "Variance", false);
  this->CheckIntegerEntry<uint32_t, int32_t>(m_MaximumKernelWidth, "MaximumKernelWidth", true);
  this->CheckVectorEntry<float, FloatVec3Type>(m_MaximumError, "MaximumError", false);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKDiscreteGaussianImage::filter()
{
  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;
  // define filter
  using FilterType = itk::DiscreteGaussianImageFilter<InputImageType, OutputImageType>;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetVariance(CastVec3ToITK<FloatVec3Type, typename FilterType::ArrayType, typename FilterType::ArrayType::ValueType>(m_Variance, FilterType::ArrayType::Dimension));
  filter->SetMaximumKernelWidth(static_cast<unsigned int>(m_MaximumKernelWidth));
  filter->SetMaximumError(CastVec3ToITK<FloatVec3Type, typename FilterType::ArrayType, typename FilterType::ArrayType::ValueType>(m_MaximumError, FilterType::ArrayType::Dimension));
  filter->SetUseImageSpacing(static_cast<bool>(m_UseImageSpacing));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKDiscreteGaussianImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKDiscreteGaussianImage::Pointer filter = ITKDiscreteGaussianImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKDiscreteGaussianImage::getHumanLabel() const
{
  return "ITK::Discrete Gaussian Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKDiscreteGaussianImage::getUuid() const
{
  return QUuid("{53df5340-f632-598f-8a9b-802296b3a95c}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKDiscreteGaussianImage::getSubGroupName() const
{
  return "ITK Smoothing";
}

// -----------------------------------------------------------------------------
ITKDiscreteGaussianImage::Pointer ITKDiscreteGaussianImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKDiscreteGaussianImage> ITKDiscreteGaussianImage::New()
{
  struct make_shared_enabler : public ITKDiscreteGaussianImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKDiscreteGaussianImage::getNameOfClass() const
{
  return QString("ITKDiscreteGaussianImage");
}

// -----------------------------------------------------------------------------
QString ITKDiscreteGaussianImage::ClassName()
{
  return QString("ITKDiscreteGaussianImage");
}

// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::setVariance(const FloatVec3Type& value)
{
  m_Variance = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKDiscreteGaussianImage::getVariance() const
{
  return m_Variance;
}

// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::setMaximumKernelWidth(int32_t value)
{
  m_MaximumKernelWidth = value;
}

// -----------------------------------------------------------------------------
int32_t ITKDiscreteGaussianImage::getMaximumKernelWidth() const
{
  return m_MaximumKernelWidth;
}

// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::setMaximumError(const FloatVec3Type& value)
{
  m_MaximumError = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKDiscreteGaussianImage::getMaximumError() const
{
  return m_MaximumError;
}

// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::setUseImageSpacing(bool value)
{
  m_UseImageSpacing = value;
}

// -----------------------------------------------------------------------------
bool ITKDiscreteGaussianImage::getUseImageSpacing() const
{
  return m_UseImageSpacing;
}
