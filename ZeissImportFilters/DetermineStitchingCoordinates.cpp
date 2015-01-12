/*
 * Your License or Copyright Information can go here
 */

#include "DetermineStitchingCoordinates.h"

#include <QtCore/QString>

#include "ZeissImport/ZeissImportConstants.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DetermineStitchingCoordinates::DetermineStitchingCoordinates() :
  AbstractFilter(),
  m_AttributeMatrixName(DREAM3D::Defaults::VolumeDataContainerName, DREAM3D::Defaults::CellFeatureAttributeMatrixName, "")
/* DO NOT FORGET TO INITIALIZE ALL YOUR DREAM3D Filter Parameters HERE */
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DetermineStitchingCoordinates::~DetermineStitchingCoordinates()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinates::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(FilterParameter::New("Attribute Matrix Name", "AttributeMatrixName", FilterParameterWidgetType::AttributeMatrixSelectionWidget, getAttributeMatrixName(), false, ""));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinates::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setAttributeMatrixName(reader->readDataArrayPath("AttributeMatrixName", getAttributeMatrixName()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int DetermineStitchingCoordinates::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  DREAM3D_FILTER_WRITE_PARAMETER(AttributeMatrixName)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinates::dataCheck()
{
    setErrorCondition(0);
    DataArrayPath tempPath;

    QString ss;

    AttributeMatrix::Pointer am = getDataContainerArray()->getAttributeMatrix(m_AttributeMatrixName);

    if (am.get() == NULL)
    {
        setErrorCondition(-76000);
        notifyErrorMessage(getHumanLabel(), "The attribute matrix has not been selected properly", -76000);
        return;
    }

   QList<QString> names = am->getAttributeArrayNames();


    QVector<size_t> dims(1, 1);


    UInt8ArrayType::Pointer imagePtr = UInt8ArrayType::NullPointer();
    IDataArray::Pointer iDataArray = IDataArray::NullPointer();

    for(int i = 0; i < names.size(); i++)
    {
        tempPath.update(getAttributeMatrixName().getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
        iDataArray = getDataContainerArray()->getExistingPrereqArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, tempPath);
        QVector<IDataArray::Pointer> pointerList;

        pointerList[i] = iDataArray;

        imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(pointerList[i]);

        if(NULL == imagePtr)
        {
            setErrorCondition(-76001);
            notifyErrorMessage(getHumanLabel(), "The data was not found", -76001);
        }


    }


}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinates::preflight()
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
const QString DetermineStitchingCoordinates::getCompiledLibraryName()
{
  return ZeissImport::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DetermineStitchingCoordinates::getGroupName()
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DetermineStitchingCoordinates::getHumanLabel()
{
  return "DetermineStitchingCoordinates";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DetermineStitchingCoordinates::getSubGroupName()
{
  return "Misc";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinates::execute()
{
  int err = 0;
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCondition() < 0) { return; }
  setErrorCondition(0);

  /* Place all your code to execute your filter here. */

  /* If some error occurs this code snippet can report the error up the call chain*/
  if (err < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer DetermineStitchingCoordinates::newFilterInstance(bool copyFilterParameters)
{
  /*
  * write code to optionally copy the filter parameters from the current filter into the new instance
  */
  DetermineStitchingCoordinates::Pointer filter = DetermineStitchingCoordinates::New();
  if(true == copyFilterParameters)
  {
    /* If the filter uses all the standard Filter Parameter Widgets you can probabaly get
     * away with using this method to copy the filter parameters from the current instance
     * into the new instance
     */
    copyFilterParameterInstanceVariables(filter.get());
    /* If your filter is using a lot of custom FilterParameterWidgets @see ReadH5Ebsd then you
     * may need to copy each filter parameter explicitly plus any other instance variables that
     * are needed into the new instance. Here is some example code from ReadH5Ebsd
     */
    //    DREAM3D_COPY_INSTANCEVAR(OutputFile)
    //    DREAM3D_COPY_INSTANCEVAR(ZStartIndex)
    //    DREAM3D_COPY_INSTANCEVAR(ZEndIndex)
    //    DREAM3D_COPY_INSTANCEVAR(ZResolution)
  }
  return filter;
}

