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
#include <map>
#include <tuple>

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
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/CoreFilters/SetOriginResolutionImageGeom.h"
#include "SIMPLib/CoreFilters/DataContainerWriter.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/util/FFTConvolutionCostFunction.h"
#include "ITKImageProcessing/Test/UnitTestSupport.hpp"
#include "ITKImageProcessing/Test/ITKImageProcessingTestFileLocations.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/CalcDewarpParameters.h"

class EdaxEbsdMontageTest
{
public:
  EdaxEbsdMontageTest() = default;
  ~EdaxEbsdMontageTest() = default;
  EdaxEbsdMontageTest(const EdaxEbsdMontageTest&) = delete;            // Copy Constructor
  EdaxEbsdMontageTest(EdaxEbsdMontageTest&&) = delete;                 // Move Constructor
  EdaxEbsdMontageTest& operator=(const EdaxEbsdMontageTest&) = delete; // Copy Assignment
  EdaxEbsdMontageTest& operator=(EdaxEbsdMontageTest&&) = delete;      // Move Assignment

  const size_t k_Rows = 2;
  const size_t k_Cols = 2;

  const QString k_MontageName = "Grid Montage";
  const QString k_ScanData = QString("Scan Data");
  const QString k_PhaseData = QString("Phase Data");
  const QString k_ImageName = QString("IPFColors");
  const QString k_DewarpTransformContainerName = QString("Dewarp Data");
  const QString k_TransformMatrix = QString("Trnasform Matrix");
  const QString k_TransformArray = QString("Transform");

  const QString k_OutputFile = QString("EbsdMontage.dream3d");

  using ImageTupleType = std::tuple<QString, QString, FloatVec3Type>;

  const std::vector<ImageTupleType> k_InputRGBImages = {{UnitTest::ITKImageProcessingDataDir + "/ITKImageProcessing/Montage/2209p230908A", "a3.osc_r0c0", {0.0f, 0.0f, 0.0f}},
                                                        {UnitTest::ITKImageProcessingDataDir + "/ITKImageProcessing/Montage/2209p230908A", "a3.osc_r0c1", {537.0f, 0.0f, 0.0f}},
                                                        {UnitTest::ITKImageProcessingDataDir + "/ITKImageProcessing/Montage/2209p230908A", "a3.osc_r1c0", {0.0f, 523.0f, 0.0f}},
                                                        {UnitTest::ITKImageProcessingDataDir + "/ITKImageProcessing/Montage/2209p230908A", "a3.osc_r1c1", {537.0f, 523.0f, 0.0f}}};

  // -----------------------------------------------------------------------------
  void importRGBImage(const DataContainerArray::Pointer& dca, const ImageTupleType& info)
  {
    Observer obs;

    QString filename = std::get<0>(info);
    const QString& dataContainerName = std::get<1>(info);
    FloatVec3Type origin = std::get<2>(info);

    filename = filename + "/" + dataContainerName + ".png";

    DataArrayPath dap(dataContainerName, k_ScanData, k_ImageName);
    ITKImageReader::Pointer reader = ITKImageReader::New();
    reader->setDataContainerArray(dca);
    reader->setFileName(filename);
    reader->setDataContainerName(dap);
    reader->setCellAttributeMatrixName(k_ScanData);
    reader->setImageDataArrayName(k_ImageName);
    QObject::connect(reader.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), &obs, SLOT(processPipelineMessage(const AbstractMessage::Pointer&)));

    reader->execute();
    int32_t err = reader->getErrorCode();
    DREAM3D_REQUIRED(err, >=, 0)

