#pragma once
/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
 * All rights reserved.
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <array>
#include <cmath>
#include <map>

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Filtering/QMetaObjectUtilities.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Montages/GridMontage.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/SIMPLib.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/util/FFTConvolutionCostFunction.h"
#include "ITKImageProcessing/Test/UnitTestSupport.hpp"

#ifndef M_PI
#define M_PI (3.141592653)
#endif

namespace
{
  const QString MontageName = "Grid Montage";
}

class BlendTest
{
public:
  BlendTest() = default;
  ~BlendTest() = default;
  BlendTest(const BlendTest&) = delete;            // Copy Constructor
  BlendTest(BlendTest&&) = delete;                 // Move Constructor
  BlendTest& operator=(const BlendTest&) = delete; // Copy Assignment
  BlendTest& operator=(BlendTest&&) = delete;      // Move Assignment

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void ReadImage(AbstractFilter::Pointer readerFilter, const QString& imagePath, const QString& imageName, DataArrayPath& dap)
  {
    dap.setDataContainerName(imageName);
    readerFilter->setProperty("FileName", QVariant(imagePath));
    readerFilter->setProperty("DataContainerName", QVariant::fromValue(dap));
    readerFilter->execute();
    DREAM3D_REQUIRE(readerFilter->getDataContainerArray()->doesDataContainerExist(imageName) == true)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void CreateGridMontage(const DataContainerArray::Pointer& dca)
  {
    
    GridMontage::Pointer gridMontage = GridMontage::New(::MontageName, 2, 2, 1);
    GridTileIndex index = gridMontage->getTileIndex(0, 0);
    DataContainer::Pointer tileDC = dca->getDataContainer("R0C0");
    gridMontage->setDataContainer(index, tileDC);

    index = gridMontage->getTileIndex(0, 1);
    tileDC = dca->getDataContainer("R0C1");
    gridMontage->setDataContainer(index, tileDC);

    index = gridMontage->getTileIndex(1, 0);
    tileDC = dca->getDataContainer("R1C0");
    gridMontage->setDataContainer(index, tileDC);

    index = gridMontage->getTileIndex(1, 1);
    tileDC = dca->getDataContainer("R1C1");
    gridMontage->setDataContainer(index, tileDC);

    DREAM3D_REQUIRE(dca->addMontage(gridMontage) == true);
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int ImportTestData()
  {
    // NOTE This should change to read the path of the DREAM3D_SDK CMake variable
    const QString image1Path = "C:\\Users\\mmarine\\Desktop\\EBSD_Alg\\2209p230908A\\Images\\Grayscale\\R0C0.jpg";
    const QString image2Path = "C:\\Users\\mmarine\\Desktop\\EBSD_Alg\\2209p230908A\\Images\\Grayscale\\R0C1.jpg";
    const QString image3Path = "C:\\Users\\mmarine\\Desktop\\EBSD_Alg\\2209p230908A\\Images\\Grayscale\\R1C0.jpg";
    const QString image4Path = "C:\\Users\\mmarine\\Desktop\\EBSD_Alg\\2209p230908A\\Images\\Grayscale\\R1C1.jpg";
    const QString image1Name = "R0C0";
    const QString image2Name = "R0C1";
    const QString image3Name = "R1C0";
    const QString image4Name = "R1C1";

    DREAM3D_REQUIRE_EQUAL(QFile::exists(image1Path), true)
    DREAM3D_REQUIRE_EQUAL(QFile::exists(image2Path), true)
    DREAM3D_REQUIRE_EQUAL(QFile::exists(image3Path), true)
    DREAM3D_REQUIRE_EQUAL(QFile::exists(image4Path), true)

    AbstractFilter::Pointer readerFilter = FilterManager::Instance()->getFactoryFromClassName("ITKImageReader")->create();
    DREAM3D_REQUIRE(readerFilter.get() != nullptr)

    DataArrayPath dap("", "CellData", "ImageData");
    ReadImage(readerFilter, image1Path, image1Name, dap);
    ReadImage(readerFilter, image2Path, image2Name, dap);
    ReadImage(readerFilter, image3Path, image3Name, dap);
    ReadImage(readerFilter, image4Path, image4Name, dap);

    m_ImageReaderDca = readerFilter->getDataContainerArray();

    CreateGridMontage(m_ImageReaderDca);

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int InitBlendFilter()
  {
    IFilterFactory::Pointer blendFactory = FilterManager::Instance()->getFactoryFromClassName("Blend");
    DREAM3D_REQUIRE(blendFactory.get() != nullptr)

    m_BlendFilter = blendFactory->create();
    DREAM3D_REQUIRE(m_BlendFilter.get() != nullptr)

    QString montageName = ::MontageName;
    DREAM3D_REQUIRE(m_ImageReaderDca->getMontage(::MontageName) != nullptr)

    // An affine transform will use degree 1 - Dave's algorithm assumes a degree 2
    // Correcting barrel/fish-eye/lens distortion requires degree 2 or higher
    const int degree = 1;
    const int maxIterations = 10000;

    const QString initialGuess = {"0.0; 0.0; 1.0; 0.0; 0.0; -1.0; 0.0; 0.0"};
    const float overlapPercentage = 0.25f;
    const QString dataArrayName = "ImageData";
    const QString attrMatrName = "CellData";
    const double lowTolerance = 1E-2;
    const double highTolerance = 1E-2;

    m_BlendFilter->setProperty("MaxIterations", QVariant(maxIterations));
    m_BlendFilter->setProperty("Degree", QVariant(degree));
    m_BlendFilter->setProperty("InitialSimplexGuess", QVariant(initialGuess));
    m_BlendFilter->setProperty("OverlapPercentage", QVariant(overlapPercentage));
    m_BlendFilter->setProperty("LowTolerance", QVariant(lowTolerance));
    m_BlendFilter->setProperty("HighTolerance", QVariant(highTolerance));
    m_BlendFilter->setProperty("DataAttributeArrayName", QVariant(dataArrayName));
    m_BlendFilter->setProperty("AttributeMatrixName", QVariant(attrMatrName));
    m_BlendFilter->setProperty("MontageName", montageName);
    m_BlendFilter->setDataContainerArray(m_ImageReaderDca);

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestFFTConvolutionCostFunction()
  {
    const QString rowChar = 'R';
    const QString colChar = 'C';
    const int degree = m_BlendFilter->property("Degree").toInt();
    const float overlapPercentage = m_BlendFilter->property("OverlapPercentage").toFloat();
    const QStringList dcNames = m_ImageReaderDca->getMontage(::MontageName)->getDataContainerNames();
    const QString attrMatName = m_BlendFilter->property("AttributeMatrixName").toString();
    const QString dataArrayName = m_BlendFilter->property("DataAttributeArrayName").toString();

    // The line below is used for testing the MultiParamCostFunction
    //    std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}
    FFTConvolutionCostFunction costFunction;
    costFunction.Initialize(dcNames, rowChar, colChar, degree, overlapPercentage, m_ImageReaderDca, attrMatName, dataArrayName);

    using GridKeys = QVector<FFTConvolutionCostFunction::GridKey>;
    GridKeys gridKeys;
    const FFTConvolutionCostFunction::ImageGrid imageGrid = costFunction.getImageGrid();
    for(const auto& gridValues : imageGrid)
    {
      gridKeys.push_back(gridValues.first);
    }

    FFTConvolutionCostFunction::GridKey key00 = std::make_pair(0, 0);
    FFTConvolutionCostFunction::GridKey key01 = std::make_pair(0, 1);
    FFTConvolutionCostFunction::GridKey key11 = std::make_pair(1, 1);
    FFTConvolutionCostFunction::GridKey key10 = std::make_pair(1, 0);

    DREAM3D_REQUIRE(gridKeys.contains(key00))
    DREAM3D_REQUIRE(gridKeys.contains(key01))
    DREAM3D_REQUIRE(gridKeys.contains(key11))
    DREAM3D_REQUIRE(gridKeys.contains(key10))

    // TODO: Test GetValue method

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int ExecuteBlendTest()
  {
    const double errTolerance = 3.0;

    // THIS ISN'T THE ACTUAL KNOWN TRANSFORMATION MATRIX
    // For one, this assumes a polynomial of degree 1 (i.e. an affine transform)
    // The real answer will probably utilize a polynomial of degree 2 (and have upwards of 16 coefficients!)
    // The answer would need to be obtained from Dave's work on these images
    // Or by actually running the filter and getting the initial guess with
    // a very small actual error inside the filter
    // Note that Dave's work utilizes a custom polynomial function with some terms ignored
    // NOTE The images I've used have been rotated 90 degrees counterclockwise
    // Which means the transform to correct the 'distortion' should be a rotation
    // transform that rotates them 90 degrees clockwise
    const std::vector<double> expectedAnswer = {0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0};

    m_BlendFilter->execute();

    DREAM3D_REQUIRE_EQUAL(m_BlendFilter->getErrorCode(), 0)
    DataContainer::Pointer dc = m_BlendFilter->getDataContainerArray()->getDataContainer("Blend Data");
    DREAM3D_REQUIRE(dc.get() != nullptr)
    AttributeMatrix::Pointer am = dc->getAttributeMatrix("Transform Matrix");
    DREAM3D_REQUIRE(am.get() != nullptr)
    DoubleArrayType::Pointer transformArray = am->getAttributeArrayAs<DoubleArrayType>("Transform");
    DREAM3D_REQUIRE(transformArray.get() != nullptr)

    double error = 0;
    for(size_t coeffIdx = 0; coeffIdx < transformArray->size(); ++coeffIdx)
    {
      error += abs(transformArray->getValue(coeffIdx) - expectedAnswer[coeffIdx]);
    }
    bool outOfTolerance = error > errTolerance;
    DREAM3D_REQUIRE_EQUAL(outOfTolerance, false)

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "---------------- BlendTest ---------------------" << std::endl;
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(ImportTestData())
    DREAM3D_REGISTER_TEST(InitBlendFilter())
    DREAM3D_REGISTER_TEST(TestFFTConvolutionCostFunction())
    DREAM3D_REGISTER_TEST(ExecuteBlendTest())
  }

private:
  AbstractFilter::Pointer m_BlendFilter;
  DataContainerArray::Pointer m_ImageReaderDca;
};
