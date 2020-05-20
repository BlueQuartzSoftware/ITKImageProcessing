// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"

class ITKBinaryThresholdImageTest : public ITKTestBase
{

public:
  ITKBinaryThresholdImageTest() = default;
  ~ITKBinaryThresholdImageTest() override = default;

  int TestITKBinaryThresholdImagedefaultTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Short.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryThresholdImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(input_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKBinaryThresholdImagedefault.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("dbd0ea7d6f16bb93e9c688cb0f1bfd85"));
    return 0;
  }

  int TestITKBinaryThresholdImageNarrowThresholdTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Short.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryThresholdImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(input_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    {
      double d3d_var;
      d3d_var = 10;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("LowerThreshold", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
      double d3d_var;
      d3d_var = 100;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("UpperThreshold", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
      int d3d_var;
      d3d_var = 255;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("InsideValue", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
      int d3d_var;
      d3d_var = 0;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("OutsideValue", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKBinaryThresholdImageNarrowThreshold.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("fc4ce029c088096a69d033ccc5bc1ae2"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKBinaryThresholdImage"));

    DREAM3D_REGISTER_TEST(TestITKBinaryThresholdImagedefaultTest());
    DREAM3D_REGISTER_TEST(TestITKBinaryThresholdImageNarrowThresholdTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKBinaryThresholdImageTest(const ITKBinaryThresholdImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKBinaryThresholdImageTest&);              // Move assignment Not Implemented
};
