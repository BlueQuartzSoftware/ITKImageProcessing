// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKGrayscaleMorphologicalClosingImage.h"
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

#include <itkFlatStructuringElement.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGrayscaleMorphologicalClosingImage::ITKGrayscaleMorphologicalClosingImage()
{
  m_SafeBorder = StaticCastScalar<bool, bool, bool>(true);
  m_KernelRadius = CastStdToVec3<std::vector<unsigned int>, FloatVec3Type, float>(std::vector<unsigned int>(3, 1));
  m_KernelType = StaticCastScalar<int, int, int>(itk::simple::sitkBall);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGrayscaleMorphologicalClosingImage::~ITKGrayscaleMorphologicalClosingImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  // Structuring element
  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Kernel Type");
    parameter->setPropertyName("KernelType");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKGrayscaleMorphologicalClosingImage, this, KernelType));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKGrayscaleMorphologicalClosingImage, this, KernelType));

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
  parameters.push_back(SIMPL_NEW_BOOL_FP("SafeBorder", SafeBorder, FilterParameter::Parameter, ITKGrayscaleMorphologicalClosingImage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("KernelRadius", KernelRadius, FilterParameter::Parameter, ITKGrayscaleMorphologicalClosingImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKGrayscaleMorphologicalClosingImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKGrayscaleMorphologicalClosingImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKGrayscaleMorphologicalClosingImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setSafeBorder(reader->readValue("SafeBorder", getSafeBorder()));
  setKernelRadius(reader->readFloatVec3("KernelRadius", getKernelRadius()));
  setKernelType(reader->readValue("KernelType", getKernelType()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKGrayscaleMorphologicalClosingImage::dataCheckImpl()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckVectorEntry<unsigned int, FloatVec3Type>(m_KernelRadius, "KernelRadius", true);

  ITKImageProcessingBase::dataCheckImpl<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::dataCheck()
{
  Dream3DArraySwitchMacro(this->dataCheckImpl, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension>
void ITKGrayscaleMorphologicalClosingImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  typedef itk::FlatStructuringElement<Dimension> StructuringElementType;
  typedef typename StructuringElementType::RadiusType RadiusType;
  RadiusType elementRadius = CastVec3ToITK<FloatVec3Type, RadiusType, typename RadiusType::SizeValueType>(m_KernelRadius, RadiusType::Dimension);
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
    setErrorCondition(-20, "Unsupported structuring element");
    return;
  }
  // define filter
  typedef itk::GrayscaleMorphologicalClosingImageFilter<InputImageType, OutputImageType, StructuringElementType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSafeBorder(static_cast<bool>(m_SafeBorder));
  filter->SetKernel(structuringElement);
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKGrayscaleMorphologicalClosingImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKGrayscaleMorphologicalClosingImage::Pointer filter = ITKGrayscaleMorphologicalClosingImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKGrayscaleMorphologicalClosingImage::getHumanLabel() const
{
  return "ITK::Grayscale Morphological Closing Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKGrayscaleMorphologicalClosingImage::getUuid() const
{
  return QUuid("{849a1903-5595-5029-bbde-6f4b68b2a25c}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKGrayscaleMorphologicalClosingImage::getSubGroupName() const
{
  return "ITK BiasCorrection";
}

// -----------------------------------------------------------------------------
ITKGrayscaleMorphologicalClosingImage::Pointer ITKGrayscaleMorphologicalClosingImage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKGrayscaleMorphologicalClosingImage> ITKGrayscaleMorphologicalClosingImage::New()
{
  struct make_shared_enabler : public ITKGrayscaleMorphologicalClosingImage
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKGrayscaleMorphologicalClosingImage::getNameOfClass() const
{
  return QString("ITKGrayscaleMorphologicalClosingImage");
}

// -----------------------------------------------------------------------------
QString ITKGrayscaleMorphologicalClosingImage::ClassName()
{
  return QString("ITKGrayscaleMorphologicalClosingImage");
}

// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::setSafeBorder(bool value)
{
  m_SafeBorder = value;
}

// -----------------------------------------------------------------------------
bool ITKGrayscaleMorphologicalClosingImage::getSafeBorder() const
{
  return m_SafeBorder;
}

// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::setKernelRadius(const FloatVec3Type& value)
{
  m_KernelRadius = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type ITKGrayscaleMorphologicalClosingImage::getKernelRadius() const
{
  return m_KernelRadius;
}

// -----------------------------------------------------------------------------
void ITKGrayscaleMorphologicalClosingImage::setKernelType(int value)
{
  m_KernelType = value;
}

// -----------------------------------------------------------------------------
int ITKGrayscaleMorphologicalClosingImage::getKernelType() const
{
  return m_KernelType;
}
