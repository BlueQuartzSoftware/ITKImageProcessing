// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "${FilterName}.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
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
$
{
  FilterName
}
::${FilterName}()
: ITKImageBase()
{
  $
  {
    InitializationParameters
  }
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
$
{
  FilterName
}
::~${FilterName}()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void $
{
  FilterName
}
::setupFilterParameters()
{
  FilterParameterVector parameters;

  $
  {
    SetupFilterParameters
  }

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
void $
{
  FilterName
}
::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  $
  {
    ReadFilterParameters
  }
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void $
{
  FilterName
}
::dataCheck()
{
  // Check consistency of parameters
  $
  {
    CheckIntegerEntry
  }
  setErrorCondition(0);
  setWarningCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void $
{
  FilterName
}
::dataCheckInternal()
{
  $
  {
    DataCheckInternal
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void $
{
  FilterName
}
::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  $
  {
    Filter
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void $
{
  FilterName
}
::filterInternal()
{
  $
  {
    FilterInternal
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer $
{
  FilterName
}
::newFilterInstance(bool copyFilterParameters)
{
  $
  {
    FilterName
  }
  ::Pointer filter = $
  {
    FilterName
  }
  ::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString $
{
  FilterName
}
::getHumanLabel()
{
  return "ITK::${FilterNameExpanded}";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString $
{
  FilterName
}
const QUuid ImageFilterTemplate::getUuid()
{
  return QUuid("{c2435bc0-b80b-54e2-936b-4bd10ac8f103}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
::getSubGroupName()
{
  return "ITK ${ITKModule}";
}
