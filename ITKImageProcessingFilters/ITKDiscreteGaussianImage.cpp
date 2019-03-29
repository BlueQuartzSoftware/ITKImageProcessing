// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKDiscreteGaussianImage.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/itkDream3DImage.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDiscreteGaussianImage::ITKDiscreteGaussianImage()
{
  m_Variance = CastStdToVec3<std::vector<double>, FloatVec3_t, float>(std::vector<double>(3,1.0));
  m_MaximumKernelWidth = StaticCastScalar<double, double, double>(32u);
  m_MaximumError = CastStdToVec3<std::vector<double>, FloatVec3_t, float>(std::vector<double>(3, 0.01));
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
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Variance", Variance, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumKernelWidth", MaximumKernelWidth, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("MaximumError", MaximumError, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("UseImageSpacing", UseImageSpacing, FilterParameter::Parameter, ITKDiscreteGaussianImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKDiscreteGaussianImage, linkedProps));
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
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKDiscreteGaussianImage::dataCheck()
{
  clearErrorCondition();
  clearWarningCondition();

  // Check consistency of parameters
  this->CheckVectorEntry<double, FloatVec3_t>(m_Variance, "Variance", false);
  this->CheckIntegerEntry<unsigned int, double>(m_MaximumKernelWidth, "MaximumKernelWidth", true);
  this->CheckVectorEntry<double, FloatVec3_t>(m_MaximumError, "MaximumError", false);

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKDiscreteGaussianImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::DiscreteGaussianImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetVariance(CastVec3ToITK<FloatVec3_t, typename FilterType::ArrayType, typename FilterType::ArrayType::ValueType>(m_Variance, FilterType::ArrayType::Dimension));
  filter->SetMaximumKernelWidth(static_cast<unsigned int>(m_MaximumKernelWidth));
  filter->SetMaximumError(CastVec3ToITK<FloatVec3_t, typename FilterType::ArrayType, typename FilterType::ArrayType::ValueType>(m_MaximumError, FilterType::ArrayType::Dimension));
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
const QString ITKDiscreteGaussianImage::getHumanLabel() const
{
  return "ITK::Discrete Gaussian Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKDiscreteGaussianImage::getUuid()
{
  return QUuid("{53df5340-f632-598f-8a9b-802296b3a95c}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKDiscreteGaussianImage::getSubGroupName() const
{
  return "ITK Smoothing";
}
