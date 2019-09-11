// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKMaskImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#define DREAM3D_USE_Vector 1
#define DREAM3D_USE_RGB_RGBA 1
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMaskImage::ITKMaskImage()
{
  m_OutsideValue = StaticCastScalar<double, double, double>(0);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMaskImage::~ITKMaskImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaskImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("OutsideValue", OutsideValue, FilterParameter::Parameter, ITKMaskImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKMaskImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKMaskImage, req));
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Mask Array", MaskCellArrayPath, FilterParameter::RequiredArray, ITKMaskImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKMaskImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaskImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setSelectedCellArrayPath(reader->readDataArrayPath("MaskCellArrayPath", getMaskCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKMaskImage::dataCheck()
{
  // Check consistency of parameters

  clearErrorCode();
  clearWarningCode();
  QVector<QString> supportedTypes;
  supportedTypes << "uint8_t"
                 << "uint16_t"
                 << "uint32_t";
  checkImageType(supportedTypes, getMaskCellArrayPath());
  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaskImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -5);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> typename std::enable_if<!std::is_scalar<InputPixelType>::value>::type ITKMaskImage::convertDataContainerType()
{
  // This should never happened as masks should be scalar images, not RGBA or vector images.
  // We need this function because vector and RGBA images are supported as input images
  // #define DREAM3D_USE_Vector 1
  // #define DREAM3D_USE_RGB_RGBA   1
  // This function is only needed for compilation purposes, but should never be called
  // at runtime.
  // checkImageType() is called before this function and limits the types of
  // supported mask images.
  // Just in case, an error is returned if this function is called.
  QString errorMessage = "Mask images are required to be scalar images.";
  setErrorCondition(-20, errorMessage);
}

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> typename std::enable_if<std::is_scalar<InputPixelType>::value>::type ITKMaskImage::convertDataContainerType()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  typedef itk::InPlaceDream3DDataToImageFilter<InputPixelType, Dimension> toITKType;
  typedef itk::InPlaceImageToDream3DDataFilter<OutputPixelType, Dimension> toDream3DType;
  typedef itk::CastImageFilter<InputImageType, OutputImageType> CastType;
  DataArrayPath dap = getMaskCellArrayPath();
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
    DataContainer::Pointer container = getMaskContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, dap.getDataContainerName());
    if(!container.get())
    {
      setErrorCondition(-3, "No container.");
      return;
    }
    std::vector<size_t> dims = ITKDream3DHelper::GetComponentsDimensions<OutputPixelType>();
    DataContainer::Pointer dcMask = getMaskContainerArray()->getDataContainer(dap.getDataContainerName());
    typename toDream3DType::Pointer toDream3D = toDream3DType::New();
    toDream3D->SetInput(cast->GetOutput());
    toDream3D->SetInPlace(true);
    toDream3D->SetAttributeMatrixArrayName(dap.getAttributeMatrixName().toStdString());
    toDream3D->SetDataArrayName(dap.getDataArrayName().toStdString());
    toDream3D->SetDataContainer(dcMask);
    toDream3D->Update();
  } catch(itk::ExceptionObject& err)
  {
    QString errorMessage = "ITK exception was thrown while converting mask image: %1";
    setErrorCondition(-55560, errorMessage.arg(err.GetDescription()));
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKMaskImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<uint32_t, Dimension> MaskImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::MaskImageFilter<InputImageType, MaskImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  // Set mask image.
  try
  {
    DataArrayPath dap = getMaskCellArrayPath();
    DataContainer::Pointer dcMask = getMaskContainerArray()->getDataContainer(dap.getDataContainerName());
    typedef itk::InPlaceDream3DDataToImageFilter<uint32_t, Dimension> toITKType;
    typename toITKType::Pointer toITK = toITKType::New();
    toITK->SetInput(dcMask);
    toITK->SetInPlace(true);
    toITK->SetAttributeMatrixArrayName(dap.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(dap.getDataArrayName().toStdString());
    toITK->Update();
    filter->SetMaskImage(toITK->GetOutput());
  } catch(itk::ExceptionObject& err)
  {
    QString errorMessage = "ITK exception was thrown while converting mask image: %1";
    setErrorCondition(-55561, errorMessage.arg(err.GetDescription()));
    return;
  }
  typename OutputImageType::PixelType v;
  size_t NumberOfComponents = 1;
  std::vector<size_t> cDims = ITKDream3DHelper::GetComponentsDimensions<InputPixelType>();
  for(int ii = 0; ii < cDims.size(); ii++)
  {
    NumberOfComponents *= cDims[ii];
  }
  itk::NumericTraits<typename OutputImageType::PixelType>::SetLength(v, NumberOfComponents);
  v = static_cast<OutputPixelType>(m_OutsideValue);
  filter->SetOutsideValue(v);
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMaskImage::filterInternal()
{
  m_MaskContainerArray = DataContainerArray::New();
  // Convert Mask image to uint32
  Dream3DArraySwitchMacroOutputType(this->convertDataContainerType, getMaskCellArrayPath(), -4, uint32_t, 0);
  if(getErrorCode() != 0)
  {
    return; // This should never happen thanks to the preflight checks.
  }
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
  m_MaskContainerArray = nullptr; // Free the memory used by the casted mask image
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKMaskImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKMaskImage::Pointer filter = ITKMaskImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMaskImage::getHumanLabel() const
{
  return "ITK::Mask Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKMaskImage::getUuid()
{
  return QUuid("{97102d65-9c32-576a-9177-c59d958bad10}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMaskImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}
