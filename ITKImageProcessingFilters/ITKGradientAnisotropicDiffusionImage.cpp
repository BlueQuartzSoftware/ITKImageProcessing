// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKGradientAnisotropicDiffusionImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

// Include the MOC generated file for this class
#include "moc_ITKGradientAnisotropicDiffusionImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGradientAnisotropicDiffusionImage::ITKGradientAnisotropicDiffusionImage() :
  ITKImageBase()
{
  m_TimeStep=StaticCastScalar<double,double,double>(0.125);
  m_ConductanceParameter=StaticCastScalar<double,double,double>(3);
  m_ConductanceScalingUpdateInterval=StaticCastScalar<double,double,double>(1u);
  m_NumberOfIterations=StaticCastScalar<double,double,double>(5u);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGradientAnisotropicDiffusionImage::~ITKGradientAnisotropicDiffusionImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::setupFilterParameters()
{
  FilterParameterVector parameters;

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
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
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
template<typename PixelType, unsigned int Dimension>
void ITKGradientAnisotropicDiffusionImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<unsigned int,double>(m_ConductanceScalingUpdateInterval, "ConductanceScalingUpdateInterval",1);
  this->CheckIntegerEntry<uint32_t,double>(m_NumberOfIterations, "NumberOfIterations",1);

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKGradientAnisotropicDiffusionImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTimeStep(static_cast<double>(m_TimeStep));
  filter->SetConductanceParameter(static_cast<double>(m_ConductanceParameter));
  filter->SetConductanceScalingUpdateInterval(static_cast<unsigned int>(m_ConductanceScalingUpdateInterval));
  filter->SetNumberOfIterations(static_cast<uint32_t>(m_NumberOfIterations));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientAnisotropicDiffusionImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKGradientAnisotropicDiffusionImage::newFilterInstance(bool copyFilterParameters)
{
  ITKGradientAnisotropicDiffusionImage::Pointer filter = ITKGradientAnisotropicDiffusionImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGradientAnisotropicDiffusionImage::getHumanLabel()
{ return "[ITK] ITK  Gradient Anisotropic Diffusion Image Filter (KW)"; }

