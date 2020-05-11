// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>

class ITKGradientAnisotropicDiffusionImageTest : public ITKTestBase
{

public:
  ITKGradientAnisotropicDiffusionImageTest() = default;
  ~ITKGradientAnisotropicDiffusionImageTest() override = default;

  int TestITKGradientAnisotropicDiffusionImagedefaultsTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGradientAnisotropicDiffusionImage";
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
      d3d_var = 0.01;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("TimeStep", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKGradientAnisotropicDiffusionImagedefaults.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_GradientAnisotropicDiffusionImageFilter_defaults.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 0);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
  }

  int TestITKGradientAnisotropicDiffusionImagelongerTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGradientAnisotropicDiffusionImage";
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
      d3d_var = 0.01;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("TimeStep", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
      double d3d_var;
      d3d_var = 10;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("NumberOfIterations", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKGradientAnisotropicDiffusionImagelonger.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_GradientAnisotropicDiffusionImageFilter_longer.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 0);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKGradientAnisotropicDiffusionImage"));

    DREAM3D_REGISTER_TEST(TestITKGradientAnisotropicDiffusionImagedefaultsTest());
    DREAM3D_REGISTER_TEST(TestITKGradientAnisotropicDiffusionImagelongerTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKGradientAnisotropicDiffusionImageTest(const ITKGradientAnisotropicDiffusionImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKGradientAnisotropicDiffusionImageTest&);                           // Move assignment Not Implemented
};
