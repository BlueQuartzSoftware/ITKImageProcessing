// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKDanielssonDistanceMapImage.h"
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
ITKDanielssonDistanceMapImage::ITKDanielssonDistanceMapImage()
{
  m_InputIsBinary = StaticCastScalar<bool, bool, bool>(false);
  m_SquaredDistance = StaticCastScalar<bool, bool, bool>(false);
  m_UseImageSpacing = StaticCastScalar<bool, bool, bool>(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDanielssonDistanceMapImage::~ITKDanielssonDistanceMapImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDanielssonDistanceMapImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_BOOL_FP("InputIsBinary", InputIsBinary, FilterParameter::Category::Parameter, ITKDanielssonDistanceMapImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("SquaredDistance", SquaredDistance, FilterParameter::Category::Parameter, ITKDanielssonDistanceMapImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("UseImageSpacing", UseImageSpacing, FilterParameter::Category::Parameter, ITKDanielssonDistanceMapImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKDanielssonDistanceMapImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKDanielssonDistanceMapImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDanielssonDistanceMapImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setInputIsBinary(reader->readValue("InputIsBinary", getInputIsBinary()));
  setSquaredDistance(reader->readValue("SquaredDistance", getSquaredDistance()));
  setUseImageSpacing(reader->readValue("UseImageSpacing", getUseImageSpacing()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKDanielssonDistanceMapImage::dataCheckImpl()
{
  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDanielssonDistanceMapImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKDanielssonDistanceMapImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::DanielssonDistanceMapImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInputIsBinary(static_cast<bool>(m_InputIsBinary));
  filter->SetSquaredDistance(static_cast<bool>(m_SquaredDistance));
  filter->SetUseImageSpacing(static_cast<bool>(m_UseImageSpacing));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDanielssonDistanceMapImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, float, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKDanielssonDistanceMapImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKDanielssonDistanceMapImage::Pointer filter = ITKDanielssonDistanceMapImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKDanielssonDistanceMapImage::getHumanLabel() const
{
  return "ITK::Danielsson Distance Map Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKDanielssonDistanceMapImage::getUuid() const
{
  return QUuid("{53d5b289-a716-559b-89d9-5ebb34f714ca}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKDanielssonDistanceMapImage::getSubGroupName() const
{
  return "ITK DistanceMap";
}

// -----------------------------------------------------------------------------
ITKDanielssonDistanceMapImage::Pointer ITKDanielssonDistanceMapImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKDanielssonDistanceMapImage> ITKDanielssonDistanceMapImage::New()
{
  struct make_shared_enabler : public ITKDanielssonDistanceMapImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKDanielssonDistanceMapImage::getNameOfClass() const
{
  return QString("ITKDanielssonDistanceMapImage");
}

// -----------------------------------------------------------------------------
QString ITKDanielssonDistanceMapImage::ClassName()
{
  return QString("ITKDanielssonDistanceMapImage");
}

// -----------------------------------------------------------------------------
void ITKDanielssonDistanceMapImage::setInputIsBinary(bool value)
{
  m_InputIsBinary = value;
}

// -----------------------------------------------------------------------------
bool ITKDanielssonDistanceMapImage::getInputIsBinary() const
{
  return m_InputIsBinary;
}

// -----------------------------------------------------------------------------
void ITKDanielssonDistanceMapImage::setSquaredDistance(bool value)
{
  m_SquaredDistance = value;
}

// -----------------------------------------------------------------------------
bool ITKDanielssonDistanceMapImage::getSquaredDistance() const
{
  return m_SquaredDistance;
}

// -----------------------------------------------------------------------------
void ITKDanielssonDistanceMapImage::setUseImageSpacing(bool value)
{
  m_UseImageSpacing = value;
}

// -----------------------------------------------------------------------------
bool ITKDanielssonDistanceMapImage::getUseImageSpacing() const
{
  return m_UseImageSpacing;
}
