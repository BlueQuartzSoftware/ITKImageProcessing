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

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkRegionalMaximaImageFilter.h>


// Include the MOC generated file for this class
#include "moc_ITKRegionalMaximaImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRegionalMaximaImage::ITKRegionalMaximaImage() :
  ITKImageBase(),
m_BackgroundValue(0.0),
m_ForegroundValue(1.0),
m_FullyConnected(false),
m_FlatIsMaxima(true)
{
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
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
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
template<typename VarType, typename SubsType>
void ITKRegionalMaximaImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKRegionalMaximaImage::dataCheck()
{
  // Check consistency of parameters

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMaximaImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKRegionalMaximaImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::RegionalMaximaImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetBackgroundValue(static_cast<double>(m_BackgroundValue));
  filter->SetForegroundValue(static_cast<double>(m_ForegroundValue));
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  filter->SetFlatIsMaxima(static_cast<bool>(m_FlatIsMaxima));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRegionalMaximaImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
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
{ return "[ITK] ITK  Regional Maxima Image Filter (KW)"; }

