// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes

class ITKSobelEdgeDetectionImageTest : public ITKTestBase
{

public:
  ITKSobelEdgeDetectionImageTest()
  {
  }
  virtual ~ITKSobelEdgeDetectionImageTest()
  {
  }

  int TestITKSobelEdgeDetectionImagedefaultsTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKSobelEdgeDetectionImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
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
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKSobelEdgeDetectionImagedefaults.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_SobelEdgeDetectionImageFilter_defaults.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 0.01);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKSobelEdgeDetectionImage"));

    DREAM3D_REGISTER_TEST(TestITKSobelEdgeDetectionImagedefaultsTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKSobelEdgeDetectionImageTest(const ITKSobelEdgeDetectionImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKSobelEdgeDetectionImageTest&);                 // Operator '=' Not Implemented
};
