// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKBoundedReciprocalImage.h"
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
ITKBoundedReciprocalImage::ITKBoundedReciprocalImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBoundedReciprocalImage::~ITKBoundedReciprocalImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBoundedReciprocalImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKBoundedReciprocalImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKBoundedReciprocalImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBoundedReciprocalImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKBoundedReciprocalImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBoundedReciprocalImage::dataCheck()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, typename itk::NumericTraits<typename InputImageType::PixelType>::RealType, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKBoundedReciprocalImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::BoundedReciprocalImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBoundedReciprocalImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, typename itk::NumericTraits<typename InputImageType::PixelType>::RealType, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKBoundedReciprocalImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKBoundedReciprocalImage::Pointer filter = ITKBoundedReciprocalImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBoundedReciprocalImage::getHumanLabel() const
{
  return "ITK::Bounded Reciprocal Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKBoundedReciprocalImage::getUuid() const
{
  return QUuid("{17f60a64-de93-59aa-a5e2-063e0aadafb7}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKBoundedReciprocalImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}

// -----------------------------------------------------------------------------
ITKBoundedReciprocalImage::Pointer ITKBoundedReciprocalImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKBoundedReciprocalImage> ITKBoundedReciprocalImage::New()
{
  struct make_shared_enabler : public ITKBoundedReciprocalImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKBoundedReciprocalImage::getNameOfClass() const
{
  return QString("ITKBoundedReciprocalImage");
}

// -----------------------------------------------------------------------------
QString ITKBoundedReciprocalImage::ClassName()
{
  return QString("ITKBoundedReciprocalImage");
}
