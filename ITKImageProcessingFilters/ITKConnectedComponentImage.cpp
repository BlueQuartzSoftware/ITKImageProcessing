// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKConnectedComponentImage.h"
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
ITKConnectedComponentImage::ITKConnectedComponentImage()
{
  m_FullyConnected = StaticCastScalar<bool, bool, bool>(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKConnectedComponentImage::~ITKConnectedComponentImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKConnectedComponentImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Parameter, ITKConnectedComponentImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKConnectedComponentImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKConnectedComponentImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKConnectedComponentImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKConnectedComponentImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setFullyConnected(reader->readValue("FullyConnected", getFullyConnected()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKConnectedComponentImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKConnectedComponentImage::dataCheck()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, uint32_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKConnectedComponentImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::ConnectedComponentImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
  {
    QString outputVal = "ObjectCount :%1";
    m_ObjectCount = filter->GetObjectCount();
    setWarningCondition(0, outputVal.arg(m_ObjectCount));
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKConnectedComponentImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, uint32_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKConnectedComponentImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKConnectedComponentImage::Pointer filter = ITKConnectedComponentImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKConnectedComponentImage::getHumanLabel() const
{
  return "ITK::Connected Component Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKConnectedComponentImage::getUuid() const
{
  return QUuid("{bf554dd5-a927-5969-b651-1c47d386afce}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKConnectedComponentImage::getSubGroupName() const
{
  return "ITK SegmentationPostProcessing";
}

// -----------------------------------------------------------------------------
ITKConnectedComponentImage::Pointer ITKConnectedComponentImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKConnectedComponentImage> ITKConnectedComponentImage::New()
{
  struct make_shared_enabler : public ITKConnectedComponentImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKConnectedComponentImage::getNameOfClass() const
{
  return QString("ITKConnectedComponentImage");
}

// -----------------------------------------------------------------------------
QString ITKConnectedComponentImage::ClassName()
{
  return QString("ITKConnectedComponentImage");
}

// -----------------------------------------------------------------------------
void ITKConnectedComponentImage::setFullyConnected(bool value)
{
  m_FullyConnected = value;
}

// -----------------------------------------------------------------------------
bool ITKConnectedComponentImage::getFullyConnected() const
{
  return m_FullyConnected;
}

// -----------------------------------------------------------------------------
void ITKConnectedComponentImage::setObjectCount(double value)
{
  m_ObjectCount = value;
}

// -----------------------------------------------------------------------------
double ITKConnectedComponentImage::getObjectCount() const
{
  return m_ObjectCount;
}
