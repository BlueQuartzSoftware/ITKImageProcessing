// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKSigmoidImage.h"

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
#include "moc_ITKSigmoidImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSigmoidImage::ITKSigmoidImage() :
  ITKImageBase()
{
  m_Alpha=StaticCastScalar<double,double,double>(1);
  m_Beta=StaticCastScalar<double,double,double>(0);
  m_OutputMaximum=StaticCastScalar<double,double,double>(255);
  m_OutputMinimum=StaticCastScalar<double,double,double>(0);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSigmoidImage::~ITKSigmoidImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSigmoidImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Alpha", Alpha, FilterParameter::Parameter, ITKSigmoidImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Beta", Beta, FilterParameter::Parameter, ITKSigmoidImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMaximum", OutputMaximum, FilterParameter::Parameter, ITKSigmoidImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMinimum", OutputMinimum, FilterParameter::Parameter, ITKSigmoidImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKSigmoidImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKSigmoidImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKSigmoidImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSigmoidImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setAlpha(reader->readValue("Alpha", getAlpha()));
  setBeta(reader->readValue("Beta", getBeta()));
  setOutputMaximum(reader->readValue("OutputMaximum", getOutputMaximum()));
  setOutputMinimum(reader->readValue("OutputMinimum", getOutputMinimum()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSigmoidImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSigmoidImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSigmoidImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef itk::SigmoidImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetAlpha(static_cast<double>(m_Alpha));
  filter->SetBeta(static_cast<double>(m_Beta));
  filter->SetOutputMaximum(static_cast<double>(m_OutputMaximum));
  filter->SetOutputMinimum(static_cast<double>(m_OutputMinimum));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSigmoidImage::filterInternal()
{
    Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKSigmoidImage::newFilterInstance(bool copyFilterParameters)
{
  ITKSigmoidImage::Pointer filter = ITKSigmoidImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKSigmoidImage::getHumanLabel()
{ return "ITK::Sigmoid Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKSigmoidImage::getSubGroupName()
{ return "ITK ImageIntensity"; }


