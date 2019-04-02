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
#include "SIMPLib/ITK/itkDream3DImage.h"

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
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKRGBToLuminanceImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKRGBToLuminanceImage));

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
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKRGBToLuminanceImage::dataCheck()
{
  // Check consistency of parameters
  clearErrorCondition();
  clearWarningCondition();
  const unsigned int length = itk::NumericTraits<InputPixelType>::GetLength();
  if(length < 3 || length > 4)
  {
    setErrorCondition(-5, "Input image pixels should have three (RGB) or four (RGBA) components.");
    return;
  }
  typedef typename itk::NumericTraits<InputPixelType>::ValueType ScalarPixelType;
  ITKImageProcessingBase::dataCheck<InputPixelType, ScalarPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRGBToLuminanceImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKRGBToLuminanceImage::filter()
{
  typedef typename itk::NumericTraits<InputPixelType>::ValueType ScalarPixelType;
  typedef itk::Dream3DImage<ScalarPixelType, Dimension> OutputImageType;

  typedef itk::RGBPixel<ScalarPixelType> RGBPixelType;
  typedef itk::Dream3DImage<RGBPixelType, Dimension> RGBImageType;
  typedef itk::RGBToLuminanceImageFilter<RGBImageType, OutputImageType> RGBFilterType;
  typename RGBFilterType::Pointer rgbFilter = RGBFilterType::New();

  typedef itk::RGBAPixel<ScalarPixelType> RGBAPixelType;
  typedef itk::Dream3DImage<RGBAPixelType, Dimension> RGBAImageType;
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
const QString ITKRGBToLuminanceImage::getHumanLabel() const
{
  return "ITK::RGB to Luminance Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKRGBToLuminanceImage::getUuid()
{
  return QUuid("{bae507d6-4d0a-5ad2-8279-c674f1c90db8}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRGBToLuminanceImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}
