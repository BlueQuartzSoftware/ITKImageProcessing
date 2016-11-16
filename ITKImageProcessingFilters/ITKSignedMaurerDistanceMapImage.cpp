// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKSignedMaurerDistanceMapImage.h"

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
#include "moc_ITKSignedMaurerDistanceMapImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSignedMaurerDistanceMapImage::ITKSignedMaurerDistanceMapImage() :
  ITKImageBase()
{
  m_InsideIsPositive=StaticCastScalar<bool,bool,bool>(false);
  m_SquaredDistance=StaticCastScalar<bool,bool,bool>(true);
  m_UseImageSpacing=StaticCastScalar<bool,bool,bool>(false);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSignedMaurerDistanceMapImage::~ITKSignedMaurerDistanceMapImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_BOOL_FP("InsideIsPositive", InsideIsPositive, FilterParameter::Parameter, ITKSignedMaurerDistanceMapImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("SquaredDistance", SquaredDistance, FilterParameter::Parameter, ITKSignedMaurerDistanceMapImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("UseImageSpacing", UseImageSpacing, FilterParameter::Parameter, ITKSignedMaurerDistanceMapImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKSignedMaurerDistanceMapImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKSignedMaurerDistanceMapImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKSignedMaurerDistanceMapImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setInsideIsPositive(reader->readValue("InsideIsPositive", getInsideIsPositive()));
  setSquaredDistance(reader->readValue("SquaredDistance", getSquaredDistance()));
  setUseImageSpacing(reader->readValue("UseImageSpacing", getUseImageSpacing()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSignedMaurerDistanceMapImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4,float,0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKSignedMaurerDistanceMapImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef itk::SignedMaurerDistanceMapImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInsideIsPositive(static_cast<bool>(m_InsideIsPositive));
  filter->SetSquaredDistance(static_cast<bool>(m_SquaredDistance));
  filter->SetUseImageSpacing(static_cast<bool>(m_UseImageSpacing));
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSignedMaurerDistanceMapImage::filterInternal()
{
    Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4,float,0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKSignedMaurerDistanceMapImage::newFilterInstance(bool copyFilterParameters)
{
  ITKSignedMaurerDistanceMapImage::Pointer filter = ITKSignedMaurerDistanceMapImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKSignedMaurerDistanceMapImage::getHumanLabel()
{ return "[ITK] Signed Maurer Distance Map Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKSignedMaurerDistanceMapImage::getSubGroupName()
{ return "ITKDistanceMap"; }


