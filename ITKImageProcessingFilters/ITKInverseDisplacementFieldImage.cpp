// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKInverseDisplacementFieldImage.h"

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
#include <itkInverseDisplacementFieldImageFilter.h>
#include <itkVector.h>


// Include the MOC generated file for this class
#include "moc_ITKInverseDisplacementFieldImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKInverseDisplacementFieldImage::ITKInverseDisplacementFieldImage() :
  ITKImageBase(),
m_Size(std::vector<uint32_t>(3, 0)),
m_OutputOrigin(std::vector<double>(3, 0.0)),
m_OutputSpacing(std::vector<double>(3, 1.0)),
m_SubsamplingFactor(16u)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKInverseDisplacementFieldImage::~ITKInverseDisplacementFieldImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKInverseDisplacementFieldImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Size", Size, FilterParameter::Parameter, ITKInverseDisplacementFieldImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputOrigin", OutputOrigin, FilterParameter::Parameter, ITKInverseDisplacementFieldImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputSpacing", OutputSpacing, FilterParameter::Parameter, ITKInverseDisplacementFieldImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("SubsamplingFactor", SubsamplingFactor, FilterParameter::Parameter, ITKInverseDisplacementFieldImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKInverseDisplacementFieldImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKInverseDisplacementFieldImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKInverseDisplacementFieldImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKInverseDisplacementFieldImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setSize(reader->readValue("Size", getSize()));
  setOutputOrigin(reader->readValue("OutputOrigin", getOutputOrigin()));
  setOutputSpacing(reader->readValue("OutputSpacing", getOutputSpacing()));
  setSubsamplingFactor(reader->readValue("SubsamplingFactor", getSubsamplingFactor()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKInverseDisplacementFieldImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKInverseDisplacementFieldImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<uint32_t,double>(m_Size, "Size");
  CheckIntegerEntry<unsigned int,double>(m_SubsamplingFactor, "SubsamplingFactor");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKInverseDisplacementFieldImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKInverseDisplacementFieldImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::InverseDisplacementFieldImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSize(static_cast<uint32_t>(m_Size));
  filter->SetOutputOrigin(static_cast<double>(m_OutputOrigin));
  filter->SetOutputSpacing(static_cast<double>(m_OutputSpacing));
  filter->SetSubsamplingFactor(static_cast<unsigned int>(m_SubsamplingFactor));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKInverseDisplacementFieldImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKInverseDisplacementFieldImage::newFilterInstance(bool copyFilterParameters)
{
  ITKInverseDisplacementFieldImage::Pointer filter = ITKInverseDisplacementFieldImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKInverseDisplacementFieldImage::getHumanLabel()
{ return "[ITK] ITK  Inverse Displacement Field Image Filter (KW)"; }

