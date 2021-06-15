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

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"

#include "UnitTestSupport.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImportImageStack.h"

#include "ITKImageProcessingTestFileLocations.h"

#include <itkImageFileWriter.h>
#include <itkImageIOBase.h>

class ITKImportImageStackTest
{

public:
  ITKImportImageStackTest() = default;
  virtual ~ITKImportImageStackTest() = default;

  static const unsigned int Dimension = 3;
  using PixelType = short;
  using ImageType = itk::Image<PixelType, Dimension>;

  void WriteTestFile(const QString& filePath, ImageType* image)
  {
    using WriterType = itk::ImageFileWriter<ImageType>;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(filePath.toStdString());
    writer->SetInput(image);
    writer->Update();
  }

  // -----------------------------------------------------------------------------
  int TestNoInput()
  {
    ITKImportImageStack::Pointer filter = ITKImportImageStack::New();
    if(!filter)
    {
      return EXIT_FAILURE;
    }

    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), ==, -64501);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  int TestNoDataContainer()
  {
    ITKImportImageStack::Pointer filter = ITKImportImageStack::New();
    if(!filter)
    {
      return EXIT_FAILURE;
    }

    StackFileListInfo fileListInfo;
    fileListInfo.InputPath = UnitTest::ITKImportImageStackTest::StackInputTestDir;

    filter->setInputFileListInfo(fileListInfo);

    bool propertySet = false;
    QVariant var;
    var.setValue(DataArrayPath());
    propertySet = filter->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);
    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), ==, -887);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  int TestNoFiles()
  {
    ITKImportImageStack::Pointer filter = ITKImportImageStack::New();
    if(!filter)
    {
      return EXIT_FAILURE;
    }

    bool propertySet = false;
    const QString containerName = "TestNoFiles";
    QVariant var;
    var.setValue(DataArrayPath(containerName, "", ""));
    propertySet = filter->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    StackFileListInfo fileListInfo;
    fileListInfo.InputPath = "doesNotExist.ghost";
    fileListInfo.StartIndex = 75;
    fileListInfo.EndIndex = 77;
    fileListInfo.FileExtension = "dcm";
    fileListInfo.FilePrefix = "Image";
    fileListInfo.FileSuffix = "";
    fileListInfo.PaddingDigits = 4;

    filter->setInputFileListInfo(fileListInfo);

    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), ==, -64501);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    return EXIT_SUCCESS;
  }

  int TestFileDoesNotExist()
  {
    ITKImportImageStack::Pointer filter = ITKImportImageStack::New();
    if(!filter)
    {
      return EXIT_FAILURE;
    }

    bool propertySet = false;
    const QString containerName = "TestFileDoesNotExist";
    QVariant var;
    var.setValue(DataArrayPath(containerName, "", ""));
    propertySet = filter->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    StackFileListInfo fileListInfo;
    fileListInfo.InputPath = UnitTest::ITKImportImageStackTest::StackInputTestDir;
    fileListInfo.StartIndex = 75;
    fileListInfo.EndIndex = 79;
    fileListInfo.FileExtension = "dcm";
    fileListInfo.FilePrefix = "Image";
    fileListInfo.FileSuffix = "";
    fileListInfo.PaddingDigits = 4;

    filter->setInputFileListInfo(fileListInfo);

    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), ==, -64502);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    return EXIT_SUCCESS;
  }

  int TestCompareImage()
  {
    FilterPipeline::Pointer pipeline = FilterPipeline::New();

    QString filtName = "ITKImportImageStack";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);

    DREAM3D_REQUIRE_NE(0, filterFactory.get());

    ITKImportImageStack::Pointer filter = ITKImportImageStack::New();
    if(!filter)
    {
      return EXIT_FAILURE;
    }

    bool propertySet = false;

    const QString containerName = "TestCompareImage";
    QVariant var;
    var.setValue(DataArrayPath(containerName, "", ""));
    propertySet = filter->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    StackFileListInfo fileListInfo;
    fileListInfo.InputPath = UnitTest::ITKImportImageStackTest::StackInputTestDir;
    fileListInfo.StartIndex = 11;
    fileListInfo.EndIndex = 13;
    fileListInfo.IncrementIndex = 1;
    fileListInfo.FileExtension = "tif";
    fileListInfo.FilePrefix = "slice_";
    fileListInfo.FileSuffix = "";
    fileListInfo.PaddingDigits = 2;
    fileListInfo.Ordering = 0;
    filter->setInputFileListInfo(fileListInfo);

    static const unsigned int Dimension = 3;
    FloatVec3Type inputOrigin;
    inputOrigin[0] = 1.f;
    inputOrigin[1] = 4.f;
    inputOrigin[2] = 8.f;
    filter->setOrigin(inputOrigin);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    FloatVec3Type inputspacing;
    inputspacing[0] = 0.3f;
    inputspacing[1] = 0.2f;
    inputspacing[2] = 0.9f;
    filter->setSpacing(inputspacing);
    DREAM3D_REQUIRE_EQUAL(propertySet, true);

    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);

    // Compare read data
    DataContainerArray::Pointer containerArray = filter->getDataContainerArray();
    DREAM3D_REQUIRE_NE(containerArray.get(), 0);
    DREAM3D_REQUIRE(containerArray->getDataContainerNames().contains(containerName));

    DataContainer::Pointer container = containerArray->getDataContainer(containerName);
    DREAM3D_REQUIRE_NE(container.get(), 0);

    IGeometry::Pointer geometry = container->getGeometry();
    DREAM3D_REQUIRE_EQUAL(geometry->getGeometryTypeAsString(), "ImageGeometry");
    ImageGeom::Pointer imageGeometry = std::dynamic_pointer_cast<ImageGeom>(geometry);
    DREAM3D_REQUIRE_NE(imageGeometry.get(), 0);

    FloatVec3Type spacing = imageGeometry->getSpacing();
    FloatVec3Type origin = imageGeometry->getOrigin();

    DREAM3D_COMPARE_FLOATS(&origin[0], &inputOrigin[0], 5);
    DREAM3D_COMPARE_FLOATS(&origin[1], &inputOrigin[1], 5);
    DREAM3D_COMPARE_FLOATS(&origin[2], &inputOrigin[2], 5);

    DREAM3D_COMPARE_FLOATS(&spacing[0], &inputspacing[0], 5);
    DREAM3D_COMPARE_FLOATS(&spacing[1], &inputspacing[1], 5);
    DREAM3D_COMPARE_FLOATS(&spacing[2], &inputspacing[2], 5);

    SizeVec3Type dimensions = imageGeometry->getDimensions();
    SizeVec3Type expectedDimensions;
    expectedDimensions[0] = 524;
    expectedDimensions[1] = 390;
    expectedDimensions[2] = 3;
    for(size_t i = 0; i < Dimension; ++i)
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
    std::cout << "--------------------- ITKImportImageStackTest ---------------------" << std::endl;
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestNoInput());
    DREAM3D_REGISTER_TEST(TestNoDataContainer());
    DREAM3D_REGISTER_TEST(TestNoFiles());
    DREAM3D_REGISTER_TEST(TestFileDoesNotExist());
    DREAM3D_REGISTER_TEST(TestCompareImage());
  }

public:
  ITKImportImageStackTest(const ITKImportImageStackTest&) = delete;            // Copy Constructor Not Implemented
  ITKImportImageStackTest(ITKImportImageStackTest&&) = delete;                 // Move Constructor Not Implemented
  ITKImportImageStackTest& operator=(const ITKImportImageStackTest&) = delete; // Copy Assignment Not Implemented
  ITKImportImageStackTest& operator=(ITKImportImageStackTest&&) = delete;      // Move Assignment Not Implemented
};
