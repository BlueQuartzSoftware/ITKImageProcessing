/*
 * Your License or Copyright can go here
 */

#include "ITKCastImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"


#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

// Include the MOC generated file for this class
#include "moc_ITKCastImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCastImage::ITKCastImage() :
  ITKImageBase()
  , m_CastingType(itk::ImageIOBase::IOComponentType::UCHAR-1)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKCastImage::~ITKCastImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCastImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Casting Type");
    parameter->setPropertyName("CastingType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKCastImage, this, CastingType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKCastImage, this, CastingType));

    QVector<QString> choices;
    choices.push_back("unsigned char");
    choices.push_back("char");
    choices.push_back("unsigned short");
    choices.push_back("short");
    choices.push_back("unsigned int");
    choices.push_back("int");
    choices.push_back("unsigned long");
    choices.push_back("long");
    choices.push_back("float");
    choices.push_back("double");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Parameter);
    parameters.push_back(parameter);
  }

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKCastImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKCastImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKCastImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCastImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setCastingType(reader->readValue("CastingType", getCastingType()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKCastImage::dataCheck()
{
  // Check consistency of parameters
  setErrorCondition(0);
  typedef typename itk::NumericTraits<InputPixelType>::ValueType   InputValueType;
  typedef typename itk::NumericTraits<OutputPixelType>::ValueType   OutputValueType;
  if(static_cast< double >(std::numeric_limits<InputValueType>::max()) > static_cast< double>(std::numeric_limits<OutputValueType>::max())
    || static_cast< double >(std::numeric_limits<InputValueType>::lowest()) < static_cast< double >(std::numeric_limits<OutputValueType>::lowest())
    )
  {
    setErrorCondition(-5);
    notifyErrorMessage(getHumanLabel(), "Boundaries values of output component type inside boundaries of input component type. Use ITK::Rescale Intensity Image Filter instead.", getErrorCondition());
    return;
  }
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCastImage::dataCheckInternal()
{
   Dream3DArraySwitchOutputComponentMacro(this->dataCheck, m_CastingType, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKCastImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  //define filter
  typedef itk::CastImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKCastImage::filterInternal()
{
   Dream3DArraySwitchOutputComponentMacro(this->filter, m_CastingType, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKCastImage::newFilterInstance(bool copyFilterParameters)
{
  ITKCastImage::Pointer filter = ITKCastImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCastImage::getHumanLabel()
{ return "ITK::Cast Image Filter (KW)"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKCastImage::getSubGroupName()
{ return "ITK ImageFilterBase"; }


