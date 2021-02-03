// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKSmoothingRecursiveGaussianImage.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#define DREAM3D_USE_RGB_RGBA 1
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSmoothingRecursiveGaussianImage::ITKSmoothingRecursiveGaussianImage()
{
  m_Sigma = CastStdToVec3<std::vector<double>, FloatVec3Type, float>(std::vector<double>(3, 1.0));
  m_NormalizeAcrossScale = StaticCastScalar<bool, bool, bool>(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSmoothingRecursiveGaussianImage::~ITKSmoothingRecursiveGaussianImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSmoothingRecursiveGaussianImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Sigma", Sigma, FilterParameter::Parameter, ITKSmoothingRecursiveGaussianImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("NormalizeAcrossScale", NormalizeAcrossScale, FilterParameter::Parameter, ITKSmoothingRecursiveGaussianImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKSmoothingRecursiveGaussianImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKSmoothingRecursiveGaussianImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSmoothingRecursiveGaussianImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSigma(reader->readFloatVec3("Sigma", getSigma()));
  setNormalizeAcrossScale(reader->readValue("NormalizeAcrossScale", getNormalizeAcrossScale()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSmoothingRecursiveGaussianImage::dataCheckImpl()
{
  // Check consistency of parameters
  this->CheckVectorEntry<double, FloatVec3Type>(m_Sigma, "Sigma", false);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSmoothingRecursiveGaussianImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, typename InputImageType::template Rebind<float>::Type::PixelType, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSmoothingRecursiveGaussianImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::SmoothingRecursiveGaussianImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  typename FilterType::SigmaArrayType itkVecSigma =
      CastVec3ToITK<FloatVec3Type, typename FilterType::SigmaArrayType, typename FilterType::SigmaArrayType::ValueType>(this->getSigma(), FilterType::SigmaArrayType::Dimension);
  filter->SetSigmaArray(itkVecSigma);
  filter->SetNormalizeAcrossScale(static_cast<bool>(m_NormalizeAcrossScale));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSmoothingRecursiveGaussianImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, typename InputImageType::template Rebind<float>::Type::PixelType, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKSmoothingRecursiveGaussianImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKSmoothingRecursiveGaussianImage::Pointer filter = ITKSmoothingRecursiveGaussianImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSmoothingRecursiveGaussianImage::getHumanLabel() const
{
  return "ITK::Smoothing Recursive Gaussian Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKSmoothingRecursiveGaussianImage::getUuid() const
{
  return QUuid("{0fd06492-06b1-5044-964c-e0555c556327}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSmoothingRecursiveGaussianImage::getSubGroupName() const
{
  return "ITK Smoothing";
}

// -----------------------------------------------------------------------------
ITKSmoothingRecursiveGaussianImage::Pointer ITKSmoothingRecursiveGaussianImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKSmoothingRecursiveGaussianImage> ITKSmoothingRecursiveGaussianImage::New()
{
  struct make_shared_enabler : public ITKSmoothingRecursiveGaussianImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKSmoothingRecursiveGaussianImage::getNameOfClass() const
{
  return QString("ITKSmoothingRecursiveGaussianImage");
}

// -----------------------------------------------------------------------------
QString ITKSmoothingRecursiveGaussianImage::ClassName()
{
  return QString("ITKSmoothingRecursiveGaussianImage");
}

// -----------------------------------------------------------------------------
void ITKSmoothingRecursiveGaussianImage::setSigma(const FloatVec3Type& value)
{
  m_Sigma = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKSmoothingRecursiveGaussianImage::getSigma() const
{
  return m_Sigma;
}

// -----------------------------------------------------------------------------
void ITKSmoothingRecursiveGaussianImage::setNormalizeAcrossScale(bool value)
{
  m_NormalizeAcrossScale = value;
}

// -----------------------------------------------------------------------------
bool ITKSmoothingRecursiveGaussianImage::getNormalizeAcrossScale() const
{
  return m_NormalizeAcrossScale;
}
