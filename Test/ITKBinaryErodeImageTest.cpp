// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
#include "SIMPLib/ITK/SimpleITKEnums.h"
// Auto includes
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"

class ITKBinaryErodeImageTest : public ITKTestBase
{

public:
  ITKBinaryErodeImageTest() = default;
  ~ITKBinaryErodeImageTest() override = default;

  int TestITKBinaryErodeImageBinaryErodeTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/STAPLE1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryErodeImage";
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
      d3d_var[0] = 1;
      d3d_var[1] = 1;
      d3d_var[2] = 1;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("KernelRadius", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
      int d3d_var;
      d3d_var = itk::simple::sitkBall;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("KernelType", var);
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
    WriteImage("ITKBinaryErodeImageBinaryErode.nrrd", containerArray, output_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, output_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("9e37516c795d7f25847851666ef53ef9"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKBinaryErodeImage"));

    DREAM3D_REGISTER_TEST(TestITKBinaryErodeImageBinaryErodeTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKBinaryErodeImageTest(const ITKBinaryErodeImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKBinaryErodeImageTest&);          // Move assignment Not Implemented
};
