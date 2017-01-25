// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKAdaptiveHistogramEqualizationImage.h"

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
#include "moc_ITKAdaptiveHistogramEqualizationImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKAdaptiveHistogramEqualizationImage::ITKAdaptiveHistogramEqualizationImage() :
  ITKImageBase()
{
  m_Radius=CastStdToVec3<std::vector<unsigned int>,FloatVec3_t,float>(std::vector<unsigned int>(3, 5));
  m_Alpha=StaticCastScalar<float,float,float>(0.3f);
  m_Beta=StaticCastScalar<float,float,float>(0.3f);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKAdaptiveHistogramEqualizationImage::~ITKAdaptiveHistogramEqualizationImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Radius", Radius, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Alpha", Alpha, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Beta", Beta, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKAdaptiveHistogramEqualizationImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKAdaptiveHistogramEqualizationImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setRadius(reader->readFloatVec3("Radius", getRadius()));
  setAlpha(reader->readValue("Alpha", getAlpha()));
  setBeta(reader->readValue("Beta", getBeta()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKAdaptiveHistogramEqualizationImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckVectorEntry<unsigned int,FloatVec3_t>(m_Radius, "Radius",1);

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKAdaptiveHistogramEqualizationImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  //define filter
  typedef itk::AdaptiveHistogramEqualizationImageFilter<InputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetRadius(CastVec3ToITK<FloatVec3_t,typename FilterType::RadiusType,typename FilterType::RadiusType::SizeValueType>(m_Radius,FilterType::RadiusType::Dimension));
  filter->SetAlpha(static_cast<float>(m_Alpha));
  filter->SetBeta(static_cast<float>(m_Beta));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::filterInternal()
{
    Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKAdaptiveHistogramEqualizationImage::newFilterInstance(bool copyFilterParameters)
{
  ITKAdaptiveHistogramEqualizationImage::Pointer filter = ITKAdaptiveHistogramEqualizationImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKAdaptiveHistogramEqualizationImage::getHumanLabel()
{ return "ITK::Adaptive Histogram Equalization Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKAdaptiveHistogramEqualizationImage::getSubGroupName()
{ return "ITK IntensityTransformation"; }


