/*
 * Your License or Copyright can go here
 */

#include "ITKBinaryThreshold.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

#include "sitkExplicitITK.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DImage.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkDream3DFilterInterruption.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/Dream3DTemplateAliasMacro.h"

//thresholding filter
#include "itkBinaryThresholdImageFilter.h"

// Include the MOC generated file for this class
#include "moc_ITKBinaryThreshold.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryThreshold::ITKBinaryThreshold() :
  AbstractFilter(),
  m_SelectedCellArrayPath("", "", ""),
  m_NewCellArrayName(""),
  m_SaveAsNewArray(true),
  m_LowerThresholdValue(128.0),
  m_UpperThresholdValue(255.0),
  m_InsideValue(255.0),
  m_OutsideValue(0),
  m_NewCellArray(NULL)
{
  initialize();
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKBinaryThreshold::~ITKBinaryThreshold()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThreshold::setupFilterParameters()
{
  FilterParameterVector parameters;

  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Lower Threshold Value", LowerThresholdValue, FilterParameter::Parameter, ITKBinaryThreshold));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Upper Threshold Value", UpperThresholdValue, FilterParameter::Parameter, ITKBinaryThreshold));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Inside Value", InsideValue, FilterParameter::Parameter, ITKBinaryThreshold));
  parameters.push_back(SIMPL_NEW_DOUBLE_FP("Outisde Value", OutsideValue, FilterParameter::Parameter, ITKBinaryThreshold));
  QStringList linkedProps;
  linkedProps << "NewCellArrayName";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Save as New Array", SaveAsNewArray, FilterParameter::Parameter, ITKBinaryThreshold, linkedProps));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
      DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize,
      SIMPL::AttributeMatrixType::Cell, SIMPL::GeometryType::ImageGeometry);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to threshold", SelectedCellArrayPath, FilterParameter::RequiredArray, ITKBinaryThreshold, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Threshold Array", NewCellArrayName, FilterParameter::CreatedArray, ITKBinaryThreshold));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThreshold::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  setSaveAsNewArray( reader->readValue( "SaveAsNewArray", getSaveAsNewArray() ) );
  setLowerThresholdValue(reader->readValue("LowerThresholdValue", getLowerThresholdValue()));
  setUpperThresholdValue(reader->readValue("UpperThresholdValue", getUpperThresholdValue()));
  setInsideValue(reader->readValue("InsideValue", getInsideValue()));
  setOutsideValue(reader->readValue("OutsideValue", getOutsideValue()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKBinaryThreshold::initialize()
{
  setErrorCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType>
void ITKBinaryThreshold::CheckLimits(double value, QString name)
{
  if (value < static_cast<double>(std::numeric_limits<PixelType>::lowest())
     || value > static_cast<double>(std::numeric_limits<PixelType>::max()))
  {
    setErrorCondition(-1);
    QString errorMessage = name + QString("must be greater or equal than %1 and lesser or equal than %2");
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(
        std::numeric_limits<PixelType>::lowest()).arg(std::numeric_limits<PixelType>::max())
                                                         , getErrorCondition()
                                                         );
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename PixelType>
void ITKBinaryThreshold::dataCheck()
{
  setErrorCondition(0);
  // Check consistency of parameters
  CheckLimits<PixelType>(m_InsideValue, "Inside value");
  CheckLimits<PixelType>(m_OutsideValue, "Outside value");
  CheckLimits<PixelType>(m_LowerThresholdValue, "Lower threshold value");
  CheckLimits<PixelType>(m_UpperThresholdValue, "Upper threshold value");

  if (m_LowerThresholdValue > m_UpperThresholdValue)
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(),"Lower threshold value must be lesser than upper threshold value", getErrorCondition());
    return;
  }
  // Check data array
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
void ITKBinaryThreshold::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  Dream3DArraySwitchMacro(dataCheck, getSelectedCellArrayPath(), -4);// Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

template<typename PixelType>
void ITKBinaryThreshold::filter()
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

    //define threshold filters
    typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;

    //create Itk's binary threshold filter object
    BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
    thresholdFilter->SetInput(filter->GetOutput());
    thresholdFilter->SetLowerThreshold(static_cast<PixelType>(m_LowerThresholdValue));
    thresholdFilter->AddObserver(itk::ProgressEvent(), interruption);
    thresholdFilter->SetUpperThreshold(static_cast<PixelType>(m_UpperThresholdValue));
    thresholdFilter->SetInsideValue(static_cast<PixelType>(m_InsideValue));
    thresholdFilter->SetOutsideValue(static_cast<PixelType>(m_OutsideValue));
    thresholdFilter->Update();

    ImageType::Pointer image = ImageType::New();
    //image = thresholdFilter->GetOutput();
    image = thresholdFilter->GetOutput();
    image->DisconnectPipeline();
    std::string outputArrayName(getNewCellArrayName().toStdString());

    if (getSaveAsNewArray() == false)
    {
      outputArrayName = m_SelectedCellArrayPath.getDataArrayName().toStdString();
      AttributeMatrix::Pointer attrMat = dc->getAttributeMatrix(m_SelectedCellArrayPath.getAttributeMatrixName());
      // Remove the original input data array
      attrMat->removeAttributeArray(m_SelectedCellArrayPath.getDataArrayName());
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
void ITKBinaryThreshold::execute()
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
AbstractFilter::Pointer ITKBinaryThreshold::newFilterInstance(bool copyFilterParameters)
{
  ITKBinaryThreshold::Pointer filter = ITKBinaryThreshold::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThreshold::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThreshold::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThreshold::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThreshold::getGroupName()
{ return "Image Processing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThreshold::getSubGroupName()
{ return "ImageProcessing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKBinaryThreshold::getHumanLabel()
{ return "[ITK] Binary Threshold (KW)"; }

