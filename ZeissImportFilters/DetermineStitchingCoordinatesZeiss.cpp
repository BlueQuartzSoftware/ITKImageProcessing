/*
 * Your License or Copyright Information can go here
 */

#include "DetermineStitchingCoordinatesZeiss.h"

#include <QtCore/QString>

#include "ZeissImport/ZeissImportConstants.h"

#include "DREAM3DLib/Common/FilterManager.h"
#include "DREAM3DLib/Common/IFilterFactory.hpp"
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DetermineStitchingCoordinatesZeiss::DetermineStitchingCoordinatesZeiss() :
  AbstractFilter(),
  m_AttributeMatrixName(DREAM3D::Defaults::VolumeDataContainerName, DREAM3D::Defaults::CellFeatureAttributeMatrixName, "")
/* DO NOT FORGET TO INITIALIZE ALL YOUR DREAM3D Filter Parameters HERE */
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
DetermineStitchingCoordinatesZeiss::~DetermineStitchingCoordinatesZeiss()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinatesZeiss::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(FilterParameter::New("Attribute Matrix Name", "AttributeMatrixName", FilterParameterWidgetType::AttributeMatrixSelectionWidget, getAttributeMatrixName(), false, ""));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinatesZeiss::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setAttributeMatrixName(reader->readDataArrayPath("AttributeMatrixName", getAttributeMatrixName()));

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int DetermineStitchingCoordinatesZeiss::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  DREAM3D_FILTER_WRITE_PARAMETER(FilterVersion)
  DREAM3D_FILTER_WRITE_PARAMETER(AttributeMatrixName)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinatesZeiss::dataCheck()
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
    m_PointerList.resize(names.size());

    for(int i = 0; i < names.size(); i++)
    {
        tempPath.update(getAttributeMatrixName().getDataContainerName(), getAttributeMatrixName().getAttributeMatrixName(), names[i]);
        iDataArray = getDataContainerArray()->getExistingPrereqArrayFromPath<DataArray<uint8_t>, AbstractFilter>(this, tempPath);

        m_PointerList[i] = iDataArray;

        imagePtr = boost::dynamic_pointer_cast<DataArray<uint8_t> >(m_PointerList[i]);

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
void DetermineStitchingCoordinatesZeiss::preflight()
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
const QString DetermineStitchingCoordinatesZeiss::getCompiledLibraryName()
{
  return ZeissImport::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DetermineStitchingCoordinatesZeiss::getGroupName()
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DetermineStitchingCoordinatesZeiss::getHumanLabel()
{
  return "Determine Stitching Coordinates (Zeiss AxioVision)";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString DetermineStitchingCoordinatesZeiss::getSubGroupName()
{
  return "Misc";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DetermineStitchingCoordinatesZeiss::execute()
{
  int err = 0;
  dataCheck();
  if(getErrorCondition() < 0) { return; }
  setErrorCondition(0);

  QString filtName = "RGBToGray";
  FilterManager* fm = FilterManager::Instance();
  fm->printFactoryNames();
  IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);

  if (NULL != filterFactory.get() )
  {
      std::cout << "not null" << std::endl;
  }

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
AbstractFilter::Pointer DetermineStitchingCoordinatesZeiss::newFilterInstance(bool copyFilterParameters)
{
  /*
  * write code to optionally copy the filter parameters from the current filter into the new instance
  */
  DetermineStitchingCoordinatesZeiss::Pointer filter = DetermineStitchingCoordinatesZeiss::New();
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

