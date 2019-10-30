// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include <memory>

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKSpeckleNoiseImage.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSpeckleNoiseImage::ITKSpeckleNoiseImage()
{
  m_StandardDeviation = StaticCastScalar<double, double, double>(1.0);
  m_Seed = StaticCastScalar<double, double, double>((uint32_t) itk::simple::sitkWallClock);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSpeckleNoiseImage::~ITKSpeckleNoiseImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSpeckleNoiseImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("StandardDeviation", StandardDeviation, FilterParameter::Parameter, ITKSpeckleNoiseImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Seed", Seed, FilterParameter::Parameter, ITKSpeckleNoiseImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKSpeckleNoiseImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKSpeckleNoiseImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKSpeckleNoiseImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSpeckleNoiseImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setStandardDeviation(reader->readValue("StandardDeviation", getStandardDeviation()));
  setSeed(reader->readValue("Seed", getSeed()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKSpeckleNoiseImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_Seed, "Seed", true);

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSpeckleNoiseImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKSpeckleNoiseImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::SpeckleNoiseImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetStandardDeviation(static_cast<double>(m_StandardDeviation));
  if(m_Seed)
  {
    filter->SetSeed(m_Seed);
  }
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSpeckleNoiseImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKSpeckleNoiseImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKSpeckleNoiseImage::Pointer filter = ITKSpeckleNoiseImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSpeckleNoiseImage::getHumanLabel() const
{
  return "ITK::Speckle Noise Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKSpeckleNoiseImage::getUuid() const
{
  return QUuid("{764085a4-6ecb-5fb7-891d-2fda208ba5d8}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSpeckleNoiseImage::getSubGroupName() const
{
  return "ITK ImageNoise";
}

// -----------------------------------------------------------------------------
ITKSpeckleNoiseImage::Pointer ITKSpeckleNoiseImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKSpeckleNoiseImage> ITKSpeckleNoiseImage::New()
{
  struct make_shared_enabler : public ITKSpeckleNoiseImage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKSpeckleNoiseImage::getNameOfClass() const
{
  return QString("ITKSpeckleNoiseImage");
}

// -----------------------------------------------------------------------------
QString ITKSpeckleNoiseImage::ClassName()
{
  return QString("ITKSpeckleNoiseImage");
}

// -----------------------------------------------------------------------------
void ITKSpeckleNoiseImage::setStandardDeviation(double value)
{
  m_StandardDeviation = value;
}

// -----------------------------------------------------------------------------
double ITKSpeckleNoiseImage::getStandardDeviation() const
{
  return m_StandardDeviation;
}

// -----------------------------------------------------------------------------
void ITKSpeckleNoiseImage::setSeed(double value)
{
  m_Seed = value;
}

// -----------------------------------------------------------------------------
double ITKSpeckleNoiseImage::getSeed() const
{
  return m_Seed;
}


