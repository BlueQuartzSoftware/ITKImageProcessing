/*
 * Your License or Copyright Information can go here
 */

#include "ZeissImportFilter.h"

#include <string.h>


#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QTextStream>


#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissXml/ZeissTagMapping.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportFilter::ZeissImportFilter() :
  AbstractFilter(),
  m_InputFile("")

{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportFilter::~ZeissImportFilter()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(FileSystemFilterParameter::New("Input File", "InputFile", FilterParameterWidgetType::InputFileWidget, getInputFile(), false, "", "*.xml"));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setInputFile(reader->readString("InputFile", getInputFile() ) );

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ZeissImportFilter::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  DREAM3D_FILTER_WRITE_PARAMETER(InputFile)

      writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::dataCheck()
{
  setErrorCondition(0);

  QString ss;
  QFileInfo fi(getInputFile());
  if (getInputFile().isEmpty() == true)
  {
    ss = QObject::tr("%1 needs the Input File Set and it was not.").arg(ClassName());
    setErrorCondition(-387);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
  else if (fi.exists() == false)
  {
    ss = QObject::tr("The input file does not exist.");
    setErrorCondition(-388);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  DataContainerArray::Pointer dca = getDataContainerArray();

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::preflight()
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
const QString ZeissImportFilter::getCompiledLibraryName()
{
  return ZeissImport::ZeissImportBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ZeissImportFilter::getGroupName()
{
  return "ZeissImport";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ZeissImportFilter::getHumanLabel()
{
  return "ZeissImportFilter";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ZeissImportFilter::getSubGroupName()
{
  return "Misc";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::execute()
{
  int err = 0;
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCondition() < 0) { return; }
  setErrorCondition(0);

  /* Place all your code to execute your filter here. */

  QFileInfo fi(m_InputFile);

  QFile xmlFile(m_InputFile);
  int success = readMetaXml(&xmlFile);



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
int ZeissImportFilter::readMetaXml(QIODevice* device)
{
  int err = 0;
  QDomDocument domDocument;
  QString errorStr;
  int errorLine;
  int errorColumn;
  if (!domDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn))   {
    QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
    setErrorCondition(-70000);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return -1;
  }
  QDomElement root = domDocument.documentElement();
  QString nodeName = root.nodeName();

  QDomElement tags = root.firstChildElement(ZeissImport::Xml::Tags);
  if (tags.isNull() == true)
  {
    QString ss = QObject::tr("Could not find the <ROOT><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file");
    setErrorCondition(-70001);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return -1;
  }

  // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
  ZeissTagsXmlSection::Pointer rootTagsSection = parseRootTagsSection(tags);
  if(NULL == rootTagsSection.get() )
  {
    return -1;
  }

  parseImages(root, rootTagsSection);

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissTagsXmlSection::Pointer ZeissImportFilter::parseRootTagsSection(QDomElement& tags)
{
  int err = 0;
  int count = -1;
  bool ok = false;
 // qDebug() << tags.nodeName() << " node type: " << tags.nodeType();

  QDomElement countEle = tags.firstChildElement(ZeissImport::Xml::Count);

  count = countEle.text().toInt(&ok, 10);
  if(!ok)
  {
    QString ss = QObject::tr("Error Parsing 'Count' Tag in Root 'Tags' DOM element");
    setErrorCondition(-70001);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return ZeissTagsXmlSection::NullPointer();
  }

  ZeissTagsXmlSection::Pointer rootTagsSection = ZeissTagsXmlSection::New();
  ZeissTagMapping::Pointer tagMapping = ZeissTagMapping::instance();

  for(int c = 0; c < count; c++)
  {
    QString Vx = QString("V%1").arg(c);
    QString Ix = QString("I%1").arg(c);
    QString Ax = QString("A%1").arg(c);

    QDomElement vxEle = tags.firstChildElement(Vx);
    QDomElement ixEle = tags.firstChildElement(Ix);
//    QDomElement axEle = tags.firstChildElement(Ax); // Not sure what this value is used for

//    qDebug() << "<" << Vx << ">" << vxEle.text();
//    qDebug() << "<" << Ix << ">" << ixEle.text();
//    qDebug() << "<" << Ax << ">" << axEle.text();


    qint32 idValue =  ixEle.text().toInt(&ok, 10);
    AbstractZeissMetaData::Pointer ptr = ZeissTagMapping::instance()->metaDataForId(idValue, vxEle.text());
    if (NULL != ptr.get() && vxEle.text().size() > 0)
    {
        rootTagsSection->addMetaDataEntry(ptr);
    }
    else
    {
      QString str;
      QTextStream ss(&str);
      ss << "<" << Ix << ">" << idValue << "</" << Ix << "> is Unknown to the Tag Mapping Software";
      qDebug() << str;
    }

  }
  #if 0
  qDebug() << "Parsed Root Tags Count: " << rootTagsSection->count();
  AbstractZeissMetaData::Pointer ptr = rootTagsSection->getEntry(Zeiss::MetaXML::ImageCountUId);

  Int32ZeissMetaEntry::Pointer imageCountU = boost::dynamic_pointer_cast<Int32ZeissMetaEntry>(ptr);
  qDebug() << "Horizontal Image Count: " << imageCountU->getValue();

  ptr = rootTagsSection->getEntry(Zeiss::MetaXML::ImageCountVId);

  Int32ZeissMetaEntry::Pointer imageCountV = boost::dynamic_pointer_cast<Int32ZeissMetaEntry>(ptr);
  qDebug() << "Vertical Image Count: " << imageCountV->getValue();


  ptr = rootTagsSection->getEntry(Zeiss::MetaXML::DocumentNameId);
  StringZeissMetaEntry::Pointer docId = boost::dynamic_pointer_cast<StringZeissMetaEntry>(ptr);
  QString fileName = docId->getValue();
  int pos = fileName.lastIndexOf('.');
  fileName = QString("/tmp/") + fileName.left(pos) + QString(".h5");

  hid_t fileId = QH5Utilities::createFile(fileName);
  int32_t junk = 666;
  H5Lite::writeScalarDataset(fileId, "RootTags", junk);

  ZeissTagsXmlSection::MetaDataType tagMap = rootTagsSection->getMetaDataMap();
  QMapIterator<int, AbstractZeissMetaData::Pointer> iter(tagMap);
  while(iter.hasNext())
  {
     iter.next();
     iter.value()->writeHDF5Attribute(fileId, "RootTags");
  }
  H5Utilities::closeFile(fileId);
#endif

  return rootTagsSection;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::parseImages(QDomElement& root, ZeissTagsXmlSection::Pointer rootTagsSection)
{
  AbstractZeissMetaData::Pointer ptr = rootTagsSection->getEntry(Zeiss::MetaXML::ImageCountUId);

  ptr = rootTagsSection->getEntry(Zeiss::MetaXML::ImageCountRawId);
  Int32ZeissMetaEntry::Pointer imageCountPtr = boost::dynamic_pointer_cast<Int32ZeissMetaEntry>(ptr);
  qint32 imageCount = imageCountPtr->getValue();

  int zeroPadding = 0;
  if(imageCount > 0) { zeroPadding++; }
  if(imageCount > 9) { zeroPadding++; }
  if(imageCount > 99) { zeroPadding++; }
  if(imageCount > 999) { zeroPadding++; }
  if(imageCount > 9999) { zeroPadding++; }

  for(int p = 0; p < imageCount; p++)
  {
    QString pTag;
    QTextStream out(&pTag);
    out << "p";
    out.setFieldWidth(zeroPadding);
    out.setFieldAlignment(QTextStream::AlignRight);
    out.setPadChar('0');
    out << p;

    QDomElement photoEle = root.firstChildElement(pTag);
    if(photoEle.isNull())
    {
      QString ss = QObject::tr("Could not find the <ROOT><%1> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-70002);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }
    QDomElement tags = photoEle.firstChildElement(ZeissImport::Xml::Tags);
    if (tags.isNull() == true)
    {
      QString ss = QObject::tr("Could not find the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-70003);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
    ZeissTagsXmlSection::Pointer photoTagsSection = parseRootTagsSection(tags);
    if(NULL == photoTagsSection.get() )
    {
      QString ss = QObject::tr("Error Parsing the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-70003);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }
    AbstractZeissMetaData::Pointer ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageIndexPId);
    Int32ZeissMetaEntry::Pointer int32Entry = boost::dynamic_pointer_cast<Int32ZeissMetaEntry>(ptr);

    // Create the Data Container
    DataContainerArray::Pointer dca = getDataContainerArray();
    VolumeDataContainer* dc = dca->createNonPrereqDataContainer<VolumeDataContainer>(this, pTag);

    //Get the Image Width and Height for the DataContainer
    setDataContainerDims(dc, photoTagsSection);
    // Generate all the Meta Data Values: We should populate the BundleArrayNames at this point also.
    generateMetaDataAttributeMatrix(dc, photoTagsSection);

  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::generateMetaDataAttributeMatrix(VolumeDataContainer* dc, ZeissTagsXmlSection::Pointer photoTagsSection)
{
  // Create the Meta Data AttributeMatrix
  QVector<size_t> attrDims(1, 1);
  AttributeMatrix::Pointer metaData = dc->createNonPrereqAttributeMatrix<AbstractFilter>(this, DataContainerBundle::GetMetaDataName(), attrDims, DREAM3D::AttributeMatrixType::MetaData);


  ZeissTagsXmlSection::MetaDataType tagMap = photoTagsSection->getMetaDataMap();
  QMapIterator<int, AbstractZeissMetaData::Pointer> iter(tagMap);
  while(iter.hasNext())
  {
     iter.next();
     IDataArray::Pointer dataArray = iter.value()->createDataArray();
     metaData->addAttributeArray(dataArray->getName(), dataArray);
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::setDataContainerDims(VolumeDataContainer* dc, ZeissTagsXmlSection::Pointer photoTagsSection)
{
  AbstractZeissMetaData::Pointer ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageWidthId);
  Int32ZeissMetaEntry::Pointer int32Entry = boost::dynamic_pointer_cast<Int32ZeissMetaEntry>(ptr);

  size_t dims[3] = {0,0,1};
  dims[0] = int32Entry->getValue();

  ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageHeightId);
  int32Entry = boost::dynamic_pointer_cast<Int32ZeissMetaEntry>(ptr);
  dims[1] = int32Entry->getValue();
  dc->setDimensions(dims);


}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ZeissImportFilter::newFilterInstance(bool copyFilterParameters)
{
  /*
  * write code to optionally copy the filter parameters from the current filter into the new instance
  */
  ZeissImportFilter::Pointer filter = ZeissImportFilter::New();
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

