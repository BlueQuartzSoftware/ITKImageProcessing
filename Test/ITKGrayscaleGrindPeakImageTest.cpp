// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>


class ITKGrayscaleGrindPeakImageTest : public ITKTestBase
{

public:
ITKGrayscaleGrindPeakImageTest() = default;
~ITKGrayscaleGrindPeakImageTest() override = default;

  int TestITKGrayscaleGrindPeakImageGrayscaleGrindPeak1Test()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Short.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGrayscaleGrindPeakImage";
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
    WriteImage("ITKGrayscaleGrindPeakImageGrayscaleGrindPeak1.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("084cdd1d64664ebfab26c2e0ed382e14"));
    return 0;
}

int TestITKGrayscaleGrindPeakImageGrayscaleGrindPeak2Test()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Slice-Short.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGrayscaleGrindPeakImage";
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
    WriteImage("ITKGrayscaleGrindPeakImageGrayscaleGrindPeak2.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("b18d75cccb9361c65b40bb5c0d3c6e0d"));
    return 0;
}



  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKGrayscaleGrindPeakImage"));

    DREAM3D_REGISTER_TEST( TestITKGrayscaleGrindPeakImageGrayscaleGrindPeak1Test());
    DREAM3D_REGISTER_TEST( TestITKGrayscaleGrindPeakImageGrayscaleGrindPeak2Test());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKGrayscaleGrindPeakImageTest(const ITKGrayscaleGrindPeakImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKGrayscaleGrindPeakImageTest&);                 // Move assignment Not Implemented
};
