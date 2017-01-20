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
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/Utilities/UnitTestSupport.hpp"
#include "SIMPLib/Utilities/QMetaObjectUtilities.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImportImageStack.h"

#include "ITKImageProcessingTestFileLocations.h"

#include <itkImageFileWriter.h>
#include <itkImageIOBase.h>

class ITKImageProcessingImportImageStackTest
{

  public:
    ITKImageProcessingImportImageStackTest() {}
    virtual ~ITKImageProcessingImportImageStackTest() {}

    static const unsigned int Dimension = 3;
    typedef short                              PixelType;
    typedef itk::Image< PixelType, Dimension > ImageType;

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

  void WriteTestFile(const QString& filePath, ImageType * image)
  {
    typedef itk::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(filePath.toStdString());
    writer->SetInput(image);
    writer->Update();
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
    AbstractFilter::Pointer reader = GetFilterByName("ITKImportImageStack");
    if (!reader)
    {
      return EXIT_FAILURE;
    }

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), == , -13);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);
    return EXIT_SUCCESS;
  }

  int TestNoDataContainer()
  {
    AbstractFilter::Pointer abstractFilter = GetFilterByName("ITKImportImageStack");
    if (!abstractFilter)
    {
      return EXIT_FAILURE;
    }

    ITKImportImageStack::Pointer reader = std::static_pointer_cast< ITKImportImageStack >( abstractFilter );

    FileListInfo_t fileListInfo;
    fileListInfo.InputPath = UnitTest::ITKImageProcessingImportImageStackTest::StackInputTestDir;

    reader->setInputFileListInfo(fileListInfo);

    bool propertySet = false;
    propertySet = reader->setProperty("DataContainerName", "");
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), == , -887);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);
    return EXIT_SUCCESS;
  }

  int TestNoFiles()
  {
    AbstractFilter::Pointer abstractFilter = GetFilterByName("ITKImportImageStack");
    if (!abstractFilter)
    {
      return EXIT_FAILURE;
    }

    ITKImportImageStack::Pointer reader = std::static_pointer_cast< ITKImportImageStack >( abstractFilter );

    bool propertySet = false;
    const QString containerName = "TestNoFiles";
    propertySet = reader->setProperty("DataContainerName", containerName);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    FileListInfo_t fileListInfo;
    fileListInfo.InputPath = "doesNotExist.ghost";
    fileListInfo.StartIndex = 75;
    fileListInfo.EndIndex = 77;
    fileListInfo.FileExtension = "dcm";
    fileListInfo.FilePrefix = "Image";
    fileListInfo.FileSuffix = "";
    fileListInfo.PaddingDigits = 4;

    reader->setInputFileListInfo(fileListInfo);

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), == , -11);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);
    return EXIT_SUCCESS;
  }

  int TestFileDoesNotExist()
  {
    AbstractFilter::Pointer abstractFilter = GetFilterByName("ITKImportImageStack");
    if (!abstractFilter)
    {
      return EXIT_FAILURE;
    }

    ITKImportImageStack::Pointer reader = std::static_pointer_cast< ITKImportImageStack >( abstractFilter );

    bool propertySet = false;
    const QString containerName = "TestFileDoesNotExist";
    propertySet = reader->setProperty("DataContainerName", containerName);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    FileListInfo_t fileListInfo;
    fileListInfo.InputPath = UnitTest::ITKImageProcessingImportImageStackTest::StackInputTestDir;
    fileListInfo.StartIndex = 75;
    fileListInfo.EndIndex = 79;
    fileListInfo.FileExtension = "dcm";
    fileListInfo.FilePrefix = "Image";
    fileListInfo.FileSuffix = "";
    fileListInfo.PaddingDigits = 4;

    reader->setInputFileListInfo(fileListInfo);

    reader->execute();
    DREAM3D_REQUIRED(reader->getErrorCondition(), == , -7);
    DREAM3D_REQUIRED(reader->getWarningCondition(), >= , 0);
    return EXIT_SUCCESS;
  }

  int TestCompareImage()
  {
    FilterPipeline::Pointer pipeline = FilterPipeline::New();

    QString filtName = "ITKImportImageStack";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);

    DREAM3D_REQUIRE_NE(0, filterFactory.get());

    AbstractFilter::Pointer abstractFilter = GetFilterByName("ITKImportImageStack");
    if (!abstractFilter)
    {
      return EXIT_FAILURE;
    }

    ITKImportImageStack::Pointer reader = std::static_pointer_cast< ITKImportImageStack >( abstractFilter );

    bool propertySet = false;

    const QString containerName = "TestCompareImage";
    propertySet = reader->setProperty("DataContainerName", containerName);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    FileListInfo_t fileListInfo;
    fileListInfo.InputPath = UnitTest::ITKImageProcessingImportImageStackTest::StackInputTestDir;
    fileListInfo.StartIndex = 75;
    fileListInfo.EndIndex = 77;
    fileListInfo.FileExtension = "dcm";
    fileListInfo.FilePrefix = "Image";
    fileListInfo.FileSuffix = "";
    fileListInfo.PaddingDigits = 4;

    reader->setInputFileListInfo(fileListInfo);

    static const unsigned int Dimension = 3;
    FloatVec3_t inputOrigin;
    inputOrigin.x = 1.f;
    inputOrigin.y = 4.f;
    inputOrigin.z = 8.f;
    reader->setOrigin(inputOrigin);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    FloatVec3_t inputResolution;
    inputResolution.x = 0.3f;
    inputResolution.y = 0.2f;
    inputResolution.z = 0.9f;
    reader->setResolution(inputResolution);
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
    float resolution[Dimension];
    imageGeometry->getResolution(resolution[0], resolution[1], resolution[2]);
    float origin[Dimension];
    imageGeometry->getOrigin(origin[0], origin[1], origin[2]);

    DREAM3D_COMPARE_FLOATS(&origin[0], &inputOrigin.x, tol);
    DREAM3D_COMPARE_FLOATS(&origin[1], &inputOrigin.y, tol);
    DREAM3D_COMPARE_FLOATS(&origin[2], &inputOrigin.z, tol);

    DREAM3D_COMPARE_FLOATS(&resolution[0], &inputResolution.x, tol);
    DREAM3D_COMPARE_FLOATS(&resolution[1], &inputResolution.y, tol);
    DREAM3D_COMPARE_FLOATS(&resolution[2], &inputResolution.z, tol);

    size_t dimensions[Dimension];
    size_t expectedDimensions[Dimension];
    expectedDimensions[0] = 256;
    expectedDimensions[1] = 256;
    expectedDimensions[2] = 3;
    imageGeometry->getDimensions(dimensions[0], dimensions[1], dimensions[2]);
    for( size_t i = 0; i < Dimension; ++i )
    {
      DREAM3D_REQUIRE_EQUAL(dimensions[i], expectedDimensions[i]);
    }
    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestAvailability("ITKImportImageStack"));
    DREAM3D_REGISTER_TEST(TestNoInput());
    DREAM3D_REGISTER_TEST(TestNoDataContainer());
    DREAM3D_REGISTER_TEST(TestNoFiles());
    DREAM3D_REGISTER_TEST(TestFileDoesNotExist());
    DREAM3D_REGISTER_TEST(TestCompareImage());
  }

  private:
    ITKImageProcessingImportImageStackTest(const ITKImageProcessingImportImageStackTest&); // Copy Constructor Not Implemented
    void operator=(const ITKImageProcessingImportImageStackTest&); // Operator '=' Not Implemented
};
