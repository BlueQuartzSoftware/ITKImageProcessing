/*
 * Your License or Copyright Information can go here
 */

#include "CalculateBackground.h"

#include <string.h>

#include <set>

#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QTextStream>

#include "DREAM3DLib/Common/FilterManager.h"
#include "DREAM3DLib/Common/IFilterFactory.hpp"

#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissXml/ZeissTagMapping.h"

#define ZIF_PRINT_DBG_MSGS 0



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::CalculateBackground() :
  AbstractFilter(),
  m_CellAttributeMatrixName(DREAM3D::Defaults::CellAttributeMatrixName),
  m_ImageDataArrayName("ImageData"),
//  m_InputFile(""),
  m_DataContainerBundleName("")
//  m_DataContainerPrefix(k_DataContainePrefixDefaultName),
//  m_ImageDataArrayName(DREAM3D::CellData::ImageData)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CalculateBackground::~CalculateBackground()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::setupFilterParameters()
{
  FilterParameterVector parameters;
  //parameters.push_back(FileSystemFilterParameter::New("Input File", "InputFile", FilterParameterWidgetType::InputFileWidget, getInputFile(), false, "", "*.xml"));
//  parameters.push_back(FilterParameter::New("DataContainer Prefix", "DataContainerPrefix", FilterParameterWidgetType::StringWidget, getDataContainerPrefix(), false));
  parameters.push_back(FilterParameter::New("DataContainerBundle Name", "DataContainerBundleName", FilterParameterWidgetType::DataBundleSelectionWidget, getDataContainerBundleName(), true));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  //setInputFile(reader->readString("InputFile", getInputFile() ) );
  setDataContainerBundleName(reader->readString("DataContainerBundleName", getDataContainerBundleName() ) );
//  setDataContainerPrefix(reader->readString("DataContainerPrefix", getDataContainerPrefix() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int CalculateBackground::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
      DREAM3D_FILTER_WRITE_PARAMETER(DataContainerBundleName)
      writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::dataCheck()
{
  setErrorCondition(0);

  QString ss;


  if(getErrorCondition() < 0) { return; }





}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::preflight()
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
void CalculateBackground::execute()
{
  int err = 0;
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCondition() < 0) { return; }
  setErrorCondition(0);

  /* If some error occurs this code snippet can report the error up the call chain*/
  if (err < 0)
  {
    QString ss = QObject::tr("Error Importing a Zeiss AxioVision file set.");
    setErrorCondition(-90000);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }



  QVector<size_t> dims(1, 1);
//  m_FeatureIdsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>, AbstractFilter>(this, getFeatureIdsArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
//  if( NULL != m_FeatureIdsPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
//  { m_FeatureIds = m_FeatureIdsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */

//  parameters.push_back(FilterParameter::New("Array to Operate On", "SelectedCellArrayPath", FilterParameterWidgetType::DataArraySelectionWidget, getSelectedCellArrayPath(), false, ""));
//  VolumeDataContainer* m = getDataContainerArray()->getDataContainerAs<VolumeDataContainer>(m_FeatureIdsArrayPath.getDataContainerName());
  IDataContainerBundle::Pointer dcb = getDataContainerArray()->getDataContainerBundle(m_DataContainerBundleName);
  QVector<QString> dcList = dcb->getDataContainerNames();
  m_ImageDataArrayPath.update(dcList[0], "CellData", "ImageData");
  m_ImageDataPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int8_t>, AbstractFilter>(this, getImageDataArrayPath(), dims);
  if(NULL != m_ImageDataPtr.lock().get())
  {
      m_ImageData = m_ImageDataPtr.lock()->getPointer(0);
  }
  int64_t totalPoints = m_ImageDataPtr.lock()->getNumberOfTuples();





  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}







// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CalculateBackground::generateMetaDataAttributeMatrix(DataContainer* dc, ZeissTagsXmlSection::Pointer photoTagsSection)
{
  // Create the Meta Data AttributeMatrix
  QVector<size_t> attrDims(1, 1);
  AttributeMatrix::Pointer metaData = dc->createNonPrereqAttributeMatrix<AbstractFilter>(this, DataContainerBundle::GetMetaDataName(), attrDims, DREAM3D::AttributeMatrixType::MetaData);

  ZeissTagsXmlSection::MetaDataType tagMap = photoTagsSection->getMetaDataMap();
  QMapIterator<int, AbstractZeissMetaData::Pointer> iter(tagMap);
  while(iter.hasNext())
  {
    iter.next();
    IDataArray::Pointer dataArray = iter.value()->createDataArray(!getInPreflight());
    metaData->addAttributeArray(dataArray->getName(), dataArray);
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getCompiledLibraryName()
{
  return ZeissImport::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getGroupName()
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getHumanLabel()
{
  return "Calculate Background";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CalculateBackground::getSubGroupName()
{
  return "IO";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CalculateBackground::newFilterInstance(bool copyFilterParameters)
{
  /*
  * write code to optionally copy the filter parameters from the current filter into the new instance
  */
  CalculateBackground::Pointer filter = CalculateBackground::New();
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

