/*
 * Your License or Copyright can go here
 */

#include "ITKHistogramMatchingImage.h"

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
ITKHistogramMatchingImage::ITKHistogramMatchingImage()
{
  m_NumberOfHistogramLevels = StaticCastScalar<double, double, double>(256u);
  m_NumberOfMatchPoints = StaticCastScalar<double, double, double>(1u);
  m_ThresholdAtMeanIntensity = StaticCastScalar<bool, bool, bool>(true);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKHistogramMatchingImage::~ITKHistogramMatchingImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKHistogramMatchingImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfHistogramLevels", NumberOfHistogramLevels, FilterParameter::Parameter, ITKHistogramMatchingImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfMatchPoints", NumberOfMatchPoints, FilterParameter::Parameter, ITKHistogramMatchingImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("ThresholdAtMeanIntensity", ThresholdAtMeanIntensity, FilterParameter::Parameter, ITKHistogramMatchingImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKHistogramMatchingImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
                                                                                                                  // Source image
                                                                                                                  AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Input Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKHistogramMatchingImage, req));
    // Reference image
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Reference Attribute Array to filter", ReferenceCellArrayPath, FilterParameter::RequiredArray, ITKHistogramMatchingImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKHistogramMatchingImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKHistogramMatchingImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setReferenceCellArrayPath(reader->readDataArrayPath("ReferenceCellArrayPath", getReferenceCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setNumberOfHistogramLevels(reader->readValue("NumberOfHistogramLevels", getNumberOfHistogramLevels()));
  setNumberOfMatchPoints(reader->readValue("NumberOfMatchPoints", getNumberOfMatchPoints()));
  setThresholdAtMeanIntensity(reader->readValue("ThresholdAtMeanIntensity", getThresholdAtMeanIntensity()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
size_t ITKHistogramMatchingImage::getImageDimension(const DataArrayPath& path)
{
  QVector<size_t> tDims(3, -1);
  QString errorMessage;
  ImageGeom::Pointer imageGeometry = getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(this, path.getDataContainerName());
  if(imageGeometry.get() != nullptr)
  {
    std::tie(tDims[0], tDims[1], tDims[2]) = imageGeometry->getDimensions();
  }
  else
  {
    setErrorCondition(-10);
    errorMessage = "Geometry not found (%1)";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(path.serialize()), getErrorCondition());
    return 0;
  }
  // Geometry is always 3D, but assume image is 2D if
  // last dimension is less than 2.
  if(tDims[2] < 2)
  {
    return 2;
  }
  return 3;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ITKHistogramMatchingImage::CheckArrayExists(const DataArrayPath& path)
{
  IDataArray::Pointer ptr = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, path);
  if(ptr.get() == nullptr)
  {
    setErrorCondition(-6);
    QString errorMessage("Data Array %1 does not exist");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(path.serialize()), getErrorCondition());
    return 1;
  }
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKHistogramMatchingImage::CompareImagePixelTypes(const DataArrayPath& path1, const DataArrayPath& path2)
{
  // Verify that both images have same data type
  IDataArray::Pointer ptr1 = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, path1);
  IDataArray::Pointer ptr2 = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, path2);
  if(ptr1->getTypeAsString() != ptr2->getTypeAsString())
  {
    setErrorCondition(-5);
    QString errorMessage("Both images must have the same pixel type.\nFound %1 and %2");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(ptr1->getTypeAsString()).arg(ptr2->getTypeAsString()), getErrorCondition());
    return;
  }
  // Compare that pixel types are the same
  QVector<size_t> cDims1 = ptr1->getComponentDimensions();
  QVector<size_t> cDims2 = ptr2->getComponentDimensions();
  if(cDims1.size() != cDims2.size())
  {
    setErrorCondition(-8);
    QString errorMessage("Both images must have components of the same size.\nFound %1 and %2");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(cDims1.size()).arg(cDims2.size()), getErrorCondition());
    return;
  }
  for(int ii = 0; ii < cDims1.size(); ii++)
  {
    if(cDims1[ii] != cDims2[ii])
    {
      setErrorCondition(-9);
      QString errorMessage("Both images must have all components of the same size.\nFound %1 and %2 for component %3");
      notifyErrorMessage(getHumanLabel(), errorMessage.arg(cDims1[ii]).arg(cDims2[ii]).arg(ii), getErrorCondition());
      return;
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKHistogramMatchingImage::CompareImageTypes(const DataArrayPath& path1, const DataArrayPath& path2)
{
  // Does data array exist
  if(CheckArrayExists(path1) || CheckArrayExists(path2))
  {
    return;
  }
  // Compare image1(path1) and image2 (path2) image dimension
  size_t dimension1 = getImageDimension(getSelectedCellArrayPath());
  size_t dimension2 = getImageDimension(getReferenceCellArrayPath());
  if(dimension1 != dimension2)
  {
    setErrorCondition(-7);
    QString errorMessage("Images have a different number of dimensions:\n %1 != %2");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(dimension1).arg(dimension2), getErrorCondition());
    return;
  }
  // image pixeltype+image component
  CompareImagePixelTypes(path1, path2);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKHistogramMatchingImage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfHistogramLevels, "NumberOfHistogramLevels", 1);
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfMatchPoints, "NumberOfMatchPoints", 1);

  // Compare source and reference image type
  CompareImageTypes(getSelectedCellArrayPath(), getReferenceCellArrayPath());
  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKHistogramMatchingImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKHistogramMatchingImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::HistogramMatchingImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetNumberOfHistogramLevels(static_cast<uint32_t>(m_NumberOfHistogramLevels));
  filter->SetNumberOfMatchPoints(static_cast<uint32_t>(m_NumberOfMatchPoints));
  filter->SetThresholdAtMeanIntensity(static_cast<bool>(m_ThresholdAtMeanIntensity));
  // Set reference image.
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::InPlaceDream3DDataToImageFilter<InputPixelType, Dimension> toITKType;
  DataArrayPath dap = getReferenceCellArrayPath();
  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());
  // Create a bridge to wrap an existing DREAM.3D array with an ItkImage container
  typename toITKType::Pointer toITK = toITKType::New();
  toITK->SetInput(dc);
  toITK->SetInPlace(true);
  toITK->SetAttributeMatrixArrayName(getReferenceCellArrayPath().getAttributeMatrixName().toStdString());
  toITK->SetDataArrayName(getReferenceCellArrayPath().getDataArrayName().toStdString());
  filter->SetReferenceImage(toITK->GetOutput());
  // Run filter
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKHistogramMatchingImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKHistogramMatchingImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKHistogramMatchingImage::Pointer filter = ITKHistogramMatchingImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKHistogramMatchingImage::getHumanLabel() const
{
  return "ITK::Histogram Matching Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKHistogramMatchingImage::getUuid()
{
  return QUuid("{33ca886c-42b9-524a-984a-dab24f8bb74c}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKHistogramMatchingImage::getSubGroupName() const
{
  return "ITK IntensityTransformation";
}
