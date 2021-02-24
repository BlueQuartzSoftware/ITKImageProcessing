// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKRelabelComponentImage.h"
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
ITKRelabelComponentImage::ITKRelabelComponentImage()
{
  m_MinimumObjectSize = StaticCastScalar<double, double, double>(0u);
  m_SortByObjectSize = StaticCastScalar<bool, bool, bool>(true);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRelabelComponentImage::~ITKRelabelComponentImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MinimumObjectSize", MinimumObjectSize, FilterParameter::Category::Parameter, ITKRelabelComponentImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("SortByObjectSize", SortByObjectSize, FilterParameter::Category::Parameter, ITKRelabelComponentImage));

  std::vector<QString> linkedProps;
  linkedProps.push_back("NewCellArrayName");
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::Category::RequiredArray, ITKRelabelComponentImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::Create("Cell Data", FilterParameter::Category::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::Category::CreatedArray, ITKRelabelComponentImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setMinimumObjectSize(reader->readValue("MinimumObjectSize", getMinimumObjectSize()));
  setSortByObjectSize(reader->readValue("SortByObjectSize", getSortByObjectSize()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKRelabelComponentImage::dataCheckImpl()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<uint64_t, double>(m_MinimumObjectSize, "MinimumObjectSize", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKRelabelComponentImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::RelabelComponentImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetMinimumObjectSize(static_cast<uint64_t>(m_MinimumObjectSize));
  filter->SetSortByObjectSize(static_cast<bool>(m_SortByObjectSize));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
  {
    QString outputVal = "NumberOfObjects :%1";
    m_NumberOfObjects = filter->GetNumberOfObjects();
    setWarningCondition(0, outputVal.arg(m_NumberOfObjects));
  }
  {
    QString outputVal = "OriginalNumberOfObjects :%1";
    m_OriginalNumberOfObjects = filter->GetOriginalNumberOfObjects();
    setWarningCondition(0, outputVal.arg(m_OriginalNumberOfObjects));
  }
  //{
  //  QString outputVal = "SizeOfObjectsInPhysicalUnits :%1";
  //  m_SizeOfObjectsInPhysicalUnits = filter->GetSizeOfObjectsInPhysicalUnits();
  //}
  //{
  //  QString outputVal = "SizeOfObjectsInPixels :%1";
  //  m_SizeOfObjectsInPixels = filter->GetSizeOfObjectsInPixels();
  //}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKRelabelComponentImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKRelabelComponentImage::Pointer filter = ITKRelabelComponentImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKRelabelComponentImage::getHumanLabel() const
{
  return "ITK::Relabel Component Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKRelabelComponentImage::getUuid() const
{
  return QUuid("{4398d76d-c9aa-5161-bb48-92dd9daaa352}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKRelabelComponentImage::getSubGroupName() const
{
  return "ITK SegmentationPostProcessing";
}

// -----------------------------------------------------------------------------
ITKRelabelComponentImage::Pointer ITKRelabelComponentImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKRelabelComponentImage> ITKRelabelComponentImage::New()
{
  struct make_shared_enabler : public ITKRelabelComponentImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKRelabelComponentImage::getNameOfClass() const
{
  return QString("ITKRelabelComponentImage");
}

// -----------------------------------------------------------------------------
QString ITKRelabelComponentImage::ClassName()
{
  return QString("ITKRelabelComponentImage");
}

// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::setMinimumObjectSize(double value)
{
  m_MinimumObjectSize = value;
}

// -----------------------------------------------------------------------------
double ITKRelabelComponentImage::getMinimumObjectSize() const
{
  return m_MinimumObjectSize;
}

// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::setSortByObjectSize(bool value)
{
  m_SortByObjectSize = value;
}

// -----------------------------------------------------------------------------
bool ITKRelabelComponentImage::getSortByObjectSize() const
{
  return m_SortByObjectSize;
}

// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::setNumberOfObjects(double value)
{
  m_NumberOfObjects = value;
}

// -----------------------------------------------------------------------------
double ITKRelabelComponentImage::getNumberOfObjects() const
{
  return m_NumberOfObjects;
}

// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::setOriginalNumberOfObjects(double value)
{
  m_OriginalNumberOfObjects = value;
}

// -----------------------------------------------------------------------------
double ITKRelabelComponentImage::getOriginalNumberOfObjects() const
{
  return m_OriginalNumberOfObjects;
}

// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::setSizeOfObjectsInPhysicalUnits(const FloatVec3Type& value)
{
  m_SizeOfObjectsInPhysicalUnits = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKRelabelComponentImage::getSizeOfObjectsInPhysicalUnits() const
{
  return m_SizeOfObjectsInPhysicalUnits;
}

// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::setSizeOfObjectsInPixels(const FloatVec3Type& value)
{
  m_SizeOfObjectsInPixels = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKRelabelComponentImage::getSizeOfObjectsInPixels() const
{
  return m_SizeOfObjectsInPixels;
}
