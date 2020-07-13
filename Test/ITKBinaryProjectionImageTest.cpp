// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"

class ITKBinaryProjectionImageTest : public ITKTestBase
{

public:
  ITKBinaryProjectionImageTest() = default;
  ~ITKBinaryProjectionImageTest() override = default;

  int TestITKBinaryProjectionImagedefaultsTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/2th_cthead1.mha");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryProjectionImage";
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
    WriteImage("ITKBinaryProjectionImagedefaults.nrrd", containerArray, output_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, output_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("3fc3603b27bf51df592190227d6cd6ed"));
    return 0;
  }

  int TestITKBinaryProjectionImageanother_dimensionTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/WhiteDots.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryProjectionImage";
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
      double d3d_var;
      d3d_var = 1;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("ProjectionDimension", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
      double d3d_var;
      d3d_var = 255;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("ForegroundValue", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKBinaryProjectionImageanother_dimension.nrrd", containerArray, output_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, output_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("827f263ef9fb63d05499d14fcef32f60"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKBinaryProjectionImage"));

    DREAM3D_REGISTER_TEST(TestITKBinaryProjectionImagedefaultsTest());
    DREAM3D_REGISTER_TEST(TestITKBinaryProjectionImageanother_dimensionTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKBinaryProjectionImageTest(const ITKBinaryProjectionImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKBinaryProjectionImageTest&);               // Move assignment Not Implemented
};
