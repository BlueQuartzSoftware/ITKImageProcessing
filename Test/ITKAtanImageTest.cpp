// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Common/FilterPipeline.h"
#include "SIMPLib/Common/FilterManager.h"
#include "SIMPLib/Common/FilterFactory.hpp"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"
#include "SIMPLib/Utilities/QMetaObjectUtilities.h"

#include "ITKImageProcessingTestFileLocations.h"

#include <itkAtanImageFilter.h>


// Testing
#include <itkTestingHashImageFilter.h>
#include <itkTestingComparisonImageFilter.h>

class ITKAtanImageTest
{

  public:
    ITKAtanImageTest() {}
    virtual ~ITKAtanImageTest() {}


  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
    foreach(QString filename, this->FilesToRemove)
    {
      QFile::remove(filename);
    }
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestFilterAvailability()
  {
    // Now instantiate the ITKAtanImageTest Filter from the FilterManager
    QString filtName = "ITKAtanImage";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);
    if (nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The ITKImageProcessing Requires the use of the " << filtName.toStdString() << " filter which is found in the ITKImageProcessing Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }
    return 0;
  }

//  // -----------------------------------------------------------------------------
//  //
//  // -----------------------------------------------------------------------------
//  int TestITKAtanImageTest()
//  {
//    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   /* Please write ITKAtanImageTest test code here.
//    *
//    * Your IO test files are:
//    * UnitTest::ITKAtanImageTest::TestFile1
//    * UnitTest::ITKAtanImageTest::TestFile2
//    *
//    * SIMPLib provides some macros that will throw exceptions when a test fails
//    * and thus report that during testing. These macros are located in the
//    * SIMPLib/Utilities/UnitTestSupport.hpp file. Some examples are:
//    *
//    * SIMPLib_REQUIRE_EQUAL(foo, 0)
//    * This means that if the variable foo is NOT equal to Zero then test will fail
//    * and the current test will exit immediately. If there are more tests registered
//    * with the SIMPLib_REGISTER_TEST() macro, the next test will execute. There are
//    * lots of examples in the SIMPLib/Test folder to look at.
//    */
//    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//    int foo = 0;
//    DREAM3D_REQUIRE_EQUAL(foo, 0)

//    return EXIT_SUCCESS;
//  }

int TestITKAtanImagedefaultsTest()
{
    QString input_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Input/Ramp-Zero-One-Float.nrrd");
    DataArrayPath input_path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    ReadImage(input_filename, containerArray, input_path);
    QString filtName = "ITKAtanImage";
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
    WriteImage("ITKAtanImagedefaults.nrrd", containerArray, input_path);
    QString baseline_filename = UnitTest::DataDir + QString("/Data/JSONFilters/Baseline/BasicFilters_AtanImageFilter_defaults.nrrd");
    DataArrayPath baseline_path("BContainer", "BAttributeMatrixName", "BAttributeArrayName");
    ReadImage(baseline_filename, containerArray, baseline_path);
    int res = CompareImages(containerArray, input_path, baseline_path, 0.01);
    DREAM3D_REQUIRE_EQUAL(res,0);
    return 0;
}



