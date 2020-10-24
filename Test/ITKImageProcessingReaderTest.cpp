/* ============================================================================
 * Copyright (c) 2009-2016 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <QtCore/QFile>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"

#include "UnitTestSupport.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"

#include "ITKImageProcessingTestFileLocations.h"

#include <itkImageFileWriter.h>
#include <itkImageIOBase.h>
#include <itkMRCImageIO.h>
#include <itkMetaImageIO.h>
#include <itkNrrdImageIO.h>
#include <itkPNGImageIO.h>
#include <itkTIFFImageIO.h>
#ifdef ITK_IMAGE_PROCESSING_HAVE_SCIFIO
#include <itkSCIFIOImageIO.h>
#endif

class ITKImageProcessingReaderTest
{

public:
  ITKImageProcessingReaderTest() = default;
  ~ITKImageProcessingReaderTest() = default;

  using DefaultPixelType = float;
  using PNGPixelType = unsigned short;

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES
    QFile::remove(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile);
    QFile::remove(UnitTest::ITKImageProcessingReaderTest::MRCIOInputTestFile);
    QFile::remove(UnitTest::ITKImageProcessingReaderTest::METAIOInputTestFile);
    QFile::remove(UnitTest::ITKImageProcessingReaderTest::TIFFIOInputTestFile);
    QFile::remove(UnitTest::ITKImageProcessingReaderTest::PNGIOInputTestFile);
    QFile::remove(UnitTest::ITKImageProcessingReaderTest::SCIFIOInputTestFile);
#endif
  }

  // -----------------------------------------------------------------------------
  //  Helper methods
  // -----------------------------------------------------------------------------
  AbstractFilter::Pointer GetFilterByName(const QString& filterName)
  {
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filterName);
    if(nullptr == filterFactory.get())
    {
      return nullptr;
    }
    return filterFactory->create();
  }

  template <class ImageType>
  typename ImageType::Pointer CreateITKImageForTests(typename ImageType::PointType& origin, typename ImageType::SizeType& size, typename ImageType::SpacingType& spacing,
                                                     typename ImageType::PixelType value)
  {
    typename ImageType::Pointer image = ImageType::New();
    typename ImageType::DirectionType direction;
    direction.SetIdentity();
    image->SetOrigin(origin);
    image->SetDirection(direction);
    image->SetSpacing(spacing);
    image->SetRegions(size);
    image->Allocate();
    image->FillBuffer(value);
    return image;
  }

  template <typename PixelType, unsigned int Dimension>
  typename itk::Image<PixelType, Dimension>::Pointer WriteTestFile(const QString& filePath, itk::ImageIOBase* io)
  {
    using ImageType = typename itk::Image<PixelType, Dimension>;
    typename ImageType::PointType origin;
    typename ImageType::SizeType size;
    typename ImageType::SpacingType spacing;
    for(unsigned int i = 0; i < Dimension; i++)
    {
      origin[i] = -1.3 + float(i);
      size[i] = 42 + i * 3;
      spacing[i] = 10.3 + float(i) * .2;
    }
    PixelType value = static_cast<PixelType>(3);
    typename ImageType::Pointer image = CreateITKImageForTests<ImageType>(origin, size, spacing, value);

    using WriterType = itk::ImageFileWriter<ImageType>;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(filePath.toStdString());
    writer->SetInput(image);
    writer->SetImageIO(io);
    writer->Update();
    return image;
  }

#ifdef ITK_IMAGE_PROCESSING_HAVE_SCIFIO
  itk::Image<DefaultPixelType, 2>::Pointer WriteSCIFIOTestFile()
  {
    itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();

    return WriteTestFile<DefaultPixelType, 2>(UnitTest::ITKImageProcessingReaderTest::SCIFIOInputTestFile, io.GetPointer());
  }
#endif

  itk::Image<PNGPixelType, 2>::Pointer WritePNGIOTestFile()
  {
    itk::PNGImageIO::Pointer io = itk::PNGImageIO::New();

    return WriteTestFile<PNGPixelType, 2>(UnitTest::ITKImageProcessingReaderTest::PNGIOInputTestFile, io.GetPointer());
  }

  itk::Image<DefaultPixelType, 2>::Pointer WriteTIFFIOTestFile()
  {
    itk::TIFFImageIO::Pointer io = itk::TIFFImageIO::New();

    return WriteTestFile<DefaultPixelType, 2>(UnitTest::ITKImageProcessingReaderTest::TIFFIOInputTestFile, io.GetPointer());
  }

  itk::Image<DefaultPixelType, 3>::Pointer WriteMetaIOTestFile()
  {
    itk::MetaImageIO::Pointer io = itk::MetaImageIO::New();

    return WriteTestFile<DefaultPixelType, 3>(UnitTest::ITKImageProcessingReaderTest::METAIOInputTestFile, io.GetPointer());
  }

  template <typename PixelType>
  typename itk::Image<PixelType, 3>::Pointer WriteNRRDIOTestFile()
  {
    itk::NrrdImageIO::Pointer io = itk::NrrdImageIO::New();

    return WriteTestFile<PixelType, 3>(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile, io.GetPointer());
  }

  itk::Image<DefaultPixelType, 3>::Pointer WriteMRCIOTestFile()
  {
    itk::MRCImageIO::Pointer io = itk::MRCImageIO::New();

    return WriteTestFile<DefaultPixelType, 3>(UnitTest::ITKImageProcessingReaderTest::MRCIOInputTestFile, io.GetPointer());
  }

  // -----------------------------------------------------------------------------
  //  Test methods
  // -----------------------------------------------------------------------------
  int TestAvailability(const QString& filterName)
  {
    if(nullptr == GetFilterByName(filterName))
    {
      QString msg;
      msg = "The test requires the use of %1 filter which is found in the ITKImageProcessing Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(msg.arg(filterName).toStdString())
    }
    return 0;
  }

  int TestNoInput()
  {
    AbstractFilter::Pointer reader = GetFilterByName("ITKImageReader");
    if(!reader)
    {
      return EXIT_FAILURE;
    }

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCode(), ==, -5);
    DREAM3D_REQUIRED(reader->getWarningCode(), >=, 0);
    return EXIT_SUCCESS;
  }

  int TestNoDataContainer()
  {
    AbstractFilter::Pointer reader = GetFilterByName("ITKImageReader");
    if(!reader)
    {
      return EXIT_FAILURE;
    }
    bool propertySet = false;
    // No data container (and bogus filename)
    propertySet = reader->setProperty("FileName", UnitTest::ITKImageProcessingReaderTest::NonExistentInputTestFile);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    QVariant var;
    var.setValue(DataArrayPath());
    propertySet = reader->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCode(), ==, -2);
    DREAM3D_REQUIRED(reader->getWarningCode(), >=, 0);
    return EXIT_SUCCESS;
  }

  int TestBadFilename()
  {
    AbstractFilter::Pointer reader = GetFilterByName("ITKImageReader");
    if(!reader)
    {
      return EXIT_FAILURE;
    }
    bool propertySet = false;

    const QString containerName = "TestContainer";
    DataContainer::Pointer inputContainer = DataContainer::New(containerName);
    DataContainerArray::Pointer inputContainerArray = DataContainerArray::New();
    inputContainerArray->addOrReplaceDataContainer(inputContainer);

    reader->setDataContainerArray(inputContainerArray);
    QVariant var;
    var.setValue(DataArrayPath(containerName, "", ""));
    propertySet = reader->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    propertySet = reader->setProperty("FileName", UnitTest::ITKImageProcessingReaderTest::NonExistentInputTestFile);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCode(), ==, -3);
    DREAM3D_REQUIRED(reader->getWarningCode(), >=, 0);
    return EXIT_SUCCESS;
  }

  template <typename PixelType, unsigned int Dimension>
  int TestCompareImage(const QString& file, typename itk::Image<PixelType, Dimension>::Pointer expectedImage)
  {
    FilterPipeline::Pointer pipeline = FilterPipeline::New();

    QString filtName = "ITKImageReader";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);

    DREAM3D_REQUIRE_NE(0, filterFactory.get());

    AbstractFilter::Pointer reader = filterFactory->create();
    DREAM3D_REQUIRE_NE(0, reader.get());

    bool propertySet = false;

    const QString containerName = "TestContainer";
    DataContainerArray::Pointer inputContainerArray = DataContainerArray::New();

    reader->setDataContainerArray(inputContainerArray);
    QVariant var;
    var.setValue(DataArrayPath(containerName, "", ""));
    propertySet = reader->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    propertySet = reader->setProperty("FileName", file);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(reader->getWarningCode(), >=, 0);

    // Compare read data
    DataContainerArray::Pointer containerArray = reader->getDataContainerArray();
    DREAM3D_REQUIRE_NE(containerArray.get(), 0);
    DREAM3D_REQUIRE(containerArray->getDataContainerNames().contains(containerName));

    DataContainer::Pointer container = containerArray->getDataContainer(containerName);
    DREAM3D_REQUIRE_NE(container.get(), 0);

    IGeometry::Pointer geometry = container->getGeometry();
    DREAM3D_REQUIRE_EQUAL(geometry->getGeometryTypeAsString(), "ImageGeometry");
    ImageGeom::Pointer imageGeometry = std::dynamic_pointer_cast<ImageGeom>(geometry);
    DREAM3D_REQUIRE_NE(imageGeometry.get(), 0);

    float tol = 1e-6;
    FloatVec3Type resolution = imageGeometry->getSpacing();
    FloatVec3Type origin = imageGeometry->getOrigin();
    SizeVec3Type dimensions = imageGeometry->getDimensions();
    for(size_t i = 0; i < Dimension; i++)
    {
      float imageSpacing = expectedImage->GetSpacing()[i];
      DREAM3D_COMPARE_FLOATS(&resolution[i], &imageSpacing, tol);

      float imageOrigin = expectedImage->GetOrigin()[i];
      DREAM3D_COMPARE_FLOATS(&origin[i], &imageOrigin, tol);

      size_t imageDimension = expectedImage->GetLargestPossibleRegion().GetSize()[i];
      DREAM3D_REQUIRE_EQUAL(dimensions[i], imageDimension);
    }

    const QString matrixName = SIMPL::Defaults::CellAttributeMatrixName;
    DREAM3D_REQUIRE_EQUAL(container->doesAttributeMatrixExist(matrixName), true);
    AttributeMatrix::Pointer attributeMatrix = container->getAttributeMatrix(matrixName);
    DREAM3D_REQUIRE_NE(attributeMatrix.get(), 0);
    const QString attributeArrayName = SIMPL::CellData::ImageData;
    DREAM3D_REQUIRE_EQUAL(attributeMatrix->doesAttributeArrayExist(attributeArrayName), true);
    IDataArray::Pointer dataArray = attributeMatrix->getAttributeArray(attributeArrayName);

    typedef typename itk::NumericTraits<PixelType>::ValueType ComponentType;
    unsigned int PixelSize = itk::NumericTraits<PixelType>::GetLength();
    for(size_t i = 0; i < expectedImage->GetLargestPossibleRegion().GetNumberOfPixels(); i++)
    {
      ComponentType* valuePtr = static_cast<ComponentType*>(dataArray->getVoidPointer(0));
      ComponentType* expectedValuePtr = reinterpret_cast<ComponentType*>(expectedImage->GetBufferPointer());
      for(unsigned int ii = 0; ii < PixelSize; ii++)
      {
        float value = static_cast<float>(valuePtr[i * PixelSize + ii]);
        float expectedValue = static_cast<float>(expectedValuePtr[i * PixelSize + ii]);
        DREAM3D_COMPARE_FLOATS(&value, &expectedValue, tol);
      }
    }
    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestAvailability("ITKImageReader"));
    DREAM3D_REGISTER_TEST(TestNoInput());
    DREAM3D_REGISTER_TEST(TestNoDataContainer());
    DREAM3D_REGISTER_TEST(TestBadFilename());

    double zeroOrigin[2] = {0.0, 0.0};
#ifdef ITK_IMAGE_PROCESSING_HAVE_SCIFIO
    // SCIFIO
    itk::Image<DefaultPixelType, 2>::Pointer scifioImage = WriteSCIFIOTestFile();
    // SCIFIO doesn't seem to take spacing into consideration
    double scifioSpacing[2] = {1, 1};
    scifioImage->SetSpacing(scifioSpacing);
    // SCIFIO doesn't seem to take origin into consideration
    scifioImage->SetOrigin(zeroOrigin);
    DREAM3D_REGISTER_TEST((TestCompareImage<DefaultPixelType, 2>(UnitTest::ITKImageProcessingReaderTest::SCIFIOInputTestFile, scifioImage)))
#endif

    // PNGIO
    // PNG only supports 'unsigned char' and 'unsigned short' pixel types.
    itk::Image<PNGPixelType, 2>::Pointer pngioImage = WritePNGIOTestFile();
    // PNGIO doesn't seem to take origin into consideration
    pngioImage->SetOrigin(zeroOrigin);
    DREAM3D_REGISTER_TEST((TestCompareImage<PNGPixelType, 2>(UnitTest::ITKImageProcessingReaderTest::PNGIOInputTestFile, pngioImage)))
    // TIFFIO
    itk::Image<DefaultPixelType, 2>::Pointer tiffioImage = WriteTIFFIOTestFile();
    // TIFFIO doesn't seem to take origin into consideration
    tiffioImage->SetOrigin(zeroOrigin);
    DREAM3D_REGISTER_TEST((TestCompareImage<DefaultPixelType, 2>(UnitTest::ITKImageProcessingReaderTest::TIFFIOInputTestFile, tiffioImage)))
    // MetaIO
    itk::Image<DefaultPixelType, 3>::Pointer metaioImage = WriteMetaIOTestFile();
    DREAM3D_REGISTER_TEST((TestCompareImage<DefaultPixelType, 3>(UnitTest::ITKImageProcessingReaderTest::METAIOInputTestFile, metaioImage)))

    // NrrdIO
    itk::Image<DefaultPixelType, 3>::Pointer nrrdioImage = WriteNRRDIOTestFile<DefaultPixelType>();
    DREAM3D_REGISTER_TEST((TestCompareImage<DefaultPixelType, 3>(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile, nrrdioImage)))

    // MRC
    itk::Image<DefaultPixelType, 3>::Pointer mrcioImage = WriteMRCIOTestFile();
    DREAM3D_REGISTER_TEST((TestCompareImage<DefaultPixelType, 3>(UnitTest::ITKImageProcessingReaderTest::MRCIOInputTestFile, mrcioImage)))

    // Vector images
    using Vector2Type = itk::Vector<DefaultPixelType, 2>;
    itk::Image<Vector2Type, 3>::Pointer vector2Image = WriteNRRDIOTestFile<Vector2Type>();
    DREAM3D_REGISTER_TEST((TestCompareImage<Vector2Type, 3>(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile, vector2Image)))
    using Vector3Type = itk::Vector<DefaultPixelType, 3>;
    itk::Image<Vector3Type, 3>::Pointer vector3Image = WriteNRRDIOTestFile<Vector3Type>();
    DREAM3D_REGISTER_TEST((TestCompareImage<Vector3Type, 3>(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile, vector3Image)))
    using Vector36Type = itk::Vector<DefaultPixelType, 36>;
    itk::Image<Vector36Type, 3>::Pointer vector36Image = WriteNRRDIOTestFile<Vector36Type>();
    DREAM3D_REGISTER_TEST((TestCompareImage<Vector36Type, 3>(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile, vector36Image)))
    // RGB images
    using RGBPixelType = itk::RGBPixel<DefaultPixelType>;
    itk::Image<RGBPixelType, 3>::Pointer rgbImage = WriteNRRDIOTestFile<RGBPixelType>();
    DREAM3D_REGISTER_TEST((TestCompareImage<RGBPixelType, 3>(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile, rgbImage)))
    // RGBA images
    using RGBAPixelType = itk::Vector<DefaultPixelType, 3>;
    itk::Image<RGBAPixelType, 3>::Pointer rgbaImage = WriteNRRDIOTestFile<RGBAPixelType>();
    DREAM3D_REGISTER_TEST((TestCompareImage<RGBAPixelType, 3>(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile, rgbaImage)))
    DREAM3D_REGISTER_TEST(RemoveTestFiles())
  }

public:
  ITKImageProcessingReaderTest(const ITKImageProcessingReaderTest&) = delete;            // Copy Constructor Not Implemented
  ITKImageProcessingReaderTest(ITKImageProcessingReaderTest&&) = delete;                 // Move Constructor Not Implemented
  ITKImageProcessingReaderTest& operator=(const ITKImageProcessingReaderTest&) = delete; // Copy Assignment Not Implemented
  ITKImageProcessingReaderTest& operator=(ITKImageProcessingReaderTest&&) = delete;      // Move Assignment Not Implemented
};
