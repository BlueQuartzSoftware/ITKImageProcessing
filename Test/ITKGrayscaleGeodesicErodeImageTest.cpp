// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>


class ITKGrayscaleGeodesicErodeImageTest: public ITKTestBase
{

  public:
    ITKGrayscaleGeodesicErodeImageTest() {}
    virtual ~ITKGrayscaleGeodesicErodeImageTest() {}

int TestITKGrayscaleGeodesicErodeImageGrayscaleGeodesicDilateTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/STAPLE2.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGrayscaleGeodesicErodeImage";
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
    WriteImage("ITKGrayscaleGeodesicErodeImageGrayscaleGeodesicDilate.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("095f00a68a84df4396914fa758f34dcc"));
    return 0;
}




  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( this->TestFilterAvailability("ITKGrayscaleGeodesicErodeImage") );

    DREAM3D_REGISTER_TEST( TestITKGrayscaleGeodesicErodeImageGrayscaleGeodesicDilateTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST( this->RemoveTestFiles() )
    }
  }

  private:
    ITKGrayscaleGeodesicErodeImageTest(const ITKGrayscaleGeodesicErodeImageTest&); // Copy Constructor Not Implemented
    void operator=(const ITKGrayscaleGeodesicErodeImageTest&); // Operator '=' Not Implemented
};

