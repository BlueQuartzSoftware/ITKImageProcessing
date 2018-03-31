// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKRelabelComponentImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/SimpleITKEnums.h"

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
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MinimumObjectSize", MinimumObjectSize, FilterParameter::Parameter, ITKRelabelComponentImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("SortByObjectSize", SortByObjectSize, FilterParameter::Parameter, ITKRelabelComponentImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKRelabelComponentImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKRelabelComponentImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKRelabelComponentImage));

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
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setMinimumObjectSize(reader->readValue("MinimumObjectSize", getMinimumObjectSize()));
  setSortByObjectSize(reader->readValue("SortByObjectSize", getSortByObjectSize()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKRelabelComponentImage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  // Check consistency of parameters
  this->CheckIntegerEntry<uint64_t, double>(m_MinimumObjectSize, "MinimumObjectSize", 1);

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRelabelComponentImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKRelabelComponentImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::RelabelComponentImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetMinimumObjectSize(static_cast<uint64_t>(m_MinimumObjectSize));
  filter->SetSortByObjectSize(static_cast<bool>(m_SortByObjectSize));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
{
  QString outputVal = "NumberOfObjects :%1";
  m_NumberOfObjects = filter->GetNumberOfObjects();
  notifyWarningMessage(getHumanLabel(),outputVal.arg(m_NumberOfObjects),0);
}
{
  QString outputVal = "OriginalNumberOfObjects :%1";
  m_OriginalNumberOfObjects = filter->GetOriginalNumberOfObjects();
  notifyWarningMessage(getHumanLabel(),outputVal.arg(m_OriginalNumberOfObjects),0);
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
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRelabelComponentImage::getHumanLabel() const
{
  return "ITK::Relabel Component Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKRelabelComponentImage::getUuid()
{
  return QUuid("{4398d76d-c9aa-5161-bb48-92dd9daaa352}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRelabelComponentImage::getSubGroupName() const
{
  return "ITK SegmentationPostProcessing";
}
