// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKVectorRescaleIntensityImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#define DREAM3D_USE_Scalar 0
#define DREAM3D_USE_Vector 1
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVectorRescaleIntensityImage::ITKVectorRescaleIntensityImage()
: m_OutputType(itk::ImageIOBase::IOComponentType::UCHAR - 1)
{
  m_OutputMaximumMagnitude = StaticCastScalar<double, double, double>(255);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVectorRescaleIntensityImage::~ITKVectorRescaleIntensityImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Output Component Type");
    parameter->setPropertyName("OutputType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKVectorRescaleIntensityImage, this, OutputType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKVectorRescaleIntensityImage, this, OutputType));

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

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMaximumMagnitude", OutputMaximumMagnitude, FilterParameter::Parameter, ITKVectorRescaleIntensityImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKVectorRescaleIntensityImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKVectorRescaleIntensityImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKVectorRescaleIntensityImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setOutputMaximumMagnitude(reader->readValue("OutputMaximumMagnitude", getOutputMaximumMagnitude()));

  reader->closeFilterGroup();
}

template <typename OutputPixelType> void ITKVectorRescaleIntensityImage::CheckEntryBounds(double value, QString name)
{
  double lowest = static_cast<double>(std::numeric_limits<OutputPixelType>::lowest());
  double max = static_cast<double>(std::numeric_limits<OutputPixelType>::max());
  if(value < lowest || value > max)
  {
    QString errorMessage = name + QString(" must be greater or equal than %1 and lesser or equal than %2 and an integer");
    setErrorCondition(-1, errorMessage.arg(lowest).arg(max));
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKVectorRescaleIntensityImage::dataCheck()
{
  // Check consistency of parameters
  CheckEntryBounds<OutputPixelType>(m_OutputMaximumMagnitude, "OutputMaximumMagnitude");
  if(getErrorCode() < 0)
  {
    return;
  }
  clearErrorCode();
  clearWarningCode();
  // OutputPixelType is based on scalar types. Create corresponding vector pixel type.
  typedef itk::Vector<OutputPixelType, InputPixelType::Dimension> VectorOutputPixelType;
  ITKImageProcessingBase::dataCheck<InputPixelType, VectorOutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::dataCheckInternal()
{
  Dream3DArraySwitchOutputComponentMacro(this->dataCheck, m_OutputType, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKVectorRescaleIntensityImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  // OutputPixelType is based on scalar types. Create corresponding vector pixel type.
  typedef itk::Vector<OutputPixelType, InputPixelType::Dimension> VectorOutputPixelType;
  typedef itk::Image<VectorOutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::VectorRescaleIntensityImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetOutputMaximumMagnitude(static_cast<double>(m_OutputMaximumMagnitude));
  this->ITKImageProcessingBase::filter<InputPixelType, VectorOutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorRescaleIntensityImage::filterInternal()
{
  Dream3DArraySwitchOutputComponentMacro(this->filter, m_OutputType, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKVectorRescaleIntensityImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKVectorRescaleIntensityImage::Pointer filter = ITKVectorRescaleIntensityImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKVectorRescaleIntensityImage::getHumanLabel() const
{
  return "ITK::Vector Rescale Intensity Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKVectorRescaleIntensityImage::getUuid()
{
  return QUuid("{bc1051ba-6c67-5391-809b-48627ed47fa7}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKVectorRescaleIntensityImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}
