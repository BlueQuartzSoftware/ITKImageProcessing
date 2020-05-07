// File automatically generated

/*
 * Your License or Copyright can go here
 */


#include "ITKImageProcessing/ITKImageProcessingFilters/ITKBinaryThresholdImage.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryThresholdImage::ITKBinaryThresholdImage()
{
  m_LowerThreshold = StaticCastScalar<double, double, double>(0.0);
  m_UpperThreshold = StaticCastScalar<double, double, double>(255.0);
  m_InsideValue = StaticCastScalar<int, int, int>(1u);
  m_OutsideValue = StaticCastScalar<int, int, int>(0u);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryThresholdImage::~ITKBinaryThresholdImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

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
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
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
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setLowerThreshold(reader->readValue("LowerThreshold", getLowerThreshold()));
  setUpperThreshold(reader->readValue("UpperThreshold", getUpperThreshold()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKBinaryThresholdImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<uint8_t, int>(m_InsideValue, "InsideValue", true);
  this->CheckIntegerEntry<uint8_t, int>(m_OutsideValue, "OutsideValue", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::dataCheck()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4,uint8_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKBinaryThresholdImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::BinaryThresholdImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetLowerThreshold(static_cast<double>(m_LowerThreshold));
  filter->SetUpperThreshold(static_cast<double>(m_UpperThreshold));
  filter->SetInsideValue(static_cast<uint8_t>(m_InsideValue));
  filter->SetOutsideValue(static_cast<uint8_t>(m_OutsideValue));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4,uint8_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKBinaryThresholdImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKBinaryThresholdImage::Pointer filter = ITKBinaryThresholdImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBinaryThresholdImage::getHumanLabel() const
{
  return "ITK::Binary Threshold Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKBinaryThresholdImage::getUuid() const
{
  return QUuid("{ba8a3f2e-3963-57c0-a8da-239e25de0526}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBinaryThresholdImage::getSubGroupName() const
{
  return "ITK Thresholding";
}

// -----------------------------------------------------------------------------
ITKBinaryThresholdImage::Pointer ITKBinaryThresholdImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKBinaryThresholdImage> ITKBinaryThresholdImage::New()
{
  struct make_shared_enabler : public ITKBinaryThresholdImage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKBinaryThresholdImage::getNameOfClass() const
{
  return QString("ITKBinaryThresholdImage");
}

// -----------------------------------------------------------------------------
QString ITKBinaryThresholdImage::ClassName()
{
  return QString("ITKBinaryThresholdImage");
}

// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::setLowerThreshold(double value)
{
  m_LowerThreshold = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryThresholdImage::getLowerThreshold() const
{
  return m_LowerThreshold;
}

// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::setUpperThreshold(double value)
{
  m_UpperThreshold = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryThresholdImage::getUpperThreshold() const
{
  return m_UpperThreshold;
}

// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::setInsideValue(int value)
{
  m_InsideValue = value;
}

// -----------------------------------------------------------------------------
int ITKBinaryThresholdImage::getInsideValue() const
{
  return m_InsideValue;
}

// -----------------------------------------------------------------------------
void ITKBinaryThresholdImage::setOutsideValue(int value)
{
  m_OutsideValue = value;
}

// -----------------------------------------------------------------------------
int ITKBinaryThresholdImage::getOutsideValue() const
{
  return m_OutsideValue;
}


