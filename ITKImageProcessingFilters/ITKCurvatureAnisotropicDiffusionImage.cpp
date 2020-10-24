// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKCurvatureAnisotropicDiffusionImage.h"
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
ITKCurvatureAnisotropicDiffusionImage::ITKCurvatureAnisotropicDiffusionImage()
{
  m_TimeStep = StaticCastScalar<double, double, double>(0.0625);
  m_ConductanceParameter = StaticCastScalar<double, double, double>(3.0);
  m_ConductanceScalingUpdateInterval = StaticCastScalar<double, double, double>(1u);
  m_NumberOfIterations = StaticCastScalar<double, double, double>(5u);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCurvatureAnisotropicDiffusionImage::~ITKCurvatureAnisotropicDiffusionImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TimeStep", TimeStep, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ConductanceParameter", ConductanceParameter, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ConductanceScalingUpdateInterval", ConductanceScalingUpdateInterval, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKCurvatureAnisotropicDiffusionImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKCurvatureAnisotropicDiffusionImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setTimeStep(reader->readValue("TimeStep", getTimeStep()));
  setConductanceParameter(reader->readValue("ConductanceParameter", getConductanceParameter()));
  setConductanceScalingUpdateInterval(reader->readValue("ConductanceScalingUpdateInterval", getConductanceScalingUpdateInterval()));
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKCurvatureAnisotropicDiffusionImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<unsigned int, double>(m_ConductanceScalingUpdateInterval, "ConductanceScalingUpdateInterval", true);
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfIterations, "NumberOfIterations", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKCurvatureAnisotropicDiffusionImage::filter()
{
  typedef typename itk::NumericTraits<InputPixelType>::RealType FloatPixelType;
  typedef itk::Image<FloatPixelType, Dimension> FloatImageType;
  typedef itk::CurvatureAnisotropicDiffusionImageFilter<FloatImageType, FloatImageType> FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTimeStep(static_cast<double>(m_TimeStep));
  filter->SetConductanceParameter(static_cast<double>(m_ConductanceParameter));
  filter->SetConductanceScalingUpdateInterval(static_cast<unsigned int>(m_ConductanceScalingUpdateInterval));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  this->ITKImageProcessingBase::filterCastToFloat<InputPixelType, InputPixelType, Dimension, FilterType, FloatImageType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKCurvatureAnisotropicDiffusionImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKCurvatureAnisotropicDiffusionImage::Pointer filter = ITKCurvatureAnisotropicDiffusionImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKCurvatureAnisotropicDiffusionImage::getHumanLabel() const
{
  return "ITK::Curvature Anisotropic Diffusion Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKCurvatureAnisotropicDiffusionImage::getUuid() const
{
  return QUuid("{009fb2d0-6f65-5406-bb2a-4a883d0bc18c}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKCurvatureAnisotropicDiffusionImage::getSubGroupName() const
{
  return "ITK AnisotropicSmoothing";
}

// -----------------------------------------------------------------------------
ITKCurvatureAnisotropicDiffusionImage::Pointer ITKCurvatureAnisotropicDiffusionImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKCurvatureAnisotropicDiffusionImage> ITKCurvatureAnisotropicDiffusionImage::New()
{
  struct make_shared_enabler : public ITKCurvatureAnisotropicDiffusionImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKCurvatureAnisotropicDiffusionImage::getNameOfClass() const
{
  return QString("ITKCurvatureAnisotropicDiffusionImage");
}

// -----------------------------------------------------------------------------
QString ITKCurvatureAnisotropicDiffusionImage::ClassName()
{
  return QString("ITKCurvatureAnisotropicDiffusionImage");
}

// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::setTimeStep(double value)
{
  m_TimeStep = value;
}

// -----------------------------------------------------------------------------
double ITKCurvatureAnisotropicDiffusionImage::getTimeStep() const
{
  return m_TimeStep;
}

// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::setConductanceParameter(double value)
{
  m_ConductanceParameter = value;
}

// -----------------------------------------------------------------------------
double ITKCurvatureAnisotropicDiffusionImage::getConductanceParameter() const
{
  return m_ConductanceParameter;
}

// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::setConductanceScalingUpdateInterval(double value)
{
  m_ConductanceScalingUpdateInterval = value;
}

// -----------------------------------------------------------------------------
double ITKCurvatureAnisotropicDiffusionImage::getConductanceScalingUpdateInterval() const
{
  return m_ConductanceScalingUpdateInterval;
}

// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::setNumberOfIterations(double value)
{
  m_NumberOfIterations = value;
}

// -----------------------------------------------------------------------------
double ITKCurvatureAnisotropicDiffusionImage::getNumberOfIterations() const
{
  return m_NumberOfIterations;
}
