// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes

class ITKBinaryThinningImageTest : public ITKTestBase
{

public:
  ITKBinaryThinningImageTest() = default;
  ~ITKBinaryThinningImageTest() override = default;

  int TestITKBinaryThinningImageBinaryThinningTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/BlackDots.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryThinningImage";
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
    WriteImage("ITKBinaryThinningImageBinaryThinning.nrrd", containerArray, output_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, output_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("153ad0b2f3658dee3b14ad93d0cfe550"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKBinaryThinningImage"));

    DREAM3D_REGISTER_TEST(TestITKBinaryThinningImageBinaryThinningTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKBinaryThinningImageTest(const ITKBinaryThinningImageTest&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKBinaryThinningImageTest&) = delete;             // Move assignment Not Implemented
};
