// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKAdaptiveHistogramEqualizationImage.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKAdaptiveHistogramEqualizationImage::ITKAdaptiveHistogramEqualizationImage()
{
  m_Radius = CastStdToVec3<std::vector<unsigned int>, FloatVec3Type, float>(std::vector<unsigned int>(3, 5));
  m_Alpha = StaticCastScalar<float, float, float>(0.3f);
  m_Beta = StaticCastScalar<float, float, float>(0.3f);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKAdaptiveHistogramEqualizationImage::~ITKAdaptiveHistogramEqualizationImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Radius", Radius, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Alpha", Alpha, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Beta", Beta, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKAdaptiveHistogramEqualizationImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKAdaptiveHistogramEqualizationImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setRadius(reader->readFloatVec3("Radius", getRadius()));
  setAlpha(reader->readValue("Alpha", getAlpha()));
  setBeta(reader->readValue("Beta", getBeta()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKAdaptiveHistogramEqualizationImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckVectorEntry<unsigned int, FloatVec3Type>(m_Radius, "Radius", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKAdaptiveHistogramEqualizationImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  // typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::AdaptiveHistogramEqualizationImageFilter<InputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetRadius(CastVec3ToITK<FloatVec3Type, typename FilterType::RadiusType, typename FilterType::RadiusType::SizeValueType>(m_Radius, FilterType::RadiusType::Dimension));
  filter->SetAlpha(static_cast<float>(m_Alpha));
  filter->SetBeta(static_cast<float>(m_Beta));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKAdaptiveHistogramEqualizationImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKAdaptiveHistogramEqualizationImage::Pointer filter = ITKAdaptiveHistogramEqualizationImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKAdaptiveHistogramEqualizationImage::getHumanLabel() const
{
  return "ITK::Adaptive Histogram Equalization Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKAdaptiveHistogramEqualizationImage::getUuid() const
{
  return QUuid("{2d5a7599-5e01-5489-a107-23b704d2b5eb}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKAdaptiveHistogramEqualizationImage::getSubGroupName() const
{
  return "ITK ImageStatistics";
}

// -----------------------------------------------------------------------------
ITKAdaptiveHistogramEqualizationImage::Pointer ITKAdaptiveHistogramEqualizationImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKAdaptiveHistogramEqualizationImage> ITKAdaptiveHistogramEqualizationImage::New()
{
  struct make_shared_enabler : public ITKAdaptiveHistogramEqualizationImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKAdaptiveHistogramEqualizationImage::getNameOfClass() const
{
  return QString("ITKAdaptiveHistogramEqualizationImage");
}

// -----------------------------------------------------------------------------
QString ITKAdaptiveHistogramEqualizationImage::ClassName()
{
  return QString("ITKAdaptiveHistogramEqualizationImage");
}

// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::setRadius(const FloatVec3Type& value)
{
  m_Radius = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKAdaptiveHistogramEqualizationImage::getRadius() const
{
  return m_Radius;
}

// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::setAlpha(float value)
{
  m_Alpha = value;
}

// -----------------------------------------------------------------------------
float ITKAdaptiveHistogramEqualizationImage::getAlpha() const
{
  return m_Alpha;
}

// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::setBeta(float value)
{
  m_Beta = value;
}

// -----------------------------------------------------------------------------
float ITKAdaptiveHistogramEqualizationImage::getBeta() const
{
  return m_Beta;
}
