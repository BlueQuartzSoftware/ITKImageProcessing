// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKProxTVImage.h"
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
ITKProxTVImage::ITKProxTVImage()
{
  m_MaximumNumberOfIterations = StaticCastScalar<double, double, double>(10u);
  m_Weights = CastStdToVec3<std::vector<double>, FloatVec3Type, float>(std::vector<double>(3, 1.0));
  m_Norms = CastStdToVec3<std::vector<double>, FloatVec3Type, float>(std::vector<double>(3, 1.0));

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKProxTVImage::~ITKProxTVImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKProxTVImage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

    parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumNumberOfIterations", MaximumNumberOfIterations, FilterParameter::Parameter, ITKProxTVImage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Weights", Weights, FilterParameter::Parameter, ITKProxTVImage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Norms", Norms, FilterParameter::Parameter, ITKProxTVImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKProxTVImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKProxTVImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKProxTVImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKProxTVImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setMaximumNumberOfIterations(reader->readValue("MaximumNumberOfIterations", getMaximumNumberOfIterations()));
  setWeights(reader->readFloatVec3("Weights", getWeights()));
  setNorms(reader->readFloatVec3("Norms", getNorms()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKProxTVImage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  // Check consistency of parameters
  this->CheckIntegerEntry<unsigned int, double>(m_MaximumNumberOfIterations, "MaximumNumberOfIterations", 1);
  this->CheckVectorEntry<double, FloatVec3Type>(m_Weights, "Weights", 0);
  this->CheckVectorEntry<double, FloatVec3Type>(m_Norms, "Norms", 0);

  ITKImageProcessingBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKProxTVImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKProxTVImage::filter()
{
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  // define filter
  typedef itk::ProxTVImageFilter<InputImageType, OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetMaximumNumberOfIterations(static_cast<unsigned int>(m_MaximumNumberOfIterations));
  filter->SetWeights(CastVec3ToITK<FloatVec3Type, typename FilterType::ArrayType, typename FilterType::ArrayType::ValueType>(m_Weights, FilterType::ArrayType::Dimension));
  filter->SetNorms(CastVec3ToITK<FloatVec3Type, typename FilterType::ArrayType, typename FilterType::ArrayType::ValueType>(m_Norms, FilterType::ArrayType::Dimension));
  this->ITKImageProcessingBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKProxTVImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKProxTVImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKProxTVImage::Pointer filter = ITKProxTVImage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKProxTVImage::getHumanLabel() const
{
  return "ITK::Prox T V Image Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKProxTVImage::getUuid()
{
  return QUuid("{d3856d4c-5651-5eab-8740-489a87fa8bdd}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKProxTVImage::getSubGroupName() const
{
  return "ITK NoModule";
}
