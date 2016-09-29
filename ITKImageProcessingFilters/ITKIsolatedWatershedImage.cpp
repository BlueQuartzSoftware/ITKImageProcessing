// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKIsolatedWatershedImage.h"

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
#include <itkIsolatedWatershedImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKIsolatedWatershedImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIsolatedWatershedImage::ITKIsolatedWatershedImage() :
  ITKImageBase(),
m_Seed1(std::vector<uint32_t>(3, 0)),
m_Seed2(std::vector<uint32_t>(3, 0)),
m_Threshold(0.0),
m_UpperValueLimit(1.0),
m_IsolatedValueTolerance(0.001),
m_ReplaceValue1(1u),
m_ReplaceValue2(2u)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIsolatedWatershedImage::~ITKIsolatedWatershedImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsolatedWatershedImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Seed1", Seed1, FilterParameter::Parameter, ITKIsolatedWatershedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Seed2", Seed2, FilterParameter::Parameter, ITKIsolatedWatershedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Threshold", Threshold, FilterParameter::Parameter, ITKIsolatedWatershedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("UpperValueLimit", UpperValueLimit, FilterParameter::Parameter, ITKIsolatedWatershedImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("IsolatedValueTolerance", IsolatedValueTolerance, FilterParameter::Parameter, ITKIsolatedWatershedImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("ReplaceValue1", ReplaceValue1, FilterParameter::Parameter, ITKIsolatedWatershedImage));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("ReplaceValue2", ReplaceValue2, FilterParameter::Parameter, ITKIsolatedWatershedImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKIsolatedWatershedImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKIsolatedWatershedImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKIsolatedWatershedImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsolatedWatershedImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setSeed1(reader->readValue("Seed1", getSeed1()));
  setSeed2(reader->readValue("Seed2", getSeed2()));
  setThreshold(reader->readValue("Threshold", getThreshold()));
  setUpperValueLimit(reader->readValue("UpperValueLimit", getUpperValueLimit()));
  setIsolatedValueTolerance(reader->readValue("IsolatedValueTolerance", getIsolatedValueTolerance()));
  setReplaceValue1(reader->readValue("ReplaceValue1", getReplaceValue1()));
  setReplaceValue2(reader->readValue("ReplaceValue2", getReplaceValue2()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKIsolatedWatershedImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKIsolatedWatershedImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<uint32_t,double>(m_Seed1, "Seed1");
  CheckIntegerEntry<uint32_t,double>(m_Seed2, "Seed2");
  CheckIntegerEntry<uint8_t,int>(m_ReplaceValue1, "ReplaceValue1");
  CheckIntegerEntry<uint8_t,int>(m_ReplaceValue2, "ReplaceValue2");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsolatedWatershedImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKIsolatedWatershedImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::IsolatedWatershedImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSeed1(static_cast<uint32_t>(m_Seed1));
  filter->SetSeed2(static_cast<uint32_t>(m_Seed2));
  filter->SetThreshold(static_cast<double>(m_Threshold));
  filter->SetUpperValueLimit(static_cast<double>(m_UpperValueLimit));
  filter->SetIsolatedValueTolerance(static_cast<double>(m_IsolatedValueTolerance));
  filter->SetReplaceValue1(static_cast<uint8_t>(m_ReplaceValue1));
  filter->SetReplaceValue2(static_cast<uint8_t>(m_ReplaceValue2));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsolatedWatershedImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKIsolatedWatershedImage::newFilterInstance(bool copyFilterParameters)
{
  ITKIsolatedWatershedImage::Pointer filter = ITKIsolatedWatershedImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKIsolatedWatershedImage::getHumanLabel()
{ return "[ITK] ITK  Isolated Watershed Image Filter (KW)"; }

