// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKVectorConfidenceConnectedImage.h"

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
#include <itkVectorConfidenceConnectedImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKVectorConfidenceConnectedImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVectorConfidenceConnectedImage::ITKVectorConfidenceConnectedImage() :
  ITKImageBase(),
m_NumberOfIterations(4u),
m_Multiplier(4.5),
m_InitialNeighborhoodRadius(1u),
m_ReplaceValue(1u)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVectorConfidenceConnectedImage::~ITKVectorConfidenceConnectedImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorConfidenceConnectedImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfIterations", NumberOfIterations, FilterParameter::Parameter, ITKVectorConfidenceConnectedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Multiplier", Multiplier, FilterParameter::Parameter, ITKVectorConfidenceConnectedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("InitialNeighborhoodRadius", InitialNeighborhoodRadius, FilterParameter::Parameter, ITKVectorConfidenceConnectedImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("ReplaceValue", ReplaceValue, FilterParameter::Parameter, ITKVectorConfidenceConnectedImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKVectorConfidenceConnectedImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKVectorConfidenceConnectedImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKVectorConfidenceConnectedImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorConfidenceConnectedImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setNumberOfIterations(reader->readValue("NumberOfIterations", getNumberOfIterations()));
  setMultiplier(reader->readValue("Multiplier", getMultiplier()));
  setInitialNeighborhoodRadius(reader->readValue("InitialNeighborhoodRadius", getInitialNeighborhoodRadius()));
  setReplaceValue(reader->readValue("ReplaceValue", getReplaceValue()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKVectorConfidenceConnectedImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKVectorConfidenceConnectedImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<unsigned int,double>(m_NumberOfIterations, "NumberOfIterations");
  CheckIntegerEntry<unsigned int,double>(m_InitialNeighborhoodRadius, "InitialNeighborhoodRadius");
  CheckIntegerEntry<uint8_t,int>(m_ReplaceValue, "ReplaceValue");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorConfidenceConnectedImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKVectorConfidenceConnectedImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::VectorConfidenceConnectedImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetNumberOfIterations(static_cast<unsigned int>(m_NumberOfIterations));
  filter->SetMultiplier(static_cast<double>(m_Multiplier));
  filter->SetInitialNeighborhoodRadius(static_cast<unsigned int>(m_InitialNeighborhoodRadius));
  filter->SetReplaceValue(static_cast<uint8_t>(m_ReplaceValue));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorConfidenceConnectedImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKVectorConfidenceConnectedImage::newFilterInstance(bool copyFilterParameters)
{
  ITKVectorConfidenceConnectedImage::Pointer filter = ITKVectorConfidenceConnectedImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKVectorConfidenceConnectedImage::getHumanLabel()
{ return "[ITK] ITK  Vector Confidence Connected Image Filter (KW)"; }

