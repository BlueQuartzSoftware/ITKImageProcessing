// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------
#pragma once

#include <QtCore/QFile>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"

#include "UnitTestSupport.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/ImportZenInfoMontage.h"
#include "ITKImageProcessingTestFileLocations.h"

class ImportZenInfoMontageTest
{

public:
  ImportZenInfoMontageTest() = default;
  ~ImportZenInfoMontageTest() = default;
  ImportZenInfoMontageTest(const ImportZenInfoMontageTest&) = delete;            // Copy Constructor
  ImportZenInfoMontageTest(ImportZenInfoMontageTest&&) = delete;                 // Move Constructor
  ImportZenInfoMontageTest& operator=(const ImportZenInfoMontageTest&) = delete; // Copy Assignment
  ImportZenInfoMontageTest& operator=(ImportZenInfoMontageTest&&) = delete;      // Move Assignment

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES
    QFile::remove(UnitTest::ImportZenInfoMontageTest::OutputDREAM3DFile);
#endif
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestImportZenInfoMontageTest()
  {
    DataContainerArray::Pointer dca = DataContainerArray::New();

    ImportZenInfoMontage::Pointer filter = ImportZenInfoMontage::New();
    filter->setDataContainerArray(dca);

    // Test without putting in a File, Should get an error
    filter->preflight();
    int32_t err = filter->getErrorCode();
    DREAM3D_REQUIRED(err, ==, -387)

    // Some file that doesn't exist
    filter->setInputFile("kasp0dfaweo8acnosei.sdkfj9e");
    filter->preflight();
    err = filter->getErrorCode();
    DREAM3D_REQUIRED(err, ==, -388)

    filter->setInputFile(UnitTest::ImportZenInfoMontageTest::ZenInfoFile);
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
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "---------------- ImportZenInfoMontageTest ---------------------" << std::endl;

    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestImportZenInfoMontageTest())

    DREAM3D_REGISTER_TEST(RemoveTestFiles())
  }

private:
  QString m_ClassName = QString("ImportZenInfoMontageTest");
};
