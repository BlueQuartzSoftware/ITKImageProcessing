// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKZeroCrossingBasedEdgeDetectionImage.h"

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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkZeroCrossingBasedEdgeDetectionImageFilter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKZeroCrossingBasedEdgeDetectionImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKZeroCrossingBasedEdgeDetectionImage::ITKZeroCrossingBasedEdgeDetectionImage() :
  ITKImageBase(),
m_Variance(1.0),
m_ForegroundValue(1u),
m_BackgroundValue(0u),
m_MaximumError(0.1)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKZeroCrossingBasedEdgeDetectionImage::~ITKZeroCrossingBasedEdgeDetectionImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKZeroCrossingBasedEdgeDetectionImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Variance", Variance, FilterParameter::Parameter, ITKZeroCrossingBasedEdgeDetectionImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("ForegroundValue", ForegroundValue, FilterParameter::Parameter, ITKZeroCrossingBasedEdgeDetectionImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("BackgroundValue", BackgroundValue, FilterParameter::Parameter, ITKZeroCrossingBasedEdgeDetectionImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumError", MaximumError, FilterParameter::Parameter, ITKZeroCrossingBasedEdgeDetectionImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKZeroCrossingBasedEdgeDetectionImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKZeroCrossingBasedEdgeDetectionImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKZeroCrossingBasedEdgeDetectionImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKZeroCrossingBasedEdgeDetectionImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setVariance(reader->readValue("Variance", getVariance()));
  setForegroundValue(reader->readValue("ForegroundValue", getForegroundValue()));
  setBackgroundValue(reader->readValue("BackgroundValue", getBackgroundValue()));
  setMaximumError(reader->readValue("MaximumError", getMaximumError()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKZeroCrossingBasedEdgeDetectionImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKZeroCrossingBasedEdgeDetectionImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<uint8_t,int>(m_ForegroundValue, "ForegroundValue");
  CheckIntegerEntry<uint8_t,int>(m_BackgroundValue, "BackgroundValue");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKZeroCrossingBasedEdgeDetectionImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKZeroCrossingBasedEdgeDetectionImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::ZeroCrossingBasedEdgeDetectionImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetVariance(static_cast<double>(m_Variance));
  filter->SetForegroundValue(static_cast<uint8_t>(m_ForegroundValue));
  filter->SetBackgroundValue(static_cast<uint8_t>(m_BackgroundValue));
  filter->SetMaximumError(static_cast<double>(m_MaximumError));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKZeroCrossingBasedEdgeDetectionImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKZeroCrossingBasedEdgeDetectionImage::newFilterInstance(bool copyFilterParameters)
{
  ITKZeroCrossingBasedEdgeDetectionImage::Pointer filter = ITKZeroCrossingBasedEdgeDetectionImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKZeroCrossingBasedEdgeDetectionImage::getHumanLabel()
{ return "[ITK] ITK  Zero Crossing Based Edge Detection Image Filter (KW)"; }

