// File automatically generated

/*
 * Your License or Copyright can go here
 */


#include "ITKImageProcessing/ITKImageProcessingFilters/ITKBinaryMinMaxCurvatureFlowImage.h"
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
ITKBinaryMinMaxCurvatureFlowImage::ITKBinaryMinMaxCurvatureFlowImage()
{
  m_TimeStep = StaticCastScalar<double, double, double>(0.05);
  m_NumberOfIterations = StaticCastScalar<double, double, double>(5u);
  m_StencilRadius = StaticCastScalar<int, int, int>(2);
  m_Threshold = StaticCastScalar<double, double, double>(0.0);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryMinMaxCurvatureFlowImage::~ITKBinaryMinMaxCurvatureFlowImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryMinMaxCurvatureFlowImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TimeStep", TimeStep, FilterParameter::Parameter, ITKBinaryMinMaxCurvatureFlowImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Parameter, ITKBinaryMinMaxCurvatureFlowImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("StencilRadius", StencilRadius, FilterParameter::Parameter, ITKBinaryMinMaxCurvatureFlowImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold", Threshold, FilterParameter::Parameter, ITKBinaryMinMaxCurvatureFlowImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKBinaryMinMaxCurvatureFlowImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKBinaryMinMaxCurvatureFlowImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKBinaryMinMaxCurvatureFlowImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryMinMaxCurvatureFlowImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setTimeStep(reader->readValue("TimeStep", getTimeStep()));
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));
  setStencilRadius(reader->readValue("StencilRadius", getStencilRadius()));
  setThreshold(reader->readValue("Threshold", getThreshold()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKBinaryMinMaxCurvatureFlowImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfIterations, "NumberOfIterations", 1);

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryMinMaxCurvatureFlowImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKBinaryMinMaxCurvatureFlowImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::BinaryMinMaxCurvatureFlowImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTimeStep(static_cast<double>(m_TimeStep));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  filter->SetStencilRadius(static_cast<int>(m_StencilRadius));
  filter->SetThreshold(static_cast<double>(m_Threshold));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryMinMaxCurvatureFlowImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKBinaryMinMaxCurvatureFlowImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKBinaryMinMaxCurvatureFlowImage::Pointer filter = ITKBinaryMinMaxCurvatureFlowImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBinaryMinMaxCurvatureFlowImage::getHumanLabel() const
{
  return "ITK::Binary Min Max Curvature Flow Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKBinaryMinMaxCurvatureFlowImage::getUuid() const
{
  return QUuid("{bf3d443b-27b4-53b1-8950-a83bca5374ec}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBinaryMinMaxCurvatureFlowImage::getSubGroupName() const
{
  return "ITK CurvatureFlow";
}

// -----------------------------------------------------------------------------
ITKBinaryMinMaxCurvatureFlowImage::Pointer ITKBinaryMinMaxCurvatureFlowImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKBinaryMinMaxCurvatureFlowImage> ITKBinaryMinMaxCurvatureFlowImage::New()
{
  struct make_shared_enabler : public ITKBinaryMinMaxCurvatureFlowImage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKBinaryMinMaxCurvatureFlowImage::getNameOfClass() const
{
  return QString("ITKBinaryMinMaxCurvatureFlowImage");
}

// -----------------------------------------------------------------------------
QString ITKBinaryMinMaxCurvatureFlowImage::ClassName()
{
  return QString("ITKBinaryMinMaxCurvatureFlowImage");
}

// -----------------------------------------------------------------------------
void ITKBinaryMinMaxCurvatureFlowImage::setTimeStep(double value)
{
  m_TimeStep = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryMinMaxCurvatureFlowImage::getTimeStep() const
{
  return m_TimeStep;
}

// -----------------------------------------------------------------------------
void ITKBinaryMinMaxCurvatureFlowImage::setNumberOfIterations(double value)
{
  m_NumberOfIterations = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryMinMaxCurvatureFlowImage::getNumberOfIterations() const
{
  return m_NumberOfIterations;
}

// -----------------------------------------------------------------------------
void ITKBinaryMinMaxCurvatureFlowImage::setStencilRadius(int value)
{
  m_StencilRadius = value;
}

// -----------------------------------------------------------------------------
int ITKBinaryMinMaxCurvatureFlowImage::getStencilRadius() const
{
  return m_StencilRadius;
}

// -----------------------------------------------------------------------------
void ITKBinaryMinMaxCurvatureFlowImage::setThreshold(double value)
{
  m_Threshold = value;
}

// -----------------------------------------------------------------------------
double ITKBinaryMinMaxCurvatureFlowImage::getThreshold() const
{
  return m_Threshold;
}


