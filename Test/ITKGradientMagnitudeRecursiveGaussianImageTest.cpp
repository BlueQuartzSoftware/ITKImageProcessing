// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


class ITKGradientMagnitudeRecursiveGaussianImageTest : public ITKTestBase
{

public:
ITKGradientMagnitudeRecursiveGaussianImageTest() = default;
~ITKGradientMagnitudeRecursiveGaussianImageTest() override = default;

  int TestITKGradientMagnitudeRecursiveGaussianImagedefaultTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGradientMagnitudeRecursiveGaussianImage";
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
    WriteImage("ITKGradientMagnitudeRecursiveGaussianImagedefault.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_GradientMagnitudeRecursiveGaussianImageFilter_default.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 0.0001);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
}



  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKGradientMagnitudeRecursiveGaussianImage"));

    DREAM3D_REGISTER_TEST( TestITKGradientMagnitudeRecursiveGaussianImagedefaultTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKGradientMagnitudeRecursiveGaussianImageTest(const ITKGradientMagnitudeRecursiveGaussianImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKGradientMagnitudeRecursiveGaussianImageTest&);                                 // Move assignment Not Implemented
};
