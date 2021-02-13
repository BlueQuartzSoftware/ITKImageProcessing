// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKIntensityWindowingImage.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIntensityWindowingImage::ITKIntensityWindowingImage()
{
  m_WindowMinimum = StaticCastScalar<double, double, double>(0.0);
  m_WindowMaximum = StaticCastScalar<double, double, double>(255.0);
  m_OutputMinimum = StaticCastScalar<double, double, double>(0.0);
  m_OutputMaximum = StaticCastScalar<double, double, double>(255.0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIntensityWindowingImage::~ITKIntensityWindowingImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("WindowMinimum", WindowMinimum, FilterParameter::Category::Parameter, ITKIntensityWindowingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("WindowMaximum", WindowMaximum, FilterParameter::Category::Parameter, ITKIntensityWindowingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMinimum", OutputMinimum, FilterParameter::Category::Parameter, ITKIntensityWindowingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMaximum", OutputMaximum, FilterParameter::Category::Parameter, ITKIntensityWindowingImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKIntensityWindowingImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKIntensityWindowingImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setWindowMinimum(reader->readValue("WindowMinimum", getWindowMinimum()));
  setWindowMaximum(reader->readValue("WindowMaximum", getWindowMaximum()));
  setOutputMinimum(reader->readValue("OutputMinimum", getOutputMinimum()));
  setOutputMaximum(reader->readValue("OutputMaximum", getOutputMaximum()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKIntensityWindowingImage::dataCheckImpl()
{
  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKIntensityWindowingImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::IntensityWindowingImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetWindowMinimum(static_cast<double>(m_WindowMinimum));
  filter->SetWindowMaximum(static_cast<double>(m_WindowMaximum));
  filter->SetOutputMinimum(static_cast<double>(m_OutputMinimum));
  filter->SetOutputMaximum(static_cast<double>(m_OutputMaximum));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
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
AbstractFilter::Pointer ITKIntensityWindowingImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKIntensityWindowingImage::Pointer filter = ITKIntensityWindowingImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKIntensityWindowingImage::getHumanLabel() const
{
  return "ITK::Intensity Windowing Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKIntensityWindowingImage::getUuid() const
{
  return QUuid("{4faf4c59-6f29-53af-bc78-5aecffce0e37}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKIntensityWindowingImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}

// -----------------------------------------------------------------------------
ITKIntensityWindowingImage::Pointer ITKIntensityWindowingImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKIntensityWindowingImage> ITKIntensityWindowingImage::New()
{
  struct make_shared_enabler : public ITKIntensityWindowingImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKIntensityWindowingImage::getNameOfClass() const
{
  return QString("ITKIntensityWindowingImage");
}

// -----------------------------------------------------------------------------
QString ITKIntensityWindowingImage::ClassName()
{
  return QString("ITKIntensityWindowingImage");
}

// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::setWindowMinimum(double value)
{
  m_WindowMinimum = value;
}

// -----------------------------------------------------------------------------
double ITKIntensityWindowingImage::getWindowMinimum() const
{
  return m_WindowMinimum;
}

// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::setWindowMaximum(double value)
{
  m_WindowMaximum = value;
}

// -----------------------------------------------------------------------------
double ITKIntensityWindowingImage::getWindowMaximum() const
{
  return m_WindowMaximum;
}

// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::setOutputMinimum(double value)
{
  m_OutputMinimum = value;
}

// -----------------------------------------------------------------------------
double ITKIntensityWindowingImage::getOutputMinimum() const
{
  return m_OutputMinimum;
}

// -----------------------------------------------------------------------------
void ITKIntensityWindowingImage::setOutputMaximum(double value)
{
  m_OutputMaximum = value;
}

// -----------------------------------------------------------------------------
double ITKIntensityWindowingImage::getOutputMaximum() const
{
  return m_OutputMaximum;
}
