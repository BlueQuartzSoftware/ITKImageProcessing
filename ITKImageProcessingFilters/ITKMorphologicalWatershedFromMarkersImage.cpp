// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKMorphologicalWatershedFromMarkersImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/itkDream3DImage.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMorphologicalWatershedFromMarkersImage::ITKMorphologicalWatershedFromMarkersImage()
{
  m_MarkWatershedLine = StaticCastScalar<bool, bool, bool>(true);
  m_FullyConnected = StaticCastScalar<bool, bool, bool>(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMorphologicalWatershedFromMarkersImage::~ITKMorphologicalWatershedFromMarkersImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_BOOL_FP("MarkWatershedLine", MarkWatershedLine, FilterParameter::Parameter, ITKMorphologicalWatershedFromMarkersImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Parameter, ITKMorphologicalWatershedFromMarkersImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKMorphologicalWatershedFromMarkersImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKMorphologicalWatershedFromMarkersImage, req));
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Marker Array", MarkerCellArrayPath, FilterParameter::RequiredArray, ITKMorphologicalWatershedFromMarkersImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKMorphologicalWatershedFromMarkersImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setSelectedCellArrayPath(reader->readDataArrayPath("MarkerCellArrayPath", getMarkerCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setMarkWatershedLine(reader->readValue("MarkWatershedLine", getMarkWatershedLine()));
  setFullyConnected(reader->readValue("FullyConnected", getFullyConnected()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKMorphologicalWatershedFromMarkersImage::dataCheck()
{
  // Check consistency of parameters
  clearErrorCondition();
  clearWarningCondition();
  QVector<QString> supportedTypes;
  supportedTypes << "uint8_t"
                 << "uint16_t";
  checkImageType(supportedTypes, getMarkerCellArrayPath());
  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4, uint16_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKMorphologicalWatershedFromMarkersImage::convertDataContainerType()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  typedef itk::InPlaceDream3DDataToImageFilter<InputPixelType, Dimension> toITKType;
  typedef itk::InPlaceImageToDream3DDataFilter<OutputPixelType, Dimension> toDream3DType;
  typedef itk::CastImageFilter<InputImageType, OutputImageType> CastType;
  DataArrayPath dap = getMarkerCellArrayPath();
  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());
  try
  {
    // Create a bridge to wrap an existing DREAM.3D array with an ItkImage container
    typename toITKType::Pointer toITK = toITKType::New();
    toITK->SetInput(dc);
    toITK->SetInPlace(true);
    toITK->SetAttributeMatrixArrayName(dap.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(dap.getDataArrayName().toStdString());
    toITK->Update();
    typename CastType::Pointer cast = CastType::New();
    cast->SetInput(toITK->GetOutput());
    cast->Update();
    // Convert back to dream3D array
    DataContainer::Pointer container = getMarkerContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, dap.getDataContainerName());
    if(!container.get())
    {
      setErrorCondition(-3);
      notifyErrorMessage("No container.", getErrorCondition());
      return;
    }
    QVector<size_t> dims = ITKDream3DHelper::GetComponentsDimensions<OutputPixelType>();
    // getMarkerContainerArray()->createNonPrereqArrayFromPath<DataArray<OutputPixelType>, AbstractFilter, OutputPixelType>(this, dap, 0, dims);
    DataContainer::Pointer dcMarker = getMarkerContainerArray()->getDataContainer(dap.getDataContainerName());
    typename toDream3DType::Pointer toDream3D = toDream3DType::New();
    toDream3D->SetInput(cast->GetOutput());
    toDream3D->SetInPlace(true);
    toDream3D->SetAttributeMatrixArrayName(dap.getAttributeMatrixName().toStdString());
    toDream3D->SetDataArrayName(dap.getDataArrayName().toStdString());
    toDream3D->SetDataContainer(dcMarker);
    toDream3D->Update();
  } catch(itk::ExceptionObject& err)
  {
    setErrorCondition(-55562);
    QString errorMessage = "ITK exception was thrown while converting marker image: %1";
    notifyErrorMessage(errorMessage.arg(err.GetDescription()), getErrorCondition());
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKMorphologicalWatershedFromMarkersImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::MorphologicalWatershedFromMarkersImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetMarkWatershedLine(static_cast<bool>(m_MarkWatershedLine));
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  // Set marker image.
  try
  {
    DataArrayPath dap = getMarkerCellArrayPath();
    DataContainer::Pointer dcMarker = getMarkerContainerArray()->getDataContainer(dap.getDataContainerName());
    typedef itk::InPlaceDream3DDataToImageFilter<uint16_t, Dimension> toITKType;
    typename toITKType::Pointer toITK = toITKType::New();
    toITK->SetInput(dcMarker);
    toITK->SetInPlace(true);
    toITK->SetAttributeMatrixArrayName(dap.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(dap.getDataArrayName().toStdString());
    toITK->Update();
    filter->SetMarkerImage(toITK->GetOutput());
  } catch(itk::ExceptionObject& err)
  {
    setErrorCondition(-55563);
    QString errorMessage = "ITK exception was thrown while converting marker image: %1";
    notifyErrorMessage(errorMessage.arg(err.GetDescription()), getErrorCondition());
    return;
  }
  // Run filter
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
  m_MarkerContainerArray = nullptr; // Free the memory used by the casted marker image
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::filterInternal()
{
  m_MarkerContainerArray = DataContainerArray::New();
  // Convert Marker image to uint16
  Dream3DArraySwitchMacroOutputType(this->convertDataContainerType, getMarkerCellArrayPath(), -4, uint16_t, 0);
  // Run filter
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, uint16_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKMorphologicalWatershedFromMarkersImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKMorphologicalWatershedFromMarkersImage::Pointer filter = ITKMorphologicalWatershedFromMarkersImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMorphologicalWatershedFromMarkersImage::getHumanLabel() const
{
  return "ITK::Morphological Watershed From Markers Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKMorphologicalWatershedFromMarkersImage::getUuid()
{
  return QUuid("{81308863-094b-511d-9aa9-865e02e2b8d5}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMorphologicalWatershedFromMarkersImage::getSubGroupName() const
{
  return "ITK Segmentation";
}
