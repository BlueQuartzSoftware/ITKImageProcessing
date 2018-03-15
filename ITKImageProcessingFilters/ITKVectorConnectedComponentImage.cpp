// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKVectorConnectedComponentImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/SimpleITKEnums.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#define DREAM3D_USE_int8_t 0
#define DREAM3D_USE_uint8_t 0
#define DREAM3D_USE_int16_t 0
#define DREAM3D_USE_uint16_t 0
#define DREAM3D_USE_int32_t 0
#define DREAM3D_USE_uint32_t 0
#define DREAM3D_USE_int64_t 0
#define DREAM3D_USE_uint64_t 0
#define DREAM3D_USE_float 1
#define DREAM3D_USE_double 1

#define DREAM3D_USE_Scalar 0
#define DREAM3D_USE_Vector 1

#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVectorConnectedComponentImage::ITKVectorConnectedComponentImage()
: ITKImageProcessingBase()
{
  m_DistanceThreshold = StaticCastScalar<double, double, double>(1.0);
  m_FullyConnected = StaticCastScalar<bool, bool, bool>(false);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKVectorConnectedComponentImage::~ITKVectorConnectedComponentImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorConnectedComponentImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("DistanceThreshold", DistanceThreshold, FilterParameter::Parameter, ITKVectorConnectedComponentImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("FullyConnected", FullyConnected, FilterParameter::Parameter, ITKVectorConnectedComponentImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKVectorConnectedComponentImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKVectorConnectedComponentImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKVectorConnectedComponentImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorConnectedComponentImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setDistanceThreshold(reader->readValue("DistanceThreshold", getDistanceThreshold()));
  setFullyConnected(reader->readValue("FullyConnected", getFullyConnected()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKVectorConnectedComponentImage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);

  // Check consistency of parameters
  QVector<QString> supportedTypes;
  // All integer types
  supportedTypes << "float"
                 << "double";
  checkImageType(supportedTypes, getSelectedCellArrayPath());

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorConnectedComponentImage::dataCheckInternal()
{
  Dream3DArraySwitchMacroOutputType(this->dataCheck, getSelectedCellArrayPath(), -4, uint32_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKVectorConnectedComponentImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::VectorConnectedComponentImageFilter<InputImageType, OutputImageType, itk::Image<uint8_t, InputImageType::ImageDimension> > FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetDistanceThreshold( static_cast<typename FilterType::InputValueType>( this->m_DistanceThreshold ) );
  filter->SetFullyConnected(static_cast<bool>(m_FullyConnected));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKVectorConnectedComponentImage::filterInternal()
{
  Dream3DArraySwitchMacroOutputType(this->filter, getSelectedCellArrayPath(), -4, uint32_t, 0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKVectorConnectedComponentImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKVectorConnectedComponentImage::Pointer filter = ITKVectorConnectedComponentImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKVectorConnectedComponentImage::getHumanLabel() const
{
  return "ITK::Vector Connected Component Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKVectorConnectedComponentImage::getUuid()
{
  return QUuid("{9d42ede4-fd4b-57fe-9595-50c16deaa3a2}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKVectorConnectedComponentImage::getSubGroupName() const
{
  return "ITK SegmentationPostProcessing";
}
