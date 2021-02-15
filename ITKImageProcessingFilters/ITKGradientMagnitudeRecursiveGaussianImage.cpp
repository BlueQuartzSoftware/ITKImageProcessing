// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKGradientMagnitudeRecursiveGaussianImage.h"
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
ITKGradientMagnitudeRecursiveGaussianImage::ITKGradientMagnitudeRecursiveGaussianImage()
{
  m_Sigma = StaticCastScalar<double, double, double>(1.0);
  m_NormalizeAcrossScale = StaticCastScalar<bool, bool, bool>(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGradientMagnitudeRecursiveGaussianImage::~ITKGradientMagnitudeRecursiveGaussianImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Sigma", Sigma, FilterParameter::Category::Parameter, ITKGradientMagnitudeRecursiveGaussianImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("NormalizeAcrossScale", NormalizeAcrossScale, FilterParameter::Category::Parameter, ITKGradientMagnitudeRecursiveGaussianImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKGradientMagnitudeRecursiveGaussianImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKGradientMagnitudeRecursiveGaussianImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSigma(reader->readValue("Sigma", getSigma()));
  setNormalizeAcrossScale(reader->readValue("NormalizeAcrossScale", getNormalizeAcrossScale()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKGradientMagnitudeRecursiveGaussianImage::dataCheckImpl()
{
  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKGradientMagnitudeRecursiveGaussianImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSigma(static_cast<double>(m_Sigma));
  filter->SetNormalizeAcrossScale(static_cast<bool>(m_NormalizeAcrossScale));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKGradientMagnitudeRecursiveGaussianImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKGradientMagnitudeRecursiveGaussianImage::Pointer filter = ITKGradientMagnitudeRecursiveGaussianImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKGradientMagnitudeRecursiveGaussianImage::getHumanLabel() const
{
  return "ITK::Gradient Magnitude Recursive Gaussian Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKGradientMagnitudeRecursiveGaussianImage::getUuid() const
{
  return QUuid("{fd688b32-d90e-5945-905b-2b7187b46265}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKGradientMagnitudeRecursiveGaussianImage::getSubGroupName() const
{
  return "ITK ImageGradient";
}

// -----------------------------------------------------------------------------
ITKGradientMagnitudeRecursiveGaussianImage::Pointer ITKGradientMagnitudeRecursiveGaussianImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKGradientMagnitudeRecursiveGaussianImage> ITKGradientMagnitudeRecursiveGaussianImage::New()
{
  struct make_shared_enabler : public ITKGradientMagnitudeRecursiveGaussianImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKGradientMagnitudeRecursiveGaussianImage::getNameOfClass() const
{
  return QString("ITKGradientMagnitudeRecursiveGaussianImage");
}

// -----------------------------------------------------------------------------
QString ITKGradientMagnitudeRecursiveGaussianImage::ClassName()
{
  return QString("ITKGradientMagnitudeRecursiveGaussianImage");
}

// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::setSigma(double value)
{
  m_Sigma = value;
}

// -----------------------------------------------------------------------------
double ITKGradientMagnitudeRecursiveGaussianImage::getSigma() const
{
  return m_Sigma;
}

// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::setNormalizeAcrossScale(bool value)
{
  m_NormalizeAcrossScale = value;
}

// -----------------------------------------------------------------------------
bool ITKGradientMagnitudeRecursiveGaussianImage::getNormalizeAcrossScale() const
{
  return m_NormalizeAcrossScale;
}
