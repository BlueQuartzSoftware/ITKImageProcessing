// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKSimpleContourExtractorImage.h"

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
#include <itkSimpleContourExtractorImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKSimpleContourExtractorImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSimpleContourExtractorImage::ITKSimpleContourExtractorImage() :
  ITKImageBase(),
m_InputForegroundValue(1.0),
m_InputBackgroundValue(0.0),
m_Radius(std::vector<unsigned int>(3, 1)),
m_OutputForegroundValue(1.0),
m_OutputBackgroundValue(0.0)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSimpleContourExtractorImage::~ITKSimpleContourExtractorImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSimpleContourExtractorImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("InputForegroundValue", InputForegroundValue, FilterParameter::Parameter, ITKSimpleContourExtractorImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("InputBackgroundValue", InputBackgroundValue, FilterParameter::Parameter, ITKSimpleContourExtractorImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Radius", Radius, FilterParameter::Parameter, ITKSimpleContourExtractorImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputForegroundValue", OutputForegroundValue, FilterParameter::Parameter, ITKSimpleContourExtractorImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputBackgroundValue", OutputBackgroundValue, FilterParameter::Parameter, ITKSimpleContourExtractorImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKSimpleContourExtractorImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKSimpleContourExtractorImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKSimpleContourExtractorImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSimpleContourExtractorImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setInputForegroundValue(reader->readValue("InputForegroundValue", getInputForegroundValue()));
  setInputBackgroundValue(reader->readValue("InputBackgroundValue", getInputBackgroundValue()));
  setRadius(reader->readValue("Radius", getRadius()));
  setOutputForegroundValue(reader->readValue("OutputForegroundValue", getOutputForegroundValue()));
  setOutputBackgroundValue(reader->readValue("OutputBackgroundValue", getOutputBackgroundValue()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKSimpleContourExtractorImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKSimpleContourExtractorImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<unsigned int,double>(m_Radius, "Radius");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSimpleContourExtractorImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKSimpleContourExtractorImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::SimpleContourExtractorImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInputForegroundValue(static_cast<double>(m_InputForegroundValue));
  filter->SetInputBackgroundValue(static_cast<double>(m_InputBackgroundValue));
  filter->SetRadius(static_cast<unsigned int>(m_Radius));
  filter->SetOutputForegroundValue(static_cast<double>(m_OutputForegroundValue));
  filter->SetOutputBackgroundValue(static_cast<double>(m_OutputBackgroundValue));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSimpleContourExtractorImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKSimpleContourExtractorImage::newFilterInstance(bool copyFilterParameters)
{
  ITKSimpleContourExtractorImage::Pointer filter = ITKSimpleContourExtractorImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKSimpleContourExtractorImage::getHumanLabel()
{ return "[ITK] ITK  Simple Contour Extractor Image Filter (KW)"; }

