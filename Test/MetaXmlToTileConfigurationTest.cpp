// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------
#pragma once

#include <QtCore/QCoreApplication>
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

#include "ZeissImportTestFileLocations.h"

class MetaXmlToTileConfigurationTest
{

  public:
    MetaXmlToTileConfigurationTest() = default;
    ~MetaXmlToTileConfigurationTest() = default;
    MetaXmlToTileConfigurationTest(const MetaXmlToTileConfigurationTest&) = delete;            // Copy Constructor
    MetaXmlToTileConfigurationTest(MetaXmlToTileConfigurationTest&&) = delete;                 // Move Constructor
    MetaXmlToTileConfigurationTest& operator=(const MetaXmlToTileConfigurationTest&) = delete; // Copy Assignment
    MetaXmlToTileConfigurationTest& operator=(MetaXmlToTileConfigurationTest&&) = delete;      // Move Assignment

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
  #if REMOVE_TEST_FILES
    QFile::remove(UnitTest::MetaXmlToTileConfigurationTest::OutputFile);
  #endif
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestFilterAvailability()
  {
    // Now instantiate the MetaXmlToTileConfigurationTest Filter from the FilterManager
    QString filtName = "MetaXmlToTileConfiguration";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    if (nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The MetaXmlToTileConfigurationTest Requires the use of the " << filtName.toStdString()
         << " filter which is found in the ZeissImport Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestMetaXmlToFijiConfigTest()
  {
    QVariant var;
    bool propWasSet = false;
    int32_t err = 0;
    QString filtName = "MetaXmlToTileConfiguration";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    AbstractFilter::Pointer import = filterFactory->create();

    var.setValue(UnitTest::MetaXmlToTileConfigurationTest::InputFile);
    propWasSet = import->setProperty("InputFile", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    var.setValue(UnitTest::MetaXmlToTileConfigurationTest::OutputFile);
    propWasSet = import->setProperty("OutputFile", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    import->preflight();
    err = import->getErrorCondition();
    DREAM3D_REQUIRED(err, ==, 0);

    import->execute();
    err = import->getErrorCondition();
    DREAM3D_REQUIRED(err, ==, 0);




    QFile writtenConfigFile(UnitTest::MetaXmlToTileConfigurationTest::OutputFile);
    writtenConfigFile.open(QFile::ReadOnly);
    QString writtenConfigContent = writtenConfigFile.readAll();
    writtenConfigFile.close();


    QFile exemplarConfigFile(UnitTest::MetaXmlToTileConfigurationTest::ExemplarOutputFile);
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
    std::cout << "---------------- MetaXmlToTileConfigurationTest ---------------------" << std::endl;
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( TestFilterAvailability() );

    DREAM3D_REGISTER_TEST( TestMetaXmlToFijiConfigTest() )

 //  DREAM3D_REGISTER_TEST( RemoveTestFiles() )
  }

  private:


};

