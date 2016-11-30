// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKDoubleThresholdImage.h"

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
#include <itkDoubleThresholdImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKDoubleThresholdImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDoubleThresholdImage::ITKDoubleThresholdImage() :
  ITKImageBase(),
m_Threshold1(0.0),
m_Threshold2(1.0),
m_Threshold3(254.0),
m_Threshold4(255.0),
m_InsideValue(1u),
m_OutsideValue(0u),
m_FullyConnected(false)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDoubleThresholdImage::~ITKDoubleThresholdImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold1", Threshold1, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold2", Threshold2, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold3", Threshold3, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold4", Threshold4, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("InsideValue", InsideValue, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Parameter, ITKDoubleThresholdImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKDoubleThresholdImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKDoubleThresholdImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKDoubleThresholdImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setThreshold1(reader->readValue("Threshold1", getThreshold1()));
  setThreshold2(reader->readValue("Threshold2", getThreshold2()));
  setThreshold3(reader->readValue("Threshold3", getThreshold3()));
  setThreshold4(reader->readValue("Threshold4", getThreshold4()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));
  setFullyConnected(reader->readValue("FullyConnected", getFullyConnected()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKDoubleThresholdImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKDoubleThresholdImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<uint8_t,int>(m_InsideValue, "InsideValue");
  CheckIntegerEntry<uint8_t,int>(m_OutsideValue, "OutsideValue");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKDoubleThresholdImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::DoubleThresholdImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetThreshold1(static_cast<double>(m_Threshold1));
  filter->SetThreshold2(static_cast<double>(m_Threshold2));
  filter->SetThreshold3(static_cast<double>(m_Threshold3));
  filter->SetThreshold4(static_cast<double>(m_Threshold4));
  filter->SetInsideValue(static_cast<uint8_t>(m_InsideValue));
  filter->SetOutsideValue(static_cast<uint8_t>(m_OutsideValue));
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKDoubleThresholdImage::newFilterInstance(bool copyFilterParameters)
{
  ITKDoubleThresholdImage::Pointer filter = ITKDoubleThresholdImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKDoubleThresholdImage::getHumanLabel()
{ return "[ITK] ITK  Double Threshold Image Filter (KW)"; }

