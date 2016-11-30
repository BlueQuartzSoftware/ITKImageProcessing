// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKFastMarchingUpwindGradientImage.h"

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
#include <itkFastMarchingUpwindGradientImageFilter.h>


// Include the MOC generated file for this class
#include "moc_ITKFastMarchingUpwindGradientImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKFastMarchingUpwindGradientImage::ITKFastMarchingUpwindGradientImage() :
  ITKImageBase(),
m_TrialPoints(std::vector< std::vector<unsigned int > >()),
m_NumberOfTargets(0u),
m_TargetPoints(std::vector< std::vector<unsigned int > >()),
m_TargetOffset(1.0),
m_NormalizationFactor(1.0)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKFastMarchingUpwindGradientImage::~ITKFastMarchingUpwindGradientImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFastMarchingUpwindGradientImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TrialPoints", TrialPoints, FilterParameter::Parameter, ITKFastMarchingUpwindGradientImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfTargets", NumberOfTargets, FilterParameter::Parameter, ITKFastMarchingUpwindGradientImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TargetPoints", TargetPoints, FilterParameter::Parameter, ITKFastMarchingUpwindGradientImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("TargetOffset", TargetOffset, FilterParameter::Parameter, ITKFastMarchingUpwindGradientImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NormalizationFactor", NormalizationFactor, FilterParameter::Parameter, ITKFastMarchingUpwindGradientImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKFastMarchingUpwindGradientImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKFastMarchingUpwindGradientImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKFastMarchingUpwindGradientImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFastMarchingUpwindGradientImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setTrialPoints(reader->readValue("TrialPoints", getTrialPoints()));
  setNumberOfTargets(reader->readValue("NumberOfTargets", getNumberOfTargets()));
  setTargetPoints(reader->readValue("TargetPoints", getTargetPoints()));
  setTargetOffset(reader->readValue("TargetOffset", getTargetOffset()));
  setNormalizationFactor(reader->readValue("NormalizationFactor", getNormalizationFactor()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKFastMarchingUpwindGradientImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKFastMarchingUpwindGradientImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<unsigned int,double>(m_TrialPoints, "TrialPoints");
  CheckIntegerEntry<unsigned int,double>(m_NumberOfTargets, "NumberOfTargets");
  CheckIntegerEntry<unsigned int,double>(m_TargetPoints, "TargetPoints");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFastMarchingUpwindGradientImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKFastMarchingUpwindGradientImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::FastMarchingUpwindGradientImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetTrialPoints(static_cast<unsigned int>(m_TrialPoints));
  filter->SetNumberOfTargets(static_cast<unsigned int>(m_NumberOfTargets));
  filter->SetTargetPoints(static_cast<unsigned int>(m_TargetPoints));
  filter->SetTargetOffset(static_cast<double>(m_TargetOffset));
  filter->SetNormalizationFactor(static_cast<double>(m_NormalizationFactor));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKFastMarchingUpwindGradientImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKFastMarchingUpwindGradientImage::newFilterInstance(bool copyFilterParameters)
{
  ITKFastMarchingUpwindGradientImage::Pointer filter = ITKFastMarchingUpwindGradientImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKFastMarchingUpwindGradientImage::getHumanLabel()
{ return "[ITK] ITK  Fast Marching Upwind Gradient Image Filter (KW)"; }

