// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKGradientMagnitudeRecursiveGaussianImage.h"

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
#include "moc_ITKGradientMagnitudeRecursiveGaussianImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGradientMagnitudeRecursiveGaussianImage::ITKGradientMagnitudeRecursiveGaussianImage() :
  ITKImageBase()
{
  m_Sigma=StaticCastScalar<double,double,double>(1.0);
  m_NormalizeAcrossScale=StaticCastScalar<bool,bool,bool>(false);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGradientMagnitudeRecursiveGaussianImage::~ITKGradientMagnitudeRecursiveGaussianImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Sigma", Sigma, FilterParameter::Parameter, ITKGradientMagnitudeRecursiveGaussianImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("NormalizeAcrossScale", NormalizeAcrossScale, FilterParameter::Parameter, ITKGradientMagnitudeRecursiveGaussianImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKGradientMagnitudeRecursiveGaussianImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKGradientMagnitudeRecursiveGaussianImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKGradientMagnitudeRecursiveGaussianImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setSigma(reader->readValue("Sigma", getSigma()));
  setNormalizeAcrossScale(reader->readValue("NormalizeAcrossScale", getNormalizeAcrossScale()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKGradientMagnitudeRecursiveGaussianImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4,float,0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKGradientMagnitudeRecursiveGaussianImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSigma(static_cast<double>(m_Sigma));
  filter->SetNormalizeAcrossScale(static_cast<bool>(m_NormalizeAcrossScale));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGradientMagnitudeRecursiveGaussianImage::filterInternal()
{
    Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4,float,0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKGradientMagnitudeRecursiveGaussianImage::newFilterInstance(bool copyFilterParameters)
{
  ITKGradientMagnitudeRecursiveGaussianImage::Pointer filter = ITKGradientMagnitudeRecursiveGaussianImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGradientMagnitudeRecursiveGaussianImage::getHumanLabel()
{ return "ITK::Gradient Magnitude Recursive Gaussian Image Filter"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGradientMagnitudeRecursiveGaussianImage::getSubGroupName()
{ return "ITK Edge"; }


