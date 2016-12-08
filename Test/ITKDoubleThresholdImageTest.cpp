// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


class ITKDoubleThresholdImageTest: public ITKTestBase
{

  public:
    ITKDoubleThresholdImageTest() {}
    virtual ~ITKDoubleThresholdImageTest() {}

int TestITKDoubleThresholdImageDoubleThreshold1Test()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Short.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKDoubleThresholdImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(),0);
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
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);
    WriteImage("ITKDoubleThresholdImageDoubleThreshold1.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("dbd0ea7d6f16bb93e9c688cb0f1bfd85"));
    return 0;
}

int TestITKDoubleThresholdImageDoubleThreshold2Test()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Slice-Short.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKDoubleThresholdImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(),0);
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
        d3d_var = 0;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("Threshold1", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    {
        double d3d_var;
        d3d_var = 0;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("Threshold2", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    {
        double d3d_var;
        d3d_var = 3000;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("Threshold3", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    {
        double d3d_var;
        d3d_var = 2700;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("Threshold4", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);
    WriteImage("ITKDoubleThresholdImageDoubleThreshold2.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("2c8fc2345ccfa980ef42aef5910efaa3"));
    return 0;
}




  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( this->TestFilterAvailability("ITKDoubleThresholdImage") );

    DREAM3D_REGISTER_TEST( TestITKDoubleThresholdImageDoubleThreshold1Test());
    DREAM3D_REGISTER_TEST( TestITKDoubleThresholdImageDoubleThreshold2Test());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST( this->RemoveTestFiles() )
    }
  }

  private:
    ITKDoubleThresholdImageTest(const ITKDoubleThresholdImageTest&); // Copy Constructor Not Implemented
    void operator=(const ITKDoubleThresholdImageTest&); // Operator '=' Not Implemented
};

