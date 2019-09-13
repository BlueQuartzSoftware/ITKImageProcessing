// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include "SIMPLib/CoreFilters/ConvertColorToGrayScale.h"
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>

#include "ITKImageProcessingFilters/ITKAdaptiveHistogramEqualizationImage.h"

class ITKAdaptiveHistogramEqualizationImageTest : public ITKTestBase
{

public:
ITKAdaptiveHistogramEqualizationImageTest() = default;
~ITKAdaptiveHistogramEqualizationImageTest() override = default;

ITKAdaptiveHistogramEqualizationImageTest(const ITKAdaptiveHistogramEqualizationImageTest&) = delete;            // Copy Constructor Not Implemented
ITKAdaptiveHistogramEqualizationImageTest(ITKAdaptiveHistogramEqualizationImageTest&&) = delete;                 // Move Constructor Not Implemented
ITKAdaptiveHistogramEqualizationImageTest& operator=(const ITKAdaptiveHistogramEqualizationImageTest&) = delete; // Copy Assignment Not Implemented
ITKAdaptiveHistogramEqualizationImageTest& operator=(ITKAdaptiveHistogramEqualizationImageTest&&) = delete;      // Move Assignment Not Implemented

void ConvertInputImage(const DataContainerArray::Pointer& containerArray, const DataArrayPath& input_path)
{
  ConvertColorToGrayScale::Pointer c2g = ConvertColorToGrayScale::New();
  c2g->setDataContainerArray(containerArray);
  c2g->setConversionAlgorithm(0);
  c2g->setColorWeights({0.2125f, 0.7154f, 0.0721f});
  c2g->setInputDataArrayVector({input_path});
  c2g->setCreateNewAttributeMatrix(false);
  c2g->setOutputArrayPrefix("GrayScale_");
  c2g->execute();
  DREAM3D_REQUIRED(c2g->getErrorCode(), >=, 0);
  DREAM3D_REQUIRED(c2g->getWarningCode(), >=, 0);
}

int ITKAdaptiveHistogramEqualizationImageTest1()
{
#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 4
  QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/RA-Slice-Float.nrrd");
#else
  QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/sf4.png");
#endif
  DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
  DataContainerArray::Pointer containerArray = DataContainerArray::New();
  this->ReadImage(input_filename, containerArray, input_path);

#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 5
  ConvertInputImage(containerArray, input_path);
  input_path.setDataArrayName("GrayScale_TestAttributeArrayName"); // Update the data array name for the next filter
#endif

  ITKAdaptiveHistogramEqualizationImage::Pointer filter = ITKAdaptiveHistogramEqualizationImage::New();
  filter->setDataContainerArray(containerArray);
  filter->setSelectedCellArrayPath(input_path);
  filter->setSaveAsNewArray(false);

#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 5
  // We change the values for the test
  filter->setRadius({10.0f, 10.0f, 10.0f});
  filter->setAlpha(0.5f);
  filter->setBeta(0.5f);
#endif

  filter->execute();
  DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
  DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
  WriteImage("ITKAdaptiveHistogramEqualizationImagedefaults.nrrd", containerArray, input_path);
#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 4
  QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_AdaptiveHistogramEqualizationImageFilter_defaults.nrrd");
#else
  QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters/ITKAdaptiveHistogramEqualizationFilterTest.png");
#endif

  DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
  this->ReadImage(baseline_filename, containerArray, baseline_path);
  int res = this->CompareImages(containerArray, input_path, baseline_path, 2e-3);
  DREAM3D_REQUIRE_EQUAL(res, 0);
  return 0;
}

int ITKAdaptiveHistogramEqualizationImageTest2()
{
#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 4
  QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/cthead1.png");
#else
  QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/sf4.png");
#endif

  DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
  DataContainerArray::Pointer containerArray = DataContainerArray::New();
  this->ReadImage(input_filename, containerArray, input_path);

#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 5
  ConvertInputImage(containerArray, input_path);
  input_path.setDataArrayName("GrayScale_TestAttributeArrayName"); // Update the data array name for the next filter
#endif

  ITKAdaptiveHistogramEqualizationImage::Pointer filter = ITKAdaptiveHistogramEqualizationImage::New();
  filter->setDataContainerArray(containerArray);
  filter->setSelectedCellArrayPath(input_path);
  filter->setSaveAsNewArray(false);

#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 4
  // We change the values for the test
  filter->setRadius({5.0f, 5.0f, 5.0f});
  filter->setAlpha(0.0f);
  filter->setBeta(0.0f);
#else
  // We change the values for the test
  filter->setRadius({10.0f, 10.0f, 10.0f}); // Default inside the filter
  filter->setAlpha(1.0f);
  filter->setBeta(0.25f);
#endif

  filter->execute();
  DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
  DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
  WriteImage("ITKAdaptiveHistogramEqualizationImagehisto.nrrd", containerArray, input_path);

#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 4
  QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_AdaptiveHistogramEqualizationImageFilter_histo.nrrd");
#else
  QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters/ITKAdaptiveHistogramEqualizationFilterTest2.png");
#endif
  DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
  this->ReadImage(baseline_filename, containerArray, baseline_path);
  int res = this->CompareImages(containerArray, input_path, baseline_path, 1e-5);
  DREAM3D_REQUIRE_EQUAL(res, 0);
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void operator()() override
{
  int err = EXIT_SUCCESS;

  DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKAdaptiveHistogramEqualizationImage"));

  DREAM3D_REGISTER_TEST(ITKAdaptiveHistogramEqualizationImageTest1());
  DREAM3D_REGISTER_TEST(ITKAdaptiveHistogramEqualizationImageTest2());

  if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
  {
    DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
  }
}
};
