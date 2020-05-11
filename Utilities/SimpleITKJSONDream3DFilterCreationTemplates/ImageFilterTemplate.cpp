// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/${FilterName}.h"
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
$
{
  FilterName
}
::${FilterName}()
{
  ${InitializationParameters} setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
$
{
  FilterName
}
::~${FilterName}() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void $
{
  FilterName
}
::setupFilterParameters()
{
  FilterParameterVectorType parameters;

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
  ${ReadFilterParameters} reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void $
{
  FilterName
}
::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  ${CheckIntegerEntry} ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
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

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void $
{
  FilterName
}
::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
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
::newFilterInstance(bool copyFilterParameters) const
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
  if(copyFilterParameters)
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
::getHumanLabel() const
{
  return "ITK::${FilterNameExpanded}";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid $
{
  FilterName
}
::getUuid()
{
  $
  {
    UUID
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString $
{
  FilterName
}
::getSubGroupName() const
{
  return "ITK ${ITKModule}";
}

// -----------------------------------------------------------------------------
ImageFilterTemplate::Pointer ImageFilterTemplate::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<${FilterName}> ImageFilterTemplate::New()
{
  struct make_shared_enabler : public ${FilterName} {};
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ImageFilterTemplate::getNameOfClass() const
{
  return QString("${FilterName}");
}

// -----------------------------------------------------------------------------
QString ImageFilterTemplate::ClassName()
{
  return QString("${FilterName}");
}
