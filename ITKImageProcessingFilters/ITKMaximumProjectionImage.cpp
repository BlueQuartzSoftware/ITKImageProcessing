// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKMaximumProjectionImage.h"
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
ITKMaximumProjectionImage::ITKMaximumProjectionImage()
{
  m_ProjectionDimension = StaticCastScalar<double, double, double>(0u);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMaximumProjectionImage::~ITKMaximumProjectionImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("ProjectionDimension", ProjectionDimension, FilterParameter::Category::Parameter, ITKMaximumProjectionImage));

  std::vector<QString> linkedProps;
  linkedProps.push_back("NewCellArrayName");
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKMaximumProjectionImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKMaximumProjectionImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setProjectionDimension(reader->readValue("ProjectionDimension", getProjectionDimension()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMaximumProjectionImage::dataCheckImpl()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<unsigned int, double>(m_ProjectionDimension, "ProjectionDimension", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMaximumProjectionImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::MaximumProjectionImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetProjectionDimension(static_cast<unsigned int>(m_ProjectionDimension));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKMaximumProjectionImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKMaximumProjectionImage::Pointer filter = ITKMaximumProjectionImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKMaximumProjectionImage::getHumanLabel() const
{
  return "ITK::Maximum Projection Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKMaximumProjectionImage::getUuid() const
{
  return QUuid("{b2cb7ad7-6f62-51c4-943d-54d19c64e7be}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKMaximumProjectionImage::getSubGroupName() const
{
  return "ITK ImageStatistics";
}

// -----------------------------------------------------------------------------
ITKMaximumProjectionImage::Pointer ITKMaximumProjectionImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKMaximumProjectionImage> ITKMaximumProjectionImage::New()
{
  struct make_shared_enabler : public ITKMaximumProjectionImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKMaximumProjectionImage::getNameOfClass() const
{
  return QString("ITKMaximumProjectionImage");
}

// -----------------------------------------------------------------------------
QString ITKMaximumProjectionImage::ClassName()
{
  return QString("ITKMaximumProjectionImage");
}

// -----------------------------------------------------------------------------
void ITKMaximumProjectionImage::setProjectionDimension(double value)
{
  m_ProjectionDimension = value;
}

// -----------------------------------------------------------------------------
double ITKMaximumProjectionImage::getProjectionDimension() const
{
  return m_ProjectionDimension;
}
