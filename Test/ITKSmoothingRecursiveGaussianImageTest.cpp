// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"

class ITKSmoothingRecursiveGaussianImageTest : public ITKTestBase
{

public:
  ITKSmoothingRecursiveGaussianImageTest() = default;
  ~ITKSmoothingRecursiveGaussianImageTest() override = default;

  int TestITKSmoothingRecursiveGaussianImagedefaultTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKSmoothingRecursiveGaussianImage";
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
    WriteImage("ITKSmoothingRecursiveGaussianImagedefault.nrrd", containerArray, output_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_SmoothingRecursiveGaussianImageFilter_default.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, output_path, baseline_path, 0.0001);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
  }

  int TestITKSmoothingRecursiveGaussianImagergb_imageTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/VM1111Shrink-RGB.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKSmoothingRecursiveGaussianImage";
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
    {
      FloatVec3Type d3d_var;
      d3d_var[0] = 5.0;
      d3d_var[1] = 5.0;
      d3d_var[2] = 5.0;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("Sigma", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKSmoothingRecursiveGaussianImagergb_image.nrrd", containerArray, output_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_SmoothingRecursiveGaussianImageFilter_rgb_image.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, output_path, baseline_path, 1e-05);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKSmoothingRecursiveGaussianImage"));

    DREAM3D_REGISTER_TEST(TestITKSmoothingRecursiveGaussianImagedefaultTest());
    DREAM3D_REGISTER_TEST(TestITKSmoothingRecursiveGaussianImagergb_imageTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKSmoothingRecursiveGaussianImageTest(const ITKSmoothingRecursiveGaussianImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKSmoothingRecursiveGaussianImageTest&);                         // Move assignment Not Implemented
};
