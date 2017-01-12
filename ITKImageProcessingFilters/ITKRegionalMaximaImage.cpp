// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKRegionalMaximaImage.h"

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
#include "moc_ITKRegionalMaximaImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRegionalMaximaImage::ITKRegionalMaximaImage() :
  ITKImageBase()
{
  m_BackgroundValue=StaticCastScalar<double,double,double>(0.0);
  m_ForegroundValue=StaticCastScalar<double,double,double>(1.0);
  m_FullyConnected=StaticCastScalar<bool,bool,bool>(false);
  m_FlatIsMaxima=StaticCastScalar<bool,bool,bool>(true);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRegionalMaximaImage::~ITKRegionalMaximaImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMaximaImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("BackgroundValue", BackgroundValue, FilterParameter::Parameter, ITKRegionalMaximaImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ForegroundValue", ForegroundValue, FilterParameter::Parameter, ITKRegionalMaximaImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Parameter, ITKRegionalMaximaImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FlatIsMaxima", FlatIsMaxima, FilterParameter::Parameter, ITKRegionalMaximaImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKRegionalMaximaImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKRegionalMaximaImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKRegionalMaximaImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMaximaImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setBackgroundValue(reader->readValue("BackgroundValue", getBackgroundValue()));
  setForegroundValue(reader->readValue("ForegroundValue", getForegroundValue()));
  setFullyConnected(reader->readValue("FullyConnected", getFullyConnected()));
  setFlatIsMaxima(reader->readValue("FlatIsMaxima", getFlatIsMaxima()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKRegionalMaximaImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMaximaImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4,uint32_t,0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKRegionalMaximaImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef itk::RegionalMaximaImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetBackgroundValue(static_cast<double>(m_BackgroundValue));
  filter->SetForegroundValue(static_cast<double>(m_ForegroundValue));
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  filter->SetFlatIsMaxima(static_cast<bool>(m_FlatIsMaxima));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMaximaImage::filterInternal()
{
    Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4,uint32_t,0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKRegionalMaximaImage::newFilterInstance(bool copyFilterParameters)
{
  ITKRegionalMaximaImage::Pointer filter = ITKRegionalMaximaImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRegionalMaximaImage::getHumanLabel()
{ return "ITK::Regional Maxima Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRegionalMaximaImage::getSubGroupName()
{ return "ITK Review"; }


