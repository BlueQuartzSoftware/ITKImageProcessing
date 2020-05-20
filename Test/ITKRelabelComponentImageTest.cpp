// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"

class ITKRelabelComponentImageTest : public ITKTestBase
{

public:
  ITKRelabelComponentImageTest() = default;
  ~ITKRelabelComponentImageTest() override = default;

  int TestITKRelabelComponentImagedefaultTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/2th_cthead1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKRelabelComponentImage";
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
    WriteImage("ITKRelabelComponentImagedefault.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("58af064e929f08f9d5bacc8be44ed92e"));
    var = filter->property("NumberOfObjects");
    DREAM3D_REQUIRE_EQUAL(var.toUInt(), 2u);
    var = filter->property("OriginalNumberOfObjects");
    DREAM3D_REQUIRE_EQUAL(var.toUInt(), 2u);
    return 0;
  }

  int TestITKRelabelComponentImageno_sortingTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/2th_cthead1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKRelabelComponentImage";
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
    {
      bool d3d_var;
      d3d_var = false;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("SortByObjectSize", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    WriteImage("ITKRelabelComponentImageno_sorting.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 4
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("c8485314800c21580cc125fb240ada6d"));
#else
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("58af064e929f08f9d5bacc8be44ed92e"));
#endif
    var = filter->property("NumberOfObjects");
    DREAM3D_REQUIRE_EQUAL(var.toUInt(), 2u);
    var = filter->property("OriginalNumberOfObjects");
    DREAM3D_REQUIRE_EQUAL(var.toUInt(), 2u);
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKRelabelComponentImage"));

    DREAM3D_REGISTER_TEST(TestITKRelabelComponentImagedefaultTest());
    DREAM3D_REGISTER_TEST(TestITKRelabelComponentImageno_sortingTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKRelabelComponentImageTest(const ITKRelabelComponentImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKRelabelComponentImageTest&);               // Move assignment Not Implemented
};
