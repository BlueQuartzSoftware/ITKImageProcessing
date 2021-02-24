// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKMinMaxCurvatureFlowImage.h"
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
ITKMinMaxCurvatureFlowImage::ITKMinMaxCurvatureFlowImage()
{
  m_TimeStep = StaticCastScalar<double, double, double>(0.05);
  m_NumberOfIterations = StaticCastScalar<double, double, double>(5u);
  m_StencilRadius = StaticCastScalar<int, int, int>(2);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMinMaxCurvatureFlowImage::~ITKMinMaxCurvatureFlowImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TimeStep", TimeStep, FilterParameter::Category::Parameter, ITKMinMaxCurvatureFlowImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Category::Parameter, ITKMinMaxCurvatureFlowImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("StencilRadius", StencilRadius, FilterParameter::Category::Parameter, ITKMinMaxCurvatureFlowImage));

  std::vector<QString> linkedProps;
  linkedProps.push_back("NewCellArrayName");
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKMinMaxCurvatureFlowImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKMinMaxCurvatureFlowImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setTimeStep(reader->readValue("TimeStep", getTimeStep()));
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));
  setStencilRadius(reader->readValue("StencilRadius", getStencilRadius()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMinMaxCurvatureFlowImage::dataCheckImpl()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfIterations, "NumberOfIterations", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMinMaxCurvatureFlowImage::filter()
{
  typedef typename itk::NumericTraits<InputPixelType>::RealType FloatPixelType;
  typedef itk::Image<FloatPixelType, Dimension> FloatImageType;
  typedef itk::MinMaxCurvatureFlowImageFilter<FloatImageType, FloatImageType> FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTimeStep(static_cast<double>(m_TimeStep));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  filter->SetStencilRadius(static_cast<int>(m_StencilRadius));
  this->ITKImageProcessingBase::filterCastToFloat<InputPixelType, InputPixelType, Dimension, FilterType, FloatImageType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKMinMaxCurvatureFlowImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKMinMaxCurvatureFlowImage::Pointer filter = ITKMinMaxCurvatureFlowImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKMinMaxCurvatureFlowImage::getHumanLabel() const
{
  return "ITK::Min Max Curvature Flow Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKMinMaxCurvatureFlowImage::getUuid() const
{
  return QUuid("{bd9bdf46-a229-544a-b158-151920261a63}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKMinMaxCurvatureFlowImage::getSubGroupName() const
{
  return "ITK CurvatureFlow";
}

// -----------------------------------------------------------------------------
ITKMinMaxCurvatureFlowImage::Pointer ITKMinMaxCurvatureFlowImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKMinMaxCurvatureFlowImage> ITKMinMaxCurvatureFlowImage::New()
{
  struct make_shared_enabler : public ITKMinMaxCurvatureFlowImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKMinMaxCurvatureFlowImage::getNameOfClass() const
{
  return QString("ITKMinMaxCurvatureFlowImage");
}

// -----------------------------------------------------------------------------
QString ITKMinMaxCurvatureFlowImage::ClassName()
{
  return QString("ITKMinMaxCurvatureFlowImage");
}

// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::setTimeStep(double value)
{
  m_TimeStep = value;
}

// -----------------------------------------------------------------------------
double ITKMinMaxCurvatureFlowImage::getTimeStep() const
{
  return m_TimeStep;
}

// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::setNumberOfIterations(double value)
{
  m_NumberOfIterations = value;
}

// -----------------------------------------------------------------------------
double ITKMinMaxCurvatureFlowImage::getNumberOfIterations() const
{
  return m_NumberOfIterations;
}

// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::setStencilRadius(int value)
{
  m_StencilRadius = value;
}

// -----------------------------------------------------------------------------
int ITKMinMaxCurvatureFlowImage::getStencilRadius() const
{
  return m_StencilRadius;
}
