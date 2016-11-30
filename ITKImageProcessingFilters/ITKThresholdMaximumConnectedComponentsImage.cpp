// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKThresholdMaximumConnectedComponentsImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

//Auto includes
#include <itkThresholdMaximumConnectedComponentsImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKThresholdMaximumConnectedComponentsImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdMaximumConnectedComponentsImage::ITKThresholdMaximumConnectedComponentsImage() :
  ITKImageBase(),
m_MinimumObjectSizeInPixels(0u),
m_UpperBoundary(std::numeric_limits<double>::max()),
m_InsideValue(1u),
m_OutsideValue(0u)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdMaximumConnectedComponentsImage::~ITKThresholdMaximumConnectedComponentsImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MinimumObjectSizeInPixels", MinimumObjectSizeInPixels, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("UpperBoundary", UpperBoundary, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("InsideValue", InsideValue, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKThresholdMaximumConnectedComponentsImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKThresholdMaximumConnectedComponentsImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKThresholdMaximumConnectedComponentsImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setMinimumObjectSizeInPixels(reader->readValue("MinimumObjectSizeInPixels", getMinimumObjectSizeInPixels()));
  setUpperBoundary(reader->readValue("UpperBoundary", getUpperBoundary()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKThresholdMaximumConnectedComponentsImage::CheckIntegerEntry(SubsType value, QString name)
{
  if (value < static_cast<SubsType>(std::numeric_limits<VarType>::lowest())
     || value > static_cast<SubsType>(std::numeric_limits<VarType>::max())
     || value != floor(value))
  {
    setErrorCondition(-1);
    QString errorMessage = name + QString(" must be greater or equal than %1 and lesser or equal than %2");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(
        std::numeric_limits<VarType>::lowest()).arg(std::numeric_limits<VarType>::max())
                                                         , getErrorCondition()
                                                         );
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType, unsigned int Dimension>
void ITKThresholdMaximumConnectedComponentsImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<uint32_t,double>(m_MinimumObjectSizeInPixels, "MinimumObjectSizeInPixels");
  CheckIntegerEntry<uint8_t,int>(m_InsideValue, "InsideValue");
  CheckIntegerEntry<uint8_t,int>(m_OutsideValue, "OutsideValue");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKThresholdMaximumConnectedComponentsImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::ThresholdMaximumConnectedComponentsImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetMinimumObjectSizeInPixels(static_cast<uint32_t>(m_MinimumObjectSizeInPixels));
  filter->SetUpperBoundary(static_cast<double>(m_UpperBoundary));
  filter->SetInsideValue(static_cast<uint8_t>(m_InsideValue));
  filter->SetOutsideValue(static_cast<uint8_t>(m_OutsideValue));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdMaximumConnectedComponentsImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKThresholdMaximumConnectedComponentsImage::newFilterInstance(bool copyFilterParameters)
{
  ITKThresholdMaximumConnectedComponentsImage::Pointer filter = ITKThresholdMaximumConnectedComponentsImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKThresholdMaximumConnectedComponentsImage::getHumanLabel()
{ return "[ITK] ITK  Threshold Maximum Connected Components Image Filter (KW)"; }

