// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


class ITKBinaryMinMaxCurvatureFlowImageTest : public ITKTestBase
{

public:
ITKBinaryMinMaxCurvatureFlowImageTest() = default;
~ITKBinaryMinMaxCurvatureFlowImageTest() override = default;

  int TestITKBinaryMinMaxCurvatureFlowImagedefaultsTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryMinMaxCurvatureFlowImage";
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
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKBinaryMinMaxCurvatureFlowImagedefaults.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_BinaryMinMaxCurvatureFlowImageFilter_defaults.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 0.1);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
}

int TestITKBinaryMinMaxCurvatureFlowImagelongerTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryMinMaxCurvatureFlowImage";
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
        d3d_var = 0.1;
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
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKBinaryMinMaxCurvatureFlowImagelonger.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_BinaryMinMaxCurvatureFlowImageFilter_longer.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 0.1);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
}



  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKBinaryMinMaxCurvatureFlowImage"));

    DREAM3D_REGISTER_TEST( TestITKBinaryMinMaxCurvatureFlowImagedefaultsTest());
    DREAM3D_REGISTER_TEST( TestITKBinaryMinMaxCurvatureFlowImagelongerTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKBinaryMinMaxCurvatureFlowImageTest(const ITKBinaryMinMaxCurvatureFlowImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKBinaryMinMaxCurvatureFlowImageTest&);                        // Move assignment Not Implemented
};
