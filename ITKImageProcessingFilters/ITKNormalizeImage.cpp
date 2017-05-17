// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKNormalizeImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"

// Include the MOC generated file for this class
#include "moc_ITKNormalizeImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKNormalizeImage::ITKNormalizeImage()
: ITKImageBase()
{

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKNormalizeImage::~ITKNormalizeImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKNormalizeImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKNormalizeImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKNormalizeImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKNormalizeImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKNormalizeImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
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
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKNormalizeImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKNormalizeImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4, typename itk::NumericTraits<typename InputImageType::PixelType>::RealType, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKNormalizeImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::NormalizeImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKNormalizeImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, typename itk::NumericTraits<typename InputImageType::PixelType>::RealType, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKNormalizeImage::newFilterInstance(bool copyFilterParameters)
{
  ITKNormalizeImage::Pointer filter = ITKNormalizeImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKNormalizeImage::getHumanLabel()
{
  return "ITK::Normalize Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKNormalizeImage::getSubGroupName()
{
  return "ITK IntensityTransformation";
}
