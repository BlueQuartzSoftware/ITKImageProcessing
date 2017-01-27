// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>


class ITKGrayscaleGeodesicDilateImageTest: public ITKTestBase
{

  public:
    ITKGrayscaleGeodesicDilateImageTest() {}
    virtual ~ITKGrayscaleGeodesicDilateImageTest() {}

int TestITKGrayscaleGeodesicDilateImageGrayscaleGeodesicDilate1Test()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/STAPLE1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGrayscaleGeodesicDilateImage";
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
        bool d3d_var;
        d3d_var = false;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("RunOneIteration", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    {
        bool d3d_var;
        d3d_var = true;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("FullyConnected", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);
    WriteImage("ITKGrayscaleGeodesicDilateImageGrayscaleGeodesicDilate1.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("301858f5eee39b092d28d9837f008fb8"));
    return 0;
}

int TestITKGrayscaleGeodesicDilateImageGrayscaleGeodesicDilate2Test()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/STAPLE1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGrayscaleGeodesicDilateImage";
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
        bool d3d_var;
        d3d_var = true;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("RunOneIteration", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    {
        bool d3d_var;
        d3d_var = true;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("FullyConnected", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);
    WriteImage("ITKGrayscaleGeodesicDilateImageGrayscaleGeodesicDilate2.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("512b043be99c423638db7cb7bc6fb2df"));
    return 0;
}




  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( this->TestFilterAvailability("ITKGrayscaleGeodesicDilateImage") );

    DREAM3D_REGISTER_TEST( TestITKGrayscaleGeodesicDilateImageGrayscaleGeodesicDilate1Test());
    DREAM3D_REGISTER_TEST( TestITKGrayscaleGeodesicDilateImageGrayscaleGeodesicDilate2Test());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST( this->RemoveTestFiles() )
    }
  }

  private:
    ITKGrayscaleGeodesicDilateImageTest(const ITKGrayscaleGeodesicDilateImageTest&); // Copy Constructor Not Implemented
    void operator=(const ITKGrayscaleGeodesicDilateImageTest&); // Operator '=' Not Implemented
};

