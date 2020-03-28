// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include <memory>

#include "SIMPLib/ITK/SimpleITKEnums.h"
#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKValuedRegionalMaximaImage.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKValuedRegionalMaximaImage::ITKValuedRegionalMaximaImage()
{
  m_FullyConnected = StaticCastScalar<bool, bool, bool>(false);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKValuedRegionalMaximaImage::~ITKValuedRegionalMaximaImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKValuedRegionalMaximaImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Parameter, ITKValuedRegionalMaximaImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKValuedRegionalMaximaImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKValuedRegionalMaximaImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKValuedRegionalMaximaImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKValuedRegionalMaximaImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
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
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKValuedRegionalMaximaImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKValuedRegionalMaximaImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKValuedRegionalMaximaImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::ValuedRegionalMaximaImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
{
  QString outputVal = "Flat :%1";
  m_Flat = filter->GetFlat();
  setWarningCondition(0, outputVal.arg(m_Flat));
}

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKValuedRegionalMaximaImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKValuedRegionalMaximaImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKValuedRegionalMaximaImage::Pointer filter = ITKValuedRegionalMaximaImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKValuedRegionalMaximaImage::getHumanLabel() const
{
  return "ITK::Valued Regional Maxima Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKValuedRegionalMaximaImage::getUuid() const
{
  return QUuid("{10aff542-81c5-5f09-9797-c7171c40b6a0}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKValuedRegionalMaximaImage::getSubGroupName() const
{
  return "ITK ITKMathematicalMorphology";
}

// -----------------------------------------------------------------------------
ITKValuedRegionalMaximaImage::Pointer ITKValuedRegionalMaximaImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKValuedRegionalMaximaImage> ITKValuedRegionalMaximaImage::New()
{
  struct make_shared_enabler : public ITKValuedRegionalMaximaImage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKValuedRegionalMaximaImage::getNameOfClass() const
{
  return QString("ITKValuedRegionalMaximaImage");
}

// -----------------------------------------------------------------------------
QString ITKValuedRegionalMaximaImage::ClassName()
{
  return QString("ITKValuedRegionalMaximaImage");
}

// -----------------------------------------------------------------------------
void ITKValuedRegionalMaximaImage::setFullyConnected(bool value)
{
  m_FullyConnected = value;
}

// -----------------------------------------------------------------------------
bool ITKValuedRegionalMaximaImage::getFullyConnected() const
{
  return m_FullyConnected;
}

// -----------------------------------------------------------------------------
void ITKValuedRegionalMaximaImage::setFlat(bool value)
{
  m_Flat = value;
}

// -----------------------------------------------------------------------------
bool ITKValuedRegionalMaximaImage::getFlat() const
{
  return m_Flat;
}


