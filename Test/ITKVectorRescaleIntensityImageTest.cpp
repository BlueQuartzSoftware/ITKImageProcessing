// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>

#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceDream3DDataToImageFilter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"

#include <itkImageIOBase.h>

class ITKVectorRescaleIntensityImageTest : public ITKTestBase
{

public:
  ITKVectorRescaleIntensityImageTest()
  {
  }
  virtual ~ITKVectorRescaleIntensityImageTest()
  {
  }

  int TestITKVectorRescaleIntensityImage3dTest()
  {
    // Copied from ITK
    // (Modules/Filtering/ImageIntensity/test/itkVectorRescaleIntensityImageFilterTest.cxx)
    const unsigned int VectorDimension = 3;

    typedef itk::Vector<int, VectorDimension> InputPixelType;
    typedef itk::Vector<float, VectorDimension> OutputPixelType;

    const unsigned int ImageDimension = 3;

    typedef itk::Dream3DImage<InputPixelType, ImageDimension> InputImageType;
    typedef itk::Dream3DImage<OutputPixelType, ImageDimension> OutputImageType;

    InputImageType::Pointer inputImage = InputImageType::New();
    InputImageType::RegionType region;
    InputImageType::SizeType size;
    InputImageType::IndexType index;

    size.Fill(20);
    index.Fill(0);

    region.SetIndex(index);
    region.SetSize(size);

    InputPixelType pixelValue;
    pixelValue[0] = 10;
    pixelValue[1] = 20;
    pixelValue[2] = 30;

    inputImage->SetRegions(region);
    inputImage->Allocate();
    inputImage->FillBuffer(pixelValue);

    const double desiredMaximum = 2.0;
    ////////////////////////////
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    // Convert ITK image to Dream3D data
    //
    typedef itk::InPlaceImageToDream3DDataFilter<InputPixelType, ImageDimension> toDream3DType;
    DataContainer::Pointer container = containerArray->createNonPrereqDataContainer<AbstractFilter>(0, input_path.getDataContainerName());
    DREAM3D_REQUIRED(container.get(), !=, 0);
    QVector<size_t> dims = ITKDream3DHelper::GetComponentsDimensions<OutputPixelType>();
    DataContainer::Pointer dc = containerArray->getDataContainer(input_path.getDataContainerName());
    toDream3DType::Pointer toDream3D = toDream3DType::New();
    toDream3D->SetInput(inputImage);
    toDream3D->SetInPlace(true);
    toDream3D->SetAttributeMatrixArrayName(input_path.getAttributeMatrixName().toStdString());
    toDream3D->SetDataArrayName(input_path.getDataArrayName().toStdString());
    toDream3D->SetDataContainer(dc);
    toDream3D->Update();
    //
    QString filtName = "ITKVectorRescaleIntensityImage";
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
    var.setValue(2.0);
    propWasSet = filter->setProperty("OutputMaximumMagnitude", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    var.setValue(itk::ImageIOBase::IOComponentType::FLOAT - 1);
    propWasSet = filter->setProperty("OutputType", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);
    filter->setDataContainerArray(containerArray);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >=, 0);
    WriteImage("ITKVectorRescaleIntensityImage3d.nrrd", containerArray, input_path);
    // Convert filter output to ITK image
    typedef itk::InPlaceDream3DDataToImageFilter<OutputPixelType, ImageDimension> toITKType;
    toITKType::Pointer toITK = toITKType::New();
    toITK->SetInput(dc);
    toITK->SetInPlace(true);
    toITK->SetAttributeMatrixArrayName(input_path.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(input_path.getDataArrayName().toStdString());
    toITK->Update();

    // Copied from ITK
    // (Modules/Filtering/ImageIntensity/test/itkVectorRescaleIntensityImageFilterTest.cxx)

    OutputImageType::ConstPointer outputImage = toITK->GetOutput();

    typedef itk::ImageRegionConstIterator<OutputImageType> IteratorType;

    IteratorType ot(outputImage, outputImage->GetBufferedRegion());

    ot.GoToBegin();

    const double tolerance = 1e-3;

    const double factor = desiredMaximum / static_cast<double>(pixelValue.GetNorm());

    while(!ot.IsAtEnd())
    {
      const OutputPixelType outputValue = ot.Get();
      for(unsigned int k = 0; k < VectorDimension; k++)
      {
        if(itk::Math::NotAlmostEquals(outputValue[k], itk::NumericTraits<itk::NumericTraits<OutputPixelType>::ValueType>::ZeroValue()))
        {
          float outputVal = static_cast<float>(outputValue[k]);
          float pixelVal = static_cast<double>(pixelValue[k] * factor);
          DREAM3D_COMPARE_FLOATS(&outputVal, &pixelVal, tolerance);
          //        if( !itk::Math::FloatAlmostEqual( static_cast< double > ( outputValue[k] ),
          //          static_cast< double >( pixelValue[k] * factor ), 10, tolerance ) )
          //          {
          //          std::cerr << "Test FAILED !" << std::endl;
          //          std::cerr.precision( static_cast< int >( itk::Math::abs( std::log10( tolerance ) ) ) );
          //          std::cerr << "Input  Pixel Value = " << pixelValue  << std::endl;
          //          std::cerr << "Output Pixel Value = " << outputValue << std::endl;
          //          return EXIT_FAILURE;
          //          }
        }
      }
      ++ot;
    }
    return 0;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKVectorRescaleIntensityImage"));

    DREAM3D_REGISTER_TEST(TestITKVectorRescaleIntensityImage3dTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }

private:
  ITKVectorRescaleIntensityImageTest(const ITKVectorRescaleIntensityImageTest&); // Copy Constructor Not Implemented
  void operator=(const ITKVectorRescaleIntensityImageTest&);                     // Operator '=' Not Implemented
};
