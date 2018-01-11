// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>

class ITKSmoothingRecursiveGaussianImageTest : public ITKTestBase
{

public:
  ITKSmoothingRecursiveGaussianImageTest()
  {
  }
  virtual ~ITKSmoothingRecursiveGaussianImageTest()
  {
  }

  int TestITKSmoothingRecursiveGaussianImagedefaultTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");

    FilterPipeline::Pointer pipeline = FilterPipeline::New();
    QString filtName = "ITKImageReader";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);

    DREAM3D_REQUIRE_VALID_POINTER(filterFactory.get());

    AbstractFilter::Pointer reader = filterFactory->create();
    DREAM3D_REQUIRE_VALID_POINTER(reader.get());
    bool propWasSet = false;
    // reader->setDataContainerArray(containerArray);
    propWasSet = reader->setProperty("DataContainerName", input_path.getDataContainerName());
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    propWasSet = reader->setProperty("CellAttributeMatrixName", input_path.getAttributeMatrixName());
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    propWasSet = reader->setProperty("ImageDataArrayName", input_path.getDataArrayName());
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    propWasSet = reader->setProperty("FileName", input_filename);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    pipeline->pushBack(reader);

    // DataContainerArray::Pointer containerArray = DataContainerArray::New();
    // this->ReadImage(input_filename, containerArray, input_path);
    filtName = "ITKSmoothingRecursiveGaussianImage";
    //    FilterManager* fm = FilterManager::Instance();
    filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    // bool propWasSet;
    var.setValue(input_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    pipeline->pushBack(filter);

    pipeline->preflightPipeline();
    int err = pipeline->getErrorCondition();
    DREAM3D_REQUIRED(err, >=, 0);

    pipeline->execute();
    err = pipeline->getErrorCondition();
    DREAM3D_REQUIRED(err, >=, 0);

#if 0
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);
    WriteImage("ITKSmoothingRecursiveGaussianImagedefault.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_SmoothingRecursiveGaussianImageFilter_default.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 0.0001);
    DREAM3D_REQUIRE_EQUAL(res,0);

#endif
    return 0;
  }

  int TestITKSmoothingRecursiveGaussianImagergb_imageTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/VM1111Shrink-RGB.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
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
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    {
      double d3d_var;
      d3d_var = 5.0;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("Sigma", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKSmoothingRecursiveGaussianImagergb_image.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_SmoothingRecursiveGaussianImageFilter_rgb_image.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 1e-05);
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
  void operator=(const ITKSmoothingRecursiveGaussianImageTest&);                         // Operator '=' Not Implemented
};
