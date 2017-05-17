// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>

class ITKBoxMeanImageTest : public ITKTestBase
{

public:
  ITKBoxMeanImageTest()
  {
  }
  virtual ~ITKBoxMeanImageTest()
  {
  }

  int TestITKBoxMeanImagedefaultsTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/VM1111Shrink-RGBFloat.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBoxMeanImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
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
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKBoxMeanImagedefaults.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("dc71425a437766b9566cb13bd4139a91"));
    return 0;
  }

  int TestITKBoxMeanImageby23Test()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/VM1111Shrink-RGB.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBoxMeanImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
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
      FloatVec3_t d3d_var;
      d3d_var.y = 3;
      d3d_var.x = 2;
      d3d_var.z = 0; // should not be taken into account. Dim <
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("Radius", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKBoxMeanImageby23.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("23c2a0d1f7816366aa55b90c0b86f6e6"));
    return 0;
  }

  int TestITKBoxMeanImageby333Test()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Short.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBoxMeanImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
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
      FloatVec3_t d3d_var;
      d3d_var.y = 3;
      d3d_var.x = 3;
      d3d_var.z = 3;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("Radius", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKBoxMeanImageby333.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("c6a8c4774f59ac92eefcaea18bd0f514"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKBoxMeanImage"));

    DREAM3D_REGISTER_TEST(TestITKBoxMeanImagedefaultsTest());
    DREAM3D_REGISTER_TEST(TestITKBoxMeanImageby23Test());
    DREAM3D_REGISTER_TEST(TestITKBoxMeanImageby333Test());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKBoxMeanImageTest(const ITKBoxMeanImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKBoxMeanImageTest&);      // Operator '=' Not Implemented
};
