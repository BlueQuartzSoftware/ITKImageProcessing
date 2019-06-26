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

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "UnitTestSupport.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageWriter.h"

#include "ITKImageProcessingTestFileLocations.h"

#include <QFileInfo>
#include <itkNumericSeriesFileNames.h>

class ITKImageProcessingWriterTest
{
  const int k_MaxIndex = 95;
  const int k_MinIndex = 0;
  const int k_Increment = 1;

public:
  ITKImageProcessingWriterTest() = default;
  virtual ~ITKImageProcessingWriterTest() = default;
  ITKImageProcessingWriterTest(const ITKImageProcessingWriterTest&) = delete;            // Copy Constructor Not Implemented
  ITKImageProcessingWriterTest(ITKImageProcessingWriterTest&&) = delete;                 // Move Constructor Not Implemented
  ITKImageProcessingWriterTest& operator=(const ITKImageProcessingWriterTest&) = delete; // Copy Assignment Not Implemented
  ITKImageProcessingWriterTest& operator=(ITKImageProcessingWriterTest&&) = delete;      // Move Assignment Not Implemented

  QList<QString> FilesToRemove;

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
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filterName);
    if(nullptr == filterFactory.get())
    {
      return nullptr;
    }
    return filterFactory->create();
  }

  // -----------------------------------------------------------------------------
  template <class PixelType, unsigned int Dimension>
  DataContainerArray::Pointer CreateTestData(const DataArrayPath& path)
  {
    // Create test data (baseline)
    DataContainer::Pointer container = DataContainer::New(path.getDataContainerName());
    ImageGeom::Pointer imageGeometry = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
    QVector<float> origin(3, 0);
    QVector<float> spacing(3, 1);
    std::vector<size_t> dimensions(3, 1);
    for(int32_t i = 0; i < Dimension; i++)
    {
      origin[i] = 1.23f + static_cast<float>(i);
      spacing[i] = 0.45f + static_cast<float>(i) * 0.2f;
      dimensions[i] = 90 + i * 3;
    }
    imageGeometry->setSpacing(spacing.data());
    imageGeometry->setOrigin(origin.data());
    imageGeometry->setDimensions(dimensions.data());
    container->setGeometry(imageGeometry);
    std::vector<size_t> arrayDimensions(1, 0);
    arrayDimensions[0] = 1;
    AttributeMatrix::Pointer matrixArray = container->createAndAddAttributeMatrix(dimensions, path.getAttributeMatrixName(), AttributeMatrix::Type::Cell);
    typename DataArray<PixelType>::Pointer data = DataArray<PixelType>::CreateArray(dimensions, arrayDimensions, path.getDataArrayName(), true);
    data->initializeWithValue(11.0);
    matrixArray->insertOrAssign(data);

    DataContainerArray::Pointer containerArray = DataContainerArray::New();
    containerArray->addOrReplaceDataContainer(container);
    return containerArray;
  }
  // -----------------------------------------------------------------------------
  ImageGeom::Pointer GetImageGeometry(DataContainer::Pointer& container)
  {
    IGeometry::Pointer geometry = container->getGeometry();
    DREAM3D_REQUIRE_VALID_POINTER(geometry.get());
    DREAM3D_REQUIRE_EQUAL(geometry->getGeometryTypeAsString(), "ImageGeometry");
    ImageGeom::Pointer imageGeometry = std::dynamic_pointer_cast<ImageGeom>(geometry);
    DREAM3D_REQUIRE_NE(imageGeometry.get(), 0);
    return imageGeometry;
  }

  // -----------------------------------------------------------------------------
  bool CompareImageGeometries(const ImageGeom::Pointer& inputImageGeometry, const ImageGeom::Pointer& baselineImageGeometry)
  {

    //    FloatVec3Type inputResolution = inputImageGeometry->getSpacing();
    //    FloatVec3Type baselineResolution = baselineImageGeometry->getSpacing();
    //    FloatVec3Type inputOrigin = inputImageGeometry->getOrigin();
    //    FloatVec3Type baselineOrigin = baselineImageGeometry->getOrigin();
    SizeVec3Type inputDimensions = inputImageGeometry->getDimensions();
    SizeVec3Type baselineDimensions = baselineImageGeometry->getDimensions();
    for(int i = 0; i < 3; i++)
    {
      // float tol = 1e-6;
      // SCIFIO does not save the spacing correctly. We disable this test until SCIFIO is fixed.
      // DREAM3D_COMPARE_FLOATS(&inputResolution[i], &baselineResolution[i], tol);
      // SCIFIO does not save the origin correctly. We disable this test until SCIFIO is fixed.
      // DREAM3D_COMPARE_FLOATS(&inputOrigin[i], &baselineOrigin[i], tol);

      DREAM3D_REQUIRE_EQUAL(inputDimensions[i], baselineDimensions[i]);
      //      DREAM3D_REQUIRE_EQUAL(inputResolution[i], baselineResolution[i]);
      //      DREAM3D_REQUIRE_EQUAL(inputOrigin[i], baselineOrigin[i]);
    }
    return true;
  }

  // -----------------------------------------------------------------------------
  void GetMatrixAndAttributeArray(const DataContainer::Pointer& container, const QString& matrixName, const QString& arrayName, AttributeMatrix::Pointer& attributeMatrix,
                                  IDataArray::Pointer& dataArray)
  {
    attributeMatrix = container->getAttributeMatrix(matrixName);
    DREAM3D_REQUIRE_NE(attributeMatrix.get(), 0);
    dataArray = attributeMatrix->getAttributeArray(arrayName);
    DREAM3D_REQUIRE_NE(dataArray.get(), 0);
  }

  // -----------------------------------------------------------------------------
  bool CompareAttributeMatrices(const AttributeMatrix::Pointer& baselineMatrix, const AttributeMatrix::Pointer& inputMatrix)
  {
    // Compare number of attributes
    DREAM3D_REQUIRE_EQUAL(baselineMatrix->getNumberOfTuples(), inputMatrix->getNumberOfTuples());
    DREAM3D_REQUIRE_EQUAL(baselineMatrix->getNumAttributeArrays(), inputMatrix->getNumAttributeArrays());
    std::vector<size_t> baselineTupleDimensions = baselineMatrix->getTupleDimensions();
    std::vector<size_t> inputTupleDimensions = inputMatrix->getTupleDimensions();
    DREAM3D_REQUIRE_EQUAL(inputTupleDimensions.size(), inputTupleDimensions.size());
    for(size_t ii = 0; ii < inputTupleDimensions.size(); ii++)
    {
      DREAM3D_REQUIRE_EQUAL(baselineTupleDimensions[ii], inputTupleDimensions[ii]);
    }
    DREAM3D_REQUIRE_EQUAL(static_cast<uint32_t>(baselineMatrix->getType()), static_cast<uint32_t>(inputMatrix->getType()));
    return true;
  }

  // -----------------------------------------------------------------------------
  template <class PixelType>
  bool CompareDataArrays(const IDataArray::Pointer& baselineArray, const IDataArray::Pointer& inputArray)
  {
    float tol = 1e-6;
    DREAM3D_REQUIRE_EQUAL(baselineArray->getSize(), inputArray->getSize());
    DREAM3D_REQUIRE_EQUAL(baselineArray->getNumberOfComponents(), inputArray->getNumberOfComponents());
    DREAM3D_REQUIRE_EQUAL(baselineArray->getNumberOfTuples(), inputArray->getNumberOfTuples());
    // DREAM3D_REQUIRE_EQUAL(baselineArray->getTypeAsString(), inputArray->getTypeAsString());->int8_t and char => should be considered as the same type.
    // Compare number of components
    for(size_t ii = 0; ii < baselineArray->getSize(); ii++)
    {
      float baselineValue = static_cast<PixelType*>(baselineArray->getVoidPointer(0))[ii];
      float inputValue = static_cast<PixelType*>(inputArray->getVoidPointer(0))[ii];
      DREAM3D_COMPARE_FLOATS(&baselineValue, &inputValue, tol);
    }
    return true;
  }

  // -----------------------------------------------------------------------------
  template <class PixelType>
  bool CompareImageContainers(DataContainer::Pointer& inputContainer, DataContainer::Pointer& baselineContainer, const DataArrayPath& baselinePath)
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
    CompareAttributeMatrices(baselineAttributeMatrix, inputAttributeMatrix);
    CompareDataArrays<PixelType>(baselineDataArray, inputDataArray);
    return true;
  }

  // -----------------------------------------------------------------------------
  template <class PixelType, unsigned int Dimension>
  bool CompareImages(const QString& inputFilename, const DataContainerArray::Pointer& baselineContainerArray, const DataArrayPath& baselinePath, const QString& dataFileExtension = "")
  {
    // Get container for baseline
    QString baselineContainerName = baselinePath.getDataContainerName();
    DREAM3D_REQUIRE(baselineContainerArray->getDataContainerNames().contains(baselineContainerName));
    DataContainer::Pointer baselineContainer = baselineContainerArray->getDataContainer(baselineContainerName);

    // Load container for input
    // Use ITKImageReader filter to avoid re-writing filter here
    FilterManager* fm = FilterManager::Instance();
    AbstractFilter::Pointer filter;
    QVariant var;
    bool propWasSet;

    if(Dimension == 2)
    {
      IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(QString("ITKImageReader"));
      DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
      filter = filterFactory->create();

      var.setValue(inputFilename);
      propWasSet = filter->setProperty("FileName", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true)
    }
    else if(Dimension == 3)
    {
      IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(QString("ITKImportImageStack"));
      DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
      filter = filterFactory->create();

      FileListInfo_t listInfo;
      listInfo.PaddingDigits = 0;
      listInfo.Ordering = 0;
      listInfo.StartIndex = 0;
      listInfo.EndIndex = 95;
      listInfo.IncrementIndex = 1;

      QFileInfo fi(inputFilename);
      listInfo.InputPath = fi.absolutePath();

      listInfo.FilePrefix = fi.baseName() + QString("_");

      listInfo.FileExtension = fi.suffix();

      var.setValue(listInfo);
      propWasSet = filter->setProperty("InputFileListInfo", var);
      DREAM3D_REQUIRE_EQUAL(propWasSet, true);

      bool hasMissingFiles = false;
      bool orderAscending = false;

      if(listInfo.Ordering == 0)
      {
        orderAscending = true;
      }
      else if(listInfo.Ordering == 1)
      {
        orderAscending = false;
      }

      // Now generate all the file names the user is asking for and populate the table
      QVector<QString> files = FilePathGenerator::GenerateFileList(listInfo.StartIndex, listInfo.EndIndex, listInfo.IncrementIndex, hasMissingFiles, orderAscending, listInfo.InputPath,
                                                                   listInfo.FilePrefix, listInfo.FileSuffix, listInfo.FileExtension, listInfo.PaddingDigits);
      this->FilesToRemove.append(files.toList());
      if(!dataFileExtension.isEmpty())
      {
        listInfo.FileExtension = dataFileExtension;
      }
      files = FilePathGenerator::GenerateFileList(listInfo.StartIndex, listInfo.EndIndex, listInfo.IncrementIndex, hasMissingFiles, orderAscending, listInfo.InputPath, listInfo.FilePrefix,
                                                  listInfo.FileSuffix, listInfo.FileExtension, listInfo.PaddingDigits);
      this->FilesToRemove.append(files.toList());
    }

    Observer obs;
    QObject::connect(filter.get(), &AbstractFilter::messageGenerated, &obs, &Observer::processPipelineMessage);

    const QString inputContainerName = "inputContainer";
    DataContainerArray::Pointer inputContainerArray = DataContainerArray::New();
    filter->setDataContainerArray(inputContainerArray);
    var.setValue(DataArrayPath(inputContainerName, "", ""));
    propWasSet = filter->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true)
    filter->execute();
    int err = filter->getErrorCode();
    DREAM3D_REQUIRE_EQUAL(err, 0)

    DREAM3D_REQUIRE(inputContainerArray->getDataContainerNames().contains(inputContainerName));
    DataContainer::Pointer inputContainer = inputContainerArray->getDataContainer(inputContainerName);
    // Compare both data containers
    DREAM3D_REQUIRE(CompareImageContainers<PixelType>(inputContainer, baselineContainer, baselinePath));
    return true;
  }

  // -----------------------------------------------------------------------------
  template <class PixelType, unsigned int Dimension>
  bool RunWriteImage(const QString& filename, DataContainerArray::Pointer containerArray, DataArrayPath& path)
  {

    QString filtName = "ITKImageWriter";
    FilterManager* fm = FilterManager::Instance();
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);

    DREAM3D_REQUIRE_NE(filterFactory.get(), 0);
    // If we get this far, the Factory is good so creating the filter should not fail unless something has
    // horribly gone wrong in which case the system is going to come down quickly after this.
    AbstractFilter::Pointer filter = filterFactory->create();

    //    Observer obs;
    //    QObject::connect(filter.get(), &AbstractFilter::filterGeneratedMessage, &obs, &Observer::processPipelineMessage);

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

    DREAM3D_REQUIRED(filter->getErrorCode(), >=, 0);
    DREAM3D_REQUIRED(filter->getWarningCode(), >=, 0);
    this->FilesToRemove << filename;
    return true;
  }

  // -----------------------------------------------------------------------------
  template <class PixelType, unsigned int Dimension>
  void TestWriteImage(const QString& suffix, const QString& extension, const QString& dataFileExtension)
  {
    //    std::cout << "******** TestWriteImageSeries " << suffix.toStdString() << ", " << extension.toStdString() << ", " << dataFileExtension.toStdString() << ", " << Dimension
    //              << " ******************************" << std::endl;

    QString filename = UnitTest::ITKImageProcessingWriterTest::OutputBaseFile + suffix + extension;
    DataArrayPath path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = CreateTestData<PixelType, Dimension>(path);
    bool success = RunWriteImage<PixelType, Dimension>(filename, containerArray, path);
    DREAM3D_REQUIRE(success);
    success = CompareImages<PixelType, Dimension>(filename, containerArray, path, dataFileExtension);
    DREAM3D_REQUIRE(success);

    this->FilesToRemove << filename;
    if(!dataFileExtension.isEmpty())
    {
      filename = UnitTest::ITKImageProcessingWriterTest::OutputBaseFile + suffix + dataFileExtension;
      this->FilesToRemove << filename;
    }
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
    AbstractFilter::Pointer writer = GetFilterByName("ITKImageWriter");
    if(!writer)
    {
      return EXIT_FAILURE;
    }

    writer->execute();
    DREAM3D_REQUIRED(writer->getErrorCode(), ==, -21003);
    DREAM3D_REQUIRED(writer->getWarningCode(), >=, 0);
    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  template <unsigned int Dimension>
  int TestWriteImage(QString extension, const QStringList& listPixelTypes, QString dataFileExtension = "")
  {
    for(QStringList::const_iterator it = listPixelTypes.constBegin(); it != listPixelTypes.constEnd(); ++it)
    {
      if(*it == "uint8_t")
      {
        TestWriteImage<uint8_t, Dimension>("_uint8.", extension, dataFileExtension);
      }
      else if(*it == "int8_t")
      {
        TestWriteImage<int8_t, Dimension>("_int8.", extension, dataFileExtension);
      }
      else if(*it == "uint16_t")
      {
        TestWriteImage<uint16_t, Dimension>("_uint16.", extension, dataFileExtension);
      }
      else if(*it == "int16_t")
      {
        TestWriteImage<int16_t, Dimension>("_int16.", extension, dataFileExtension);
      }
      else if(*it == "uint32_t")
      {
        TestWriteImage<uint32_t, Dimension>("_uint32.", extension, dataFileExtension);
      }
      else if(*it == "int32_t")
      {
        TestWriteImage<int32_t, Dimension>("_int32.", extension, dataFileExtension);
      }
      // else if (*it == "uint64_t")
      //{
      //  TestWriteImage<uint64_t, Dimension>("_uint64.", extension, dataFileExtension);
      //}
      // else if (*it == "int64_t")
      //{
      //  TestWriteImage<int64_t, Dimension>("_int64.", extension, dataFileExtension);
      //}
      else if(*it == "float")
      {
        TestWriteImage<float, Dimension>("_float.", extension, dataFileExtension);
      }
      else if(*it == "double")
      {
        TestWriteImage<double, Dimension>("_double.", extension, dataFileExtension);
      }
    }
    RemoveTestFiles();
    this->FilesToRemove.clear();
    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestWriteImageSeries()
  {
    //    std::cout << "************************ TestWriteImageSeries ******************************" << std::endl;
    QString filename = UnitTest::ITKImageProcessingWriterTest::OutputBaseFile + ".png";
    DataArrayPath path("TestContainer", "TestAttributeMatrixName", "TestAttributeArrayName");
    DataContainerArray::Pointer containerArray = CreateTestData<uint8_t, 3>(path);
    bool success = RunWriteImage<uint8_t, 2>(filename, containerArray, path);
    DREAM3D_REQUIRE(success);
    using NamesGeneratorType = itk::NumericSeriesFileNames;
    QString seriesfilename = UnitTest::ITKImageProcessingWriterTest::OutputBaseFile + "_%d.png";
    NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
    namesGenerator->SetSeriesFormat(seriesfilename.toStdString());
    namesGenerator->SetIncrementIndex(k_Increment);
    namesGenerator->SetStartIndex(k_MinIndex);
    namesGenerator->SetEndIndex(k_MaxIndex); // There should be 96 slices: 90 + 3*2 (see CreateTestData)
    std::vector<std::string> listFileNames = namesGenerator->GetFileNames();
    // for(size_t ii = 0; ii < listFileNames.size(); ii++)
    for(const auto& fileName : listFileNames)
    {
      // Check that all files exist
      QFileInfo check_file(fileName.c_str());
      if(!check_file.exists())
      {
        std::cout << fileName << " Does not exist" << std::endl;
      }
      DREAM3D_REQUIRE((check_file.exists() && check_file.isFile()));
      // Remove file
      this->FilesToRemove << QString(fileName.c_str());
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

    // TIFF
    QStringList listTIFFPixelTypes;
    listTIFFPixelTypes << "uint8_t"
                       << "int8_t"
                       << "float";
    DREAM3D_REGISTER_TEST(TestWriteImage<2>("tif", listTIFFPixelTypes));

    // PNG
    QStringList listPNGPixelTypes;
    listPNGPixelTypes << "uint8_t"
                      << "uint16_t";
    DREAM3D_REGISTER_TEST(TestWriteImage<2>("png", listPNGPixelTypes));

    // JPG
    QStringList listJPGPixelTypes;
    listJPGPixelTypes << "uint8_t";
    DREAM3D_REGISTER_TEST(TestWriteImage<2>("jpg", listJPGPixelTypes));

    // MRC
    QStringList listMRCPixelTypes;
    listMRCPixelTypes << "uint8_t"
                      << "int16_t"
                      << "uint16_t"
                      << "float";
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("mrc", listMRCPixelTypes));

    // Meta
    QStringList listMetaPixelTypes;
    listMetaPixelTypes << "uint8_t"
                       << "int8_t"
                       << "uint16_t"
                       << "int16_t"
                       << "uint32_t"
                       << "int32_t"
                       << "uint64_t"
                       << "int64_t"
                       << "float"
                       << "double";
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("mha", listMetaPixelTypes));
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("mhd", listMetaPixelTypes, "zraw"));

    // NRRD
    QStringList listNRRDPixelTypes;
    listNRRDPixelTypes << "uint8_t"
                       << "int8_t"
                       << "uint16_t"
                       << "int16_t"
                       << "uint32_t"
                       << "int32_t"
                       << "uint64_t"
                       << "int64_t"
                       << "float"
                       << "double";
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("nrrd", listNRRDPixelTypes));
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("nhdr", listNRRDPixelTypes, "raw.gz"));
    // NII
    QStringList listNIIPixelTypes;
    listNIIPixelTypes << "uint8_t"
                      << "int8_t"
                      << "uint16_t"
                      << "int16_t"
                      << "uint32_t"
                      << "int32_t"
                      << "uint64_t"
                      << "int64_t"
                      << "float"
                      << "double";
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("nii", listNIIPixelTypes));
#ifdef ITK_IMAGE_PROCESSING_HAVE_SCIFIO
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("nii.gz", listNIIPixelTypes));
#endif

    // GIPL
    QStringList listGIPLPixelTypes;
    listGIPLPixelTypes << "uint8_t"
                       << "int8_t"
                       << "uint16_t"
                       << "int16_t"
                       << "float";
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("gipl", listGIPLPixelTypes));

#ifdef ITK_IMAGE_PROCESSING_HAVE_SCIFIO
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("gipl.gz", listGIPLPixelTypes));
#endif

    // ANALYZE
    QStringList listHDRPixelTypes;
    listHDRPixelTypes << "uint8_t"
                      << "int8_t"
                      << "uint16_t"
                      << "int16_t"
                      << "uint32_t"
                      << "int32_t"
                      << "uint64_t"
                      << "int64_t"
                      << "float"
                      << "double";
    DREAM3D_REGISTER_TEST(TestWriteImage<3>("hdr", listHDRPixelTypes, "img"));

    // BMP -> Load all images as RGB in ITK??
    //    QStringList listBMPPixelTypes;
    //    listBMPPixelTypes << "uint8_t" ;
    //    DREAM3D_REGISTER_TEST(TestWriteImage<2>("bmp", listBMPPixelTypes));

    // Test image series
    DREAM3D_REGISTER_TEST(TestWriteImageSeries())

#if REMOVE_TEST_FILES
    //   if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(RemoveTestFiles())
    }
#endif
  }
};
