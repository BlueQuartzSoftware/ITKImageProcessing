// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"

#include <itkImageIOBase.h>

class ITKRGBToLuminanceImageTest : public ITKTestBase
{

public:
ITKRGBToLuminanceImageTest() = default;
~ITKRGBToLuminanceImageTest() override = default;

  int TestITKRGBToLuminanceImageInputTest()
  {
    QString inputFilename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/fruit.png");
    DataArrayPath inputPath("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(inputFilename, containerArray, inputPath);

    QString filtName = "ITKRGBToLuminanceImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();

    QVariant var;
    bool propWasSet;
    var.setValue(inputPath);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);

    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);

    WriteImage("ITKRGBToLuminanceImageInput.nrrd", containerArray, inputPath);
    QString baselineFilename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_RGBToLuminanceImageFilter_Input.nrrd");
    DataArrayPath baselinePath("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baselineFilename, containerArray, baselinePath);
    int result = this->CompareImages(containerArray, inputPath, baselinePath, 0.01);
    DREAM3D_REQUIRE_EQUAL(result, 0);

    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKRGBToLuminanceImage"));

    DREAM3D_REGISTER_TEST(TestITKRGBToLuminanceImageInputTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKRGBToLuminanceImageTest(const ITKRGBToLuminanceImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKRGBToLuminanceImageTest&);             // Move assignment Not Implemented
};
