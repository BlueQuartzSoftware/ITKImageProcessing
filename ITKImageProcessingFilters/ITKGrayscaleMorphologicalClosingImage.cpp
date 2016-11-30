// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKGrayscaleMorphologicalClosingImage.h"

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

//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkGrayscaleMorphologicalClosingImageFilter.h>


// Include the MOC generated file for this class
#include "moc_ITKGrayscaleMorphologicalClosingImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGrayscaleMorphologicalClosingImage::ITKGrayscaleMorphologicalClosingImage() :
  ITKImageBase(),
m_SafeBorder(true)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGrayscaleMorphologicalClosingImage::~ITKGrayscaleMorphologicalClosingImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_BOOL_FP("SafeBorder", SafeBorder, FilterParameter::Parameter, ITKGrayscaleMorphologicalClosingImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKGrayscaleMorphologicalClosingImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKGrayscaleMorphologicalClosingImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKGrayscaleMorphologicalClosingImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setSafeBorder(reader->readValue("SafeBorder", getSafeBorder()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKGrayscaleMorphologicalClosingImage::CheckIntegerEntry(SubsType value, QString name)
{
  if (value < static_cast<SubsType>(std::numeric_limits<VarType>::lowest())
     || value > static_cast<SubsType>(std::numeric_limits<VarType>::max())
     || value != floor(value))
  {
    setErrorCondition(-1);
    QString errorMessage = name + QString(" must be greater or equal than %1 and lesser or equal than %2");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(
        std::numeric_limits<VarType>::lowest()).arg(std::numeric_limits<VarType>::max())
                                                         , getErrorCondition()
                                                         );
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType, unsigned int Dimension>
void ITKGrayscaleMorphologicalClosingImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKGrayscaleMorphologicalClosingImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::GrayscaleMorphologicalClosingImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSafeBorder(static_cast<bool>(m_SafeBorder));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKGrayscaleMorphologicalClosingImage::newFilterInstance(bool copyFilterParameters)
{
  ITKGrayscaleMorphologicalClosingImage::Pointer filter = ITKGrayscaleMorphologicalClosingImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGrayscaleMorphologicalClosingImage::getHumanLabel()
{ return "[ITK] ITK  Grayscale Morphological Closing Image Filter (KW)"; }

