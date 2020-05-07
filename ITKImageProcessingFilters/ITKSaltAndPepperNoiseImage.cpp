// File automatically generated

/*
 * Your License or Copyright can go here
 */


#include "ITKImageProcessing/ITKImageProcessingFilters/ITKSaltAndPepperNoiseImage.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"


#define DREAM3D_USE_RGB_RGBA 1
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSaltAndPepperNoiseImage::ITKSaltAndPepperNoiseImage()
{
  m_Probability = StaticCastScalar<double, double, double>(0.01);
  m_Seed = StaticCastScalar<double, double, double>((uint32_t) itk::simple::sitkWallClock);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKSaltAndPepperNoiseImage::~ITKSaltAndPepperNoiseImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSaltAndPepperNoiseImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Probability", Probability, FilterParameter::Parameter, ITKSaltAndPepperNoiseImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Seed", Seed, FilterParameter::Parameter, ITKSaltAndPepperNoiseImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKSaltAndPepperNoiseImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKSaltAndPepperNoiseImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKSaltAndPepperNoiseImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSaltAndPepperNoiseImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setProbability(reader->readValue("Probability", getProbability()));
  setSeed(reader->readValue("Seed", getSeed()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKSaltAndPepperNoiseImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_Seed, "Seed", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSaltAndPepperNoiseImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKSaltAndPepperNoiseImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::SaltAndPepperNoiseImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetProbability(static_cast<double>(m_Probability));
  if(m_Seed)
  {
    filter->SetSeed(m_Seed);
  }
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKSaltAndPepperNoiseImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKSaltAndPepperNoiseImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKSaltAndPepperNoiseImage::Pointer filter = ITKSaltAndPepperNoiseImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSaltAndPepperNoiseImage::getHumanLabel() const
{
  return "ITK::Salt And Pepper Noise Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKSaltAndPepperNoiseImage::getUuid() const
{
  return QUuid("{602c270d-ec55-580c-9108-785ba8530964}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKSaltAndPepperNoiseImage::getSubGroupName() const
{
  return "ITK ImageNoise";
}

// -----------------------------------------------------------------------------
ITKSaltAndPepperNoiseImage::Pointer ITKSaltAndPepperNoiseImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKSaltAndPepperNoiseImage> ITKSaltAndPepperNoiseImage::New()
{
  struct make_shared_enabler : public ITKSaltAndPepperNoiseImage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKSaltAndPepperNoiseImage::getNameOfClass() const
{
  return QString("ITKSaltAndPepperNoiseImage");
}

// -----------------------------------------------------------------------------
QString ITKSaltAndPepperNoiseImage::ClassName()
{
  return QString("ITKSaltAndPepperNoiseImage");
}

// -----------------------------------------------------------------------------
void ITKSaltAndPepperNoiseImage::setProbability(double value)
{
  m_Probability = value;
}

// -----------------------------------------------------------------------------
double ITKSaltAndPepperNoiseImage::getProbability() const
{
  return m_Probability;
}

// -----------------------------------------------------------------------------
void ITKSaltAndPepperNoiseImage::setSeed(double value)
{
  m_Seed = value;
}

// -----------------------------------------------------------------------------
double ITKSaltAndPepperNoiseImage::getSeed() const
{
  return m_Seed;
}


