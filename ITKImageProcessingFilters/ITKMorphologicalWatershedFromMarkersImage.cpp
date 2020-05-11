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

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  DataArrayID30 = 30,
  DataArrayID31 = 31,
};

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
  FilterParameterVectorType parameters;

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
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMorphologicalWatershedFromMarkersImage::dataCheckImpl()
{
  // Check consistency of parameters
  clearErrorCode();
  clearWarningCode();
  QVector<QString> supportedTypes;
  supportedTypes << "uint8_t"
                 << "uint16_t";
  checkImageType(supportedTypes, getMarkerCellArrayPath());
  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::dataCheck()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheckImpl, getSelectedCellArrayPath(), -4, uint16_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMorphologicalWatershedFromMarkersImage::convertDataContainerType()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
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
    DataContainer::Pointer container = getMarkerContainerArray()->createNonPrereqDataContainer(this, dap.getDataContainerName());
    if(!container.get())
    {
      setErrorCondition(-3, "No container.");
      return;
    }
    std::vector<size_t> dims = ITKDream3DHelper::GetComponentsDimensions<OutputPixelType>();
    // getMarkerContainerArray()->createNonPrereqArrayFromPath<DataArray<OutputPixelType>, AbstractFilter, OutputPixelType>(this, dap, 0, dims, "", DataArrayID31);
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
    QString errorMessage = "ITK exception was thrown while converting marker image: %1";
    setErrorCondition(-55562, errorMessage.arg(err.GetDescription()));
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKMorphologicalWatershedFromMarkersImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
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
    QString errorMessage = "ITK exception was thrown while converting marker image: %1";
    setErrorCondition(-55563, errorMessage.arg(err.GetDescription()));
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
QString ITKMorphologicalWatershedFromMarkersImage::getHumanLabel() const
{
  return "ITK::Morphological Watershed From Markers Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKMorphologicalWatershedFromMarkersImage::getUuid() const
{
  return QUuid("{81308863-094b-511d-9aa9-865e02e2b8d5}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKMorphologicalWatershedFromMarkersImage::getSubGroupName() const
{
  return "ITK Segmentation";
}

// -----------------------------------------------------------------------------
ITKMorphologicalWatershedFromMarkersImage::Pointer ITKMorphologicalWatershedFromMarkersImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKMorphologicalWatershedFromMarkersImage> ITKMorphologicalWatershedFromMarkersImage::New()
{
  struct make_shared_enabler : public ITKMorphologicalWatershedFromMarkersImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKMorphologicalWatershedFromMarkersImage::getNameOfClass() const
{
  return QString("ITKMorphologicalWatershedFromMarkersImage");
}

// -----------------------------------------------------------------------------
QString ITKMorphologicalWatershedFromMarkersImage::ClassName()
{
  return QString("ITKMorphologicalWatershedFromMarkersImage");
}

// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::setMarkWatershedLine(bool value)
{
  m_MarkWatershedLine = value;
}

// -----------------------------------------------------------------------------
bool ITKMorphologicalWatershedFromMarkersImage::getMarkWatershedLine() const
{
  return m_MarkWatershedLine;
}

// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::setFullyConnected(bool value)
{
  m_FullyConnected = value;
}

// -----------------------------------------------------------------------------
bool ITKMorphologicalWatershedFromMarkersImage::getFullyConnected() const
{
  return m_FullyConnected;
}

// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::setMarkerCellArrayPath(const DataArrayPath& value)
{
  m_MarkerCellArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ITKMorphologicalWatershedFromMarkersImage::getMarkerCellArrayPath() const
{
  return m_MarkerCellArrayPath;
}

// -----------------------------------------------------------------------------
void ITKMorphologicalWatershedFromMarkersImage::setMarkerContainerArray(const DataContainerArray::Pointer& value)
{
  m_MarkerContainerArray = value;
}

// -----------------------------------------------------------------------------
DataContainerArray::Pointer ITKMorphologicalWatershedFromMarkersImage::getMarkerContainerArray() const
{
  return m_MarkerContainerArray;
}
