// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKCosImage.h"

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
#include "moc_ITKCosImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCosImage::ITKCosImage()
: ITKImageBase()
{

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCosImage::~ITKCosImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCosImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKCosImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKCosImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKCosImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCosImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
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
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKCosImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCosImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKCosImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::CosImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCosImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKCosImage::newFilterInstance(bool copyFilterParameters)
{
  ITKCosImage::Pointer filter = ITKCosImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCosImage::getHumanLabel()
{
  return "ITK::Cos Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCosImage::getSubGroupName()
{
  return "ITK IntensityTransformation";
}
