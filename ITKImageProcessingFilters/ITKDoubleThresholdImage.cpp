// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKDoubleThresholdImage.h"
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
ITKDoubleThresholdImage::ITKDoubleThresholdImage()
{
  m_Threshold1 = StaticCastScalar<double, double, double>(0.0);
  m_Threshold2 = StaticCastScalar<double, double, double>(1.0);
  m_Threshold3 = StaticCastScalar<double, double, double>(254.0);
  m_Threshold4 = StaticCastScalar<double, double, double>(255.0);
  m_InsideValue = StaticCastScalar<int, int, int>(1u);
  m_OutsideValue = StaticCastScalar<int, int, int>(0u);
  m_FullyConnected = StaticCastScalar<bool, bool, bool>(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDoubleThresholdImage::~ITKDoubleThresholdImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold1", Threshold1, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold2", Threshold2, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold3", Threshold3, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold4", Threshold4, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("InsideValue", InsideValue, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKDoubleThresholdImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Parameter, ITKDoubleThresholdImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
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
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
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
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKDoubleThresholdImage::dataCheckImpl()
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
void ITKDoubleThresholdImage::dataCheck()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, uint8_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKDoubleThresholdImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::DoubleThresholdImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetThreshold1(static_cast<double>(m_Threshold1));
  filter->SetThreshold2(static_cast<double>(m_Threshold2));
  filter->SetThreshold3(static_cast<double>(m_Threshold3));
  filter->SetThreshold4(static_cast<double>(m_Threshold4));
  filter->SetInsideValue(static_cast<uint8_t>(m_InsideValue));
  filter->SetOutsideValue(static_cast<uint8_t>(m_OutsideValue));
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, uint8_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKDoubleThresholdImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKDoubleThresholdImage::Pointer filter = ITKDoubleThresholdImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKDoubleThresholdImage::getHumanLabel() const
{
  return "ITK::Double Threshold Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKDoubleThresholdImage::getUuid() const
{
  return QUuid("{7fcfa231-519e-58da-bf8f-ad0f728bf0fe}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKDoubleThresholdImage::getSubGroupName() const
{
  return "ITK BiasCorrection";
}

// -----------------------------------------------------------------------------
ITKDoubleThresholdImage::Pointer ITKDoubleThresholdImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKDoubleThresholdImage> ITKDoubleThresholdImage::New()
{
  struct make_shared_enabler : public ITKDoubleThresholdImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKDoubleThresholdImage::getNameOfClass() const
{
  return QString("ITKDoubleThresholdImage");
}

// -----------------------------------------------------------------------------
QString ITKDoubleThresholdImage::ClassName()
{
  return QString("ITKDoubleThresholdImage");
}

// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setThreshold1(double value)
{
  m_Threshold1 = value;
}

// -----------------------------------------------------------------------------
double ITKDoubleThresholdImage::getThreshold1() const
{
  return m_Threshold1;
}

// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setThreshold2(double value)
{
  m_Threshold2 = value;
}

// -----------------------------------------------------------------------------
double ITKDoubleThresholdImage::getThreshold2() const
{
  return m_Threshold2;
}

// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setThreshold3(double value)
{
  m_Threshold3 = value;
}

// -----------------------------------------------------------------------------
double ITKDoubleThresholdImage::getThreshold3() const
{
  return m_Threshold3;
}

// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setThreshold4(double value)
{
  m_Threshold4 = value;
}

// -----------------------------------------------------------------------------
double ITKDoubleThresholdImage::getThreshold4() const
{
  return m_Threshold4;
}

// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setInsideValue(int value)
{
  m_InsideValue = value;
}

// -----------------------------------------------------------------------------
int ITKDoubleThresholdImage::getInsideValue() const
{
  return m_InsideValue;
}

// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setOutsideValue(int value)
{
  m_OutsideValue = value;
}

// -----------------------------------------------------------------------------
int ITKDoubleThresholdImage::getOutsideValue() const
{
  return m_OutsideValue;
}

// -----------------------------------------------------------------------------
void ITKDoubleThresholdImage::setFullyConnected(bool value)
{
  m_FullyConnected = value;
}

// -----------------------------------------------------------------------------
bool ITKDoubleThresholdImage::getFullyConnected() const
{
  return m_FullyConnected;
}
