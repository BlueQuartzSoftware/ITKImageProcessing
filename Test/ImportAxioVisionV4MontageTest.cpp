// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------
#pragma once

#include <QtCore/QCoreApplication>
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

#include "UnitTestSupport.hpp"

#include "ZeissImport/ZeissImportFilters/ImportAxioVisionV4Montage.h"
#include "ZeissImportTestFileLocations.h"

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

      // Observer obs;

      DataContainerArray::Pointer dca = DataContainerArray::New();

      ImportAxioVisionV4Montage::Pointer filter = ImportAxioVisionV4Montage::New();
      filter->setDataContainerArray(dca);

      // obs.connect(filter.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), &obs, SLOT(processPipelineMessage(const AbstractMessage::Pointer&)));

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
      filter->setDataContainerName(DataArrayPath());
      filter->preflight();
      err = filter->getErrorCode();
      DREAM3D_REQUIRED(err, ==, -392)

      filter->setDataContainerName(DataArrayPath("DataContainer"));
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
    //
    // -----------------------------------------------------------------------------
    void operator()()
    {
      std::cout << "---------------- ImportAxioVisionV4MontageTest ---------------------" << std::endl;

      int err = EXIT_SUCCESS;

      DREAM3D_REGISTER_TEST(TestImportAxioVisionV4MontageTest())

      DREAM3D_REGISTER_TEST(RemoveTestFiles())
    }

  private:
    QString m_ClassName = QString("ImportAxioVisionV4MontageTest");
};
