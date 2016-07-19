/*
 * Your License or Copyright can go here
 */

#include "Itk_GaussianBlur.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"


#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"

#include "sitkExplicitITK.h"

#include "itkGaussianBlurImageFunction.h"
#include "itkImageRegionConstIterator.h"

// Include the MOC generated file for this class
#include "moc_Itk_GaussianBlur.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Itk_GaussianBlur::Itk_GaussianBlur() :
  AbstractFilter(),
  m_SelectedCellArrayPath("", "", ""),
  m_NewCellArrayName(""),
  m_SaveAsNewArray(true)
{
  initialize();
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Itk_GaussianBlur::~Itk_GaussianBlur()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::setupFilterParameters()
{
  FilterParameterVector parameters;

  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(LinkedBooleanFilterParameter::New("Save as New Array", "SaveAsNewArray", getSaveAsNewArray(), linkedProps, FilterParameter::Parameter, SIMPL_BIND_SETTER(Itk_GaussianBlur, this, SaveAsNewArray), SIMPL_BIND_GETTER(Itk_GaussianBlur, this, SaveAsNewArray)));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req
          = DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, 1, SIMPL::Defaults::AnyAttributeMatrix, SIMPL::Defaults::AnyGeometry);
    QVector<QString> vec;
    vec.push_back(SIMPL::TypeNames::UInt8);
    vec.push_back(SIMPL::TypeNames::Int16);
    vec.push_back(SIMPL::TypeNames::Int32);
    vec.push_back(SIMPL::TypeNames::Int64);
    req.daTypes = vec;
    parameters.push_back(DataArraySelectionFilterParameter::New("Attribute Array to Blur", "SelectedCellArrayPath", getSelectedCellArrayPath(), FilterParameter::RequiredArray, req, SIMPL_BIND_SETTER(Itk_GaussianBlur, this, SelectedCellArrayPath), SIMPL_BIND_GETTER(Itk_GaussianBlur, this, SelectedCellArrayPath)));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(StringFilterParameter::New("Blurred Array", "NewCellArrayName", getNewCellArrayName(), FilterParameter::CreatedArray, SIMPL_BIND_SETTER(Itk_GaussianBlur, this, NewCellArrayName), SIMPL_BIND_GETTER(Itk_GaussianBlur, this, NewCellArrayName)));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::initialize()
{
  setErrorCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::dataCheck()
{
  setErrorCondition(0);
  DataArrayPath tempPath;

  QVector<size_t> dims(1, 1);
  m_SelectedCellArrayPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<DefaultPixelType>, AbstractFilter>(this, getSelectedCellArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_SelectedCellArrayPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_SelectedCellArray = m_SelectedCellArrayPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCondition() < 0) { return; }

  ImageGeom::Pointer image = getDataContainerArray()->getDataContainer(getSelectedCellArrayPath().getDataContainerName())->getPrereqGeometry<ImageGeom, AbstractFilter>(this);
  if(getErrorCondition() < 0 || NULL == image.get()) { return; }

  if(m_SaveAsNewArray == true)
  {
    tempPath.update(getSelectedCellArrayPath().getDataContainerName(), getSelectedCellArrayPath().getAttributeMatrixName(), getNewCellArrayName() );
    m_NewCellArrayPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<DefaultPixelType>, AbstractFilter, DefaultPixelType>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if( NULL != m_NewCellArrayPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
    { m_NewCellArray = m_NewCellArrayPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  }
  else
  {
    m_NewCellArrayPtr = DataArray<DefaultPixelType>::NullPointer();
    m_NewCellArray = nullptr;
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_GaussianBlur::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  if (getCancel() == true) { return; }

  DataArrayPath dap = getSelectedCellArrayPath();
  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer( dap.getDataContainerName() );


  const unsigned int Dimension = 3;
  typedef DefaultPixelType PixelType;
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  typedef itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension> FilterType;
  const QString outputArrayName("Output Array");

  // The user has elected NOT to save the output as a new array but instead to do an "inplace"
  // filtering of the data. This means the final input data array will be replaced with the
  // filtered array
  if(getSaveAsNewArray() == false)
  {
    QVector<size_t> dims(1, 1);
    DataArrayPath tempPath = dap;
    tempPath.setDataArrayName(outputArrayName);
    m_NewCellArrayPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<DefaultPixelType>, AbstractFilter, DefaultPixelType>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
    if( NULL != m_NewCellArrayPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
    { m_NewCellArray = m_NewCellArrayPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  }

  // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(dc);
  filter->SetInPlace(true);
  filter->SetDataArrayPath(getSelectedCellArrayPath());
  filter->Update();
  ImageType::Pointer wrappedInputImage = filter->GetOutput();

  typedef itk::ImageRegionConstIterator<ImageType> ConstIteratorType;
  ImageType::RegionType filterRegion = wrappedInputImage->GetBufferedRegion();
  ConstIteratorType it(wrappedInputImage, filterRegion);
  //create guassian blur filter
  typedef itk::GaussianBlurImageFunction< ImageType > GFunctionType;
  GFunctionType::Pointer gaussianFunction = GFunctionType::New();
  gaussianFunction->SetInputImage( wrappedInputImage );

  //set guassian blur parameters
  GFunctionType::ErrorArrayType setError;
  setError.Fill( 0.01 );
  gaussianFunction->SetMaximumError( setError );
  gaussianFunction->SetSigma( 4 );
  gaussianFunction->SetMaximumKernelWidth( 5 );

  IGeometry::Pointer geometry = dc->getGeometry();
  size_t totalElements = geometry->getNumberOfElements();
  size_t increment = totalElements/100;
  size_t progress = 0;
  size_t subProgress = 0;

  //loop over image running filter
  it.GoToBegin();
  int index = 0;
  while( !it.IsAtEnd() )
  {
    m_NewCellArray[index] = gaussianFunction->EvaluateAtIndex(it.GetIndex());
    ++it;
    ++index;
    subProgress++;
    if(subProgress == increment)
    {
      progress++;
      subProgress = 0;
      QString str = QString("Blurring Complete %1%").arg(progress);
      notifyStatusMessage(getMessagePrefix(), getHumanLabel(), str);
      if(getCancel()) { break; }
    }
  }

  //array name changing/cleanup
  if(m_SaveAsNewArray == false)
  {
    AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(m_SelectedCellArrayPath.getAttributeMatrixName());
    // Remove the original input data array
    attrMat->removeAttributeArray(m_SelectedCellArrayPath.getDataArrayName());
    // Replace it with the output array by a rename.
    attrMat->renameAttributeArray(outputArrayName, m_SelectedCellArrayPath.getDataArrayName());
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer Itk_GaussianBlur::newFilterInstance(bool copyFilterParameters)
{
  Itk_GaussianBlur::Pointer filter = Itk_GaussianBlur::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_GaussianBlur::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_GaussianBlur::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_GaussianBlur::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_GaussianBlur::getGroupName()
{ return "Image Processing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_GaussianBlur::getSubGroupName()
{ return "ITKImageProcessing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_GaussianBlur::getHumanLabel()
{ return "[ITK] GaussianBlur (KW)"; }

