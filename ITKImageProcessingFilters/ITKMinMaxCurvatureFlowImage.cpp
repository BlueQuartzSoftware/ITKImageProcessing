// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKMinMaxCurvatureFlowImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"



#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

// Include the MOC generated file for this class
#include "moc_ITKMinMaxCurvatureFlowImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMinMaxCurvatureFlowImage::ITKMinMaxCurvatureFlowImage() :
  ITKImageBase()
{
  m_TimeStep=StaticCastScalar<double,double,double>(0.05);
  m_NumberOfIterations=StaticCastScalar<double,double,double>(5u);
  m_StencilRadius=StaticCastScalar<int,int,int>(2);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMinMaxCurvatureFlowImage::~ITKMinMaxCurvatureFlowImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TimeStep", TimeStep, FilterParameter::Parameter, ITKMinMaxCurvatureFlowImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Parameter, ITKMinMaxCurvatureFlowImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("StencilRadius", StencilRadius, FilterParameter::Parameter, ITKMinMaxCurvatureFlowImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKMinMaxCurvatureFlowImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKMinMaxCurvatureFlowImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKMinMaxCurvatureFlowImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setTimeStep(reader->readValue("TimeStep", getTimeStep()));
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));
  setStencilRadius(reader->readValue("StencilRadius", getStencilRadius()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMinMaxCurvatureFlowImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t,double>(m_NumberOfIterations, "NumberOfIterations",1);

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMinMaxCurvatureFlowImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMinMaxCurvatureFlowImage::filter()
{
  typedef typename itk::NumericTraits<InputPixelType>::RealType                 FloatPixelType;
  typedef itk::Dream3DImage< FloatPixelType, Dimension >                        FloatImageType;
  typedef itk::MinMaxCurvatureFlowImageFilter< FloatImageType, FloatImageType > FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTimeStep(static_cast<double>(m_TimeStep));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  filter->SetStencilRadius(static_cast<int>(m_StencilRadius));

  this->ITKImageBase::filterCastToFloat< InputPixelType, InputPixelType, Dimension, FilterType, FloatImageType >(filter);
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
AbstractFilter::Pointer ITKMinMaxCurvatureFlowImage::newFilterInstance(bool copyFilterParameters)
{
  ITKMinMaxCurvatureFlowImage::Pointer filter = ITKMinMaxCurvatureFlowImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMinMaxCurvatureFlowImage::getHumanLabel()
{ return "ITK::Min Max Curvature Flow Image Filter"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMinMaxCurvatureFlowImage::getSubGroupName()
{ return "ITK Smoothing"; }


