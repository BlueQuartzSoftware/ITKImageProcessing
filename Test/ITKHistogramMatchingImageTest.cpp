// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>

class ITKHistogramMatchingImageTest : public ITKTestBase
{

public:
  ITKHistogramMatchingImageTest()
  {
  }
  virtual ~ITKHistogramMatchingImageTest()
  {
  }

  int TestITKHistogramMatchingImagedefaultsTest()
  {
    QString source_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/cthead1.png");
    QString reference_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/LargeWhiteCircle.nrrd");
    DataArrayPath source_path("SourceContainer", "SourceAttributeMatrixName", "SourceAttributeArrayName");
    DataArrayPath reference_path("ReferenceContainer", "ReferenceAttributeMatrixName", "ReferenceAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(source_filename, containerArray, source_path);
    this->ReadImage(reference_filename, containerArray, reference_path);
    QString filtName = "ITKHistogramMatchingImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(source_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(reference_path);
    propWasSet = filter->setProperty("ReferenceCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKHistogramMatchingImagedefaults.nrrd", containerArray, source_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_HistogramMatchingImageFilter_defaults.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    this->ReadImage(baseline_filename, containerArray, baseline_path);
    int res = this->CompareImages(containerArray, source_path, baseline_path, 0.0001);
    DREAM3D_REQUIRE_EQUAL(res, 0);
    return 0;
  }

  int TestITKHistogramMatchingImageSameDimension()
  {
    QString source_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/Ramp-One-Zero-Float.nrrd");
    QString reference_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Slice-Float.nrrd");
    DataArrayPath source_path("SourceContainer", "SourceAttributeMatrixName", "SourceAttributeArrayName");
    DataArrayPath reference_path("ReferenceContainer", "ReferenceAttributeMatrixName", "ReferenceAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(source_filename, containerArray, source_path);
    this->ReadImage(reference_filename, containerArray, reference_path);
    QString filtName = "ITKHistogramMatchingImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(source_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(reference_path);
    propWasSet = filter->setProperty("ReferenceCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCondition(), -7);
    return 0;
  }

  int TestITKHistogramMatchingImageTestSamePixelType()
  {
    QString source_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/cthead1.png");
    QString reference_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Slice-Float.nrrd");
    DataArrayPath source_path("SourceContainer", "SourceAttributeMatrixName", "SourceAttributeArrayName");
    DataArrayPath reference_path("ReferenceContainer", "ReferenceAttributeMatrixName", "ReferenceAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(source_filename, containerArray, source_path);
    this->ReadImage(reference_filename, containerArray, reference_path);
    QString filtName = "ITKHistogramMatchingImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(source_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(reference_path);
    propWasSet = filter->setProperty("ReferenceCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCondition(), -5);
    return 0;
  }

  int TestITKHistogramMatchingImageTestDataArrayDoesnotExist()
  {
    QString source_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/cthead1.png");
    DataArrayPath source_path("SourceContainer", "SourceAttributeMatrixName", "SourceAttributeArrayName");
    DataArrayPath reference_path("ReferenceContainer", "ReferenceAttributeMatrixName", "ReferenceAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(source_filename, containerArray, source_path);
    QString filtName = "ITKHistogramMatchingImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(source_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(reference_path);
    propWasSet = filter->setProperty("ReferenceCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCondition(), -6);
    return 0;
  }

  int TestITKHistogramMatchingImagenear_identityTest()
  {
    QString source_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Short.nrrd");
    DataArrayPath source_path("SourceContainer", "SourceAttributeMatrixName", "SourceAttributeArrayName");
    DataArrayPath reference_path("ReferenceContainer", "ReferenceAttributeMatrixName", "ReferenceAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    this->ReadImage(source_filename, containerArray, source_path);
    this->ReadImage(source_filename, containerArray, reference_path);
    QString filtName = "ITKHistogramMatchingImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    AbstractFilter::Pointer filter = filterFactory->create();
    QVariant var;
    bool propWasSet;
    var.setValue(source_path);
    propWasSet = filter->setProperty("SelectedCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(reference_path);
    propWasSet = filter->setProperty("ReferenceCellArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(false);
    propWasSet = filter->setProperty("SaveAsNewArray", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    {
      double d3d_var;
      d3d_var = 65536;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("NumberOfHistogramLevels", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    {
      bool d3d_var;
      d3d_var = false;
      var.setValue(d3d_var);
      propWasSet = filter->setProperty("ThresholdAtMeanIntensity", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    }
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKHistogramMatchingImagenear_identity.nrrd", containerArray, source_path);
    QString md5Output;
    GetMD5FromDataContainer(containerArray, source_path, md5Output);
    DREAM3D_REQUIRE_EQUAL(QString(md5Output), QString("a963bd6a755b853103a2d195e01a50d3"));
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKHistogramMatchingImage"));

    DREAM3D_REGISTER_TEST(TestITKHistogramMatchingImagedefaultsTest());
    DREAM3D_REGISTER_TEST(TestITKHistogramMatchingImagenear_identityTest());
    DREAM3D_REGISTER_TEST(TestITKHistogramMatchingImageSameDimension());
    DREAM3D_REGISTER_TEST(TestITKHistogramMatchingImageTestSamePixelType());
    DREAM3D_REGISTER_TEST(TestITKHistogramMatchingImageTestDataArrayDoesnotExist());
    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKHistogramMatchingImageTest(const ITKHistogramMatchingImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKHistogramMatchingImageTest&);                // Operator '=' Not Implemented
};
