// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKThresholdImage.h"

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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkThresholdImageFilter.h>


// Include the MOC generated file for this class
#include "moc_ITKThresholdImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdImage::ITKThresholdImage() :
  ITKImageBase(),
m_Lower(0.0),
m_Upper(1.0),
m_OutsideValue(0.0)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKThresholdImage::~ITKThresholdImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Lower", Lower, FilterParameter::Parameter, ITKThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Upper", Upper, FilterParameter::Parameter, ITKThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKThresholdImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKThresholdImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKThresholdImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKThresholdImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setLower(reader->readValue("Lower", getLower()));
  setUpper(reader->readValue("Upper", getUpper()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKThresholdImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKThresholdImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKThresholdImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::ThresholdImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetLower(static_cast<double>(m_Lower));
  filter->SetUpper(static_cast<double>(m_Upper));
  filter->SetOutsideValue(static_cast<double>(m_OutsideValue));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKThresholdImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKThresholdImage::newFilterInstance(bool copyFilterParameters)
{
  ITKThresholdImage::Pointer filter = ITKThresholdImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKThresholdImage::getHumanLabel()
{ return "[ITK] ITK  Threshold Image Filter (KW)"; }

