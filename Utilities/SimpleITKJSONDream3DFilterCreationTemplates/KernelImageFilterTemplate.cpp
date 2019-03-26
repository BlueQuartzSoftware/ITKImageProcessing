// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/${FilterName}.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/itkDream3DImage.h"
#include <itkFlatStructuringElement.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
${FilterName}::${FilterName}()
{
${InitializationParameters}
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
${FilterName}::~${FilterName}() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ${FilterName}::setupFilterParameters()
{
  FilterParameterVector parameters;

  // Structuring element
  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Kernel Type");
    parameter->setPropertyName("KernelType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(${FilterName}, this, KernelType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(${FilterName}, this, KernelType));

    QVector<QString> choices;
    choices.push_back("Annulus");
    choices.push_back("Ball");
    choices.push_back("Box");
    choices.push_back("Cross");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Parameter);
    parameters.push_back(parameter);
  }
  // Other parameters
${SetupFilterParameters}

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ${FilterName}, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ${FilterName}, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ${FilterName}));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ${FilterName}::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
${ReadFilterParameters}
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ${FilterName}::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  // Check consistency of parameters
${CheckIntegerEntry}
  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ${FilterName}::dataCheckInternal()
{
${DataCheckInternal}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ${FilterName}::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  typedef itk::FlatStructuringElement<Dimension> StructuringElementType;
  typedef typename StructuringElementType::RadiusType RadiusType;
  RadiusType elementRadius = CastVec3ToITK<FloatVec3_t, RadiusType, typename RadiusType::SizeValueType>(m_KernelRadius, RadiusType::Dimension);
  StructuringElementType structuringElement;
  switch(getKernelType())
  {
  case 0:
    structuringElement = StructuringElementType::Annulus(elementRadius, false);
    break;
  case 1:
    structuringElement = StructuringElementType::Ball(elementRadius, false);
    break;
  case 2:
    structuringElement = StructuringElementType::Box(elementRadius);
    break;
  case 3:
    structuringElement = StructuringElementType::Cross(elementRadius);
    break;
  default:
    notifyErrorMessage("", "Unsupported structuring element", -20);
    return;
  }
  // define filter
${Filter}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ${FilterName}::filterInternal()
{
${FilterInternal}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ${FilterName}::newFilterInstance(bool copyFilterParameters) const
{
  ${FilterName}::Pointer filter = ${FilterName}::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ${FilterName}::getHumanLabel() const
{
  return "ITK::${FilterNameExpanded}";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ${FilterName}::getUuid()
{
${UUID}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ${FilterName}::getSubGroupName() const
{
  return "ITK ${ITKModule}";
}