    SetOriginResolutionImageGeom::Pointer originUpdate = SetOriginResolutionImageGeom::New();
    originUpdate->setDataContainerArray(dca);
    originUpdate->setDataContainerName(dap);
    originUpdate->setChangeOrigin(true);
    originUpdate->setOrigin(origin);
    QObject::connect(originUpdate.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), &obs, SLOT(processPipelineMessage(const AbstractMessage::Pointer&)));

    originUpdate->execute();
    err = originUpdate->getErrorCode();
    DREAM3D_REQUIRED(err, >=, 0)
  }

  // -----------------------------------------------------------------------------
  DataContainerArray::Pointer ImportImages()
  {
    DataContainerArray::Pointer dca = DataContainerArray::New();
    for(const auto& file : k_InputRGBImages)
    {
      importRGBImage(dca, file);
    }
    return dca;
  }

  // -----------------------------------------------------------------------------
  void executeCalcDewarp(const DataContainerArray::Pointer& dca)
  {
    Observer obs;

    CalcDewarpParameters::Pointer dewarp = CalcDewarpParameters::New();
    dewarp->setDataContainerArray(dca);
    dewarp->setMontageName(k_MontageName);
    dewarp->setMaxIterations(1000);
    dewarp->setDelta(5);
    dewarp->setFractionalTolerance(1E-5);
    dewarp->setSpecifyInitialSimplex(false);
    dewarp->setAttributeMatrixName(k_ScanData);
    dewarp->setIPFColorsArrayName(k_ImageName);
    dewarp->setTransformDCName(k_DewarpTransformContainerName);
    dewarp->setTransformMatrixName(k_TransformMatrix);
    dewarp->setTransformArrayName(k_TransformArray);
    QObject::connect(dewarp.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), &obs, SLOT(processPipelineMessage(const AbstractMessage::Pointer&)));

    dewarp->execute();
    int32_t err = dewarp->getErrorCode();
    DREAM3D_REQUIRED(err, >=, 0)
  }

  // -----------------------------------------------------------------------------
  void executeTest()
  {
    Observer obs;
    // Import our 2x2 montage starting with a pre-existing set of IPF Colors
    DataContainerArray::Pointer dca = ImportImages();
    DREAM3D_REQUIRE_VALID_POINTER(dca.get())

    // Manually Create a "Grid Montage"
    GridMontage::Pointer gridMontage = GridMontage::New(k_MontageName, k_Rows, k_Cols);
    GridTileIndex gridIndex = gridMontage->getTileIndex(0, 0);
    gridMontage->setDataContainer(gridIndex, dca->getDataContainer(std::get<1>(k_InputRGBImages[0])));

    gridIndex = gridMontage->getTileIndex(0, 1);
    gridMontage->setDataContainer(gridIndex, dca->getDataContainer(std::get<1>(k_InputRGBImages[1])));

    gridIndex = gridMontage->getTileIndex(1, 0);
    gridMontage->setDataContainer(gridIndex, dca->getDataContainer(std::get<1>(k_InputRGBImages[2])));

    gridIndex = gridMontage->getTileIndex(1, 1);
    gridMontage->setDataContainer(gridIndex, dca->getDataContainer(std::get<1>(k_InputRGBImages[3])));

    dca->addOrReplaceMontage(gridMontage);

    // Write out the .dream3d file for debugging in ParaView
    DataContainerWriter::Pointer writer = DataContainerWriter::New();
    writer->setDataContainerArray(dca);
    writer->setOutputFile(UnitTest::TestTempDir + "/" + k_OutputFile);
    writer->setWriteXdmfFile(true);
    QObject::connect(writer.get(), SIGNAL(messageGenerated(const AbstractMessage::Pointer&)), &obs, SLOT(processPipelineMessage(const AbstractMessage::Pointer&)));

    writer->execute();
    int32_t err = writer->getErrorCode();
    DREAM3D_REQUIRED(err, >=, 0)

    // Calculate the Dewarping Parameters
    executeCalcDewarp(dca);

    // Write out the .dream3d file for debugging in ParaView
    writer->execute();
    err = writer->getErrorCode();
    DREAM3D_REQUIRED(err, >=, 0)
  }

  // -----------------------------------------------------------------------------
  void operator()()
  {
    std::cout << "---------------- EdaxEbsdMontageTest ---------------------" << std::endl;
    int err = EXIT_SUCCESS;
    DREAM3D_REGISTER_TEST(executeTest())
  }

private:
  AbstractFilter::Pointer m_ApplyDewarpParameters;
  DataContainerArray::Pointer m_ImageReaderDca;
};
