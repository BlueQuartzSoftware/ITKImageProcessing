/*
 * Your License or Copyright can go here
 */

#include "ITKGaussianBlur.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

#include "sitkExplicitITK.h"

#include <itkDiscreteGaussianImageFilter.h>

// Include the MOC generated file for this class
#include "moc_ITKGaussianBlur.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGaussianBlur::ITKGaussianBlur() :
  ITKImageBase(),
  m_Variance(1),
  m_MaximumError(0.01),
  m_MaximumKernelWidth(4)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGaussianBlur::~ITKGaussianBlur()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGaussianBlur::setupFilterParameters()
{
  FilterParameterVector parameters;

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKGaussianBlur, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Blurred Array", NewCellArrayName, FilterParameter::CreatedArray, ITKGaussianBlur));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to Blur", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKGaussianBlur, req));
  }
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Maximum Kernel Width", MaximumKernelWidth, FilterParameter::Parameter, ITKGaussianBlur));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Variance", Variance, FilterParameter::Parameter, ITKGaussianBlur));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Maximum Error", MaximumError, FilterParameter::Parameter, ITKGaussianBlur));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGaussianBlur::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setMaximumKernelWidth(reader->readValue("MaximumKernelWidth", getMaximumKernelWidth()));
  setVariance(reader->readValue("Variance", getVariance()));
  setMaximumError(reader->readValue("MaximumError", getMaximumError()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKGaussianBlur::dataCheck()
{
  setErrorCondition(0);
  if (m_MaximumKernelWidth <= 0)
  {
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), "Maximum kernel width must be >0", getErrorCondition());
    return;
  }
  if (m_Variance <= 0)
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(), "Variance must be >0", getErrorCondition());
    return;
  }
  if (m_MaximumError <= 0)
  {
    setErrorCondition(-3);
    notifyErrorMessage(getHumanLabel(), "Maximum error must be >0", getErrorCondition());
    return;
  }
  ITKImageBase::dataCheck<PixelType, Dimension>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void ITKGaussianBlur::dataCheckInternal()
{
  Dream3DArraySwitchMacro(this->dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension>
void ITKGaussianBlur::filter()
{
    DataArrayPath dap = getSelectedCellArrayPath();
    DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());

    typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
    typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType> GaussianFilterType;
    typename GaussianFilterType::Pointer gaussianFilter = GaussianFilterType::New();
    gaussianFilter->SetVariance(getVariance());
    gaussianFilter->SetMaximumKernelWidth(getMaximumKernelWidth());
    gaussianFilter->SetMaximumError(getMaximumError());

    this->ITKImageBase::filter<PixelType, Dimension, GaussianFilterType>(gaussianFilter);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void ITKGaussianBlur::filterInternal()
{
  Dream3DArraySwitchMacro(this->filter, getSelectedCellArrayPath(), -4);// Run filter
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKGaussianBlur::newFilterInstance(bool copyFilterParameters)
{
  ITKGaussianBlur::Pointer filter = ITKGaussianBlur::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGaussianBlur::getHumanLabel()
{ return "[ITK] GaussianBlur (KW)"; }

