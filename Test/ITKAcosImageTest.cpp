// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes

class ITKAcosImageTest : public ITKTestBase
{

public:
  ITKAcosImageTest() = default;
  ~ITKAcosImageTest() override = default;

  int TestITKAcosImagedefaultsTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/Ramp-Zero-One-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKAcosImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(input_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(outputName);
    propWasSet = filter->setProperty("NewCellArrayName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKAcosImagedefaults.nrrd", containerArray, output_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_AcosImageFilter_defaults.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, output_path, baseline_path, 0.01);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKAcosImage"));

    DREAM3D_REGISTER_TEST(TestITKAcosImagedefaultsTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKAcosImageTest(const ITKAcosImageTest&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKAcosImageTest&) = delete;   // Move assignment Not Implemented
};
