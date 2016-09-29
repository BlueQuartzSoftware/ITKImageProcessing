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

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

//Auto includes
#include <itkAdaptiveHistogramEqualizationImageFilter.h>
#include <SIMPLib/FilterParameters/FloatFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKAdaptiveHistogramEqualizationImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKAdaptiveHistogramEqualizationImage::ITKAdaptiveHistogramEqualizationImage() :
  ITKImageBase(),
m_Radius(std::vector<unsigned int>(3, 5)),
m_Alpha(0.3f),
m_Beta(0.3f),
m_UseLookupTable(false)
{
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

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Radius", Radius, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Alpha", Alpha, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Beta", Beta, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("UseLookupTable", UseLookupTable, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKAdaptiveHistogramEqualizationImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
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
  setRadius(reader->readValue("Radius", getRadius()));
  setAlpha(reader->readValue("Alpha", getAlpha()));
  setBeta(reader->readValue("Beta", getBeta()));
  setUseLookupTable(reader->readValue("UseLookupTable", getUseLookupTable()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKAdaptiveHistogramEqualizationImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKAdaptiveHistogramEqualizationImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<unsigned int,double>(m_Radius, "Radius");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKAdaptiveHistogramEqualizationImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::AdaptiveHistogramEqualizationImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetRadius(static_cast<unsigned int>(m_Radius));
  filter->SetAlpha(static_cast<float>(m_Alpha));
  filter->SetBeta(static_cast<float>(m_Beta));
  filter->SetUseLookupTable(static_cast<bool>(m_UseLookupTable));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKAdaptiveHistogramEqualizationImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
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
{ return "[ITK] ITK  Adaptive Histogram Equalization Image Filter (KW)"; }

