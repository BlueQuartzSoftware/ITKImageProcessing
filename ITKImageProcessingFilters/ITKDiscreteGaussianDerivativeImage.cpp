// File automatically generated

/*
 * Your License or Copyright can go here
 */

#include "ITKDiscreteGaussianDerivativeImage.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkDiscreteGaussianDerivativeImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


// Include the MOC generated file for this class
#include "moc_ITKDiscreteGaussianDerivativeImage.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDiscreteGaussianDerivativeImage::ITKDiscreteGaussianDerivativeImage() :
  ITKImageBase(),
m_Variance(std::vector<double>(3, 0.0)),
m_Order(std::vector<unsigned int>(3, 1)),
m_MaximumKernelWidth(32u),
m_MaximumError(0.01),
m_UseImageSpacing(true),
m_NormalizeAcrossScale(false)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKDiscreteGaussianDerivativeImage::~ITKDiscreteGaussianDerivativeImage()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianDerivativeImage::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Variance", Variance, FilterParameter::Parameter, ITKDiscreteGaussianDerivativeImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Order", Order, FilterParameter::Parameter, ITKDiscreteGaussianDerivativeImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumKernelWidth", MaximumKernelWidth, FilterParameter::Parameter, ITKDiscreteGaussianDerivativeImage));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("MaximumError", MaximumError, FilterParameter::Parameter, ITKDiscreteGaussianDerivativeImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("UseImageSpacing", UseImageSpacing, FilterParameter::Parameter, ITKDiscreteGaussianDerivativeImage));
  parameters.push_back(SIMPL_NEW_BOOL_FP("NormalizeAcrossScale", NormalizeAcrossScale, FilterParameter::Parameter, ITKDiscreteGaussianDerivativeImage));


  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKDiscreteGaussianDerivativeImage, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to filter", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKDiscreteGaussianDerivativeImage, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Filtered Array", NewCellArrayName, FilterParameter::CreatedArray, ITKDiscreteGaussianDerivativeImage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianDerivativeImage::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setVariance(reader->readValue("Variance", getVariance()));
  setOrder(reader->readValue("Order", getOrder()));
  setMaximumKernelWidth(reader->readValue("MaximumKernelWidth", getMaximumKernelWidth()));
  setMaximumError(reader->readValue("MaximumError", getMaximumError()));
  setUseImageSpacing(reader->readValue("UseImageSpacing", getUseImageSpacing()));
  setNormalizeAcrossScale(reader->readValue("NormalizeAcrossScale", getNormalizeAcrossScale()));

  reader->closeFilterGroup();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename VarType, typename SubsType>
void ITKDiscreteGaussianDerivativeImage::CheckIntegerEntry(SubsType value, QString name)
{
  if (value < static_cast<SubsType>(std::numeric_limits<VarType>::lowest())
     || value > static_cast<SubsType>(std::numeric_limits<VarType>::max())
     || value != floor(value))
  {
    setErrorCondition(-1);
    QString errorMessage = name + QString(" must be greater or equal than %1 and lesser or equal than %2");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(
        std::numeric_limits<VarType>::lowest()).arg(std::numeric_limits<VarType>::max())
                                                         , getErrorCondition()
                                                         );
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType, unsigned int Dimension>
void ITKDiscreteGaussianDerivativeImage::dataCheck()
{
  // Check consistency of parameters
  CheckIntegerEntry<unsigned int,double>(m_Order, "Order");
  CheckIntegerEntry<unsigned int,double>(m_MaximumKernelWidth, "MaximumKernelWidth");

  setErrorCondition(0);
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianDerivativeImage::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKDiscreteGaussianDerivativeImage::filter()
{
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  //define filter
  typedef itk::DiscreteGaussianDerivativeImageFilter<ImageType, ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetVariance(static_cast<double>(m_Variance));
  filter->SetOrder(static_cast<unsigned int>(m_Order));
  filter->SetMaximumKernelWidth(static_cast<unsigned int>(m_MaximumKernelWidth));
  filter->SetMaximumError(static_cast<double>(m_MaximumError));
  filter->SetUseImageSpacing(static_cast<bool>(m_UseImageSpacing));
  filter->SetNormalizeAcrossScale(static_cast<bool>(m_NormalizeAcrossScale));
  this->ITKImageBase::filter<PixelType, Dimension, FilterType>(filter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKDiscreteGaussianDerivativeImage::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKDiscreteGaussianDerivativeImage::newFilterInstance(bool copyFilterParameters)
{
  ITKDiscreteGaussianDerivativeImage::Pointer filter = ITKDiscreteGaussianDerivativeImage::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKDiscreteGaussianDerivativeImage::getHumanLabel()
{ return "[ITK] ITK  Discrete Gaussian Derivative Image Filter (KW)"; }

