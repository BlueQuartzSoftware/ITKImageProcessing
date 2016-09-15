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

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"

#include "ITKImageProcessingTestFileLocations.h"

#include <itkImageFileWriter.h>
#include <itkImageIOBase.h>

#include <itkMetaImageIO.h>
#include <itkNrrdImageIO.h>
#include <itkSCIFIOImageIO.h>
#include <itkTIFFImageIO.h>
#include <itkPNGImageIO.h>

class ITKImageProcessingReaderTest
{

  public:
    ITKImageProcessingReaderTest() {}
    virtual ~ITKImageProcessingReaderTest() {}

    typedef float DefaultPixelType;
    typedef unsigned short PNGPixelType;

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES
    QFile::remove(UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile);
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
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filterName);
    if (nullptr == filterFactory.get())
    {
      return nullptr;
    }
    return filterFactory->create();
  }

  template<class ImageType>
  typename ImageType::Pointer
    CreateITKImageForTests(typename ImageType::PointType &origin,
    typename ImageType::SizeType &size,
    typename ImageType::SpacingType &spacing,
    typename ImageType::PixelType value
    )
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

  template<typename PixelType, unsigned int Dimension>
  typename itk::Dream3DImage<PixelType, Dimension>::Pointer WriteTestFile(const QString& filePath, itk::ImageIOBase* io)
  {
    typedef typename itk::Dream3DImage<PixelType, Dimension> ImageType;
    typename ImageType::PointType origin;
    typename ImageType::SizeType size;
    typename ImageType::SpacingType spacing;
    for (unsigned int i = 0; i < Dimension; i++)
    {
      origin[i] = -1.3 + float(i);
      size[i] = 42 + i * 3;
      spacing[i] = 10.3 + float(i)*.2;
    }
    typename  ImageType::Pointer image = CreateITKImageForTests<ImageType>(origin, size, spacing, 3);

    typedef itk::ImageFileWriter<ImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(filePath.toStdString());
    writer->SetInput(image);
    writer->SetImageIO(io);
    writer->Update();
    return image;
  }

  itk::Dream3DImage<DefaultPixelType, 2>::Pointer
  WriteSCIFIOTestFile()
  {
    itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();

    return WriteTestFile<DefaultPixelType,2>(
      UnitTest::ITKImageProcessingReaderTest::SCIFIOInputTestFile,
      io.GetPointer()
      );
  }

  itk::Dream3DImage<PNGPixelType, 2>::Pointer
    WritePNGIOTestFile()
  {
    itk::PNGImageIO::Pointer io = itk::PNGImageIO::New();

    return WriteTestFile<PNGPixelType,2>(
      UnitTest::ITKImageProcessingReaderTest::PNGIOInputTestFile,
      io.GetPointer()
      );
  }

  itk::Dream3DImage<DefaultPixelType, 2>::Pointer
    WriteTIFFIOTestFile()
  {
    itk::TIFFImageIO::Pointer io = itk::TIFFImageIO::New();

    return WriteTestFile<DefaultPixelType,2>(
      UnitTest::ITKImageProcessingReaderTest::TIFFIOInputTestFile,
      io.GetPointer()
      );
  }

  itk::Dream3DImage<DefaultPixelType, 3>::Pointer
  WriteMetaIOTestFile()
  {
    itk::MetaImageIO::Pointer io = itk::MetaImageIO::New();

    return WriteTestFile<DefaultPixelType,3>(
      UnitTest::ITKImageProcessingReaderTest::METAIOInputTestFile,
      io.GetPointer()
      );
  }

  itk::Dream3DImage<DefaultPixelType, 3>::Pointer
  WriteNRRDIOTestFile()
  {
    itk::NrrdImageIO::Pointer io = itk::NrrdImageIO::New();

    return WriteTestFile<DefaultPixelType,3>(
      UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile,
      io.GetPointer()
      );
  }


  // -----------------------------------------------------------------------------
  //  Test methods
  // -----------------------------------------------------------------------------
  int TestAvailability(const QString& filterName)
  {
    if (nullptr == GetFilterByName(filterName))
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
    if (!reader)
    {
      return EXIT_FAILURE;
    }

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), == , -1);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);
    return EXIT_SUCCESS;
  }

  int TestNoDataContainer()
  {
    AbstractFilter::Pointer reader = GetFilterByName("ITKImageReader");
    if (!reader)
    {
      return EXIT_FAILURE;
    }
    bool propertySet = false;
    // No data container (and bogus filename)
    propertySet =
      reader->setProperty("FileName", UnitTest::ITKImageProcessingReaderTest::NonExistentInputTestFile);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    propertySet = reader->setProperty("DataContainerName", "");
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), == , -2);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);
    return EXIT_SUCCESS;
  }

  int TestBadFilename()
  {
    AbstractFilter::Pointer reader = GetFilterByName("ITKImageReader");
    if (!reader)
    {
      return EXIT_FAILURE;
    }
    bool propertySet = false;

    const QString containerName = "TestContainer";
    DataContainer::Pointer inputContainer = DataContainer::New(containerName);
    DataContainerArray::Pointer inputContainerArray = DataContainerArray::New();
    inputContainerArray->addDataContainer(inputContainer);

    reader->setDataContainerArray(inputContainerArray);
    propertySet = reader->setProperty("DataContainerName", containerName);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    propertySet =
      reader->setProperty("FileName", UnitTest::ITKImageProcessingReaderTest::NonExistentInputTestFile);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), == , -3);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);
    return EXIT_SUCCESS;
  }

  template<typename PixelType, unsigned int Dimension>
  int TestCompareImage(const QString& file, typename itk::Dream3DImage<PixelType, Dimension>::Pointer expectedImage)
  {
    FilterPipeline::Pointer pipeline = FilterPipeline::New();

    QString filtName = "ITKImageReader";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);

    DREAM3D_REQUIRE_NE(0, filterFactory.get());

    AbstractFilter::Pointer reader = filterFactory->create();
    DREAM3D_REQUIRE_NE(0, reader.get());

    bool propertySet = false;

    const QString containerName = "TestContainer";
    DataContainerArray::Pointer inputContainerArray = DataContainerArray::New();

    reader->setDataContainerArray(inputContainerArray);
    propertySet = reader->setProperty("DataContainerName", containerName);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    propertySet = reader->setProperty("FileName", file);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);

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
    float resolution[3];
    imageGeometry->getResolution(resolution[0], resolution[1], resolution[2]);
    float origin[3];
    imageGeometry->getOrigin(origin[0], origin[1], origin[2]);
    size_t dimensions[3];
    imageGeometry->getDimensions(dimensions[0], dimensions[1], dimensions[2]);
    for (int i = 0; i < Dimension; i++)
    {
      float imageSpacing = expectedImage->GetSpacing()[i];
      DREAM3D_COMPARE_FLOATS(&resolution[i], &imageSpacing, tol);
     
      float imageOrigin = expectedImage->GetOrigin()[i];
      DREAM3D_COMPARE_FLOATS(&origin[i], &imageOrigin, tol);

      size_t imageDimension = expectedImage->GetLargestPossibleRegion().GetSize()[i];
      DREAM3D_REQUIRE_EQUAL(dimensions[i], imageDimension);
    }

    const QString matrixName = SIMPL::Defaults::CellAttributeMatrixName;
    DREAM3D_REQUIRE_EQUAL(container->doesAttributeMatrixExist(matrixName),true);
    AttributeMatrix::Pointer attributeMatrix = container->getAttributeMatrix(matrixName);
    DREAM3D_REQUIRE_NE(attributeMatrix.get(), 0);
    const QString attributeArrayName = SIMPL::CellData::ImageData;
    DREAM3D_REQUIRE_EQUAL(attributeMatrix->doesAttributeArrayExist(attributeArrayName),true);
    IDataArray::Pointer dataArray = attributeMatrix->getAttributeArray(attributeArrayName);

    for (size_t i = 0; i < dataArray->getSize(); i++)
    {
      float value = static_cast<PixelType*>(dataArray->getVoidPointer(0))[i];
      float expectedValue = expectedImage->GetBufferPointer()[i];
      DREAM3D_COMPARE_FLOATS(&value, &expectedValue, tol);
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

    // SCIFIO
    itk::Dream3DImage<DefaultPixelType, 2>::Pointer scifioImage = WriteSCIFIOTestFile();
    // SCIFIO doesn't seem to take spacing into consideration
    double scifioSpacing[2] = { 1, 1 };
    scifioImage->SetSpacing(scifioSpacing);
    // SCIFIO doesn't seem to take origin into consideration
    double zeroOrigin[2] = { 0.0, 0.0 };
    scifioImage->SetOrigin(zeroOrigin);
    DREAM3D_REGISTER_TEST(
      (TestCompareImage<DefaultPixelType,2>(
        UnitTest::ITKImageProcessingReaderTest::SCIFIOInputTestFile, scifioImage))
      )
    // PNGIO
    // PNG only supports 'unsigned char' and 'unsigned short' pixel types.
    itk::Dream3DImage<PNGPixelType, 2>::Pointer pngioImage = WritePNGIOTestFile();
    // PNGIO doesn't seem to take origin into consideration
    pngioImage->SetOrigin(zeroOrigin);
    DREAM3D_REGISTER_TEST(
      (TestCompareImage<PNGPixelType,2>(
      UnitTest::ITKImageProcessingReaderTest::PNGIOInputTestFile, pngioImage))
      )
    // TIFFIO
    itk::Dream3DImage<DefaultPixelType, 2>::Pointer tiffioImage = WriteTIFFIOTestFile();
    // TIFFIO doesn't seem to take origin into consideration
    tiffioImage->SetOrigin(zeroOrigin);
    DREAM3D_REGISTER_TEST(
      (TestCompareImage<DefaultPixelType,2>(
      UnitTest::ITKImageProcessingReaderTest::TIFFIOInputTestFile, tiffioImage))
      )
    // MetaIO
    itk::Dream3DImage<DefaultPixelType, 3>::Pointer metaioImage = WriteMetaIOTestFile();
    DREAM3D_REGISTER_TEST(
      (TestCompareImage<DefaultPixelType,3>(
      UnitTest::ITKImageProcessingReaderTest::METAIOInputTestFile, metaioImage))
      )

    // NrrdIO
    itk::Dream3DImage<DefaultPixelType, 3>::Pointer nrrdioImage = WriteNRRDIOTestFile();
    DREAM3D_REGISTER_TEST(
      (TestCompareImage<DefaultPixelType,3>(
      UnitTest::ITKImageProcessingReaderTest::NRRDIOInputTestFile, nrrdioImage))
      )


    DREAM3D_REGISTER_TEST( RemoveTestFiles() )
  }

  private:
    ITKImageProcessingReaderTest(const ITKImageProcessingReaderTest&); // Copy Constructor Not Implemented
    void operator=(const ITKImageProcessingReaderTest&); // Operator '=' Not Implemented
};
