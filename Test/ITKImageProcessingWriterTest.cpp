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

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageWriter.h"

#include "ITKImageProcessingTestFileLocations.h"

#include <itkMetaImageIO.h>
#include <itkNrrdImageIO.h>
#include <itkSCIFIOImageIO.h>

class ITKImageProcessingWriterTest
{

  public:
    ITKImageProcessingWriterTest() {}
    virtual ~ITKImageProcessingWriterTest() {}

    QList<QString> FilesToRemove;

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
  //  Helper methods
  // -----------------------------------------------------------------------------
  AbstractFilter::Pointer GetFilterByName(const QString& filterName)
  {
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filterName);
    if (NULL == filterFactory.get())
    {
      return NULL;
    }
    return filterFactory->create();
  }

  QString FilenameWithDifferentExtension(const QString& filename, const QString& extension = ".raw")
  {
    QString nameWithoutExtension = filename.left(filename.lastIndexOf("."));
    return nameWithoutExtension + extension;
  }

  template<class PixelType, unsigned int Dimension>
  DataContainerArray::Pointer CreateTestData(const DataArrayPath& path)
  {
    // Create test data (baseline)
    DataContainer::Pointer container =
      DataContainer::New(path.getDataContainerName());
    ImageGeom::Pointer imageGeometry =
      ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
    QVector<float> origin(3, 0);
    QVector<float> spacing(3, 1);
    QVector<size_t> dimensions(3, 1);
    for (size_t i = 0; i < Dimension; i++)
    {
      origin[i] = 1.23 + float(i);
      spacing[i] = .45 + float(i)*.2;
      dimensions[i] = 90 + i * 3;
    }
    imageGeometry->setResolution(spacing[0], spacing[1], spacing[2]);
    imageGeometry->setOrigin(origin[0], origin[1], origin[2]);
    imageGeometry->setDimensions(dimensions[0], dimensions[1], dimensions[2]);
    container->setGeometry(imageGeometry);
    QVector<size_t> arrayDimensions(1, 0);
    arrayDimensions[0] = 1;
    AttributeMatrix::Pointer matrixArray =
      container->createAndAddAttributeMatrix(
        dimensions, path.getAttributeMatrixName(),
        SIMPL::AttributeMatrixType::Cell);
    typename DataArray<PixelType>::Pointer data =
      DataArray<PixelType>::CreateArray(
        dimensions, arrayDimensions, path.getDataArrayName(), true);
    data->initializeWithValue(11.0);
    matrixArray->addAttributeArray(path.getDataArrayName(), data);

    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    containerArray->addDataContainer(container);
    return containerArray;
  }

  ImageGeom::Pointer GetImageGeometry(DataContainer::Pointer &container)
  {
      IGeometry::Pointer geometry = container->getGeometry();
      DREAM3D_REQUIRE_EQUAL(geometry->getGeometryTypeAsString(), "ImageGeometry");
      ImageGeom::Pointer imageGeometry = std::dynamic_pointer_cast<ImageGeom>(geometry);
      DREAM3D_REQUIRE_NE(imageGeometry.get(), 0);
      return imageGeometry;
  }

  bool CompareImageGeometries(const ImageGeom::Pointer &inputImageGeometry, const ImageGeom::Pointer &baselineImageGeometry)
  {
      float tol = 1e-6;
      float inputResolution[3];
      float baselineResolution[3];
      inputImageGeometry->getResolution(inputResolution[0], inputResolution[1], inputResolution[2]);
      baselineImageGeometry->getResolution(baselineResolution[0], baselineResolution[1], baselineResolution[2]);
      float inputOrigin[3];
      float baselineOrigin[3];
      inputImageGeometry->getOrigin(inputOrigin[0], inputOrigin[1], inputOrigin[2]);
      baselineImageGeometry->getOrigin(baselineOrigin[0], baselineOrigin[1], baselineOrigin[2]);
      size_t inputDimensions[3];
      size_t baselineDimensions[3];
      inputImageGeometry->getDimensions(inputDimensions[0], inputDimensions[1], inputDimensions[2]);
      baselineImageGeometry->getDimensions(baselineDimensions[0], baselineDimensions[1], baselineDimensions[2]);
      for (int i = 0; i < 3; i++)
      {
          // SCIFIO does not save the spacing correctly. We disable this test until SCIFIO is fixed.
          //DREAM3D_COMPARE_FLOATS(&inputResolution[i], &baselineResolution[i], tol);
          // SCIFIO does not save the origin correctly. We disable this test until SCIFIO is fixed.
          //DREAM3D_COMPARE_FLOATS(&inputOrigin[i], &baselineOrigin[i], tol);

          DREAM3D_REQUIRE_EQUAL(inputDimensions[i], baselineDimensions[i]);
      }
      return true;
  }


  void GetMatrixAndAttributeArray(const DataContainer::Pointer &container,
                                  const QString &matrixName,
                                  const QString &arrayName,
                                  AttributeMatrix::Pointer & attributeMatrix,
                                  IDataArray::Pointer &dataArray)
  {
      attributeMatrix = container->getAttributeMatrix(matrixName);
      DREAM3D_REQUIRE_NE(attributeMatrix.get(), 0);
      dataArray = attributeMatrix->getAttributeArray(arrayName);
      DREAM3D_REQUIRE_NE(dataArray.get(), 0);
  }

  bool CompareAttributeMatrices(const AttributeMatrix::Pointer &baselineMatrix, const AttributeMatrix::Pointer &inputMatrix)
  {
      // Compare number of attributes
      DREAM3D_REQUIRE_EQUAL(baselineMatrix->getNumTuples(),inputMatrix->getNumTuples());
      DREAM3D_REQUIRE_EQUAL(baselineMatrix->getNumAttributeArrays(), inputMatrix->getNumAttributeArrays());
      QVector<size_t> baselineTupleDimensions = baselineMatrix->getTupleDimensions();
      QVector<size_t> inputTupleDimensions = inputMatrix->getTupleDimensions();
      DREAM3D_REQUIRE_EQUAL(inputTupleDimensions.size(), inputTupleDimensions.size());
      for (size_t ii = 0; ii < inputTupleDimensions.size(); ii++)
      {
          DREAM3D_REQUIRE_EQUAL(baselineTupleDimensions[ii], inputTupleDimensions[ii]);
      }
      DREAM3D_REQUIRE_EQUAL(baselineMatrix->getType(), inputMatrix->getType());
      return true;
  }

  template<class PixelType>
  bool CompareDataArrays(const IDataArray::Pointer &baselineArray, const IDataArray::Pointer &inputArray)
  {
      float tol = 1e-6;
      DREAM3D_REQUIRE_EQUAL(baselineArray->getSize(),inputArray->getSize());
      DREAM3D_REQUIRE_EQUAL(baselineArray->getNumberOfComponents(), inputArray->getNumberOfComponents());
      DREAM3D_REQUIRE_EQUAL(baselineArray->getNumberOfTuples(), inputArray->getNumberOfTuples());
      //DREAM3D_REQUIRE_EQUAL(baselineArray->getTypeAsString(), inputArray->getTypeAsString());->int8_t and char => should be considered as the same type.
      // Compare number of components
      for (size_t ii = 0; ii < baselineArray->getSize(); ii++)
      {
          float baselineValue = static_cast<PixelType*>(baselineArray->getVoidPointer(0))[ii];
          float inputValue = static_cast<PixelType*>(inputArray->getVoidPointer(0))[ii];
          DREAM3D_COMPARE_FLOATS(&baselineValue, &inputValue, tol);
      }
      return true;
  }

  template<class PixelType>
  bool CompareImageContainers(DataContainer::Pointer &inputContainer, DataContainer::Pointer &baselineContainer, const DataArrayPath& baselinePath)
  {
      // First compare geometries
      ImageGeom::Pointer inputImageGeometry = GetImageGeometry(inputContainer);
      ImageGeom::Pointer baselineImageGeometry = GetImageGeometry(baselineContainer);
      DREAM3D_REQUIRE(CompareImageGeometries(inputImageGeometry, baselineImageGeometry));
      // Then compare values
      AttributeMatrix::Pointer inputAttributeMatrix;
      IDataArray::Pointer inputDataArray;
      GetMatrixAndAttributeArray(inputContainer, SIMPL::Defaults::CellAttributeMatrixName, SIMPL::CellData::ImageData, inputAttributeMatrix, inputDataArray);
      AttributeMatrix::Pointer baselineAttributeMatrix;
      IDataArray::Pointer baselineDataArray;
      GetMatrixAndAttributeArray(baselineContainer, baselinePath.getAttributeMatrixName(), baselinePath.getDataArrayName(), baselineAttributeMatrix, baselineDataArray);
      CompareAttributeMatrices(baselineAttributeMatrix,inputAttributeMatrix);
      CompareDataArrays<PixelType>(baselineDataArray, inputDataArray);
      return true;
  }

  template<class PixelType>
  bool CompareImages(const QString &inputFilename, const DataContainerArray::Pointer &baselineContainerArray, const DataArrayPath& baselinePath)
  {
      // Get container for baseline
      QString baselineContainerName = baselinePath.getDataContainerName();
      DREAM3D_REQUIRE(baselineContainerArray->getDataContainerNames().contains(baselineContainerName));
      DataContainer::Pointer baselineContainer = baselineContainerArray->getDataContainer(baselineContainerName);

      // Load container for input
      // Use ITKImageReader filter to avoid re-writing filter here
      FilterManager* fm = FilterManager::Instance();
      IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter("ITKImageReader");
      DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
      AbstractFilter::Pointer filter = filterFactory->create();
      QVariant var;
      bool propWasSet;

      var.setValue(inputFilename);
      propWasSet = filter->setProperty("FileName", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)

      const QString inputContainerName = "inputContainer";
      DataContainerArray::Pointer inputContainerArray = DataContainerArray::New();
      filter->setDataContainerArray(inputContainerArray);
      var.setValue(inputContainerName);
      propWasSet = filter->setProperty("DataContainerName", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)
      filter->execute();

      DREAM3D_REQUIRE(inputContainerArray->getDataContainerNames().contains(inputContainerName));
      DataContainer::Pointer inputContainer = inputContainerArray->getDataContainer(inputContainerName);
      // Compare both data containers
      DREAM3D_REQUIRE(CompareImageContainers<PixelType>(inputContainer, baselineContainer, baselinePath));
      return true;
  }

  template<class PixelType>
  bool TestWriteImage(const QString& filename,
    DataContainerArray::Pointer containerArray,
    DataArrayPath& path)
  {

    QString filtName = "ITKImageWriter";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryForFilter(filtName);

    DREAM3D_REQUIRE_NE(filterFactory.get(),0);
    // If we get this far, the Factory is good so creating the filter should not fail unless something has
    // horribly gone wrong in which case the system is going to come down quickly after this.
    AbstractFilter::Pointer filter = filterFactory->create();

    QVariant var;
    bool propWasSet;

    var.setValue(filename);
    propWasSet = filter->setProperty("FileName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true)
     
    var.setValue(path);
    propWasSet = filter->setProperty("ImageArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true)

    filter->setDataContainerArray(containerArray);

    filter->execute();

    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);

    return CompareImages<PixelType>(filename, containerArray, path);
  }

  template<class PixelType, unsigned Dimension>
  void TestWriteImage(const QString& extension)
  {
    QString filename = FilenameWithDifferentExtension(
      UnitTest::ITKImageProcessingWriterTest::OutputBaseFile, extension);

    DataArrayPath path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = CreateTestData<PixelType,Dimension>(path);
    DREAM3D_REQUIRE(
      TestWriteImage<PixelType>(filename, containerArray, path));
    this->FilesToRemove << filename;
  }

  // -----------------------------------------------------------------------------
  //  Test methods
  // -----------------------------------------------------------------------------
  int TestAvailability(const QString& filterName)
  {
    if (NULL == GetFilterByName(filterName))
    {
      QString msg;
      msg = "The test requires the use of %1 filter which is found in the ITKImageProcessing Plugin";
      DREAM3D_TEST_THROW_EXCEPTION(msg.arg(filterName).toStdString())
    }
    return 0;
  }

  int TestNoInput()
  {
    AbstractFilter::Pointer writer = GetFilterByName("ITKImageWriter");
    if (!writer)
    {
      return EXIT_FAILURE;
    }

    writer->execute();
    DREAM3D_REQUIRED(writer->getErrorCondition(), == , -1);
    DREAM3D_REQUIRED(writer->getWarningCondition(), >= , 0);
    return EXIT_SUCCESS;
  }

  int TestWriteMetaImage()
  {
    // uint8_t
    {
      TestWriteImage<uint8_t,3>("_uint8.mha");
    }
    // int8_t
    {
      TestWriteImage<int8_t,3>("_int8.mha");
    }
    // uint32_t
    {
      TestWriteImage<uint32_t,3>("_uint32_t.mha");
    }
    // int32_t
    {
      TestWriteImage<int32_t,3>("_int32_t.mha");
    }
    // float
    {
      TestWriteImage<float,3>("_float.mha");
    }

    return EXIT_SUCCESS;
  }

  int TestWriteNrrdImage()
  {
    // uint8_t
    {
      TestWriteImage<uint8_t,3>("_uint8.nrrd");
    }
    // int8_t
    {
      TestWriteImage<int8_t,3>("_int8.nrrd");
    }
    // uint32_t
    {
      TestWriteImage<uint32_t,3>("_uint32_t.nrrd");
    }
    // int32_t
    {
      TestWriteImage<int32_t,3>("_int32_t.nrrd");
    }
    // float
    {
      TestWriteImage<float,3>("_float.nrrd");
    }

    return EXIT_SUCCESS;
  }

  int TestWriteSCIFIOImage()
  {
    // uint8_t
    {
      TestWriteImage<uint8_t,2>("_uint8.tif");
    }
    // int8_t
    {
      TestWriteImage<int8_t,2>("_int8.tif");
    }
    // float
    {
      TestWriteImage<float,2>("_float.tif");
    }

    return EXIT_SUCCESS;
  }
  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    int err = EXIT_SUCCESS;

    // Clean up before running the tests
    this->RemoveTestFiles();

    DREAM3D_REGISTER_TEST(TestAvailability("ITKImageWriter"));
    DREAM3D_REGISTER_TEST(TestNoInput());

    // Meta
    DREAM3D_REGISTER_TEST(TestWriteMetaImage());

    // NRRD
    DREAM3D_REGISTER_TEST(TestWriteNrrdImage());


    // SCIFIO
    DREAM3D_REGISTER_TEST(TestWriteSCIFIOImage());

#if REMOVE_TEST_FILES
    DREAM3D_REGISTER_TEST( RemoveTestFiles() )
#endif
  }

  private:
    ITKImageProcessingWriterTest(const ITKImageProcessingWriterTest&); // Copy Constructor Not Implemented
    void operator=(const ITKImageProcessingWriterTest&); // Operator '=' Not Implemented
};
