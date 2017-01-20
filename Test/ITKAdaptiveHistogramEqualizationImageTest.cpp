// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/FloatFilterParameter.h>


class ITKAdaptiveHistogramEqualizationImageTest: public ITKTestBase
{

  public:
    ITKAdaptiveHistogramEqualizationImageTest() {}
    virtual ~ITKAdaptiveHistogramEqualizationImageTest() {}

int TestITKAdaptiveHistogramEqualizationImagedefaultsTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Slice-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKAdaptiveHistogramEqualizationImage";
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
    WriteImage("ITKAdaptiveHistogramEqualizationImagedefaults.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_AdaptiveHistogramEqualizationImageFilter_defaults.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 2e-3);
    DREAM3D_REQUIRE_EQUAL(res,0);
    return 0;
}

int TestITKAdaptiveHistogramEqualizationImagehistoTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/cthead1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKAdaptiveHistogramEqualizationImage";
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
        float d3d_var;
        d3d_var = 0.0;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("Alpha", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    {
        float d3d_var;
        d3d_var = 0.0;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("Beta", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);
    WriteImage("ITKAdaptiveHistogramEqualizationImagehisto.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_AdaptiveHistogramEqualizationImageFilter_histo.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, input_path, baseline_path, 1e-5);
    DREAM3D_REQUIRE_EQUAL(res,0);
    return 0;
}




  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( this->TestFilterAvailability("ITKAdaptiveHistogramEqualizationImage") );

    DREAM3D_REGISTER_TEST( TestITKAdaptiveHistogramEqualizationImagedefaultsTest());
    DREAM3D_REGISTER_TEST( TestITKAdaptiveHistogramEqualizationImagehistoTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST( this->RemoveTestFiles() )
    }
  }

  private:
    ITKAdaptiveHistogramEqualizationImageTest(const ITKAdaptiveHistogramEqualizationImageTest&); // Copy Constructor Not Implemented
    void operator=(const ITKAdaptiveHistogramEqualizationImageTest&); // Operator '=' Not Implemented
};

