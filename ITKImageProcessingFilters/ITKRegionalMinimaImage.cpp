// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKRegionalMinimaImage.h"
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
ITKRegionalMinimaImage::ITKRegionalMinimaImage()
{
  m_BackgroundValue = StaticCastScalar<double, double, double>(0.0);
  m_ForegroundValue = StaticCastScalar<double, double, double>(1.0);
  m_FullyConnected = StaticCastScalar<bool, bool, bool>(false);
  m_FlatIsMinima = StaticCastScalar<bool, bool, bool>(true);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRegionalMinimaImage::~ITKRegionalMinimaImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMinimaImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("BackgroundValue", BackgroundValue, FilterParameter::Category::Parameter, ITKRegionalMinimaImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ForegroundValue", ForegroundValue, FilterParameter::Category::Parameter, ITKRegionalMinimaImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Category::Parameter, ITKRegionalMinimaImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FlatIsMinima", FlatIsMinima, FilterParameter::Category::Parameter, ITKRegionalMinimaImage));

  std::vector<QString> linkedProps;
  linkedProps.push_back("NewCellArrayName");
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKRegionalMinimaImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKRegionalMinimaImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMinimaImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setBackgroundValue(reader->readValue("BackgroundValue", getBackgroundValue()));
  setForegroundValue(reader->readValue("ForegroundValue", getForegroundValue()));
  setFullyConnected(reader->readValue("FullyConnected", getFullyConnected()));
  setFlatIsMinima(reader->readValue("FlatIsMinima", getFlatIsMinima()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKRegionalMinimaImage::dataCheckImpl()
{
  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMinimaImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, uint32_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKRegionalMinimaImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::RegionalMinimaImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetBackgroundValue(static_cast<double>(m_BackgroundValue));
  filter->SetForegroundValue(static_cast<double>(m_ForegroundValue));
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  filter->SetFlatIsMinima(static_cast<bool>(m_FlatIsMinima));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMinimaImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, uint32_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKRegionalMinimaImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKRegionalMinimaImage::Pointer filter = ITKRegionalMinimaImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKRegionalMinimaImage::getHumanLabel() const
{
  return "ITK::Regional Minima Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKRegionalMinimaImage::getUuid() const
{
  return QUuid("{f8ed09ae-1f84-5668-a4ad-d770e264f01e}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKRegionalMinimaImage::getSubGroupName() const
{
  return "ITK BiasCorrection";
}

// -----------------------------------------------------------------------------
ITKRegionalMinimaImage::Pointer ITKRegionalMinimaImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKRegionalMinimaImage> ITKRegionalMinimaImage::New()
{
  struct make_shared_enabler : public ITKRegionalMinimaImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKRegionalMinimaImage::getNameOfClass() const
{
  return QString("ITKRegionalMinimaImage");
}

// -----------------------------------------------------------------------------
QString ITKRegionalMinimaImage::ClassName()
{
  return QString("ITKRegionalMinimaImage");
}

// -----------------------------------------------------------------------------
void ITKRegionalMinimaImage::setBackgroundValue(double value)
{
  m_BackgroundValue = value;
}

// -----------------------------------------------------------------------------
double ITKRegionalMinimaImage::getBackgroundValue() const
{
  return m_BackgroundValue;
}

// -----------------------------------------------------------------------------
void ITKRegionalMinimaImage::setForegroundValue(double value)
{
  m_ForegroundValue = value;
}

// -----------------------------------------------------------------------------
double ITKRegionalMinimaImage::getForegroundValue() const
{
  return m_ForegroundValue;
}

// -----------------------------------------------------------------------------
void ITKRegionalMinimaImage::setFullyConnected(bool value)
{
  m_FullyConnected = value;
}

// -----------------------------------------------------------------------------
bool ITKRegionalMinimaImage::getFullyConnected() const
{
  return m_FullyConnected;
}

// -----------------------------------------------------------------------------
void ITKRegionalMinimaImage::setFlatIsMinima(bool value)
{
  m_FlatIsMinima = value;
}

// -----------------------------------------------------------------------------
bool ITKRegionalMinimaImage::getFlatIsMinima() const
{
  return m_FlatIsMinima;
}
