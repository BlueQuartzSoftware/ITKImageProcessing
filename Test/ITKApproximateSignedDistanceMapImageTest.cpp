// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


class ITKApproximateSignedDistanceMapImageTest : public ITKTestBase
{

public:
  ITKApproximateSignedDistanceMapImageTest()
  {
  }
  virtual ~ITKApproximateSignedDistanceMapImageTest()
  {
  }

  int TestITKApproximateSignedDistanceMapImagedefaultTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/2th_cthead1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKApproximateSignedDistanceMapImage";
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
    WriteImage("ITKApproximateSignedDistanceMapImagedefault.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_ApproximateSignedDistanceMapImageFilter_default.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);

//    DataContainerWriter::Pointer dcWriter = DataContainerWriter::New();
//    dcWriter->setOutputFile("/tmp/ITKApproximateSignedDistanceMapImageTest.dream3d");
//    dcWriter->setDataContainerArray(containerArray);
//    dcWriter->execute();

    int res = this->CompareImages(containerArray, input_path, baseline_path, 0.01);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
}

int TestITKApproximateSignedDistanceMapImagemodified_parmsTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/2th_cthead1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKApproximateSignedDistanceMapImage";
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
        d3d_var = 100;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("InsideValue", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
        double d3d_var;
        d3d_var = 0;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("OutsideValue", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKApproximateSignedDistanceMapImagemodified_parms.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_ApproximateSignedDistanceMapImageFilter_modified_parms.nrrd");
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

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKApproximateSignedDistanceMapImage"));

    DREAM3D_REGISTER_TEST( TestITKApproximateSignedDistanceMapImagedefaultTest());
    DREAM3D_REGISTER_TEST( TestITKApproximateSignedDistanceMapImagemodified_parmsTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKApproximateSignedDistanceMapImageTest(const ITKApproximateSignedDistanceMapImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKApproximateSignedDistanceMapImageTest&);                           // Move assignment Not Implemented
};
