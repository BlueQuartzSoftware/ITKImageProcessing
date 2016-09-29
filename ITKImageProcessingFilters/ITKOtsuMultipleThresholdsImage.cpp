// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKOtsuMultipleThresholdsImage.h"

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
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKOtsuMultipleThresholdsImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKOtsuMultipleThresholdsImage::ITKOtsuMultipleThresholdsImage() :
  ITKImageBase(),
m_NumberOfThresholds(1u),
m_LabelOffset(0u),
m_NumberOfHistogramBins(128u),
m_ValleyEmphasis(false)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKOtsuMultipleThresholdsImage::~ITKOtsuMultipleThresholdsImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKOtsuMultipleThresholdsImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_INTEGER_FP("NumberOfThresholds", NumberOfThresholds, FilterParameter::Parameter, ITKOtsuMultipleThresholdsImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("LabelOffset", LabelOffset, FilterParameter::Parameter, ITKOtsuMultipleThresholdsImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfHistogramBins", NumberOfHistogramBins, FilterParameter::Parameter, ITKOtsuMultipleThresholdsImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("ValleyEmphasis", ValleyEmphasis, FilterParameter::Parameter, ITKOtsuMultipleThresholdsImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKOtsuMultipleThresholdsImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKOtsuMultipleThresholdsImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKOtsuMultipleThresholdsImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKOtsuMultipleThresholdsImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setNumberOfThresholds(reader->readValue("NumberOfThresholds", getNumberOfThresholds()));
  setLabelOffset(reader->readValue("LabelOffset", getLabelOffset()));
  setNumberOfHistogramBins(reader->readValue("NumberOfHistogramBins", getNumberOfHistogramBins()));
  setValleyEmphasis(reader->readValue("ValleyEmphasis", getValleyEmphasis()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKOtsuMultipleThresholdsImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKOtsuMultipleThresholdsImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<uint8_t,int>(m_NumberOfThresholds, "NumberOfThresholds");
  CheckIntegerEntry<uint8_t,int>(m_LabelOffset, "LabelOffset");
  CheckIntegerEntry<uint32_t,double>(m_NumberOfHistogramBins, "NumberOfHistogramBins");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKOtsuMultipleThresholdsImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKOtsuMultipleThresholdsImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::OtsuMultipleThresholdsImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetNumberOfThresholds(static_cast<uint8_t>(m_NumberOfThresholds));
  filter->SetLabelOffset(static_cast<uint8_t>(m_LabelOffset));
  filter->SetNumberOfHistogramBins(static_cast<uint32_t>(m_NumberOfHistogramBins));
  filter->SetValleyEmphasis(static_cast<bool>(m_ValleyEmphasis));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKOtsuMultipleThresholdsImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKOtsuMultipleThresholdsImage::newFilterInstance(bool copyFilterParameters)
{
  ITKOtsuMultipleThresholdsImage::Pointer filter = ITKOtsuMultipleThresholdsImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKOtsuMultipleThresholdsImage::getHumanLabel()
{ return "[ITK] ITK  Otsu Multiple Thresholds Image Filter (KW)"; }

