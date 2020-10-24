// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKIsoContourDistanceImage.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIsoContourDistanceImage::ITKIsoContourDistanceImage()
{
  m_LevelSetValue = StaticCastScalar<double, double, double>(0.0);
  m_FarValue = StaticCastScalar<double, double, double>(10);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKIsoContourDistanceImage::~ITKIsoContourDistanceImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsoContourDistanceImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("LevelSetValue", LevelSetValue, FilterParameter::Parameter, ITKIsoContourDistanceImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("FarValue", FarValue, FilterParameter::Parameter, ITKIsoContourDistanceImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKIsoContourDistanceImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKIsoContourDistanceImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsoContourDistanceImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setLevelSetValue(reader->readValue("LevelSetValue", getLevelSetValue()));
  setFarValue(reader->readValue("FarValue", getFarValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKIsoContourDistanceImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsoContourDistanceImage::dataCheck()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKIsoContourDistanceImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::IsoContourDistanceImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetLevelSetValue(static_cast<double>(m_LevelSetValue));
  filter->SetFarValue(static_cast<double>(m_FarValue));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKIsoContourDistanceImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKIsoContourDistanceImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKIsoContourDistanceImage::Pointer filter = ITKIsoContourDistanceImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKIsoContourDistanceImage::getHumanLabel() const
{
  return "ITK::Iso Contour Distance Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKIsoContourDistanceImage::getUuid() const
{
  return QUuid("{e9952e0e-97e4-53aa-8c6c-115fd18f0b89}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKIsoContourDistanceImage::getSubGroupName() const
{
  return "ITK DistanceMap";
}

// -----------------------------------------------------------------------------
ITKIsoContourDistanceImage::Pointer ITKIsoContourDistanceImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKIsoContourDistanceImage> ITKIsoContourDistanceImage::New()
{
  struct make_shared_enabler : public ITKIsoContourDistanceImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKIsoContourDistanceImage::getNameOfClass() const
{
  return QString("ITKIsoContourDistanceImage");
}

// -----------------------------------------------------------------------------
QString ITKIsoContourDistanceImage::ClassName()
{
  return QString("ITKIsoContourDistanceImage");
}

// -----------------------------------------------------------------------------
void ITKIsoContourDistanceImage::setLevelSetValue(double value)
{
  m_LevelSetValue = value;
}

// -----------------------------------------------------------------------------
double ITKIsoContourDistanceImage::getLevelSetValue() const
{
  return m_LevelSetValue;
}

// -----------------------------------------------------------------------------
void ITKIsoContourDistanceImage::setFarValue(double value)
{
  m_FarValue = value;
}

// -----------------------------------------------------------------------------
double ITKIsoContourDistanceImage::getFarValue() const
{
  return m_FarValue;
}
