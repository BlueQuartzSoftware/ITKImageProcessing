// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKImageProcessing/ITKImageProcessingFilters/SimpleITKEnums.h"
#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>

class ITKGrayscaleDilateImageTest : public ITKTestBase
{

public:
  ITKGrayscaleDilateImageTest()
  {
  }
  virtual ~ITKGrayscaleDilateImageTest()
  {
  }

  int TestITKGrayscaleDilateImageGrayscaleDilateTest()
  {
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/STAPLE1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKGrayscaleDilateImage";
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
      d3d_var.x = 1;
      d3d_var.y = 1;
      d3d_var.z = 1;
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
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKGrayscaleDilateImageGrayscaleDilate.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("cb692559f1eb21e4c932f6bbb3850ad3"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKGrayscaleDilateImage"));

    DREAM3D_REGISTER_TEST(TestITKGrayscaleDilateImageGrayscaleDilateTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKGrayscaleDilateImageTest(const ITKGrayscaleDilateImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKGrayscaleDilateImageTest&);              // Operator '=' Not Implemented
};
