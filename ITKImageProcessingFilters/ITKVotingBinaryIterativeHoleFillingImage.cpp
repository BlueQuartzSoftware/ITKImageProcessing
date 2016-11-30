// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKVotingBinaryIterativeHoleFillingImage.h"

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
#include <itkVotingBinaryIterativeHoleFillingImageFilter.h>


// Include the MOC generated file for this class
#include "moc_ITKVotingBinaryIterativeHoleFillingImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVotingBinaryIterativeHoleFillingImage::ITKVotingBinaryIterativeHoleFillingImage() :
  ITKImageBase(),
m_Radius(std::vector<unsigned int>(3, 1)),
m_MaximumNumberOfIterations(10u),
m_MajorityThreshold(1u),
m_ForegroundValue(1.0),
m_BackgroundValue(0.0)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVotingBinaryIterativeHoleFillingImage::~ITKVotingBinaryIterativeHoleFillingImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVotingBinaryIterativeHoleFillingImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Radius", Radius, FilterParameter::Parameter, ITKVotingBinaryIterativeHoleFillingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumNumberOfIterations", MaximumNumberOfIterations, FilterParameter::Parameter, ITKVotingBinaryIterativeHoleFillingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MajorityThreshold", MajorityThreshold, FilterParameter::Parameter, ITKVotingBinaryIterativeHoleFillingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ForegroundValue", ForegroundValue, FilterParameter::Parameter, ITKVotingBinaryIterativeHoleFillingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("BackgroundValue", BackgroundValue, FilterParameter::Parameter, ITKVotingBinaryIterativeHoleFillingImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKVotingBinaryIterativeHoleFillingImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKVotingBinaryIterativeHoleFillingImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKVotingBinaryIterativeHoleFillingImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVotingBinaryIterativeHoleFillingImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setRadius(reader->readValue("Radius", getRadius()));
  setMaximumNumberOfIterations(reader->readValue("MaximumNumberOfIterations", getMaximumNumberOfIterations()));
  setMajorityThreshold(reader->readValue("MajorityThreshold", getMajorityThreshold()));
  setForegroundValue(reader->readValue("ForegroundValue", getForegroundValue()));
  setBackgroundValue(reader->readValue("BackgroundValue", getBackgroundValue()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKVotingBinaryIterativeHoleFillingImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKVotingBinaryIterativeHoleFillingImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<unsigned int,double>(m_Radius, "Radius");
  CheckIntegerEntry<unsigned int,double>(m_MaximumNumberOfIterations, "MaximumNumberOfIterations");
  CheckIntegerEntry<unsigned int,double>(m_MajorityThreshold, "MajorityThreshold");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVotingBinaryIterativeHoleFillingImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKVotingBinaryIterativeHoleFillingImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::VotingBinaryIterativeHoleFillingImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetRadius(static_cast<unsigned int>(m_Radius));
  filter->SetMaximumNumberOfIterations(static_cast<unsigned int>(m_MaximumNumberOfIterations));
  filter->SetMajorityThreshold(static_cast<unsigned int>(m_MajorityThreshold));
  filter->SetForegroundValue(static_cast<double>(m_ForegroundValue));
  filter->SetBackgroundValue(static_cast<double>(m_BackgroundValue));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVotingBinaryIterativeHoleFillingImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKVotingBinaryIterativeHoleFillingImage::newFilterInstance(bool copyFilterParameters)
{
  ITKVotingBinaryIterativeHoleFillingImage::Pointer filter = ITKVotingBinaryIterativeHoleFillingImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKVotingBinaryIterativeHoleFillingImage::getHumanLabel()
{ return "[ITK] ITK  Voting Binary Iterative Hole Filling Image Filter (KW)"; }

