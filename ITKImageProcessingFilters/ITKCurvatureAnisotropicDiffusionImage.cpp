// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKCurvatureAnisotropicDiffusionImage.h"

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
#include "moc_ITKCurvatureAnisotropicDiffusionImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCurvatureAnisotropicDiffusionImage::ITKCurvatureAnisotropicDiffusionImage() :
  ITKImageBase()
{
  m_TimeStep=StaticCastScalar<double,double,double>(0.0625);
  m_ConductanceParameter=StaticCastScalar<double,double,double>(3.0);
  m_ConductanceScalingUpdateInterval=StaticCastScalar<double,double,double>(1u);
  m_NumberOfIterations=StaticCastScalar<double,double,double>(5u);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCurvatureAnisotropicDiffusionImage::~ITKCurvatureAnisotropicDiffusionImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TimeStep", TimeStep, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ConductanceParameter", ConductanceParameter, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ConductanceScalingUpdateInterval", ConductanceScalingUpdateInterval, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKCurvatureAnisotropicDiffusionImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
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
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setTimeStep(reader->readValue("TimeStep", getTimeStep()));
  setConductanceParameter(reader->readValue("ConductanceParameter", getConductanceParameter()));
  setConductanceScalingUpdateInterval(reader->readValue("ConductanceScalingUpdateInterval", getConductanceScalingUpdateInterval()));
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKCurvatureAnisotropicDiffusionImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<unsigned int,double>(m_ConductanceScalingUpdateInterval, "ConductanceScalingUpdateInterval",1);
  this->CheckIntegerEntry<uint32_t,double>(m_NumberOfIterations, "NumberOfIterations",1);

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCurvatureAnisotropicDiffusionImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKCurvatureAnisotropicDiffusionImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef itk::CurvatureAnisotropicDiffusionImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTimeStep(static_cast<double>(m_TimeStep));
  filter->SetConductanceParameter(static_cast<double>(m_ConductanceParameter));
  filter->SetConductanceScalingUpdateInterval(static_cast<unsigned int>(m_ConductanceScalingUpdateInterval));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

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
AbstractFilter::Pointer ITKCurvatureAnisotropicDiffusionImage::newFilterInstance(bool copyFilterParameters)
{
  ITKCurvatureAnisotropicDiffusionImage::Pointer filter = ITKCurvatureAnisotropicDiffusionImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCurvatureAnisotropicDiffusionImage::getHumanLabel()
{ return "ITK::Curvature Anisotropic Diffusion Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCurvatureAnisotropicDiffusionImage::getSubGroupName()
{ return "ITK AnisotropicSmoothing"; }


