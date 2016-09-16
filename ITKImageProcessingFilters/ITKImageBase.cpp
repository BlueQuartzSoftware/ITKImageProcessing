/*
 * Your License or Copyright can go here
 */

#include "ITKImageBase.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DFilterInterruption.h"

#include "moc_ITKImageBase.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageBase::ITKImageBase() :
  AbstractFilter(),
  m_SelectedCellArrayPath("", "", ""),
  m_NewCellArrayName(""),
  m_SaveAsNewArray(true),
  m_NewCellArray(NULL)
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageBase::~ITKImageBase()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::initialize()
{
  setErrorCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType, unsigned int Dimension>
void ITKImageBase::dataCheck()
{
  // Check data array
  typename DataArray<PixelType>::WeakPointer selectedCellArrayPtr;
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
void ITKImageBase::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  this->dataCheckInternal();
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType, unsigned int Dimension, typename FilterType>
void ITKImageBase::filter(FilterType* filter)
{
  try
  {
    DataArrayPath dap = getSelectedCellArrayPath();
    DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dap.getDataContainerName());

    typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
    typedef itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension> toITKType;
    // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container
    typename toITKType::Pointer toITK = toITKType::New();
    toITK->SetInput(dc);
    toITK->SetInPlace(true);
    toITK->SetAttributeMatrixArrayName(getSelectedCellArrayPath().getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(getSelectedCellArrayPath().getDataArrayName().toStdString());

    itk::Dream3DFilterInterruption::Pointer interruption = itk::Dream3DFilterInterruption::New();
    interruption->SetFilter(this);

    // Set up filter
    filter->SetInput(filter->GetOutput());
    filter->AddObserver(itk::ProgressEvent(), interruption);
    filter->Update();

    typename ImageType::Pointer image = ImageType::New();
    image = filter->GetOutput();
    image->DisconnectPipeline();
    std::string outputArrayName(getNewCellArrayName().toStdString());

    if (getSaveAsNewArray() == false)
    {
      outputArrayName = getSelectedCellArrayPath().getDataArrayName().toStdString();
      AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(getSelectedCellArrayPath().getAttributeMatrixName());
      // Remove the original input data array
      attrMat->removeAttributeArray(getSelectedCellArrayPath().getDataArrayName());
    }

    typename itk::InPlaceImageToDream3DDataFilter<PixelType, 3>::Pointer toDream3DFilter = itk::InPlaceImageToDream3DDataFilter<PixelType, 3>::New();
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
void ITKImageBase::execute()
{
  initialize();
  this->dataCheckInternal();
  if (getErrorCondition() < 0) { return; }
  if (getCancel() == true) { return; }
  this->filterInternal();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getGroupName()
{ return "Image Processing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getSubGroupName()
{ return "ImageProcessing"; }

