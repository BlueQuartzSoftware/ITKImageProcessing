// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKCurvatureFlowImage.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/itkDream3DImage.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCurvatureFlowImage::ITKCurvatureFlowImage()
{
  m_TimeStep = StaticCastScalar<double, double, double>(0.05);
  m_NumberOfIterations = StaticCastScalar<double, double, double>(5u);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCurvatureFlowImage::~ITKCurvatureFlowImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureFlowImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TimeStep", TimeStep, FilterParameter::Parameter, ITKCurvatureFlowImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Parameter, ITKCurvatureFlowImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKCurvatureFlowImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKCurvatureFlowImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKCurvatureFlowImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureFlowImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setTimeStep(reader->readValue("TimeStep", getTimeStep()));
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKCurvatureFlowImage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfIterations, "NumberOfIterations", true);

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureFlowImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4,typename itk::NumericTraits<typename InputImageType::PixelType>::RealType, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKCurvatureFlowImage::filter()
{
  typedef typename itk::NumericTraits<InputPixelType>::RealType FloatPixelType;
  typedef itk::Dream3DImage<FloatPixelType, Dimension> FloatImageType;
  typedef itk::CurvatureFlowImageFilter<FloatImageType, FloatImageType> FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTimeStep(static_cast<double>(m_TimeStep));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  this->ITKImageProcessingBase::filterCastToFloat<InputPixelType, InputPixelType, Dimension, FilterType, FloatImageType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureFlowImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4,typename itk::NumericTraits<typename InputImageType::PixelType>::RealType, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKCurvatureFlowImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKCurvatureFlowImage::Pointer filter = ITKCurvatureFlowImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCurvatureFlowImage::getHumanLabel() const
{
  return "ITK::Curvature Flow Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKCurvatureFlowImage::getUuid()
{
  return QUuid("{653f26dd-a5ef-5c75-b6f6-bc096268f25e}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCurvatureFlowImage::getSubGroupName() const
{
  return "ITK CurvatureFlow";
}
