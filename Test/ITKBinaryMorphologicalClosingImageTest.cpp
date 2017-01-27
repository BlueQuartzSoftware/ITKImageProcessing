// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/SimpleITKEnums.h"
//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


class ITKBinaryMorphologicalClosingImageTest: public ITKTestBase
{

  public:
    ITKBinaryMorphologicalClosingImageTest() {}
    virtual ~ITKBinaryMorphologicalClosingImageTest() {}

int TestITKBinaryMorphologicalClosingImageBinaryMorphologicalClosingTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/STAPLE1.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryMorphologicalClosingImage";
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
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);
    WriteImage("ITKBinaryMorphologicalClosingImageBinaryMorphologicalClosing.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("095f00a68a84df4396914fa758f34dcc"));
    return 0;
}

int TestITKBinaryMorphologicalClosingImageBinaryMorphologicalClosingWithBorderTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/WhiteDots.png");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKBinaryMorphologicalClosingImage";
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
        FloatVec3_t d3d_var;
        d3d_var.x = 5;
        d3d_var.y = 5;
        d3d_var.z = 5;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("KernelRadius", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    {
        bool d3d_var;
        d3d_var = false;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("SafeBorder", var);
        DREAM3D_REQUIRE_EQUAL(propWasSet, true);
        }
    {
        double d3d_var;
        d3d_var = 255;
        var.setValue(d3d_var);
        propWasSet = filter->setProperty("ForegroundValue", var);
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
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);
    WriteImage("ITKBinaryMorphologicalClosingImageBinaryMorphologicalClosingWithBorder.nrrd", containerArray, input_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, input_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("506d365dd92db16c2ade264fca46890c"));
    return 0;
}




  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( this->TestFilterAvailability("ITKBinaryMorphologicalClosingImage") );

    DREAM3D_REGISTER_TEST( TestITKBinaryMorphologicalClosingImageBinaryMorphologicalClosingTest());
    DREAM3D_REGISTER_TEST( TestITKBinaryMorphologicalClosingImageBinaryMorphologicalClosingWithBorderTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST( this->RemoveTestFiles() )
    }
  }

  private:
    ITKBinaryMorphologicalClosingImageTest(const ITKBinaryMorphologicalClosingImageTest&); // Copy Constructor Not Implemented
    void operator=(const ITKBinaryMorphologicalClosingImageTest&); // Operator '=' Not Implemented
};

