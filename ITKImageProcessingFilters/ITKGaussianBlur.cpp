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


#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

#include "sitkExplicitITK.h"

#include <itkDiscreteGaussianImageFilter.h>

// Include the MOC generated file for this class
#include "moc_ITKGaussianBlur.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKGaussianBlur::ITKGaussianBlur() :
  AbstractFilter(),
  m_SelectedCellArrayPath("", "", ""),
  m_NewCellArrayName(""),
  m_Variance(1),
  m_MaximumError(0.01),
  m_MaximumKernelWidth(4),
  m_SaveAsNewArray(true)
{
  initialize();
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
void ITKGaussianBlur::initialize()
{
  setErrorCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType>
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
  DataArray<PixelType>::WeakPointer selectedCellArrayPtr;
  PixelType* selectedCellArray;
    
  DataArrayPath tempPath;

  QVector<size_t> dims(1, 1);
  selectedCellArrayPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<PixelType>, AbstractFilter>(this, getSelectedCellArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != selectedCellArrayPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { selectedCellArray = selectedCellArrayPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCondition() < 0) { return; }

  ImageGeom::Pointer image = getDataContainerArray()->getDataContainer(getSelectedCellArrayPath().getDataContainerName())->getPrereqGeometry<ImageGeom, AbstractFilter>(this);
  if(getErrorCondition() < 0 || NULL == image.get()) { return; }

  if(m_SaveAsNewArray == true)
  {
    tempPath.update(getSelectedCellArrayPath().getDataContainerName(), getSelectedCellArrayPath().getAttributeMatrixName(), getNewCellArrayName() );
    m_NewCellArrayPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<PixelType>, AbstractFilter, PixelType>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if( NULL != m_NewCellArrayPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
    { m_NewCellArray = m_NewCellArrayPtr.lock()->getVoidPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  }
  else
  {
    m_NewCellArrayPtr = DataArray<PixelType>::NullPointer();
    m_NewCellArray = nullptr;
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGaussianBlur::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  Dream3DArraySwitchMacro(dataCheck, getSelectedCellArrayPath(),-4);// Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType>
void ITKGaussianBlur::filter()
{
  try
  {
    DataArrayPath dap = getSelectedCellArrayPath();
    DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());

    const unsigned int Dimension = 3;
    typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
    typedef itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension> FilterType;
    // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(dc);
    filter->SetInPlace(true);
    filter->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
    filter->SetDataArrayName(getSelectedCellArrayPath().getDataArrayName().toStdString());

    itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
    interruption->SetFilter(this);

    typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType> GaussianFilterType;

    GaussianFilterType::Pointer gaussianFilter = GaussianFilterType::New();
    gaussianFilter->SetInput(filter->GetOutput());
    gaussianFilter->AddObserver(itk::ProgressEvent(), interruption);
    gaussianFilter->SetVariance(getVariance());
    gaussianFilter->SetMaximumKernelWidth(getMaximumKernelWidth());
    gaussianFilter->SetMaximumError(getMaximumError());
    gaussianFilter->Update();

    ImageType::Pointer image = ImageType::New();
    //image = thresholdFilter->GetOutput();
    image=gaussianFilter->GetOutput();
    image->DisconnectPipeline();
    std::string outputArrayName(getNewCellArrayName().toStdString());

    if (getSaveAsNewArray() == false)
    {
      outputArrayName = m_SelectedCellArrayPath.getDataArrayName().toStdString();
      AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(m_SelectedCellArrayPath.getAttributeMatrixName());
      // Remove the original input data array
      attrMat->removeAttributeArray(m_SelectedCellArrayPath.getDataArrayName());
    }

    typename itk::InPlaceImageToDream3DDataFilter<PixelType,3>::Pointer toDream3DFilter = itk::InPlaceImageToDream3DDataFilter<PixelType,3>::New();
    toDream3DFilter->SetInput(image);
    toDream3DFilter->SetInPlace(true);
    toDream3DFilter->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
    toDream3DFilter->SetDataArrayName(outputArrayName);
    toDream3DFilter->SetDataContainer(dc);
    toDream3DFilter->Update();


  }
  catch (itk::ExceptionObject & err)
  {
    setErrorCondition(-5);
    QString errorMessage = "ITK exception was thrown while filtering input image: %1";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(err.GetDescription()), getErrorCondition());
    return;
  }

  notifyStatusMessage(getHumanLabel(), "Complete");

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKGaussianBlur::execute()
{
  initialize();
  Dream3DArraySwitchMacro(dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
  if (getErrorCondition() < 0) { return; }
  if (getCancel() == true) { return; }
  Dream3DArraySwitchMacro(filter, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
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
const QString ITKGaussianBlur::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGaussianBlur::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGaussianBlur::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGaussianBlur::getGroupName()
{ return "Image Processing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGaussianBlur::getSubGroupName()
{ return "ITKImageProcessing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKGaussianBlur::getHumanLabel()
{ return "[ITK] GaussianBlur (KW)"; }

