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

#include <QFile>
#include <QtCore/QCoreApplication>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Filtering/FilterPipeline.h"
#include "SIMPLib/Plugin/ISIMPLibPlugin.h"
#include "SIMPLib/Plugin/SIMPLibPluginLoader.h"
#include "SIMPLib/SIMPLib.h"

#include "SIMPLib/Filtering/QMetaObjectUtilities.h"

#include "SIMPLib/Geometry/ImageGeom.h"
#include "UnitTestSupport.hpp"

#include <array>
#include <cmath>
#include <map>
#ifndef M_PI
#define M_PI (3.141592653)
#endif

class BlendTest
{
  const QString m_filtName = "Blend";
  const QString m_ImageReaderClassName = "ITKImageReader";

  // NOTE This should change to read the path of the DREAM3D_SDK CMake variable
  const QString m_Image1Path = "C:\\Users\\nathan\\repos\\Super\\DREAM3D_SDK\\DREAM3D_Data\\Data\\Image\\R0C0.jpeg";
  const QString m_Image2Path = "C:\\Users\\nathan\\repos\\Super\\DREAM3D_SDK\\DREAM3D_Data\\Data\\Image\\R0C1.jpeg";
  const QString m_Image3Path = "C:\\Users\\nathan\\repos\\Super\\DREAM3D_SDK\\DREAM3D_Data\\Data\\Image\\R1C0.jpeg";
  const QString m_Image4Path = "C:\\Users\\nathan\\repos\\Super\\DREAM3D_SDK\\DREAM3D_Data\\Data\\Image\\R1C1.jpeg";
  const QString m_Image1Name = "R0C0";
  const QString m_Image2Name = "R0C1";
  const QString m_Image3Name = "R1C0";
  const QString m_Image4Name = "R1C1";
  const QString m_cellAMName = "CellData";
  const QString m_imageAAName = "ImageData";

  const QString m_dataAAName = m_imageAAName;
  const QString m_AMName = m_cellAMName;
  const QString m_XAAName = "X Coords";
  const QString m_YAAName = "Y Coords";
  const QString m_outDCName = "Blend Data";
  const QString m_outAMName = "Transform Matrix";
  const QString m_outAAName = "Transform";
  AbstractFilter::Pointer m_blendFilter;

  static const int m_d = 1;
  static const int m_maxIterations = 10000;

  static constexpr double m_errTolerance = 3.0;
  static constexpr float m_overlapPercentage = 0.25f;
  static constexpr double m_lowTolerance = 1E-2;
  static constexpr double m_highTolerance = 1E-2;

  const std::vector<double> m_a{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
  const QString m_i{"1.0; 2.0; 3.0; 4.0; 5.0; 6.0; 7.0; 8.0"};

  void ReadImage(AbstractFilter::Pointer readerFilter, const QString& imagePath, const QString& imageName,
                 DataArrayPath& dap)
  {
    dap.setDataContainerName(imageName);
    readerFilter->setProperty("FileName", QVariant(imagePath));
    readerFilter->setProperty("DataContainerName", QVariant::fromValue(dap));
    readerFilter->execute();
    DREAM3D_REQUIRE(readerFilter->getDataContainerArray()->doesDataContainerExist(imageName) == true)
  }

  int RunTest()
  {
    m_blendFilter->execute();

    DREAM3D_REQUIRE_EQUAL(m_blendFilter->getErrorCode(), 0)
    DoubleArrayType::Pointer transform = m_blendFilter->getDataContainerArray()->getDataContainer(m_outDCName)->getAttributeMatrix(m_outAMName)->getAttributeArrayAs<DoubleArrayType>(m_outAAName);

    double error = 0;
    for(size_t coeffIdx = 0; coeffIdx < transform->size(); ++coeffIdx)
    {
      error += abs(transform->getValue(coeffIdx) - m_a[coeffIdx]);
    }
    bool outOfTolerance = error > m_errTolerance;
    DREAM3D_REQUIRE_EQUAL(outOfTolerance, false)

    return EXIT_SUCCESS;
  }

public:
  BlendTest(const BlendTest&) = delete;            // Copy Constructor
  BlendTest(BlendTest&&) = delete;                 // Move Constructor
  BlendTest& operator=(const BlendTest&) = delete; // Copy Assignment
  BlendTest& operator=(BlendTest&&) = delete;      // Move Assignment

  ~BlendTest() = default;
  BlendTest()
  {
    IFilterFactory::Pointer blendFactory = FilterManager::Instance()->getFactoryFromClassName(m_filtName);
    DREAM3D_REQUIRE(blendFactory.get() != nullptr)

    m_blendFilter = blendFactory->create();
    DREAM3D_REQUIRE(m_blendFilter.get() != nullptr)
  }

  void operator()()
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REQUIRE_EQUAL(QFile::exists(m_Image1Path), true)
    DREAM3D_REQUIRE_EQUAL(QFile::exists(m_Image2Path), true)
    DREAM3D_REQUIRE_EQUAL(QFile::exists(m_Image3Path), true)
    DREAM3D_REQUIRE_EQUAL(QFile::exists(m_Image4Path), true)

    AbstractFilter::Pointer readerFilter = FilterManager::Instance()->getFactoryFromClassName(m_ImageReaderClassName)->create();

    DataArrayPath dap;
    dap.setDataArrayName(m_imageAAName);
    dap.setAttributeMatrixName(m_cellAMName);

    ReadImage(readerFilter, m_Image1Path, m_Image1Name, dap);
    ReadImage(readerFilter, m_Image2Path, m_Image2Name, dap);
    ReadImage(readerFilter, m_Image3Path, m_Image3Name, dap);
    ReadImage(readerFilter, m_Image4Path, m_Image4Name, dap);

    QStringList chosenDataContainers;
    for(const auto& eachDC : readerFilter->getDataContainerArray()->getDataContainers())
    {
      chosenDataContainers.push_back(eachDC->getName());
    }

    m_blendFilter->setProperty("MaxIterations", QVariant(m_maxIterations));
    m_blendFilter->setProperty("Degree", QVariant(m_d));
    m_blendFilter->setProperty("InitialSimplexGuess", QVariant(m_i));
    m_blendFilter->setProperty("OverlapPercentage", QVariant(m_overlapPercentage));
    m_blendFilter->setProperty("LowTolerance", QVariant(m_lowTolerance));
    m_blendFilter->setProperty("HighTolerance", QVariant(m_highTolerance));
    m_blendFilter->setProperty("DataAttributeArrayName", QVariant(m_dataAAName));
    m_blendFilter->setProperty("AttributeMatrixName", QVariant(m_AMName));
    m_blendFilter->setProperty("XAttributeArrayName", QVariant(m_XAAName));
    m_blendFilter->setProperty("YAttributeArrayName", QVariant(m_YAAName));
    m_blendFilter->setProperty("ChosenDataContainers", QVariant(chosenDataContainers));
    m_blendFilter->setDataContainerArray(readerFilter->getDataContainerArray());

    DREAM3D_REGISTER_TEST(RunTest())
  }
};
