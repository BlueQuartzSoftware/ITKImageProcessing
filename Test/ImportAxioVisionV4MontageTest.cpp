// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------
#pragma once

#include <QtCore/QFile>

#include "SIMPLib/Common/Observer.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "UnitTestSupport.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/ImportAxioVisionV4Montage.h"
#include "ITKImageProcessingTestFileLocations.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagMapping.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/MetaXmlUtils.h"

class ImportAxioVisionV4MontageTest
{

public:
  ImportAxioVisionV4MontageTest() = default;
  ~ImportAxioVisionV4MontageTest() = default;
  ImportAxioVisionV4MontageTest(const ImportAxioVisionV4MontageTest&) = delete;            // Copy Constructor
  ImportAxioVisionV4MontageTest(ImportAxioVisionV4MontageTest&&) = delete;                 // Move Constructor
  ImportAxioVisionV4MontageTest& operator=(const ImportAxioVisionV4MontageTest&) = delete; // Copy Assignment
  ImportAxioVisionV4MontageTest& operator=(ImportAxioVisionV4MontageTest&&) = delete;      // Move Assignment

  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES
    QFile::remove(UnitTest::ImportAxioVisionV4MontageTest::OutputDREAM3DFile);
#endif
  }

  // -----------------------------------------------------------------------------
  int TestImportAxioVisionV4MontageTest()
  {

    //   Observer obs;

    DataContainerArray::Pointer dca = DataContainerArray::New();

    ImportAxioVisionV4Montage::Pointer filter = ImportAxioVisionV4Montage::New();
    filter->setDataContainerArray(dca);

    // obs.connect(filter.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), &obs, SLOT(processPipelineMessage(const AbstractMessage::Pointer&)));
    //
    // Test without putting in a File, Should get an error
    filter->preflight();
    int32_t err = filter->getErrorCode();
    DREAM3D_REQUIRED(err, ==, -387)

    // Some file that doesn't exist
    filter->setInputFile("kasp0dfaweo8acnosei.sdkfj9e");
    filter->preflight();
    err = filter->getErrorCode();
    DREAM3D_REQUIRED(err, ==, -388)

    filter->setInputFile(UnitTest::ImportAxioVisionV4MontageTest::AxioVisionMetaXmlFile);
    filter->setDataContainerPath(DataArrayPath());
    filter->preflight();
    err = filter->getErrorCode();
    DREAM3D_REQUIRED(err, ==, -392)

    filter->setDataContainerPath(DataArrayPath("DataContainer"));
    filter->setCellAttributeMatrixName("");
    filter->preflight();
    err = filter->getErrorCode();
    DREAM3D_REQUIRED(err, ==, -393)

    filter->setCellAttributeMatrixName("AttributeMatrix");
    filter->setImageDataArrayName("");
    filter->preflight();
    err = filter->getErrorCode();
    DREAM3D_REQUIRED(err, ==, -394)

    // Should be able to preflight now..
    filter->setImageDataArrayName("Image");
    filter->setConvertToGrayScale(false);
    filter->preflight();
    err = filter->getErrorCode();
    DREAM3D_REQUIRED(err, >=, 0)

    {
      DataContainerArray::Pointer dca = DataContainerArray::New();
      filter->setDataContainerArray(dca);
      filter->execute();
      err = filter->getErrorCode();
      DREAM3D_REQUIRED(err, >=, 0)
    }

    {
      DataContainerArray::Pointer dca = DataContainerArray::New();
      filter->setDataContainerArray(dca);
      filter->setConvertToGrayScale(true);
      filter->execute();
      err = filter->getErrorCode();
      DREAM3D_REQUIRED(err, >=, 0)
    }

    {
      DataContainerArray::Pointer dca = DataContainerArray::New();
      filter->setDataContainerArray(dca);
      filter->setConvertToGrayScale(false);
      filter->setChangeOrigin(true);
      filter->execute();
      err = filter->getErrorCode();
      DREAM3D_REQUIRED(err, >=, 0)
    }

    {
      DataContainerArray::Pointer dca = DataContainerArray::New();
      filter->setDataContainerArray(dca);
      filter->setConvertToGrayScale(true);
      filter->setChangeSpacing(true);
      filter->execute();
      err = filter->getErrorCode();
      DREAM3D_REQUIRED(err, >=, 0)
    }
    DREAM3D_REQUIRED(err, >=, 0)

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  void TestMetaDump()
  {
    // Parse the XML file to get all the meta-data information and create all the
    // data structure that is needed.
    QFile xmlFile(UnitTest::ImportAxioVisionV4MontageTest::AxioVisionMetaXmlFile);
    QDomDocument domDocument;
    QDomElement root;

    QFile hOut(UnitTest::TestTempDir + "/AxioVision_Dump.txt");
    hOut.open(QFile::WriteOnly);
    QTextStream stream(&hOut);

    QString errorStr;
    int errorLine = -1;
    int errorColumn = -1;

    domDocument.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn);
    root = domDocument.documentElement();

    // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
    QDomElement tags = root.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
    ZeissTagsXmlSection::Pointer rootTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
    if(nullptr == rootTagsSection.get())
    {
      return;
    }

    ZeissTagMapping::Pointer tagMapping = ZeissTagMapping::instance();


    ZeissTagsXmlSection::MetaDataType tagMetaData = rootTagsSection->getMetaDataMap();
    stream << "####### Tags #######\n";
    for(const auto& entry : tagMetaData)
    {
      int32_t tagId = entry->getZeissIdTag();
      QString name = tagMapping->nameForId(tagId);
      stream << tagId << "\t" << name << "\t" << entry->toString() << "\n";
    }

    QDomElement scaling = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling);
    ZeissTagsXmlSection::Pointer scalingTagsSection = MetaXmlUtils::ParseScalingSection(nullptr, scaling);
    if(nullptr == scalingTagsSection.get())
    {
      stream << "scalingTagsSection was null\n";
      return;
    }

    tagMetaData = scalingTagsSection->getMetaDataMap();
    stream << "####### Scaling #######\n";
    for(const auto& entry : tagMetaData)
    {
      int32_t tagId = entry->getZeissIdTag();
      QString name = tagMapping->nameForId(tagId);
      stream << tagId << "\t" << name << "\t" << entry->toString() << "\n";
    }

    int32_t imageCount = MetaXmlUtils::GetInt32Entry(nullptr, rootTagsSection.get(), Zeiss::MetaXML::ImageCountRawId);

    //    int32_t m_RowCount = MetaXmlUtils::GetInt32Entry(nullptr, rootTagsSection.get(), Zeiss::MetaXML::ImageCountVId);

    //    int32_t m_ColumnCount = MetaXmlUtils::GetInt32Entry(nullptr, rootTagsSection.get(), Zeiss::MetaXML::ImageCountUId);
    int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(imageCount);

    // Loop over every image in the _meta.xml file
    for(int p = 0; p < imageCount; p++)
    {
      // Generate the xml tag that is for this image
      QString pTag;
      QTextStream out(&pTag);
      out << "p";
      out.setFieldWidth(imageCountPadding);
      out.setFieldAlignment(QTextStream::AlignRight);
      out.setPadChar('0');
      out << p;

      // Drill down into the XML document....
      QDomElement photoEle = root.firstChildElement(pTag);
      if(photoEle.isNull())
      {
        QString ss = QObject::tr("Could not find the <ROOT><%1> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
        qDebug() << ss;
        return;
      }
      // Get the TAGS section
      QDomElement tags = photoEle.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
      if(tags.isNull())
      {
        QString ss = QObject::tr("Could not find the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
        qDebug() << ss;
        return;
      }
      // Now Parse the TAGS section
      ZeissTagsXmlSection::Pointer photoTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
      if(nullptr == photoTagsSection.get())
      {
        QString ss = QObject::tr("Error Parsing the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss AxioVision _meta.xml file").arg(pTag);
        qDebug() << ss;
        return;
      }
      tagMetaData = photoTagsSection->getMetaDataMap();
      stream << "####### p" << p << " #######\n";
      for(const auto& entry : tagMetaData)
      {
        int32_t tagId = entry->getZeissIdTag();
        QString name = tagMapping->nameForId(tagId);
        stream << tagId << "<-->" << name << "<-->" << entry->toString() << "\n";
      }
    }
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "---------------- ImportAxioVisionV4MontageTest ---------------------" << std::endl;

    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestImportAxioVisionV4MontageTest())
    DREAM3D_REGISTER_TEST(TestMetaDump())
    DREAM3D_REGISTER_TEST(RemoveTestFiles())
  }

private:
  QString m_ClassName = QString("ImportAxioVisionV4MontageTest");
};