template<typename PixelType, unsigned int Dimensions>
int CompareImages(DataContainer::Pointer input_container,
                 const DataArrayPath& input_path,
                 DataContainer::Pointer baseline_container,
                 const DataArrayPath& baseline_path,
                 double tolerance)
{
    typedef itk::Dream3DImage<PixelType, Dimensions>   ImageType;
    typedef itk::InPlaceDream3DDataToImageFilter<PixelType, Dimensions> ToITKType;
    typename ToITKType::Pointer toITK = ToITKType::New();
    // Convert input to ITK image
    toITK->SetInput(input_container);
    toITK->SetAttributeMatrixArrayName(input_path.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(input_path.getDataArrayName().toStdString());
    toITK->SetInPlace(true);
    toITK->Update();
    typename ImageType::Pointer input = toITK->GetOutput();
    input->DisconnectPipeline();
    // Convert baseline to ITK image
    toITK->SetInput(baseline_container);
    toITK->SetAttributeMatrixArrayName(baseline_path.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(baseline_path.getDataArrayName().toStdString());
    toITK->Update();
    typename ImageType::Pointer baseline = toITK->GetOutput();
    baseline->DisconnectPipeline();
    typedef itk::Testing::ComparisonImageFilter<ImageType,ImageType> ComparisonFilterType;
    typename ComparisonFilterType::Pointer comparisonFilter = ComparisonFilterType::New();
    comparisonFilter->SetTestInput(input);
    comparisonFilter->SetValidInput(baseline);
    comparisonFilter->Update();
    if(static_cast<double>(comparisonFilter->GetMaximumDifference()) > tolerance)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

template<typename PixelType>
int CompareImages(size_t dimension,
                 DataContainer::Pointer input_container,
                 const DataArrayPath& input_path,
                 DataContainer::Pointer baseline_container,
                 const DataArrayPath& baseline_path,
                 double tolerance)
{
    if (dimension == 1)
    {
      /* 2D image */
      return CompareImages<PixelType, 2>(input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else
    {
      /* 3D */
      return CompareImages<PixelType, 3>(input_container, input_path, baseline_container, baseline_path, tolerance);
    }
}

int GetDataArray(DataContainerArray::Pointer &containerArray,
                 const DataArrayPath &path,
                 DataContainer::Pointer &container,
                 QVector<size_t> &tDims,
                 QString &type)
{
    container = containerArray->getDataContainer(path.getDataContainerName());
    AttributeMatrix::Pointer matrix = containerArray->getAttributeMatrix(path);
    IDataArray::Pointer ptr = matrix->getAttributeArray(path.getDataArrayName());
    DREAM3D_REQUIRE_VALID_POINTER(ptr.get());
    // Container geometry should be of type Geometry
	ImageGeom::Pointer imageGeometry = ImageGeom::New();
	imageGeometry = container->getGeometryAs<ImageGeom>();
    DREAM3D_REQUIRE_VALID_POINTER(imageGeometry.get());
    imageGeometry->getDimensions(tDims[0], tDims[1], tDims[2]);
    type = ptr->getTypeAsString();
    return 0;
}

int CompareImages(DataContainerArray::Pointer &containerArray,
                 const DataArrayPath &input_path,
                 const DataArrayPath &baseline_path,
                 double tolerance)
{
    DataContainer::Pointer input_container;
    const int dimSize = 3;
    QVector<size_t> input_tDims(dimSize, 0);
    QString input_type;
    int res_i = GetDataArray(containerArray, input_path, input_container, input_tDims, input_type);
    DREAM3D_REQUIRE_EQUAL(res_i, 0);
    DataContainer::Pointer baseline_container;
    QVector<size_t> baseline_tDims(dimSize, 0);
    QString baseline_type;
    int res_b = GetDataArray(containerArray, baseline_path, baseline_container, baseline_tDims, baseline_type);
    DREAM3D_REQUIRE_EQUAL(res_b, 0);    
    DREAM3D_REQUIRE_EQUAL(input_type, baseline_type);
    for(int ii = 0; ii < dimSize ; ii++)
    {
        DREAM3D_REQUIRE_EQUAL(input_tDims[ii], baseline_tDims[ii]);
    }
    if( input_type.compare("float") == 0 )
    {
        return CompareImages<float>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("double") == 0 )
    {
        return CompareImages<double>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("int8_t") == 0 )
    {
        return CompareImages<int8_t>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("uint8_t") == 0 )
    {
        return CompareImages<uint8_t>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("int16_t") == 0 )
    {
        return CompareImages<int16_t>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("uint16_t") == 0 )
    {
        return CompareImages<uint16_t>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("int32_t") == 0 )
    {
        return CompareImages<int32_t>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("uint32_t") == 0 )
    {
        return CompareImages<uint32_t>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("int64_t") == 0 )
    {
        return CompareImages<int64_t>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if( input_type.compare("uint64_t") == 0 )
    {
        return CompareImages<uint64_t>(input_tDims[2], input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else
    {
        return 1 ;
    }
}

int ReadImage(const QString& filename, DataContainerArray::Pointer &containerArray, const DataArrayPath &path)
{
    FilterPipeline::Pointer pipeline = FilterPipeline::New();

    QString filtName = "ITKImageReader";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);

    DREAM3D_REQUIRE_VALID_POINTER(filterFactory.get());

    AbstractFilter::Pointer reader = filterFactory->create();
    DREAM3D_REQUIRE_VALID_POINTER(reader.get());

    bool propertySet = false;
    reader->setDataContainerArray(containerArray);
    propertySet = reader->setProperty("DataContainerName", path.getDataContainerName());
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    propertySet = reader->setProperty("CellAttributeMatrixName", path.getAttributeMatrixName());
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    propertySet = reader->setProperty("ImageDataArrayName", path.getDataArrayName());
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    propertySet = reader->setProperty("FileName", filename);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);

    containerArray = reader->getDataContainerArray();

    return 0;
}


int WriteImage(const QString& filename, DataContainerArray::Pointer &containerArray, const DataArrayPath &path)
{
    QString fileWithPath = UnitTest::TestTempDir+QString("/")+QString(filename);

    FilterPipeline::Pointer pipeline = FilterPipeline::New();

    QString filtName = "ITKImageWriter";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);

    DREAM3D_REQUIRE_VALID_POINTER(filterFactory.get());

    AbstractFilter::Pointer writer = filterFactory->create();
    DREAM3D_REQUIRE_VALID_POINTER(writer.get());

    bool propertySet = false;
    writer->setDataContainerArray(containerArray);
    QVariant var;
    var.setValue(path);
    propertySet = writer->setProperty("ImageArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    propertySet = writer->setProperty("FileName", fileWithPath);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    writer->execute();
    DREAM3D_REQUIRED(writer->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(writer->getWarningCondition(), >= , 0);

    // Remove file at the end of the tests_settings
    FilesToRemove << fileWithPath;
    return 0;
}


template<typename ImageType>
int GetMD5FromITKImage(const typename ImageType::Pointer &image, QString &md5)
{
    try
    {
        typedef itk::Testing::HashImageFilter<ImageType> MD5FilterType;
        typename MD5FilterType::Pointer md5Filter = MD5FilterType::New();
        md5Filter->SetInput(image);
        md5Filter->Update();
        md5 = md5Filter->GetHash().c_str();
    }
    catch(itk::ExceptionObject &e)
    {
        std::cerr << "Problems computing MD5" << std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return 1;
    }
    return 0;
}

template<typename TPixel, unsigned int Dimensions>
int GetMD5FromDataContainer(DataContainer::Pointer container, const DataArrayPath &path , QString &md5)
{
  try
  {
      typedef itk::Dream3DImage<TPixel, Dimensions>   ImageType;
      typedef itk::InPlaceDream3DDataToImageFilter<TPixel, Dimensions> ToITKType;
      typename ToITKType::Pointer toITK = ToITKType::New();
      toITK->SetInput(container);
      toITK->SetAttributeMatrixArrayName(path.getAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(path.getDataArrayName().toStdString());
      toITK->SetInPlace(true);
      toITK->Update();
      return GetMD5FromITKImage<ImageType>(toITK->GetOutput(), md5);
  }
  catch(itk::ExceptionObject &e)
  {
      std::cerr << "Problems converting data container to ITK image" << std::endl;
      std::cerr << e.GetDescription() << std::endl;
      return 1;
  }
}

template<typename PixelType>
int GetMD5FromDataContainer(size_t dimension,
                 DataContainer::Pointer container,
                 const DataArrayPath& path,
                 QString &md5)
{
    if (dimension == 1)
    {
      /* 2D image */
      return GetMD5FromDataContainer<PixelType, 2>(container, path, md5);
    }
    else
    {
      /* 3D */
      return GetMD5FromDataContainer<PixelType, 3>(container, path, md5);
    }
}


int GetMD5FromDataContainer(DataContainerArray::Pointer &containerArray,
                 const DataArrayPath &path,
                 QString &md5)
{
    DataContainer::Pointer container;
    const int dimSize = 3;
    QVector<size_t> tDims(dimSize, 0);
    QString data_type;
    int res_i = GetDataArray(containerArray, path, container, tDims, data_type);
    DREAM3D_REQUIRE_EQUAL(res_i, 0);
    DataContainer::Pointer baseline_container;
    if( data_type.compare("float") == 0 )
    {
        return GetMD5FromDataContainer<float>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("double") == 0 )
    {
        return GetMD5FromDataContainer<double>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("int8_t") == 0 )
    {
        return GetMD5FromDataContainer<int8_t>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("uint8_t") == 0 )
    {
        return GetMD5FromDataContainer<uint8_t>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("int16_t") == 0 )
    {
        return GetMD5FromDataContainer<int16_t>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("uint16_t") == 0 )
    {
        return GetMD5FromDataContainer<uint16_t>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("int32_t") == 0 )
    {
        return GetMD5FromDataContainer<int32_t>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("uint32_t") == 0 )
    {
        return GetMD5FromDataContainer<uint32_t>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("int64_t") == 0 )
    {
        return GetMD5FromDataContainer<int64_t>(tDims[2], container, path, md5);
    }
    else if( data_type.compare("uint64_t") == 0 )
    {
        return GetMD5FromDataContainer<uint64_t>(tDims[2], container, path, md5);
    }
    else
    {
        return 1 ;
    }
}









  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( TestFilterAvailability() );

    //DREAM3D_REGISTER_TEST( TestITKAtanImageTest() )
    DREAM3D_REGISTER_TEST( TestITKAtanImagedefaultsTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST( RemoveTestFiles() )
    }
  }

  private:
    ITKAtanImageTest(const ITKAtanImageTest&); // Copy Constructor Not Implemented
    void operator=(const ITKAtanImageTest&); // Operator '=' Not Implemented
    QList<QString> FilesToRemove;
};

