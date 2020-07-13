// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"

class ITKFFTNormalizedCorrelationImageTest : public ITKTestBase
{

public:
  ITKFFTNormalizedCorrelationImageTest() = default;
  ~ITKFFTNormalizedCorrelationImageTest() override = default;

  int TestITKFFTNormalizedCorrelationImagedefaultTest()
  {
    DataContainerArray::Pointer containerArray = DataContainerArray::New();

    QString fixedFilename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/FixedRectangle1.png");
    DataArrayPath fixedPath("FixedTestContainer", "FixedTestAttributeMatrixName", "FixedTestAttributeArrayName");
    this->ReadImage(fixedFilename, containerArray, fixedPath);

    QString movingFilename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/MovingRectangles.png");
    DataArrayPath movingPath("MovingTestContainer", "MovingTestAttributeMatrixName", "MovingTestAttributeArrayName");
    this->ReadImage(movingFilename, containerArray, movingPath);

    QString newCellArrayName = "OutputTestAttributeArrayName";
    DataArrayPath outputPath("FixedTestContainer", "FixedTestAttributeMatrixName", newCellArrayName);

    QString filtName = "ITKFFTNormalizedCorrelationImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(fixedPath);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(movingPath);
    propWasSet = filter->setProperty("MovingCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(newCellArrayName);
    propWasSet = filter->setProperty("NewCellArrayName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);

    WriteImage("ITKFFTNormalizedCorrelationImagedefault.nrrd", containerArray, outputPath);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_FFTNormalizedCorrelationImageFilter_default.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, outputPath, baseline_path, 0.0001);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKFFTNormalizedCorrelationImage"));

    DREAM3D_REGISTER_TEST(TestITKFFTNormalizedCorrelationImagedefaultTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKFFTNormalizedCorrelationImageTest(const ITKFFTNormalizedCorrelationImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKFFTNormalizedCorrelationImageTest&);                       // Move assignment Not Implemented
};
