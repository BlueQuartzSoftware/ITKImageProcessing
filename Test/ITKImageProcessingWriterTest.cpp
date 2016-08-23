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
#include <QtCore/QCryptographicHash>

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

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  QString FilenameWithDifferentExtension(const QString& filename, const QString& extension = ".raw")
  {
    QString nameWithoutExtension = filename.left(filename.lastIndexOf("."));
    return nameWithoutExtension + extension;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  template<class PixelType>
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
    for (size_t i = 0; i < 3; i++)
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

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  bool TestFileByHash(const QString& filename, const QByteArray& expectedHash)
  {
    // Compare files using their hashes
    QCryptographicHash hash(QCryptographicHash::Sha1);
    QFile file(filename);
    DREAM3D_ASSERT(file.open(QIODevice::ReadOnly));
    hash.addData(file.readAll());

    DREAM3D_ASSERT(!hash.result().isNull());

    // Retrieve the SHA1 signature of the file
    DREAM3D_REQUIRE_EQUAL(expectedHash, hash.result().toHex());
    return true;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  bool TestWriteImage(const QString& filename,
    DataContainerArray::Pointer containerArray,
    DataArrayPath& path,
    const QByteArray& expectedHash)
  {
    AbstractFilter::Pointer filter = GetFilterByName("ITKImageWriter");
    if (!filter)
    {
      return false;
    }
    bool propWasSet;
    QVariant var;

    // We can use the method from Abstract filter to set the DataContainerArray
    filter->setDataContainerArray(containerArray);

    // We must go through the QMetaSystem to set the other properties of the filter because
    // we do not have access to the actual types because those are loaded up through
    // a plugin and we can not link to a "module".
    var.setValue(path);
    propWasSet = filter->setProperty("ImageArrayPath", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true)

    var.setValue(filename);
    propWasSet = filter->setProperty("FileName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true)

    filter->execute();
    DREAM3D_REQUIRED(filter->getErrorCondition(), >= , 0);
    DREAM3D_REQUIRED(filter->getWarningCondition(), >= , 0);

    return TestFileByHash(filename, expectedHash);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  template<class PixelType>
  void TestWriteImage(const QString& extension, const QByteArray& expectedHash)
  {
    QString filename = FilenameWithDifferentExtension(
      UnitTest::ITKImageProcessingWriterTest::OutputBaseFile, extension);

    DataArrayPath path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = CreateTestData<PixelType>(path);
    DREAM3D_REQUIRE(
      TestWriteImage(filename, containerArray, path,
      expectedHash
      ));
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
      TestWriteImage<uint8_t>("_uint8.mha", "34bfdf119323fef41383bf35bda6e98765adca39");
    }
    // int8_t
    {
      TestWriteImage<int8_t>("_int8.mha", "e4f81d16c8b73b28644ab6bab0c2e4d18f25c2c2");
    }
    // uint32_t
    {
      TestWriteImage<uint32_t>("_uint32_t.mha", "4f1f83eb90585ff605d4754270b42549050da0f4");
    }
    // int32_t
    {
      TestWriteImage<int32_t>("_int32_t.mha", "83e1d1f4fc21904b8872bc0427bc616b49fbc3af");
    }
    // float
    {
      TestWriteImage<float>("_float.mha", "91aeeeaa74c0786a00ac92b0cb53090cdcb209fe");
    }

    return EXIT_SUCCESS;
  }

  int TestWriteNrrdImage()
  {
    // uint8_t
    {
      TestWriteImage<uint8_t>("_uint8.nrrd", "648a95abb10b63f95aa63cc47873444b0d083c66");
    }
    // int8_t
    {
      TestWriteImage<int8_t>("_int8.nrrd", "22c1caf246ce90e299234daec7116ee53c79ee08");
    }
    // uint32_t
    {
      TestWriteImage<uint32_t>("_uint32_t.nrrd", "87c68da74918acb5399e312fec714a97d77acba3");
    }
    // int32_t
    {
      TestWriteImage<int32_t>("_int32_t.nrrd", "392d5a065d29498902b81b1d23bed81fba3e9f43");
    }
    // float
    {
      TestWriteImage<float>("_float.nrrd", "bdbcdf0ee130284bcc3f2c78029ebaaf1616c75e");
    }

    return EXIT_SUCCESS;
  }

  int TestWriteSCIFIOImage()
  {
    // uint8_t
    {
      TestWriteImage<uint8_t>("_uint8.tif", "43d51b0eebbf74a6e68dd8e5b6597a62adfac228");
    }
    // int8_t
    {
      TestWriteImage<int8_t>("_int8.tif", "313c8e11119dbcd6eaff5931c5cf8833ec3a51bf");
    }
    // float
    {
      TestWriteImage<float>("_float.tif", "16932e0885e564e5c601fbd25daa2ee52c02caef");
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
