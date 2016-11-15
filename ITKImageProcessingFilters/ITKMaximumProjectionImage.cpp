// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKMaximumProjectionImage.h"

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
#include "moc_ITKMaximumProjectionImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMaximumProjectionImage::ITKMaximumProjectionImage() :
  ITKImageBase()
{
  m_ProjectionDimension=StaticCastScalar<double,double,double>(0u);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMaximumProjectionImage::~ITKMaximumProjectionImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ProjectionDimension", ProjectionDimension, FilterParameter::Parameter, ITKMaximumProjectionImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKMaximumProjectionImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKMaximumProjectionImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKMaximumProjectionImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setProjectionDimension(reader->readValue("ProjectionDimension", getProjectionDimension()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType, unsigned int Dimension>
void ITKMaximumProjectionImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<unsigned int,double>(m_ProjectionDimension, "ProjectionDimension",1);

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKMaximumProjectionImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::MaximumProjectionImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetProjectionDimension(static_cast<unsigned int>(m_ProjectionDimension));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKMaximumProjectionImage::newFilterInstance(bool copyFilterParameters)
{
  ITKMaximumProjectionImage::Pointer filter = ITKMaximumProjectionImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMaximumProjectionImage::getHumanLabel()
{ return "[ITK] ITK  Maximum Projection Image Filter (KW)"; }
