// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKInvertDisplacementFieldImage.h"

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
#include <itkInvertDisplacementFieldImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkVector.h>


// Include the MOC generated file for this class
#include "moc_ITKInvertDisplacementFieldImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKInvertDisplacementFieldImage::ITKInvertDisplacementFieldImage() :
  ITKImageBase(),
m_MaximumNumberOfIterations(10u),
m_MaxErrorToleranceThreshold(0.1),
m_MeanErrorToleranceThreshold(0.001),
m_EnforceBoundaryCondition(true)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKInvertDisplacementFieldImage::~ITKInvertDisplacementFieldImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKInvertDisplacementFieldImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumNumberOfIterations", MaximumNumberOfIterations, FilterParameter::Parameter, ITKInvertDisplacementFieldImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaxErrorToleranceThreshold", MaxErrorToleranceThreshold, FilterParameter::Parameter, ITKInvertDisplacementFieldImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MeanErrorToleranceThreshold", MeanErrorToleranceThreshold, FilterParameter::Parameter, ITKInvertDisplacementFieldImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("EnforceBoundaryCondition", EnforceBoundaryCondition, FilterParameter::Parameter, ITKInvertDisplacementFieldImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKInvertDisplacementFieldImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKInvertDisplacementFieldImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKInvertDisplacementFieldImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKInvertDisplacementFieldImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setMaximumNumberOfIterations(reader->readValue("MaximumNumberOfIterations", getMaximumNumberOfIterations()));
  setMaxErrorToleranceThreshold(reader->readValue("MaxErrorToleranceThreshold", getMaxErrorToleranceThreshold()));
  setMeanErrorToleranceThreshold(reader->readValue("MeanErrorToleranceThreshold", getMeanErrorToleranceThreshold()));
  setEnforceBoundaryCondition(reader->readValue("EnforceBoundaryCondition", getEnforceBoundaryCondition()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKInvertDisplacementFieldImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKInvertDisplacementFieldImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<uint32_t,double>(m_MaximumNumberOfIterations, "MaximumNumberOfIterations");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKInvertDisplacementFieldImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKInvertDisplacementFieldImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::InvertDisplacementFieldImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetMaximumNumberOfIterations(static_cast<uint32_t>(m_MaximumNumberOfIterations));
  filter->SetMaxErrorToleranceThreshold(static_cast<double>(m_MaxErrorToleranceThreshold));
  filter->SetMeanErrorToleranceThreshold(static_cast<double>(m_MeanErrorToleranceThreshold));
  filter->SetEnforceBoundaryCondition(static_cast<bool>(m_EnforceBoundaryCondition));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKInvertDisplacementFieldImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKInvertDisplacementFieldImage::newFilterInstance(bool copyFilterParameters)
{
  ITKInvertDisplacementFieldImage::Pointer filter = ITKInvertDisplacementFieldImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKInvertDisplacementFieldImage::getHumanLabel()
{ return "[ITK] ITK  Invert Displacement Field Image Filter (KW)"; }

