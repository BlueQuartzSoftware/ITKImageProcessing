// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKDiscreteGaussianImage.h"

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
#include "moc_ITKDiscreteGaussianImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDiscreteGaussianImage::ITKDiscreteGaussianImage() :
  ITKImageBase()
{
  m_Variance=StaticCastScalar<double,double,double>(1.0);
  m_MaximumKernelWidth=StaticCastScalar<double,double,double>(32u);
  m_MaximumError=StaticCastScalar<double,double,double>(0.01);
  m_UseImageSpacing=StaticCastScalar<bool,bool,bool>(true);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDiscreteGaussianImage::~ITKDiscreteGaussianImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Variance", Variance, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumKernelWidth", MaximumKernelWidth, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumError", MaximumError, FilterParameter::Parameter, ITKDiscreteGaussianImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("UseImageSpacing", UseImageSpacing, FilterParameter::Parameter, ITKDiscreteGaussianImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKDiscreteGaussianImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKDiscreteGaussianImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKDiscreteGaussianImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setVariance(reader->readValue("Variance", getVariance()));
  setMaximumKernelWidth(reader->readValue("MaximumKernelWidth", getMaximumKernelWidth()));
  setMaximumError(reader->readValue("MaximumError", getMaximumError()));
  setUseImageSpacing(reader->readValue("UseImageSpacing", getUseImageSpacing()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKDiscreteGaussianImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<unsigned int,double>(m_MaximumKernelWidth, "MaximumKernelWidth",1);

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKDiscreteGaussianImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef itk::DiscreteGaussianImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetVariance(static_cast<double>(m_Variance));
  filter->SetMaximumKernelWidth(static_cast<unsigned int>(m_MaximumKernelWidth));
  filter->SetMaximumError(static_cast<double>(m_MaximumError));
  filter->SetUseImageSpacing(static_cast<bool>(m_UseImageSpacing));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianImage::filterInternal()
{
    Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKDiscreteGaussianImage::newFilterInstance(bool copyFilterParameters)
{
  ITKDiscreteGaussianImage::Pointer filter = ITKDiscreteGaussianImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKDiscreteGaussianImage::getHumanLabel()
{ return "ITK::Discrete Gaussian Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKDiscreteGaussianImage::getSubGroupName()
{ return "ITK Smoothing"; }


