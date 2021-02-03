// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKMorphologicalWatershedImage.h"
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
ITKMorphologicalWatershedImage::ITKMorphologicalWatershedImage()
{
  m_Level = StaticCastScalar<double, double, double>(0.0);
  m_MarkWatershedLine = StaticCastScalar<bool, bool, bool>(true);
  m_FullyConnected = StaticCastScalar<bool, bool, bool>(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMorphologicalWatershedImage::~ITKMorphologicalWatershedImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Level", Level, FilterParameter::Parameter, ITKMorphologicalWatershedImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("MarkWatershedLine", MarkWatershedLine, FilterParameter::Parameter, ITKMorphologicalWatershedImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Parameter, ITKMorphologicalWatershedImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKMorphologicalWatershedImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKMorphologicalWatershedImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setLevel(reader->readValue("Level", getLevel()));
  setMarkWatershedLine(reader->readValue("MarkWatershedLine", getMarkWatershedLine()));
  setFullyConnected(reader->readValue("FullyConnected", getFullyConnected()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMorphologicalWatershedImage::dataCheckImpl()
{
  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, uint32_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMorphologicalWatershedImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::MorphologicalWatershedImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetLevel(static_cast<double>(m_Level));
  filter->SetMarkWatershedLine(static_cast<bool>(m_MarkWatershedLine));
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, uint32_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKMorphologicalWatershedImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKMorphologicalWatershedImage::Pointer filter = ITKMorphologicalWatershedImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKMorphologicalWatershedImage::getHumanLabel() const
{
  return "ITK::Morphological Watershed Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKMorphologicalWatershedImage::getUuid() const
{
  return QUuid("{b2248340-a371-5899-90a2-86047950f0a2}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKMorphologicalWatershedImage::getSubGroupName() const
{
  return "ITK Watersheds";
}

// -----------------------------------------------------------------------------
ITKMorphologicalWatershedImage::Pointer ITKMorphologicalWatershedImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKMorphologicalWatershedImage> ITKMorphologicalWatershedImage::New()
{
  struct make_shared_enabler : public ITKMorphologicalWatershedImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKMorphologicalWatershedImage::getNameOfClass() const
{
  return QString("ITKMorphologicalWatershedImage");
}

// -----------------------------------------------------------------------------
QString ITKMorphologicalWatershedImage::ClassName()
{
  return QString("ITKMorphologicalWatershedImage");
}

// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedImage::setLevel(double value)
{
  m_Level = value;
}

// -----------------------------------------------------------------------------
double ITKMorphologicalWatershedImage::getLevel() const
{
  return m_Level;
}

// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedImage::setMarkWatershedLine(bool value)
{
  m_MarkWatershedLine = value;
}

// -----------------------------------------------------------------------------
bool ITKMorphologicalWatershedImage::getMarkWatershedLine() const
{
  return m_MarkWatershedLine;
}

// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedImage::setFullyConnected(bool value)
{
  m_FullyConnected = value;
}

// -----------------------------------------------------------------------------
bool ITKMorphologicalWatershedImage::getFullyConnected() const
{
  return m_FullyConnected;
}
