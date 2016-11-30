// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKBinaryThresholdImage.h"

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
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkBinaryThresholdImageFilter.h>


// Include the MOC generated file for this class
#include "moc_ITKBinaryThresholdImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryThresholdImage::ITKBinaryThresholdImage() :
  ITKImageBase(),
m_LowerThreshold(0.0),
m_UpperThreshold(255.0),
m_InsideValue(1u),
m_OutsideValue(0u)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryThresholdImage::~ITKBinaryThresholdImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("LowerThreshold", LowerThreshold, FilterParameter::Parameter, ITKBinaryThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("UpperThreshold", UpperThreshold, FilterParameter::Parameter, ITKBinaryThresholdImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("InsideValue", InsideValue, FilterParameter::Parameter, ITKBinaryThresholdImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKBinaryThresholdImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKBinaryThresholdImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKBinaryThresholdImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKBinaryThresholdImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setLowerThreshold(reader->readValue("LowerThreshold", getLowerThreshold()));
  setUpperThreshold(reader->readValue("UpperThreshold", getUpperThreshold()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKBinaryThresholdImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKBinaryThresholdImage::dataCheck()
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
void ITKBinaryThresholdImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKBinaryThresholdImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::BinaryThresholdImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetLowerThreshold(static_cast<double>(m_LowerThreshold));
  filter->SetUpperThreshold(static_cast<double>(m_UpperThreshold));
  filter->SetInsideValue(static_cast<uint8_t>(m_InsideValue));
  filter->SetOutsideValue(static_cast<uint8_t>(m_OutsideValue));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKBinaryThresholdImage::newFilterInstance(bool copyFilterParameters)
{
  ITKBinaryThresholdImage::Pointer filter = ITKBinaryThresholdImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThresholdImage::getHumanLabel()
{ return "[ITK] ITK  Binary Threshold Image Filter (KW)"; }

