/*
 * Your License or Copyright Information can go here
 */

#include "ZeissImportFilter.h"

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
#include "DREAM3DLib/DataArrays/StringDataArray.hpp"
#include "DREAM3DLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "DREAM3DLib/FilterParameters/AbstractFilterParametersReader.h"
#include "DREAM3DLib/FilterParameters/FileSystemFilterParameter.h"
#include "DREAM3DLib/FilterParameters/LinkedBooleanFilterParameter.h"

#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissXml/ZeissTagMapping.h"

#define ZIF_PRINT_DBG_MSGS 0

static const QString k_AttributeArrayNames("AttributeArrayNames");
static const QString k_DataContaineNameDefaultName("Zeiss Axio Vision Montage");
static const QString k_TileAttributeMatrixDefaultName("Tile AttributeMatrix");
static const QString k_GrayScaleTempArrayName("gray_scale_temp");

/* ############## Start Private Implementation ############################### */
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ZeissImportFilterPrivate
{
  Q_DISABLE_COPY(ZeissImportFilterPrivate)
    Q_DECLARE_PUBLIC(ZeissImportFilter)
    ZeissImportFilter* const q_ptr;
  ZeissImportFilterPrivate(ZeissImportFilter* ptr);

  QDomElement m_Root;
  ZeissTagsXmlSection::Pointer m_RootTagsSection;
  QString m_InputFile_Cache;
  QDateTime m_LastRead;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportFilterPrivate::ZeissImportFilterPrivate(ZeissImportFilter* ptr) :
q_ptr(ptr),
m_InputFile_Cache(""),
m_LastRead(QDateTime()),
m_Root(QDomElement())
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportFilter::ZeissImportFilter() :
  AbstractFilter(),
  m_InputFile(""),
  m_FileWasRead(false),
  m_DataContainerName(k_DataContaineNameDefaultName),
  m_ImageDataArrayPrefix(DREAM3D::CellData::ImageData),
  m_ImageAttributeMatrixName(k_TileAttributeMatrixDefaultName),
  d_ptr(new ZeissImportFilterPrivate(this))
{
  m_ColorWeights.x = 0.2125f;
  m_ColorWeights.y = 0.7154f;
  m_ColorWeights.z = 0.0721f;
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissImportFilter::~ZeissImportFilter()
{
}

DREAM3D_PIMPL_PROPERTY_DEF(ZeissImportFilter, QString, InputFile_Cache)
DREAM3D_PIMPL_PROPERTY_DEF(ZeissImportFilter, QDateTime, LastRead)
DREAM3D_PIMPL_PROPERTY_DEF(ZeissImportFilter, QDomElement, Root)
DREAM3D_PIMPL_PROPERTY_DEF(ZeissImportFilter, ZeissTagsXmlSection::Pointer, RootTagsSection)


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(FileSystemFilterParameter::New("Input File", "InputFile", FilterParameterWidgetType::InputFileWidget, getInputFile(), false, "", "*.xml"));
  parameters.push_back(FilterParameter::New("DataContainer Name", "DataContainerName", FilterParameterWidgetType::StringWidget, getDataContainerName(), false));
  parameters.push_back(FilterParameter::New("Attribute Matrix Name", "ImageAttributeMatrixName", FilterParameterWidgetType::StringWidget, getImageAttributeMatrixName(), false));

  QStringList linkedProps("ColorWeights");
  parameters.push_back(LinkedBooleanFilterParameter::New("Convert To GrayScale", "ConvertToGrayScale", getConvertToGrayScale(), linkedProps, false));
  parameters.push_back(FilterParameter::New("Color Weighting", "ColorWeights", FilterParameterWidgetType::FloatVec3Widget, getColorWeights(), false));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setInputFile(reader->readString("InputFile", getInputFile() ) );
  setDataContainerName(reader->readString("DataContainerName", getDataContainerName() ) );
  setImageAttributeMatrixName(reader->readString("ImageAttributeMatrixName", getImageAttributeMatrixName() ) );
  setImageDataArrayPrefix(reader->readString("ImageDataArrayPrefix", getImageDataArrayPrefix() ) );
  setConvertToGrayScale(reader->readValue("ConvertToGrayScale", getConvertToGrayScale() ) );
  setColorWeights( reader->readFloatVec3("ColorWeights", getColorWeights() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ZeissImportFilter::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  DREAM3D_FILTER_WRITE_PARAMETER(FilterVersion)
  DREAM3D_FILTER_WRITE_PARAMETER(InputFile)
      DREAM3D_FILTER_WRITE_PARAMETER(DataContainerName)
      DREAM3D_FILTER_WRITE_PARAMETER(ImageAttributeMatrixName)
      DREAM3D_FILTER_WRITE_PARAMETER(ImageDataArrayPrefix)
      DREAM3D_FILTER_WRITE_PARAMETER(ConvertToGrayScale)
      DREAM3D_FILTER_WRITE_PARAMETER(ColorWeights)
      writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::dataCheck()
{
  qDebug() << "Begin: " << QDateTime::currentDateTime().toString();
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


  QString filtName = "ReadImage";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
  if (NULL != filterFactory.get() )
  {
    // If we get this far, the Factory is good so creating the filter should not fail unless something has
    // horribly gone wrong in which case the system is going to come down quickly after this.
    AbstractFilter::Pointer filter = filterFactory->create();
    if(NULL == filter.get())
    {
      ss = QObject::tr("The 'ReadImage' filter is not Available, did the ImageProcessing Plugin Load.");
      setErrorCondition(-391);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());

    }
  }
  if(getConvertToGrayScale())
  {
    QString filtName = "RGBToGray";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if (NULL != filterFactory.get() )
    {
      // If we get this far, the Factory is good so creating the filter should not fail unless something has
      // horribly gone wrong in which case the system is going to come down quickly after this.
      AbstractFilter::Pointer filter = filterFactory->create();
      if(NULL == filter.get())
      {
        ss = QObject::tr("The 'RGBToGray' filter is not Available, did the ImageProcessing Plugin Load.");
        setErrorCondition(-391);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      }
    }
  }
  if(getErrorCondition() < 0) { return; }

  DataContainerArray::Pointer dca = getDataContainerArray();
  if(NULL == dca.get())
  {
    ss = QObject::tr("%1 needs a valid DataContainerArray").arg(ClassName());
    setErrorCondition(-390);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }
  DataContainer::Pointer m = dca->createNonPrereqDataContainer<AbstractFilter>(this, getDataContainerName());
  if(getErrorCondition() < 0) { return; }

  // Parse the XML file to get all the meta-data information and create all the
  // data structure that is needed.
  QFile xmlFile(getInputFile());
  int success = readMetaXml(&xmlFile);
  if(success < 0)
  {
    ss = QObject::tr("Could not parse Zeiss XML file");
    setErrorCondition(-389);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  qDebug() << "End: " << QDateTime::currentDateTime().toString();
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
void ZeissImportFilter::execute()
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

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ZeissImportFilter::readMetaXml(QIODevice* device)
{
  int err = 0;
  QString errorStr;
  int errorLine;
  int errorColumn;

  QFileInfo fi(getInputFile());
  QDateTime lastModified(fi.lastModified());

  QDomDocument domDocument;
  QDomElement root;
  ZeissTagsXmlSection::Pointer rootTagsSection;

  if (getInputFile() == getInputFile_Cache() && getLastRead().isValid() && lastModified.msecsTo(getLastRead()) >= 0)
  {
    // We are reading from the cache, so set the FileWasRead flag to false
    m_FileWasRead = false;

    root = getRoot();
    rootTagsSection = getRootTagsSection();
  }
  else
  {
    // We are reading from the file, so set the FileWasRead flag to true
    m_FileWasRead = true;

    if (!domDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn))   {
      QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
      setErrorCondition(-70000);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return -1;
    }

    root = domDocument.documentElement();

    QDomElement tags = root.firstChildElement(ZeissImportConstants::Xml::Tags);
    if (tags.isNull() == true)
    {
      QString ss = QObject::tr("Could not find the <ROOT><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file");
      setErrorCondition(-70001);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return -1;
    }

    // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
    rootTagsSection = parseTagsSection(tags);
    if (NULL == rootTagsSection.get())
    {
      return -1;
    }

    // Set the data into the cache
    setRootTagsSection(rootTagsSection);

    QDomElement rootCopy = root.cloneNode().toElement();
    setRoot(rootCopy);

    // Set the file path and time stamp into the cache
    setLastRead(QDateTime::currentDateTime());
    setInputFile_Cache(getInputFile());
  }

  // Now parse each of the <pXXX> tags
  parseImages(root, rootTagsSection);

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissTagsXmlSection::Pointer ZeissImportFilter::parseTagsSection(QDomElement& tags)
{
  int count = -1;
  bool ok = false;
  // qDebug() << tags.nodeName() << " node type: " << tags.nodeType();

  QDomElement countEle = tags.firstChildElement(ZeissImportConstants::Xml::Count);

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

  std::set<int> unknownTags;

  for(int c = 0; c < count; c++)
  {
    QString Vx = QString("V%1").arg(c);
    QString Ix = QString("I%1").arg(c);
    QString Ax = QString("A%1").arg(c);

    QDomElement vxEle = tags.firstChildElement(Vx);
    QDomElement ixEle = tags.firstChildElement(Ix);

    qint32 idValue =  ixEle.text().toInt(&ok, 10);
    AbstractZeissMetaData::Pointer ptr = ZeissTagMapping::instance()->metaDataForId(idValue, vxEle.text());
    if (NULL != ptr.get() && vxEle.text().size() > 0)
    {
      rootTagsSection->addMetaDataEntry(ptr);
    }
#if ZIF_PRINT_DBG_MSGS
    else
    {
      unknownTags.insert(idValue);
      //      QString str;
      //      QTextStream ss(&str);
      //      ss << "<" << Ix << ">" << idValue << "</" << Ix << "> is Unknown to the Tag Mapping Software";
      //      qDebug() << str;
    }
#endif
  }

#if ZIF_PRINT_DBG_MSGS

  if(unknownTags.size() > 0)
  {
    QString str;
    QTextStream ss(&str);
    ss << "======= Unknown Zeiss Axio Vision _Meta XML Tags ===================\n";
    for(std::set<int>::iterator iter = unknownTags.begin(); iter != unknownTags.end(); ++iter)
    {
      ss <<  *iter << " is Unknown to the Tag Mapping Software\n";
    }
    qDebug() << str;
  }
#endif

  return rootTagsSection;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::parseImages(QDomElement& root, ZeissTagsXmlSection::Pointer rootTagsSection)
{
  AbstractZeissMetaData::Pointer ptr = rootTagsSection->getEntry(Zeiss::MetaXML::ImageCountRawId);

  Int32ZeissMetaEntry::Pointer imageCountPtr = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
  Q_ASSERT_X(imageCountPtr.get() != NULL, "Could not Cast to Int32ZeissMetaEntry", "");

  qint32 imageCount = imageCountPtr->getValue();

  ptr = rootTagsSection->getEntry(Zeiss::MetaXML::FilenameId);
  StringZeissMetaEntry::Pointer imageNamePtr = boost::dynamic_pointer_cast<StringZeissMetaEntry>(ptr);
  QString imageName = imageNamePtr->getValue();

  int zeroPadding = 0;
  if(imageCount > 0) { zeroPadding++; }
  if(imageCount > 9) { zeroPadding++; }
  if(imageCount > 99) { zeroPadding++; }
  if(imageCount > 999) { zeroPadding++; }
  if(imageCount > 9999) { zeroPadding++; }


  DataContainerArray::Pointer dca = getDataContainerArray();

  DataContainer::Pointer dc = dca->getDataContainer(getDataContainerName());

  AttributeMatrix::Pointer tileAm = AttributeMatrix::NullPointer();
  AttributeMatrix::Pointer metaAm = AttributeMatrix::NullPointer();


  // Create the Image Geometry
  ImageGeom::Pointer image = ImageGeom::CreateGeometry(DREAM3D::Geometry::ImageGeometry);
  dc->setGeometry(image);
  float origin[3] = { 0.0f, 0.0f, 0.0f};
  image->setOrigin(origin);
  float resolution[3] = { 1.0f, 1.0f, 1.0f};
  image->setResolution(resolution);

  StringDataArray::Pointer attributeArrayNames = StringDataArray::NullPointer();

  for(int p = 0; p < imageCount; p++)
  {

    // Generate the xml tag that is for this image
    QString pTag;
    QTextStream out(&pTag);
    out << "p";
    out.setFieldWidth(zeroPadding);
    out.setFieldAlignment(QTextStream::AlignRight);
    out.setPadChar('0');
    out << p;

    // Send a status update on the progress
    QString msg = QString("%1: Importing file %2 of %3").arg(getHumanLabel()).arg(p).arg(imageCount);
    notifyStatusMessage(getHumanLabel(), msg);


    QDomElement photoEle = root.firstChildElement(pTag);
    if(photoEle.isNull())
    {
      QString ss = QObject::tr("Could not find the <ROOT><%1> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-70002);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }
    QDomElement tags = photoEle.firstChildElement(ZeissImportConstants::Xml::Tags);
    if (tags.isNull() == true)
    {
      QString ss = QObject::tr("Could not find the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-70003);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }


    ZeissTagsXmlSection::Pointer photoTagsSection = parseTagsSection(tags);
    if(NULL == photoTagsSection.get() )
    {
      QString ss = QObject::tr("Error Parsing the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
      setErrorCondition(-70004);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    AbstractZeissMetaData::Pointer ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageTileIndexId);

    Int32ZeissMetaEntry::Pointer int32Entry = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
    Q_ASSERT_X(int32Entry.get() != NULL, "Could not Cast to Int32ZeissMetaEntry", "");

    if(p == 0)
    {
      QVector<size_t> dims = getImageDimensions(photoTagsSection);
      tileAm = dc->createAndAddAttributeMatrix(dims, getImageAttributeMatrixName(), DREAM3D::AttributeMatrixType::Cell);
      image->setDimensions(dims[0], dims[1], 1);

      dims.resize(1);
      dims[0] = imageCount;
      QString metaName = getImageAttributeMatrixName() + DREAM3D::StringConstants::MetaData;
      metaAm = dc->createAndAddAttributeMatrix(dims, metaName, DREAM3D::AttributeMatrixType::Generic);
      ZeissTagsXmlSection::MetaDataType tagMap = photoTagsSection->getMetaDataMap();
      QMapIterator<int, AbstractZeissMetaData::Pointer> iter(tagMap);
      while(iter.hasNext())
      {
        iter.next();
        IDataArray::Pointer dataArray = iter.value()->createDataArray(!getInPreflight());
        dataArray->resize(imageCount);
        metaAm->addAttributeArray(dataArray->getName(), dataArray);
      }

      // Add an extra AttributeArray that stores the names of the actual AttributeArray names that holds the image data
      attributeArrayNames = StringDataArray::CreateArray(imageCount, k_AttributeArrayNames);
      metaAm->addAttributeArray(attributeArrayNames->getName(), attributeArrayNames);

      ZeissTagsXmlSection::MetaDataType tagMapRoot = rootTagsSection->getMetaDataMap();
      QMapIterator<int, AbstractZeissMetaData::Pointer> iterRoot(tagMapRoot);


      while(iterRoot.hasNext())
      {
        iterRoot.next();
        IDataArray::Pointer dataArrayRoot = iterRoot.value()->createDataArray(!getInPreflight());
        dataArrayRoot->resize(imageCount);
        if (dataArrayRoot->getName() == "ScaleFactorForX" || dataArrayRoot->getName()  == "ScaleFactorForY")
        {
            metaAm->addAttributeArray(dataArrayRoot->getName(), dataArrayRoot);
        }
      }
    }
    // Generate all the Meta Data Values:
    addMetaData(metaAm, photoTagsSection, p);

    addRootMetaData(metaAm, rootTagsSection, p);

    // Read the image into a data array
    importImage(imageName, pTag, dc->getName(), p, attributeArrayNames);

    if(getConvertToGrayScale())
    {
      convertToGrayScale(imageName, pTag, dc->getName());
    }
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::addMetaData(AttributeMatrix::Pointer metaAm, ZeissTagsXmlSection::Pointer photoTagsSection, int index)
{
  //qDebug() << "-------\np" << index;
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();

  ZeissTagsXmlSection::MetaDataType tags = photoTagsSection->getMetaDataMap();

  QMapIterator<int, AbstractZeissMetaData::Pointer> iter(tags);
  while(iter.hasNext())
  {
    iter.next();
    StringZeissMetaEntry::Pointer zStrVal = boost::dynamic_pointer_cast<StringZeissMetaEntry>(iter.value());

    //qDebug() << iter.key() << "  " << zStrVal->getValue();
    QString tagName = tagMap->nameForId(iter.key());
    IDataArray::Pointer iDataArray = metaAm->getAttributeArray(tagName);
    StringDataArray::Pointer strArray = boost::dynamic_pointer_cast<StringDataArray>(iDataArray);
    strArray->setValue(index, zStrVal->getValue());
    //IDataArray::Pointer dataArray = iter.value()->createDataArray(!getInPreflight());
    //metaData->addAttributeArray(dataArray->getName(), dataArray);
  }
}

void ZeissImportFilter::addRootMetaData(AttributeMatrix::Pointer metaAm, ZeissTagsXmlSection::Pointer rootTagsSection, int index)
{
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();
    ZeissTagsXmlSection::MetaDataType tags = rootTagsSection->getMetaDataMap();

    QMapIterator<int, AbstractZeissMetaData::Pointer> iter(tags);
    while(iter.hasNext())
    {
      iter.next();
      StringZeissMetaEntry::Pointer zStrVal = boost::dynamic_pointer_cast<StringZeissMetaEntry>(iter.value());

          //qDebug() << iter.key() << "  " << zStrVal->getValue();
      QString tagName = tagMap->nameForId(iter.key());
      if (tagName == "ScaleFactorForX" || tagName == "ScaleFactorForY")
      {
          IDataArray::Pointer iDataArray = metaAm->getAttributeArray(tagName);
          StringDataArray::Pointer strArray = boost::dynamic_pointer_cast<StringDataArray>(iDataArray);
          strArray->setValue(index, zStrVal->getValue());
          //IDataArray::Pointer dataArray = iter.value()->createDataArray(!getInPreflight());
          //metaData->addAttributeArray(dataArray->getName(), dataArray);
      }
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissImportFilter::importImage(const QString &imageName, const QString &pTag,
                                    const QString &dcName, int imageIndex, StringDataArray::Pointer attributeArrayNames)
{

  QFileInfo fi(imageName);
  QString imagePath = fi.completeBaseName() + "_" + pTag + "." + fi.suffix();
  QString dataArrayName = fi.completeBaseName() + "_" + pTag;

  //Add this Array name to the DataArray that will ensure the correct ordering
  attributeArrayNames->setValue(imageIndex, dataArrayName);

  fi = QFileInfo(getInputFile());
  imagePath = fi.absoluteDir().path() + "/" + imagePath;
  //   std::string sPath = imagePath.toStdString();

  QString filtName = "ReadImage";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
  if (NULL != filterFactory.get() )
  {
    // If we get this far, the Factory is good so creating the filter should not fail unless something has
    // horribly gone wrong in which case the system is going to come down quickly after this.
    AbstractFilter::Pointer filter = filterFactory->create();

    // Connect up the Error/Warning/Progress object so the filter can report those things
    connect(filter.get(), SIGNAL(filterGeneratedMessage(const PipelineMessage&)),
            this, SLOT(broadcastPipelineMessage(const PipelineMessage&)));
    filter->setMessagePrefix(getMessagePrefix());
    filter->setDataContainerArray(getDataContainerArray()); // AbstractFilter implements this so no problem

    bool propWasSet = filter->setProperty("InputFileName", imagePath);
    if(false == propWasSet)
    {
      QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a Q_PROPERTY macro. Please notify the developers.")
          .arg("InputFileName").arg(filtName).arg(getHumanLabel());
      setErrorCondition(-70005);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    }

    propWasSet = filter->setProperty("DataContainerName", dcName);
    if(false == propWasSet)
    {
      QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a Q_PROPERTY macro. Please notify the developers.")
          .arg("DataContainerName").arg(filtName).arg(getHumanLabel());
      setErrorCondition(-70006);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    }
    propWasSet = filter->setProperty("CellAttributeMatrixName", getImageAttributeMatrixName());
    if(false == propWasSet)
    {
      QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a Q_PROPERTY macro. Please notify the developers.")
          .arg("CellAttributeMatrixName").arg(filtName).arg(getHumanLabel());
      setErrorCondition(-70007);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    }
    propWasSet = filter->setProperty("ImageDataArrayName", dataArrayName);
    if(false == propWasSet)
    {
      QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a Q_PROPERTY macro. Please notify the developers.")
          .arg("ImageDataArrayName").arg(filtName).arg(getHumanLabel());
      setErrorCondition(-70008);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    }
    if(getInPreflight() == true)
    {
      filter->preflight();
    }
    else
    {
      filter->execute();
    }

  }
  else {
    QString ss = QObject::tr("Error trying to instantiate the 'ReadImage' filter which is typically included in the 'ImageProcessing' plugin.");
    setErrorCondition(-70009);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

}


// -----------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
void ZeissImportFilter::convertToGrayScale(const QString &imageName, const QString &pTag, const QString &dcName)
{
  QFileInfo fi(imageName);
  QString dataArrayName = fi.completeBaseName() + "_" + pTag;

  QString filtName = "RGBToGray";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
  if (NULL != filterFactory.get() )
  {
    // If we get this far, the Factory is good so creating the filter should not fail unless something has
    // horribly gone wrong in which case the system is going to come down quickly after this.
    AbstractFilter::Pointer filter = filterFactory->create();

    // Connect up the Error/Warning/Progress object so the filter can report those things
    connect(filter.get(), SIGNAL(filterGeneratedMessage(const PipelineMessage&)),
            this, SLOT(broadcastPipelineMessage(const PipelineMessage&)));
    filter->setMessagePrefix(getMessagePrefix());
    filter->setDataContainerArray(getDataContainerArray()); // AbstractFilter implements this so no problem

    DataArrayPath dap(getDataContainerName(), getImageAttributeMatrixName(), dataArrayName);

    QVariant variant;
    variant.setValue(dap);
    bool propWasSet = filter->setProperty("SelectedCellArrayArrayPath", variant);
    if(false == propWasSet)
    {
      QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a Q_PROPERTY macro. Please notify the developers.")
          .arg("InputFileName").arg(filtName).arg(getHumanLabel());
      setErrorCondition(-70005);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    }

    variant.setValue(getColorWeights());
    propWasSet = filter->setProperty("ColorWeights", variant);
    if(false == propWasSet)
    {
      QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a Q_PROPERTY macro. Please notify the developers.")
          .arg("DataContainerName").arg(filtName).arg(getHumanLabel());
      setErrorCondition(-70006);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    }
    propWasSet = filter->setProperty("NewCellArrayName", k_GrayScaleTempArrayName);
    if(false == propWasSet)
    {
      QString ss = QObject::tr("Error Setting Property '%1' into filter '%2' which is a subfilter called by %3. The property was not set which could mean the property was not exposed with a Q_PROPERTY macro. Please notify the developers.")
          .arg("CellAttributeMatrixName").arg(filtName).arg(getHumanLabel());
      setErrorCondition(-70007);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    }
    if(getInPreflight() == true)
    {
      filter->preflight();

    }
    else
    {
      filter->execute();
    }

    if(filter->getErrorCondition() < 0)
    {

    }
    else {
      DataContainerArray::Pointer dca = getDataContainerArray();
      DataContainer::Pointer dc = dca->getDataContainer(getDataContainerName());
      AttributeMatrix::Pointer am = dc->getAttributeMatrix(getImageAttributeMatrixName());
      IDataArray::Pointer rgb = am->removeAttributeArray(dataArrayName);
      IDataArray::Pointer gray = am->removeAttributeArray(k_GrayScaleTempArrayName);
      gray->setName(rgb->getName());
      am->addAttributeArray(gray->getName(), gray);
    }
  }
  else {
    QString ss = QObject::tr("Error trying to instantiate the 'ReadImage' filter which is typically included in the 'ImageProcessing' plugin.");
    setErrorCondition(-70009);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVector<size_t> ZeissImportFilter::getImageDimensions(ZeissTagsXmlSection::Pointer photoTagsSection)
{
  QVector<size_t> dims(3);
  AbstractZeissMetaData::Pointer ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageWidthPixelId);
  Int32ZeissMetaEntry::Pointer int32Entry = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
  Q_ASSERT_X(int32Entry.get() != NULL, "Could not Cast to Int32ZeissMetaEntry", "");

  dims[0] = int32Entry->getValue();

  ptr = photoTagsSection->getEntry(Zeiss::MetaXML::ImageHeightPixelId);
  int32Entry = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
  Q_ASSERT_X(int32Entry.get() != NULL, "Could not Cast to Int32ZeissMetaEntry", "");

  dims[1] = int32Entry->getValue();
  dims[2] = 1;
  return dims;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ZeissImportFilter::getCompiledLibraryName()
{
  return ZeissImportConstants::ZeissImportBaseName;
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
  return "Zeiss AxioVision Import";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ZeissImportFilter::getSubGroupName()
{
  return "IO";
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

