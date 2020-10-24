// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKSigmoidImage.h"
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
ITKSigmoidImage::ITKSigmoidImage()
{
  m_Alpha = StaticCastScalar<double, double, double>(1);
  m_Beta = StaticCastScalar<double, double, double>(0);
  m_OutputMaximum = StaticCastScalar<double, double, double>(255);
  m_OutputMinimum = StaticCastScalar<double, double, double>(0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSigmoidImage::~ITKSigmoidImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSigmoidImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Alpha", Alpha, FilterParameter::Parameter, ITKSigmoidImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Beta", Beta, FilterParameter::Parameter, ITKSigmoidImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMaximum", OutputMaximum, FilterParameter::Parameter, ITKSigmoidImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMinimum", OutputMinimum, FilterParameter::Parameter, ITKSigmoidImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKSigmoidImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKSigmoidImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSigmoidImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setAlpha(reader->readValue("Alpha", getAlpha()));
  setBeta(reader->readValue("Beta", getBeta()));
  setOutputMaximum(reader->readValue("OutputMaximum", getOutputMaximum()));
  setOutputMinimum(reader->readValue("OutputMinimum", getOutputMinimum()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSigmoidImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSigmoidImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSigmoidImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::SigmoidImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetAlpha(static_cast<double>(m_Alpha));
  filter->SetBeta(static_cast<double>(m_Beta));
  filter->SetOutputMaximum(static_cast<double>(m_OutputMaximum));
  filter->SetOutputMinimum(static_cast<double>(m_OutputMinimum));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSigmoidImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKSigmoidImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKSigmoidImage::Pointer filter = ITKSigmoidImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSigmoidImage::getHumanLabel() const
{
  return "ITK::Sigmoid Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKSigmoidImage::getUuid() const
{
  return QUuid("{e6675be7-e98d-5e0f-a088-ba15cc301038}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSigmoidImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}

// -----------------------------------------------------------------------------
ITKSigmoidImage::Pointer ITKSigmoidImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKSigmoidImage> ITKSigmoidImage::New()
{
  struct make_shared_enabler : public ITKSigmoidImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKSigmoidImage::getNameOfClass() const
{
  return QString("ITKSigmoidImage");
}

// -----------------------------------------------------------------------------
QString ITKSigmoidImage::ClassName()
{
  return QString("ITKSigmoidImage");
}

// -----------------------------------------------------------------------------
void ITKSigmoidImage::setAlpha(double value)
{
  m_Alpha = value;
}

// -----------------------------------------------------------------------------
double ITKSigmoidImage::getAlpha() const
{
  return m_Alpha;
}

// -----------------------------------------------------------------------------
void ITKSigmoidImage::setBeta(double value)
{
  m_Beta = value;
}

// -----------------------------------------------------------------------------
double ITKSigmoidImage::getBeta() const
{
  return m_Beta;
}

// -----------------------------------------------------------------------------
void ITKSigmoidImage::setOutputMaximum(double value)
{
  m_OutputMaximum = value;
}

// -----------------------------------------------------------------------------
double ITKSigmoidImage::getOutputMaximum() const
{
  return m_OutputMaximum;
}

// -----------------------------------------------------------------------------
void ITKSigmoidImage::setOutputMinimum(double value)
{
  m_OutputMinimum = value;
}

// -----------------------------------------------------------------------------
double ITKSigmoidImage::getOutputMinimum() const
{
  return m_OutputMinimum;
}
