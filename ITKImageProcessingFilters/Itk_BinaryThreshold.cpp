/*
 * Your License or Copyright can go here
 */

#include "Itk_BinaryThreshold.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"

//thresholding filter
#include "itkBinaryThresholdImageFilter.h"

// Include the MOC generated file for this class
#include "moc_Itk_BinaryThreshold.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Itk_BinaryThreshold::Itk_BinaryThreshold() :
  AbstractFilter(),
  m_SelectedCellArrayPath("", "", ""),
  m_NewCellArrayName(""),
  m_SaveAsNewArray(true),
  m_ManualParameter(128),
  m_SelectedCellArray(NULL),
  m_NewCellArray(NULL)
{
  initialize();
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Itk_BinaryThreshold::~Itk_BinaryThreshold()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_BinaryThreshold::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Threshold Value", ManualParameter, FilterParameter::Parameter, Itk_BinaryThreshold));
  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(LinkedBooleanFilterParameter::New("Save as New Array", "SaveAsNewArray", getSaveAsNewArray(), FilterParameter::Parameter, SIMPL_BIND_SETTER(Itk_BinaryThreshold, this, SaveAsNewArray), SIMPL_BIND_GETTER(Itk_BinaryThreshold, this, SaveAsNewArray), linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::UInt8, 1, SIMPL::AttributeMatrixObjectType::Any);
    parameters.push_back(DataArraySelectionFilterParameter::New("Attribute Array to Threshold", "SelectedCellArrayPath", getSelectedCellArrayPath(), FilterParameter::RequiredArray, SIMPL_BIND_SETTER(Itk_BinaryThreshold, this, SelectedCellArrayPath), SIMPL_BIND_GETTER(Itk_BinaryThreshold, this, SelectedCellArrayPath), req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(StringFilterParameter::New("Threshold Array", "NewCellArrayName", getNewCellArrayName(), FilterParameter::CreatedArray, SIMPL_BIND_SETTER(Itk_BinaryThreshold, this, NewCellArrayName), SIMPL_BIND_GETTER(Itk_BinaryThreshold, this, NewCellArrayName)));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_BinaryThreshold::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setManualParameter( reader->readValue( "ManualParameter", getManualParameter() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_BinaryThreshold::initialize()
{
  setErrorCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Itk_BinaryThreshold::dataCheck()
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
void Itk_BinaryThreshold::preflight()
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
void Itk_BinaryThreshold::execute()
{
  initialize();
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  if (getCancel() == true) { return; }


  DataArrayPath dap = getSelectedCellArrayPath();
  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer( dap.getDataContainerName() );
  AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(dap.getAttributeMatrixName());


  const unsigned int Dimension = 3;
  typedef DefaultPixelType PixelType;
  typedef itk::Dream3DImage<PixelType, Dimension> ImageType;
  typedef itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension> InPlaceDream3DDataToImageFilterType;
  const QString outputArrayName("Output Array");

  // The user has elected NOT to save the output as a new array but instead to do an "inplace"
  // filtering of the data. This means the final input data array will be replaced with the
  // filtered array
#if 1
  if(getSaveAsNewArray() == false)
  {
    QVector<size_t> dims(1, 1);
    DataArrayPath tempPath = dap;
    tempPath.setDataArrayName(outputArrayName);
    m_NewCellArrayPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<DefaultPixelType>, AbstractFilter, DefaultPixelType>(this, tempPath, 0, dims);
    if( NULL != m_NewCellArrayPtr.lock().get() )
    { m_NewCellArray = m_NewCellArrayPtr.lock()->getPointer(0); }
  }
#endif

  // Create a Bridge to wrap an existing DREAM.3D array with an ItkImage container
  InPlaceDream3DDataToImageFilterType::Pointer dataArrayToItkImage = InPlaceDream3DDataToImageFilterType::New();
  dataArrayToItkImage->SetInput(dc);
  dataArrayToItkImage->SetInPlace(false);
  dataArrayToItkImage->SetDataArrayPath(getSelectedCellArrayPath());

  //define threshold filters
  typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;

  //create Itk's binary threshold filter object
  BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
  thresholdFilter->SetInput(dataArrayToItkImage->GetOutput());
  thresholdFilter->SetLowerThreshold(m_ManualParameter);
  thresholdFilter->SetUpperThreshold(255);
  thresholdFilter->SetInsideValue(255);
  thresholdFilter->SetOutsideValue(0);
  thresholdFilter->GetOutput()->GetPixelContainer()->SetImportPointer(m_NewCellArray, m_NewCellArrayPtr.lock()->getNumberOfTuples(), false);
  // Execute the BinaryThreshold Filter
  try
  {
     thresholdFilter->Update();
  }
  catch( itk::ExceptionObject& err )
  {
    setErrorCondition(-5);
    QString ss = QObject::tr("Failed to execute itk::BinaryThreshold filter. Error Message returned from ITK:\n   %1").arg(err.GetDescription());
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
#if 0
  // Create converter to convert from Itk's Image Container back to the DataArray<> object
  const unsigned int dimension = 3;
  typedef itk::InPlaceImageToDream3DDataFilter<PixelType, dimension> InPlaceImageToDream3DDataFilterType;
  InPlaceImageToDream3DDataFilterType::Pointer imageToDataArray = InPlaceImageToDream3DDataFilterType::New();
  imageToDataArray->SetInput(thresholdFilter->GetOutput());
  imageToDataArray->SetInPlace(false);
  DataArrayPath tempPath = dap;
  if(getSaveAsNewArray()) {
    tempPath.setDataArrayName(getNewCellArrayName());
  }
  imageToDataArray->SetDataContainer(dc);
  imageToDataArray->SetDataArrayPath(tempPath);

  // We need to remove the array that was added in the DataCheck() function because the bridge will not add an array
  // if it already exists
  attrMat->removeAttributeArray(tempPath.getDataArrayName());


  // Execute the filter
  try
  {
    imageToDataArray->Update();
  }
  catch( itk::ExceptionObject& err )
  {
    setErrorCondition(-5);
    QString ss = QObject::tr("Failed to execute imageToDataArray filter. Error Message returned from ITK:\n   %1").arg(err.GetDescription());
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
#endif

  //array name changing/cleanup
#if 1
  if(m_SaveAsNewArray == false)
  {
    attrMat->removeAttributeArray(m_SelectedCellArrayPath.getDataArrayName());
    attrMat->renameAttributeArray(outputArrayName, m_SelectedCellArrayPath.getDataArrayName());
  }
#endif

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer Itk_BinaryThreshold::newFilterInstance(bool copyFilterParameters)
{
  Itk_BinaryThreshold::Pointer filter = Itk_BinaryThreshold::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_BinaryThreshold::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_BinaryThreshold::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_BinaryThreshold::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_BinaryThreshold::getGroupName()
{ return "Image Processing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_BinaryThreshold::getSubGroupName()
{ return "ImageProcessing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Itk_BinaryThreshold::getHumanLabel()
{ return "[ITK] Binary Threshold (KW)"; }

