// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKRescaleIntensityImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRescaleIntensityImage::ITKRescaleIntensityImage()
: m_OutputType(itk::ImageIOBase::IOComponentType::UCHAR - 1)
{
  m_OutputMinimum = StaticCastScalar<double, double, double>(0);
  m_OutputMaximum = StaticCastScalar<double, double, double>(255);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRescaleIntensityImage::~ITKRescaleIntensityImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRescaleIntensityImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Output Type");
    parameter->setPropertyName("OutputType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKRescaleIntensityImage, this, OutputType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKRescaleIntensityImage, this, OutputType));

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


  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMinimum", OutputMinimum, FilterParameter::Parameter, ITKRescaleIntensityImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutputMaximum", OutputMaximum, FilterParameter::Parameter, ITKRescaleIntensityImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKRescaleIntensityImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKRescaleIntensityImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKRescaleIntensityImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRescaleIntensityImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setOutputMinimum(reader->readValue("OutputMinimum", getOutputMinimum()));
  setOutputMaximum(reader->readValue("OutputMaximum", getOutputMaximum()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename OutputPixelType> void ITKRescaleIntensityImage::CheckEntryBounds(double value, QString name)
{
  double lowest = static_cast<double>(std::numeric_limits<OutputPixelType>::lowest());
  double max = static_cast<double>(std::numeric_limits<OutputPixelType>::max());
  if(value < lowest || value > max)
  {
    setErrorCondition(-1);
    QString errorMessage = name + QString(" must be greater or equal than %1 and lesser or equal than %2 and an integer");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(lowest).arg(max), getErrorCondition());
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKRescaleIntensityImage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  // Check consistency of parameters
  CheckEntryBounds<OutputPixelType>(m_OutputMaximum, "OutputMaximum");
  CheckEntryBounds<OutputPixelType>(m_OutputMinimum, "OutputMinimum");

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRescaleIntensityImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKRescaleIntensityImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::RescaleIntensityImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetOutputMinimum(static_cast<double>(m_OutputMinimum));
  filter->SetOutputMaximum(static_cast<double>(m_OutputMaximum));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRescaleIntensityImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKRescaleIntensityImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKRescaleIntensityImage::Pointer filter = ITKRescaleIntensityImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRescaleIntensityImage::getHumanLabel() const
{
  return "ITK::Rescale Intensity Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKRescaleIntensityImage::getUuid()
{
  return QUuid("{77bf2192-851d-5127-9add-634c1ef4f67f}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRescaleIntensityImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}
