// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------
#pragma once

#include <QtCore/QFile>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"

#include "UnitTestSupport.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/AxioVisionV4ToTileConfiguration.h"
#include "ITKImageProcessingTestFileLocations.h"

class AxioVisionV4ToTileConfigurationTest
{

  public:
    AxioVisionV4ToTileConfigurationTest() = default;
    ~AxioVisionV4ToTileConfigurationTest() = default;
    AxioVisionV4ToTileConfigurationTest(const AxioVisionV4ToTileConfigurationTest&) = delete;            // Copy Constructor
    AxioVisionV4ToTileConfigurationTest(AxioVisionV4ToTileConfigurationTest&&) = delete;                 // Move Constructor
    AxioVisionV4ToTileConfigurationTest& operator=(const AxioVisionV4ToTileConfigurationTest&) = delete; // Copy Assignment
    AxioVisionV4ToTileConfigurationTest& operator=(AxioVisionV4ToTileConfigurationTest&&) = delete;      // Move Assignment

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
  #if REMOVE_TEST_FILES
    QFile::remove(UnitTest::AxioVisionV4ToTileConfigurationTest::OutputFile);
  #endif
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestMetaXmlToFijiConfigTest()
  {
    int32_t err = 0;

    AxioVisionV4ToTileConfiguration::Pointer import = AxioVisionV4ToTileConfiguration::New();
    import->setInputFile(UnitTest::AxioVisionV4ToTileConfigurationTest::InputFile);
    import->setOutputFile(UnitTest::AxioVisionV4ToTileConfigurationTest::OutputFile);

    import->preflight();
    err = import->getErrorCode();
    DREAM3D_REQUIRED(err, ==, 0);

    import->execute();
    err = import->getErrorCode();
    DREAM3D_REQUIRED(err, ==, 0);

    QFile writtenConfigFile(UnitTest::AxioVisionV4ToTileConfigurationTest::OutputFile);
    writtenConfigFile.open(QFile::ReadOnly);
    QString writtenConfigContent = writtenConfigFile.readAll();
    writtenConfigFile.close();


    QFile exemplarConfigFile(UnitTest::AxioVisionV4ToTileConfigurationTest::ExemplarOutputFile);
    exemplarConfigFile.open(QFile::ReadOnly);
    QString exemplarConfigContent = exemplarConfigFile.readAll();
    exemplarConfigFile.close();

    DREAM3D_REQUIRED(writtenConfigContent, ==, exemplarConfigContent)
    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "---------------- AxioVisionV4ToTileConfigurationTest ---------------------" << std::endl;
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( TestMetaXmlToFijiConfigTest() )

 //  DREAM3D_REGISTER_TEST( RemoveTestFiles() )
  }

  private:


};

