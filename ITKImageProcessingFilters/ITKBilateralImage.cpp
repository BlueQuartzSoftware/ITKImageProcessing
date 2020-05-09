// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKBilateralImage.h"
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
ITKBilateralImage::ITKBilateralImage()
{
  m_DomainSigma = StaticCastScalar<double, double, double>(4.0);
  m_RangeSigma = StaticCastScalar<double, double, double>(50.0);
  m_NumberOfRangeGaussianSamples = StaticCastScalar<double, double, double>(100u);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBilateralImage::~ITKBilateralImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBilateralImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("DomainSigma", DomainSigma, FilterParameter::Parameter, ITKBilateralImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("RangeSigma", RangeSigma, FilterParameter::Parameter, ITKBilateralImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfRangeGaussianSamples", NumberOfRangeGaussianSamples, FilterParameter::Parameter, ITKBilateralImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKBilateralImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKBilateralImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKBilateralImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBilateralImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setDomainSigma(reader->readValue("DomainSigma", getDomainSigma()));
  setRangeSigma(reader->readValue("RangeSigma", getRangeSigma()));
  setNumberOfRangeGaussianSamples(reader->readValue("NumberOfRangeGaussianSamples", getNumberOfRangeGaussianSamples()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKBilateralImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<unsigned int, double>(m_NumberOfRangeGaussianSamples, "NumberOfRangeGaussianSamples", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBilateralImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKBilateralImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::BilateralImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetDomainSigma(static_cast<double>(m_DomainSigma));
  filter->SetRangeSigma(static_cast<double>(m_RangeSigma));
  filter->SetNumberOfRangeGaussianSamples(static_cast<unsigned int>(m_NumberOfRangeGaussianSamples));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBilateralImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKBilateralImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKBilateralImage::Pointer filter = ITKBilateralImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBilateralImage::getHumanLabel() const
{
  return "ITK::Bilateral Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKBilateralImage::getUuid() const
{
  return QUuid("{18ab754c-3219-59c8-928e-5fb4a09174e0}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBilateralImage::getSubGroupName() const
{
  return "ITK Smoothing";
}

// -----------------------------------------------------------------------------
ITKBilateralImage::Pointer ITKBilateralImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKBilateralImage> ITKBilateralImage::New()
{
  struct make_shared_enabler : public ITKBilateralImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKBilateralImage::getNameOfClass() const
{
  return QString("ITKBilateralImage");
}

// -----------------------------------------------------------------------------
QString ITKBilateralImage::ClassName()
{
  return QString("ITKBilateralImage");
}

// -----------------------------------------------------------------------------
void ITKBilateralImage::setDomainSigma(double value)
{
  m_DomainSigma = value;
}

// -----------------------------------------------------------------------------
double ITKBilateralImage::getDomainSigma() const
{
  return m_DomainSigma;
}

// -----------------------------------------------------------------------------
void ITKBilateralImage::setRangeSigma(double value)
{
  m_RangeSigma = value;
}

// -----------------------------------------------------------------------------
double ITKBilateralImage::getRangeSigma() const
{
  return m_RangeSigma;
}

// -----------------------------------------------------------------------------
void ITKBilateralImage::setNumberOfRangeGaussianSamples(double value)
{
  m_NumberOfRangeGaussianSamples = value;
}

// -----------------------------------------------------------------------------
double ITKBilateralImage::getNumberOfRangeGaussianSamples() const
{
  return m_NumberOfRangeGaussianSamples;
}
