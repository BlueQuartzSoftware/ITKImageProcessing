// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKCollidingFrontsImage.h"

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
#include <itkCollidingFrontsImageFilter.h>


// Include the MOC generated file for this class
#include "moc_ITKCollidingFrontsImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCollidingFrontsImage::ITKCollidingFrontsImage() :
  ITKImageBase(),
m_SeedPoints1(std::vector< std::vector<unsigned int > >()),
m_SeedPoints2(std::vector< std::vector<unsigned int > >()),
m_ApplyConnectivity(true),
m_NegativeEpsilon(-1e-6),
m_StopOnTargets(false)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCollidingFrontsImage::~ITKCollidingFrontsImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCollidingFrontsImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("SeedPoints1", SeedPoints1, FilterParameter::Parameter, ITKCollidingFrontsImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("SeedPoints2", SeedPoints2, FilterParameter::Parameter, ITKCollidingFrontsImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("ApplyConnectivity", ApplyConnectivity, FilterParameter::Parameter, ITKCollidingFrontsImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NegativeEpsilon", NegativeEpsilon, FilterParameter::Parameter, ITKCollidingFrontsImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("StopOnTargets", StopOnTargets, FilterParameter::Parameter, ITKCollidingFrontsImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKCollidingFrontsImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKCollidingFrontsImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKCollidingFrontsImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCollidingFrontsImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setSeedPoints1(reader->readValue("SeedPoints1", getSeedPoints1()));
  setSeedPoints2(reader->readValue("SeedPoints2", getSeedPoints2()));
  setApplyConnectivity(reader->readValue("ApplyConnectivity", getApplyConnectivity()));
  setNegativeEpsilon(reader->readValue("NegativeEpsilon", getNegativeEpsilon()));
  setStopOnTargets(reader->readValue("StopOnTargets", getStopOnTargets()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKCollidingFrontsImage::CheckIntegerEntry(SubsType value, QString name)
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
void ITKCollidingFrontsImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<unsigned int,double>(m_SeedPoints1, "SeedPoints1");
  CheckIntegerEntry<unsigned int,double>(m_SeedPoints2, "SeedPoints2");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCollidingFrontsImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKCollidingFrontsImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::CollidingFrontsImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSeedPoints1(static_cast<unsigned int>(m_SeedPoints1));
  filter->SetSeedPoints2(static_cast<unsigned int>(m_SeedPoints2));
  filter->SetApplyConnectivity(static_cast<bool>(m_ApplyConnectivity));
  filter->SetNegativeEpsilon(static_cast<double>(m_NegativeEpsilon));
  filter->SetStopOnTargets(static_cast<bool>(m_StopOnTargets));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCollidingFrontsImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKCollidingFrontsImage::newFilterInstance(bool copyFilterParameters)
{
  ITKCollidingFrontsImage::Pointer filter = ITKCollidingFrontsImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCollidingFrontsImage::getHumanLabel()
{ return "[ITK] ITK  Colliding Fronts Image Filter (KW)"; }

