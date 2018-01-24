// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKMultiScaleHessianBasedObjectnessImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include <itkHessianToObjectnessMeasureImageFilter.h>
#include <itkMultiScaleHessianBasedMeasureImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMultiScaleHessianBasedObjectnessImage::ITKMultiScaleHessianBasedObjectnessImage()
: ITKImageBase()
{
  m_ObjectDimension = StaticCastScalar<int, int, int>(1u);
  m_Alpha = StaticCastScalar<double, double, double>(0.5);
  m_Beta = StaticCastScalar<double, double, double>(0.5);
  m_Gamma = StaticCastScalar<double, double, double>(5.0);
  m_BrightObject = StaticCastScalar<bool, bool, bool>(true);
  m_ScaleObjectnessMeasure = StaticCastScalar<bool, bool, bool>(true);

  m_SigmaMinimum = StaticCastScalar<double, double, double>(0.2);
  m_SigmaMaximum = StaticCastScalar<double, double, double>(2.0);
  m_NumberOfSigmaSteps = StaticCastScalar<double, double, double>(10);

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKMultiScaleHessianBasedObjectnessImage::~ITKMultiScaleHessianBasedObjectnessImage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMultiScaleHessianBasedObjectnessImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_INTEGER_FP("ObjectDimension", ObjectDimension, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Alpha", Alpha, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Beta", Beta, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Gamma", Gamma, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("BrightObject", BrightObject, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("ScaleObjectnessMeasure", ScaleObjectnessMeasure, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("SigmaMinimum", SigmaMinimum, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("SigmaMaximum", SigmaMaximum, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("NumberOfSigmaSteps", NumberOfSigmaSteps, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage));

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKMultiScaleHessianBasedObjectnessImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKMultiScaleHessianBasedObjectnessImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKMultiScaleHessianBasedObjectnessImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMultiScaleHessianBasedObjectnessImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath(reader->readDataArrayPath("SelectedCellArrayPath", getSelectedCellArrayPath()));
  setNewCellArrayName(reader->readString("NewCellArrayName", getNewCellArrayName()));
  setSaveAsNewArray(reader->readValue("SaveAsNewArray", getSaveAsNewArray()));
  setObjectDimension(reader->readValue("ObjectDimension", getObjectDimension()));
  setAlpha(reader->readValue("Alpha", getAlpha()));
  setBeta(reader->readValue("Beta", getBeta()));
  setGamma(reader->readValue("Gamma", getGamma()));
  setBrightObject(reader->readValue("BrightObject", getBrightObject()));
  setScaleObjectnessMeasure(reader->readValue("ScaleObjectnessMeasure", getScaleObjectnessMeasure()));
  setSigmaMinimum(reader->readValue("SigmaMinimum", getSigmaMinimum()));
  setSigmaMaximum(reader->readValue("SigmaMaximum", getSigmaMaximum()));
  setNumberOfSigmaSteps(reader->readValue("NumberOfSigmaSteps", getNumberOfSigmaSteps()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKMultiScaleHessianBasedObjectnessImage::dataCheck()
{
  // Check consistency of parameters
  this->CheckIntegerEntry<uint32_t, double>(m_NumberOfSigmaSteps, "NumberOfSigmaSteps", 1);

  setErrorCondition(0);
  setWarningCondition(0);
  ITKImageBase::dataCheck<InputPixelType, OutputPixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMultiScaleHessianBasedObjectnessImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void ITKMultiScaleHessianBasedObjectnessImage::filter()
{
  typedef itk::Dream3DImage<InputPixelType, Dimension> InputImageType;
  typedef itk::Dream3DImage<OutputPixelType, Dimension> OutputImageType;

  typedef typename itk::NumericTraits<InputPixelType>::RealType RealType;
  typedef itk::SymmetricSecondRankTensor<RealType, Dimension> HessianPixelType;
  typedef itk::Image<HessianPixelType, Dimension> HessianImageType;

  typedef itk::HessianToObjectnessMeasureImageFilter<HessianImageType, OutputImageType> ObjectnessFilterType;
  typename ObjectnessFilterType::Pointer objectnessFilter = ObjectnessFilterType::New();
  objectnessFilter->SetObjectDimension(static_cast<unsigned int>(m_ObjectDimension));
  objectnessFilter->SetAlpha(static_cast<double>(m_Alpha));
  objectnessFilter->SetBeta(static_cast<double>(m_Beta));
  objectnessFilter->SetGamma(static_cast<double>(m_Gamma));
  objectnessFilter->SetBrightObject(static_cast<bool>(m_BrightObject));
  objectnessFilter->SetScaleObjectnessMeasure(static_cast<bool>(m_ScaleObjectnessMeasure));

  typedef itk::MultiScaleHessianBasedMeasureImageFilter<InputImageType, HessianImageType, OutputImageType> FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  filter->SetSigmaMinimum(static_cast<double>(m_SigmaMinimum));
  filter->SetSigmaMaximum(static_cast<double>(m_SigmaMaximum));
  filter->SetNumberOfSigmaSteps(static_cast<double>(m_NumberOfSigmaSteps));
  filter->SetHessianToMeasureFilter(objectnessFilter);

  this->ITKImageBase::filter<InputPixelType, OutputPixelType, Dimension, FilterType>(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKMultiScaleHessianBasedObjectnessImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKMultiScaleHessianBasedObjectnessImage::newFilterInstance(bool copyFilterParameters) const
{
  ITKMultiScaleHessianBasedObjectnessImage::Pointer filter = ITKMultiScaleHessianBasedObjectnessImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMultiScaleHessianBasedObjectnessImage::getHumanLabel() const
{
  return "ITK::Multi-scale Hessian Based Objectness Filter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKMultiScaleHessianBasedObjectnessImage::getUuid()
{
  return QUuid("{c080e143-1895-5f71-9799-06b8c2d58faf}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKMultiScaleHessianBasedObjectnessImage::getSubGroupName() const
{
  return "ITK Edge";
}
