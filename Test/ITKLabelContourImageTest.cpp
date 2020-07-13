// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"

class ITKLabelContourImageTest : public ITKTestBase
{

public:
  ITKLabelContourImageTest() = default;
  ~ITKLabelContourImageTest() override = default;

  int TestITKLabelContourImagedefaultTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/2th_cthead1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKLabelContourImage";
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
    WriteImage("ITKLabelContourImagedefault.nrrd", containerArray, output_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, output_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("d742c05a8d8aa9b41f58b8d2aad6b5d0"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKLabelContourImage"));

    DREAM3D_REGISTER_TEST(TestITKLabelContourImagedefaultTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKLabelContourImageTest(const ITKLabelContourImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKLabelContourImageTest&);           // Move assignment Not Implemented
};
