// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"

class ITKMorphologicalWatershedFromMarkersImageTest : public ITKTestBase
{

public:
  ITKMorphologicalWatershedFromMarkersImageTest() = default;
  ~ITKMorphologicalWatershedFromMarkersImageTest() override = default;

  int TestITKMorphologicalWatershedFromMarkersImagedefaultsTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/cthead1-grad-mag.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    QString outputName = "TestAttributeArrayName_Output";
    DataArrayPath output_path("TestContainer", "TestAttributeMatrixName", outputName);
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString marker_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/cthead1-marker.png");
    DataArrayPath marker_path("MarkerContainer", "MarkerAttributeMatrixName", "MarkerAttributeArrayName");
    this->ReadImage(marker_filename, containerArray, marker_path);
    QString filtName = "ITKMorphologicalWatershedFromMarkersImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(input_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(marker_path);
    propWasSet = filter->setProperty("MarkerCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(outputName);
    propWasSet = filter->setProperty("NewCellArrayName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKMorphologicalWatershedFromMarkersImagedefaults.nrrd", containerArray, output_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, output_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("9de771247c8cac2272854dcc932ca2be"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKMorphologicalWatershedFromMarkersImage"));

    DREAM3D_REGISTER_TEST(TestITKMorphologicalWatershedFromMarkersImagedefaultsTest());

    //    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    //    {
    //      DREAM3D_REGISTER_TEST( this->RemoveTestFiles() )
    //    }
  }

private:
  ITKMorphologicalWatershedFromMarkersImageTest(const ITKMorphologicalWatershedFromMarkersImageTest&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKMorphologicalWatershedFromMarkersImageTest&) = delete;                                // Move assignment Not Implemented
};
