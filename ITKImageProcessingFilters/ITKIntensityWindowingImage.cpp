// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKIntensityWindowingImage.h"

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

// Include the MOC generated file for this class
#include "moc_ITKIntensityWindowingImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIntensityWindowingImage::ITKIntensityWindowingImage() :
  ITKImageBase()
{
  m_WindowMinimum=StaticCastScalar<double,double,double>(0.0);
  m_WindowMaximum=StaticCastScalar<double,double,double>(255.0);
  m_OutputMinimum=StaticCastScalar<double,double,double>(0.0);
  m_OutputMaximum=StaticCastScalar<double,double,double>(255.0);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIntensityWindowingImage::~ITKIntensityWindowingImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("WindowMinimum", WindowMinimum, FilterParameter::Parameter, ITKIntensityWindowingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("WindowMaximum", WindowMaximum, FilterParameter::Parameter, ITKIntensityWindowingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMinimum", OutputMinimum, FilterParameter::Parameter, ITKIntensityWindowingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMaximum", OutputMaximum, FilterParameter::Parameter, ITKIntensityWindowingImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKIntensityWindowingImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKIntensityWindowingImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKIntensityWindowingImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setWindowMinimum(reader->readValue("WindowMinimum", getWindowMinimum()));
  setWindowMaximum(reader->readValue("WindowMaximum", getWindowMaximum()));
  setOutputMinimum(reader->readValue("OutputMinimum", getOutputMinimum()));
  setOutputMaximum(reader->readValue("OutputMaximum", getOutputMaximum()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKIntensityWindowingImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKIntensityWindowingImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef itk::IntensityWindowingImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetWindowMinimum(static_cast<double>(m_WindowMinimum));
  filter->SetWindowMaximum(static_cast<double>(m_WindowMaximum));
  filter->SetOutputMinimum(static_cast<double>(m_OutputMinimum));
  filter->SetOutputMaximum(static_cast<double>(m_OutputMaximum));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::filterInternal()
{
    Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKIntensityWindowingImage::newFilterInstance(bool copyFilterParameters)
{
  ITKIntensityWindowingImage::Pointer filter = ITKIntensityWindowingImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKIntensityWindowingImage::getHumanLabel()
{ return "[ITK] Intensity Windowing Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKIntensityWindowingImage::getSubGroupName()
{ return "ITKImageIntensity"; }


