// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#pragma once

#include <QtCore/QFile>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"

#include "UnitTestSupport.hpp"

#include "ITKImageProcessingTestFileLocations.h"

// Testing
#include <itkTestingHashImageFilter.h>

class ITKTestBase
{

public:
  ITKTestBase()
  {
  }
  virtual ~ITKTestBase()
  {
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
    for(QString filename : this->FilesToRemove)
    {
      QFile::remove(filename);
    }
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestFilterAvailability(QString filtName)
  {
    // Now instantiate the Filter from the FilterManager
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    if(nullptr == filterFactory.get())
    {
      std::stringstream ss;
      ss << "The ITKImageProcessing Requires the use of the " << filtName.toStdString() << " filter which is found in the ITKImageProcessing Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(ss.str())
    }
    return 0;
  }

  template <typename PixelType>
  double ComputeDiff(itk::Vector<PixelType, 2> p1, itk::Vector<PixelType, 2> p2)
  {
    double diff = static_cast<double>((p1 - p2).GetNorm());
    return diff;
  }

  template <typename PixelType>
  double ComputeDiff(itk::Vector<PixelType, 3> p1, itk::Vector<PixelType, 3> p2)
  {
    double diff = static_cast<double>((p1 - p2).GetNorm());
    return diff;
  }

  template <typename PixelType>
  double ComputeDiff(itk::RGBAPixel<PixelType> p1, itk::RGBAPixel<PixelType> p2)
  {
    double diff = static_cast<double>((p1 - p2).GetScalarValue());
    return diff;
  }

  template <typename PixelType>
  double ComputeDiff(itk::RGBPixel<PixelType> p1, itk::RGBPixel<PixelType> p2)
  {
    double diff = static_cast<double>((p1 - p2).GetScalarValue());
    return diff;
  }

  template <typename PixelType>
  double ComputeDiff(PixelType p1, PixelType p2)
  {
    return static_cast<double>(p1 - p2);
  }

  template <typename PixelType, unsigned int Dimensions>
  int CompareImages(DataContainer::Pointer input_container, const DataArrayPath& input_path, DataContainer::Pointer baseline_container, const DataArrayPath& baseline_path, double tolerance)
  {
    typedef itk::Image<PixelType, Dimensions> ImageType;
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

    // Compare baseline and output
    // itk::Testing::ComparisonImageFilter only works for scalar pixels
    typedef typename itk::ImageRegionIterator<ImageType> IteratorType;
    IteratorType it(input, input->GetLargestPossibleRegion());
    IteratorType itb(baseline, baseline->GetLargestPossibleRegion());
    double largest_error = 0.0;
    double diff;
    for(it.GoToBegin(), itb.GoToBegin(); !it.IsAtEnd() && !itb.IsAtEnd(); ++it, ++itb)
    {
      diff = ComputeDiff(it.Get(), itb.Get());
      diff = (diff > 0 ? diff : -diff);
      largest_error = std::max(diff, largest_error);
    }
    if(largest_error > tolerance)
    {
      return EXIT_FAILURE;
    }
    else
    {
      return EXIT_SUCCESS;
    }
  }

  template <typename PixelType, unsigned int Dimensions>
  int CompareImages(std::vector<size_t> cDims, DataContainer::Pointer input_container, const DataArrayPath& input_path, DataContainer::Pointer baseline_container, const DataArrayPath& baseline_path,
                    double tolerance)
  {
    // Vector images
    if(cDims.size() > 1)
    {
      if(cDims.size() == 2)
      {
        return CompareImages<itk::Vector<PixelType, 2>, Dimensions>(input_container, input_path, baseline_container, baseline_path, tolerance);
      }
      else if(cDims.size() == 3)
      {
        return CompareImages<itk::Vector<PixelType, 3>, Dimensions>(input_container, input_path, baseline_container, baseline_path, tolerance);
      }
      else
      {
        std::cerr << "Vector size not supported." << std::endl;
        return EXIT_FAILURE;
      }
    }
    else
    {
      // Scalar images
      if(cDims[0] == 1)
      {
        return CompareImages<PixelType, Dimensions>(input_container, input_path, baseline_container, baseline_path, tolerance);
      }
      // RGB images
      else if(cDims[0] == 3)
      {
        return CompareImages<itk::RGBPixel<PixelType>, Dimensions>(input_container, input_path, baseline_container, baseline_path, tolerance);
      }
      // RGBA images
      else if(cDims[0] == 4)
      {
        return CompareImages<itk::RGBAPixel<PixelType>, Dimensions>(input_container, input_path, baseline_container, baseline_path, tolerance);
      }
      else
      {
        std::cerr << "Number of tuples not supported." << std::endl;
        return EXIT_FAILURE;
      }
    }
    std::cerr << "Test should never reach this point." << std::endl;
    return EXIT_FAILURE;
  }

  template <typename PixelType>
  int CompareImages(size_t dimension, std::vector<size_t> cDims, DataContainer::Pointer input_container, const DataArrayPath& input_path, DataContainer::Pointer baseline_container,
                    const DataArrayPath& baseline_path, double tolerance)
  {
    if(dimension == 1)
    {
      /* 2D image */
      return CompareImages<PixelType, 2>(cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else
    {
      /* 3D */
      return CompareImages<PixelType, 3>(cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
  }

  int GetDataArray(DataContainerArray::Pointer& containerArray, const DataArrayPath& path, DataContainer::Pointer& container, std::vector<size_t>& tDims, QString& type, std::vector<size_t>& cDims)
  {
    container = containerArray->getDataContainer(path.getDataContainerName());
    AttributeMatrix::Pointer matrix = containerArray->getAttributeMatrix(path);
    IDataArray::Pointer ptr = matrix->getAttributeArray(path.getDataArrayName());
    DREAM3D_REQUIRE_VALID_POINTER(ptr.get());
    // Container geometry should be of type Geometry
    ImageGeom::Pointer imageGeometry = ImageGeom::New();
    imageGeometry = container->getGeometryAs<ImageGeom>();
    DREAM3D_REQUIRE_VALID_POINTER(imageGeometry.get());
    SizeVec3Type dims = imageGeometry->getDimensions();
    tDims[0] = dims[0];
    tDims[1] = dims[1];
    tDims[2] = dims[2];

    type = ptr->getTypeAsString();
    cDims = ptr->getComponentDimensions();
    return 0;
  }

  int CompareImages(DataContainerArray::Pointer& containerArray, const DataArrayPath& input_path, const DataArrayPath& baseline_path, double tolerance)
  {
    DataContainer::Pointer input_container;
    const int dimSize = 3;
    std::vector<size_t> input_tDims(dimSize, 0);
    QString input_type;
    std::vector<size_t> input_cDims;
    int res_i = GetDataArray(containerArray, input_path, input_container, input_tDims, input_type, input_cDims);
    DREAM3D_REQUIRE_EQUAL(res_i, 0);
    DataContainer::Pointer baseline_container;
    std::vector<size_t> baseline_tDims(dimSize, 0);
    QString baseline_type;
    std::vector<size_t> baseline_cDims;
    int res_b = GetDataArray(containerArray, baseline_path, baseline_container, baseline_tDims, baseline_type, baseline_cDims);
    DREAM3D_REQUIRE_EQUAL(res_b, 0);
    DREAM3D_REQUIRE_EQUAL(input_type, baseline_type);
    DREAM3D_REQUIRE_EQUAL(input_cDims.size(), baseline_cDims.size());
    for(int ii = 0; ii < input_cDims.size(); ii++)
    {
      DREAM3D_REQUIRE_EQUAL(input_cDims[ii], baseline_cDims[ii]);
    }
    for(int ii = 0; ii < dimSize; ii++) // both vectors should have size of dimSize
    {
      DREAM3D_REQUIRE_EQUAL(input_tDims[ii], baseline_tDims[ii]);
    }
    if(input_type.compare("float") == 0)
    {
      return CompareImages<float>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("double") == 0)
    {
      return CompareImages<double>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("int8_t") == 0)
    {
      return CompareImages<int8_t>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("uint8_t") == 0)
    {
      return CompareImages<uint8_t>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("int16_t") == 0)
    {
      return CompareImages<int16_t>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("uint16_t") == 0)
    {
      return CompareImages<uint16_t>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("int32_t") == 0)
    {
      return CompareImages<int32_t>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("uint32_t") == 0)
    {
      return CompareImages<uint32_t>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("int64_t") == 0)
    {
      return CompareImages<int64_t>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else if(input_type.compare("uint64_t") == 0)
    {
      return CompareImages<uint64_t>(input_tDims[2], input_cDims, input_container, input_path, baseline_container, baseline_path, tolerance);
    }
    else
    {
      return 1;
    }
  }

  int ReadImage(const QString& filename, DataContainerArray::Pointer& containerArray, const DataArrayPath& path)
  {
    FilterPipeline::Pointer pipeline = FilterPipeline::New();

    QString filtName = "ITKImageReader";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);

    DREAM3D_REQUIRE_VALID_POINTER(filterFactory.get());

    AbstractFilter::Pointer reader = filterFactory->create();
    DREAM3D_REQUIRE_VALID_POINTER(reader.get());

    bool propertySet = false;
    reader->setDataContainerArray(containerArray);
    QVariant var;
    var.setValue(DataArrayPath(path.getDataContainerName(), "", ""));
    propertySet = reader->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    propertySet = reader->setProperty("CellAttributeMatrixName", path.getAttributeMatrixName());
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    propertySet = reader->setProperty("ImageDataArrayName", path.getDataArrayName());
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    propertySet = reader->setProperty("FileName", filename);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(reader->getWarningCode(), >=, 0);

    containerArray = reader->getDataContainerArray();

    return 0;
  }

  int WriteImage(const QString& filename, DataContainerArray::Pointer& containerArray, const DataArrayPath& path)
  {
    QString fileWithPath = UnitTest::TestTempDir + QString("/") + QString(filename);

    FilterPipeline::Pointer pipeline = FilterPipeline::New();

    QString filtName = "ITKImageWriter";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);

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
    DREAM3D_REQUIRED(writer->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(writer->getWarningCode(), >=, 0);

    // Remove file at the end of the tests_settings
    FilesToRemove << fileWithPath;
    return 0;
  }

  template <typename ImageType>
  int GetMD5FromITKImage(const typename ImageType::Pointer& image, QString& md5)
  {
    try
    {
      typedef itk::Testing::HashImageFilter<ImageType> MD5FilterType;
      typename MD5FilterType::Pointer md5Filter = MD5FilterType::New();
      md5Filter->SetInput(image);
      md5Filter->Update();
      md5 = md5Filter->GetHash().c_str();
    } catch(itk::ExceptionObject& e)
    {
      std::cerr << "Problems computing MD5" << std::endl;
      std::cerr << e.GetDescription() << std::endl;
      return 1;
    }
    return 0;
  }

  template <typename TPixel, unsigned int Dimensions>
  int GetMD5FromDataContainer(DataContainer::Pointer container, const DataArrayPath& path, QString& md5)
  {
    try
    {
      typedef itk::Image<TPixel, Dimensions> ImageType;
      typedef itk::InPlaceDream3DDataToImageFilter<TPixel, Dimensions> ToITKType;
      typename ToITKType::Pointer toITK = ToITKType::New();
      toITK->SetInput(container);
      toITK->SetAttributeMatrixArrayName(path.getAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(path.getDataArrayName().toStdString());
      toITK->SetInPlace(true);
      toITK->Update();
      return GetMD5FromITKImage<ImageType>(toITK->GetOutput(), md5);
    } catch(itk::ExceptionObject& e)
    {
      std::cerr << "Problems converting data container to ITK image" << std::endl;
      std::cerr << e.GetDescription() << std::endl;
      return 1;
    }
  }

  template <typename PixelType, unsigned int Dimensions>
  int GetMD5FromDataContainer(std::vector<size_t> cDims, DataContainer::Pointer container, const DataArrayPath& path, QString& md5)
  {
    // Vector images
    if(cDims.size() > 1)
    {
      if(cDims.size() == 2)
      {
        return GetMD5FromDataContainer<itk::Vector<PixelType, 2>, Dimensions>(container, path, md5);
      }
      else if(cDims.size() == 3)
      {
        return GetMD5FromDataContainer<itk::Vector<PixelType, 3>, Dimensions>(container, path, md5);
      }
      else
      {
        std::cerr << "Vector size not supported." << std::endl;
        return EXIT_FAILURE;
      }
    }
    else
    {
      // Scalar images
      if(cDims[0] == 1)
      {
        return GetMD5FromDataContainer<PixelType, Dimensions>(container, path, md5);
      }
      // RGB images
      else if(cDims[0] == 3)
      {
        return GetMD5FromDataContainer<itk::RGBPixel<PixelType>, Dimensions>(container, path, md5);
      }
      // RGBA images
      else if(cDims[0] == 4)
      {
        return GetMD5FromDataContainer<itk::RGBAPixel<PixelType>, Dimensions>(container, path, md5);
      }
      else
      {
        std::cerr << "Number of tuples not supported." << std::endl;
        return EXIT_FAILURE;
      }
    }
    std::cerr << "Test should never reach this point." << std::endl;
    return EXIT_FAILURE;
  }

  template <typename PixelType>
  int GetMD5FromDataContainer(size_t dimension, std::vector<size_t> cDims, DataContainer::Pointer container, const DataArrayPath& path, QString& md5)
  {
    if(dimension == 1)
    {
      /* 2D image */
      return GetMD5FromDataContainer<PixelType, 2>(cDims, container, path, md5);
    }
    else
    {
      /* 3D */
      return GetMD5FromDataContainer<PixelType, 3>(cDims, container, path, md5);
    }
  }

  int GetMD5FromDataContainer(DataContainerArray::Pointer& containerArray, const DataArrayPath& path, QString& md5)
  {
    DataContainer::Pointer container;
    const int dimSize = 3;
    std::vector<size_t> tDims(dimSize, 0);
    QString data_type;
    std::vector<size_t> cDims;
    int res_i = GetDataArray(containerArray, path, container, tDims, data_type, cDims);
    DREAM3D_REQUIRE_EQUAL(res_i, 0);
    DataContainer::Pointer baseline_container;
    if(data_type.compare("float") == 0)
    {
      return GetMD5FromDataContainer<float>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("double") == 0)
    {
      return GetMD5FromDataContainer<double>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("int8_t") == 0)
    {
      return GetMD5FromDataContainer<int8_t>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("uint8_t") == 0)
    {
      return GetMD5FromDataContainer<uint8_t>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("int16_t") == 0)
    {
      return GetMD5FromDataContainer<int16_t>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("uint16_t") == 0)
    {
      return GetMD5FromDataContainer<uint16_t>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("int32_t") == 0)
    {
      return GetMD5FromDataContainer<int32_t>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("uint32_t") == 0)
    {
      return GetMD5FromDataContainer<uint32_t>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("int64_t") == 0)
    {
      return GetMD5FromDataContainer<int64_t>(tDims[2], cDims, container, path, md5);
    }
    else if(data_type.compare("uint64_t") == 0)
    {
      return GetMD5FromDataContainer<uint64_t>(tDims[2], cDims, container, path, md5);
    }
    else
    {
      return 1;
    }
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  virtual void operator()() = 0;

protected:
  QList<QString> FilesToRemove;

private:
  ITKTestBase(const ITKTestBase&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKTestBase&) = delete; // Move assignment Not Implemented
};
