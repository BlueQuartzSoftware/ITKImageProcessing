/*
 * Your License or Copyright can go here
 */

#include "ITKBinaryThreshold.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

//thresholding filter
#include "itkBinaryThresholdImageFilter.h"

// Include the MOC generated file for this class
#include "moc_ITKBinaryThreshold.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryThreshold::ITKBinaryThreshold() :
  ITKImageBase(),
  m_LowerThresholdValue(128.0),
  m_UpperThresholdValue(255.0),
  m_InsideValue(255.0),
  m_OutsideValue(0)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryThreshold::~ITKBinaryThreshold()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThreshold::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Lower Threshold Value", LowerThresholdValue, FilterParameter::Parameter, ITKBinaryThreshold));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Upper Threshold Value", UpperThresholdValue, FilterParameter::Parameter, ITKBinaryThreshold));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Inside Value", InsideValue, FilterParameter::Parameter, ITKBinaryThreshold));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Outisde Value", OutsideValue, FilterParameter::Parameter, ITKBinaryThreshold));
  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKBinaryThreshold, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to threshold", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKBinaryThreshold, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Threshold Array", NewCellArrayName, FilterParameter::CreatedArray, ITKBinaryThreshold));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThreshold::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setLowerThresholdValue(reader->readValue("LowerThresholdValue", getLowerThresholdValue()));
  setUpperThresholdValue(reader->readValue("UpperThresholdValue", getUpperThresholdValue()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType>
void ITKBinaryThreshold::CheckLimits(double value, QString name)
{
  if (value < static_cast<double>(std::numeric_limits<PixelType>::lowest())
     || value > static_cast<double>(std::numeric_limits<PixelType>::max()))
  {
    setErrorCondition(-1);
    QString errorMessage = name + QString("must be greater or equal than %1 and lesser or equal than %2");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(
        std::numeric_limits<PixelType>::lowest()).arg(std::numeric_limits<PixelType>::max())
                                                         , getErrorCondition()
                                                         );
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InPixelType, typename OutPixelType, unsigned int Dimension>
void ITKBinaryThreshold::dataCheck()
{
  setErrorCondition(0);
  // Check consistency of parameters
  CheckLimits<OutPixelType>(m_InsideValue, "Inside value");
  CheckLimits<OutPixelType>(m_OutsideValue, "Outside value");
  CheckLimits<InPixelType>(m_LowerThresholdValue, "Lower threshold value");
  CheckLimits<InPixelType>(m_UpperThresholdValue, "Upper threshold value");

  if (m_LowerThresholdValue > m_UpperThresholdValue)
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(),"Lower threshold value must be lesser than upper threshold value", getErrorCondition());
    return;
  }
  ITKImageBase::dataCheck<InPixelType, OutPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThreshold::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InPixelType, typename OutPixelType, unsigned int Dimension>
void ITKBinaryThreshold::filter()
{
    typedef itk::Dream3DImage<InPixelType, Dimension> InImageType;
    typedef itk::Dream3DImage<OutPixelType, Dimension> OutImageType;
    //define threshold filters
    typedef itk::BinaryThresholdImageFilter <InImageType, OutImageType> BinaryThresholdImageFilterType;
    //create Itk's binary threshold filter object
    typename BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
    thresholdFilter->SetLowerThreshold(static_cast<InPixelType>(m_LowerThresholdValue));
    thresholdFilter->SetUpperThreshold(static_cast<InPixelType>(m_UpperThresholdValue));
    thresholdFilter->SetInsideValue(static_cast<InPixelType>(m_InsideValue));
    thresholdFilter->SetOutsideValue(static_cast<InPixelType>(m_OutsideValue));
    this->ITKImageBase::filter<InPixelType, OutPixelType, Dimension, BinaryThresholdImageFilterType>(thresholdFilter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThreshold::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKBinaryThreshold::newFilterInstance(bool copyFilterParameters)
{
  ITKBinaryThreshold::Pointer filter = ITKBinaryThreshold::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThreshold::getHumanLabel()
{ return "[ITK] Binary Threshold (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThreshold::getSubGroupName()
{ return "ITKThresholding"; }

