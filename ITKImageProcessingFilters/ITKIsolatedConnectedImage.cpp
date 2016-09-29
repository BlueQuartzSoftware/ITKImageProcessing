// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKIsolatedConnectedImage.h"

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
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkIsolatedConnectedImageFilter.h>


// Include the MOC generated file for this class
#include "moc_ITKIsolatedConnectedImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIsolatedConnectedImage::ITKIsolatedConnectedImage() :
  ITKImageBase(),
m_Seed1(std::vector<unsigned int>(3, 0)),
m_Seed2(std::vector<unsigned int>(3, 0)),
m_Lower(0),
m_Upper(1),
m_ReplaceValue(1u),
m_IsolatedValueTolerance(1.0),
m_FindUpperThreshold(true)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIsolatedConnectedImage::~ITKIsolatedConnectedImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsolatedConnectedImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Seed1", Seed1, FilterParameter::Parameter, ITKIsolatedConnectedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Seed2", Seed2, FilterParameter::Parameter, ITKIsolatedConnectedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Lower", Lower, FilterParameter::Parameter, ITKIsolatedConnectedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Upper", Upper, FilterParameter::Parameter, ITKIsolatedConnectedImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("ReplaceValue", ReplaceValue, FilterParameter::Parameter, ITKIsolatedConnectedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("IsolatedValueTolerance", IsolatedValueTolerance, FilterParameter::Parameter, ITKIsolatedConnectedImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FindUpperThreshold", FindUpperThreshold, FilterParameter::Parameter, ITKIsolatedConnectedImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKIsolatedConnectedImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKIsolatedConnectedImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKIsolatedConnectedImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsolatedConnectedImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setSeed1(reader->readValue("Seed1", getSeed1()));
  setSeed2(reader->readValue("Seed2", getSeed2()));
  setLower(reader->readValue("Lower", getLower()));
  setUpper(reader->readValue("Upper", getUpper()));
  setReplaceValue(reader->readValue("ReplaceValue", getReplaceValue()));
  setIsolatedValueTolerance(reader->readValue("IsolatedValueTolerance", getIsolatedValueTolerance()));
  setFindUpperThreshold(reader->readValue("FindUpperThreshold", getFindUpperThreshold()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKIsolatedConnectedImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKIsolatedConnectedImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<unsigned int,double>(m_Seed1, "Seed1");
  CheckIntegerEntry<unsigned int,double>(m_Seed2, "Seed2");
  CheckIntegerEntry<uint8_t,int>(m_ReplaceValue, "ReplaceValue");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsolatedConnectedImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKIsolatedConnectedImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::IsolatedConnectedImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSeed1(static_cast<unsigned int>(m_Seed1));
  filter->SetSeed2(static_cast<unsigned int>(m_Seed2));
  filter->SetLower(static_cast<double>(m_Lower));
  filter->SetUpper(static_cast<double>(m_Upper));
  filter->SetReplaceValue(static_cast<uint8_t>(m_ReplaceValue));
  filter->SetIsolatedValueTolerance(static_cast<double>(m_IsolatedValueTolerance));
  filter->SetFindUpperThreshold(static_cast<bool>(m_FindUpperThreshold));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsolatedConnectedImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKIsolatedConnectedImage::newFilterInstance(bool copyFilterParameters)
{
  ITKIsolatedConnectedImage::Pointer filter = ITKIsolatedConnectedImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKIsolatedConnectedImage::getHumanLabel()
{ return "[ITK] ITK  Isolated Connected Image Filter (KW)"; }

