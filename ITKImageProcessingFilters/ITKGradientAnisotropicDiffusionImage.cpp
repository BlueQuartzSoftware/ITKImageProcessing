// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include <memory>

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKGradientAnisotropicDiffusionImage.h"
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
ITKGradientAnisotropicDiffusionImage::ITKGradientAnisotropicDiffusionImage()
{
  m_TimeStep = StaticCastScalar<double, double, double>(0.125);
  m_ConductanceParameter = StaticCastScalar<double, double, double>(3);
  m_ConductanceScalingUpdateInterval = StaticCastScalar<double, double, double>(1u);
  m_NumberOfIterations = StaticCastScalar<double, double, double>(5u);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGradientAnisotropicDiffusionImage::~ITKGradientAnisotropicDiffusionImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TimeStep", TimeStep, FilterParameter::Parameter, ITKGradientAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ConductanceParameter", ConductanceParameter, FilterParameter::Parameter, ITKGradientAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ConductanceScalingUpdateInterval", ConductanceScalingUpdateInterval, FilterParameter::Parameter, ITKGradientAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Parameter, ITKGradientAnisotropicDiffusionImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKGradientAnisotropicDiffusionImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKGradientAnisotropicDiffusionImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKGradientAnisotropicDiffusionImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setTimeStep(reader->readValue("TimeStep", getTimeStep()));
  setConductanceParameter(reader->readValue("ConductanceParameter", getConductanceParameter()));
  setConductanceScalingUpdateInterval(reader->readValue("ConductanceScalingUpdateInterval", getConductanceScalingUpdateInterval()));
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKGradientAnisotropicDiffusionImage::dataCheckImpl()
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
void ITKGradientAnisotropicDiffusionImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKGradientAnisotropicDiffusionImage::filter()
{
  typedef typename itk::NumericTraits<InputPixelType>::RealType FloatPixelType;
  typedef itk::Image<FloatPixelType, Dimension> FloatImageType;
  typedef itk::GradientAnisotropicDiffusionImageFilter<FloatImageType, FloatImageType> FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTimeStep(static_cast<double>(m_TimeStep));
  filter->SetConductanceParameter(static_cast<double>(m_ConductanceParameter));
  filter->SetConductanceScalingUpdateInterval(static_cast<unsigned int>(m_ConductanceScalingUpdateInterval));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  this->ITKImageProcessingBase::filterCastToFloat<InputPixelType, OutputPixelType, Dimension, FilterType, FloatImageType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKGradientAnisotropicDiffusionImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKGradientAnisotropicDiffusionImage::Pointer filter = ITKGradientAnisotropicDiffusionImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKGradientAnisotropicDiffusionImage::getHumanLabel() const
{
  return "ITK::Gradient Anisotropic Diffusion Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKGradientAnisotropicDiffusionImage::getUuid() const
{
  return QUuid("{98d0b50b-9639-53a0-9e30-2fae6f7ab869}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKGradientAnisotropicDiffusionImage::getSubGroupName() const
{
  return "ITK AnisotropicSmoothing";
}

// -----------------------------------------------------------------------------
ITKGradientAnisotropicDiffusionImage::Pointer ITKGradientAnisotropicDiffusionImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKGradientAnisotropicDiffusionImage> ITKGradientAnisotropicDiffusionImage::New()
{
  struct make_shared_enabler : public ITKGradientAnisotropicDiffusionImage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKGradientAnisotropicDiffusionImage::getNameOfClass() const
{
  return QString("ITKGradientAnisotropicDiffusionImage");
}

// -----------------------------------------------------------------------------
QString ITKGradientAnisotropicDiffusionImage::ClassName()
{
  return QString("ITKGradientAnisotropicDiffusionImage");
}

// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::setTimeStep(double value)
{
  m_TimeStep = value;
}

// -----------------------------------------------------------------------------
double ITKGradientAnisotropicDiffusionImage::getTimeStep() const
{
  return m_TimeStep;
}

// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::setConductanceParameter(double value)
{
  m_ConductanceParameter = value;
}

// -----------------------------------------------------------------------------
double ITKGradientAnisotropicDiffusionImage::getConductanceParameter() const
{
  return m_ConductanceParameter;
}

// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::setConductanceScalingUpdateInterval(double value)
{
  m_ConductanceScalingUpdateInterval = value;
}

// -----------------------------------------------------------------------------
double ITKGradientAnisotropicDiffusionImage::getConductanceScalingUpdateInterval() const
{
  return m_ConductanceScalingUpdateInterval;
}

// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::setNumberOfIterations(double value)
{
  m_NumberOfIterations = value;
}

// -----------------------------------------------------------------------------
double ITKGradientAnisotropicDiffusionImage::getNumberOfIterations() const
{
  return m_NumberOfIterations;
}


