/*
 * Your License or Copyright can go here
 */

#include "ITKRGBToLuminanceImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#define DREAM3D_USE_RGB_RGBA 1
#define DREAM3D_USE_Scalar 0
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRGBToLuminanceImage::ITKRGBToLuminanceImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRGBToLuminanceImage::~ITKRGBToLuminanceImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRGBToLuminanceImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKRGBToLuminanceImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKRGBToLuminanceImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRGBToLuminanceImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKRGBToLuminanceImage::dataCheckImpl()
{
  // Check consistency of parameters
  clearErrorCode();
  clearWarningCode();
  const unsigned int length = itk::NumericTraits<InputPixelType>::GetLength();
  if(length < 3 || length > 4)
  {
    setErrorCondition(-5, "Input image pixels should have three (RGB) or four (RGBA) components.");
    return;
  }
  typedef typename itk::NumericTraits<InputPixelType>::ValueType ScalarPixelType;
  ITKImageProcessingBase::dataCheckImpl<InputPixelType, ScalarPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRGBToLuminanceImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKRGBToLuminanceImage::filter()
{
  typedef typename itk::NumericTraits<InputPixelType>::ValueType ScalarPixelType;
  typedef itk::Image<ScalarPixelType, Dimension> OutputImageType;

  typedef itk::RGBPixel<ScalarPixelType> RGBPixelType;
  typedef itk::Image<RGBPixelType, Dimension> RGBImageType;
  typedef itk::RGBToLuminanceImageFilter<RGBImageType, OutputImageType> RGBFilterType;
  typename RGBFilterType::Pointer rgbFilter = RGBFilterType::New();

  typedef itk::RGBAPixel<ScalarPixelType> RGBAPixelType;
  typedef itk::Image<RGBAPixelType, Dimension> RGBAImageType;
  typedef itk::RGBToLuminanceImageFilter<RGBAImageType, OutputImageType> RGBAFilterType;
  typename RGBAFilterType::Pointer rgbaFilter = RGBAFilterType::New();

  const unsigned int length = itk::NumericTraits<InputPixelType>::GetLength();
  switch(length)
  {
  case 3:
    this->ITKImageProcessingBase::filter<RGBPixelType, ScalarPixelType, Dimension, RGBFilterType>(rgbFilter);
    break;
  case 4:
    this->ITKImageProcessingBase::filter<RGBAPixelType, ScalarPixelType, Dimension, RGBAFilterType>(rgbaFilter);
    break;
  default:
    setErrorCondition(-7, "Input image pixels should have three (RGB) or four (RGBA) components.");
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRGBToLuminanceImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKRGBToLuminanceImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKRGBToLuminanceImage::Pointer filter = ITKRGBToLuminanceImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKRGBToLuminanceImage::getHumanLabel() const
{
  return "ITK::RGB to Luminance Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKRGBToLuminanceImage::getUuid() const
{
  return QUuid("{bae507d6-4d0a-5ad2-8279-c674f1c90db8}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKRGBToLuminanceImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}

// -----------------------------------------------------------------------------
ITKRGBToLuminanceImage::Pointer ITKRGBToLuminanceImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKRGBToLuminanceImage> ITKRGBToLuminanceImage::New()
{
  struct make_shared_enabler : public ITKRGBToLuminanceImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKRGBToLuminanceImage::getNameOfClass() const
{
  return QString("ITKRGBToLuminanceImage");
}

// -----------------------------------------------------------------------------
QString ITKRGBToLuminanceImage::ClassName()
{
  return QString("ITKRGBToLuminanceImage");
}
