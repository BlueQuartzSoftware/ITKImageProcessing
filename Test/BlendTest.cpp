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

#include <QtCore/QCoreApplication>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
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
#include <map>
#include <cmath>
#ifndef M_PI
#define M_PI (3.141592653)
#endif

class BlendTest
{
  using Cell_T = size_t;
  using GrayScaleColor = uint8_t;
  using RGBColor = std::tuple<uint8_t, uint8_t, uint8_t>;
  using Image_T = GrayScaleColor;
  using PixelCoord = std::pair<int64_t, int64_t>;
  using Image = std::map<PixelCoord, Image_T>;
  using ImageGrid = std::map<std::pair<Cell_T, Cell_T>, Image>;
  using ImageArray = std::vector<std::vector<Image_T>>;

  template <class DataType> using DataPair = std::vector<std::pair<DataType, DataType>>;

  const QChar m_rowChar = 'R';
  const QChar m_colChar = 'C';
  const QString m_filtName = "Blend";
  const QString m_dataAAName = "ValueArray";
  const QString m_XAAName = "X Coords";
  const QString m_YAAName = "Y Coords";
  const QString m_AMName = "Image";
  const QString m_geoName = "Image";
  const QString m_outDCName = "Blend Data";
  const QString m_outAMName = "Transform Matrix";
  const QString m_outAAName = "Transform";
  AbstractFilter::Pointer m_blendFilter;
  ImageGrid m_originalImages{};
  ImageGrid m_distortedImages{};

  const uint8_t m_data[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  static const int m_tileRows = 2;
  static const int m_tileColumns = 2;

  static const int64_t m_resWidth = 10;
  static const int64_t m_resHeight = 10;

  static const int m_d = 1;
  static const int m_maxIterations = 10000;

  static const int m_thetaDegrees = 90;
  static constexpr double m_n = 0.0;
  static constexpr double m_m = 1.0;
  static constexpr double m_theta = M_PI * m_thetaDegrees / 180;
  static constexpr double m_errTolerance = 3.0;

  static constexpr float m_overlapPercentage = 0.25f;
  const int64_t m_x_overlapDimension = static_cast<int64_t>(roundf(m_resWidth * m_overlapPercentage));
  const int64_t m_y_overlapDimension = static_cast<int64_t>(roundf(m_resHeight * m_overlapPercentage));

  static constexpr double m_lowTolerance = 1E-2;
  static constexpr double m_highTolerance = 1E-2;

  static const int m_arrLength = 8;
  const std::vector<double> m_identity{0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0};
  const std::vector<double> m_translation{m_n, 1.0, 0.0, 0.0, m_m, 0.0, 1.0, 0.0};
  const std::vector<double> m_rotation{
      0.0, cos(m_theta), -sin(m_theta), 0.0, 0.0, sin(m_theta), cos(m_theta), 0.0
  };
  const std::vector<double> m_rotation_uv{
      0, cos(m_theta), -sin(m_theta), 1.0, 0, sin(m_theta), cos(m_theta), 1.0
  };
  const std::vector<double> m_poly{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
  const std::vector<double> m_a{m_poly};

  const QString m_i{"1.0; 2.0; 3.0; 4.0; 5.0; 6.0; 7.0; 8.0"};

  template <class T> DataPair<T> make_product(T listOneSize, T listTwoSize) const
  {
    DataPair<T> product{};
    for(T listOneIndex = 0; listOneIndex < listOneSize; listOneIndex++)
    {
      for(T listTwoIndex = 0; listTwoIndex < listTwoSize; listTwoIndex++)
      {
        product.push_back(std::make_pair(listTwoIndex, listOneIndex));
      }
    }
    return product;
  }

  Image Transform(Image image, std::vector<double> a) const
  {
    const double tolerance = 0.05;
    const double lastXIndex = m_resWidth - 1 + tolerance;
    const double lastYIndex = m_resHeight - 1 + tolerance;
    const size_t coeff_len = static_cast<size_t>(a.size() / 2);
    const int d = static_cast<int>(round(sqrt(coeff_len)));
    const DataPair<int> i_j = make_product<int>(d, d);

    const double x_trans = (m_resWidth - 1) / 2.0;
    const double y_trans = (m_resHeight - 1) / 2.0;
    double x = 0;
    double y = 0;
    double u_v = 0;
    double term = 0;

    std::pair<int, int> eachIJ{};

    Image distortedImage{};
    for(const auto& pixel : image)
    {
      x = x_trans;
      y = y_trans;
      for(size_t idx = 0; idx < a.size(); ++idx)
      {
        eachIJ = i_j[idx - (idx >= i_j.size() ? i_j.size() : 0)];

        u_v = pow((pixel.first.first - x_trans), eachIJ.first) *
              pow((pixel.first.second - y_trans), eachIJ.second);

        term = u_v * a[idx];
        idx < coeff_len ? x += term : y += term;
      }

      // This check effectively "clips" data
      if(x >= -tolerance && x <= lastXIndex && y >= -tolerance && y <= lastYIndex)
      {
        distortedImage[std::make_pair(round(x), round(y))] = image[pixel.first];
      }
    }
    return distortedImage;
  }

  bool CompareTransform(const DoubleArrayType::Pointer& transform) const
  {
    double error = 0;
    for(size_t coeffIdx = 0; coeffIdx < transform->size(); ++coeffIdx)
    {
      error += abs(transform->getValue(coeffIdx) - m_a[coeffIdx]);
    }
    qDebug() << "Error: " << error;
    return error > m_errTolerance;
  }

  void SetUp()
  {
    DataPair<int> tiles{make_product<int>(m_tileRows, m_tileColumns)};
    DataPair<Image_T> pixels{make_product<Image_T>(m_resWidth, m_resHeight)};

    for(const auto& eachTile : tiles)
    {

      std::vector<uint8_t> data(pixels.size());
      for(size_t eachIndex = 0; eachIndex < pixels.size(); eachIndex++)
      {
        data[eachIndex] = m_data[rand() % 10];
      }
      size_t pixelIndex = 0;
      Image img{};
      for(const auto& eachPixel : pixels)
      {
        img[eachPixel] = data[pixelIndex++];
      }
      m_originalImages[eachTile] = img;
    }

    for(auto& eachImage : m_originalImages)
    {
      // Skip the top-left image
      if(eachImage.first.first == 0 && eachImage.first.second == 0)
      {
        continue;
      }

      size_t r = eachImage.first.first;
      size_t c = eachImage.first.second;
      auto upperImage{m_originalImages.find(std::make_pair(r - 1, c))};
      auto lefterImage{m_originalImages.find(std::make_pair(r, c - 1))};

      // Overlap Data
      for(auto& eachPixel : eachImage.second)
      {
        int64_t x = eachPixel.first.first;
        int64_t y = eachPixel.first.second;
        // Overlap the data if inside the horizontal overlap region
        if(x < m_x_overlapDimension && lefterImage != m_originalImages.end())
        {
          eachPixel.second = lefterImage->second[std::make_pair(m_resWidth - m_x_overlapDimension + x, y)];
        }
        // Overlap the data if inside the vertical overlap region
        if(y >= m_resHeight - m_y_overlapDimension && upperImage != m_originalImages.end())
        {
          eachPixel.second = upperImage->second[std::make_pair(x, m_y_overlapDimension + y - m_resHeight)];
        }
      }
    }

    for(const auto& eachImage : m_originalImages)
    {
      m_distortedImages[eachImage.first] = Transform(eachImage.second, m_a);
    }

    QStringList chosenDataContainers;
    DataContainerArrayShPtr dca = DataContainerArray::New();
    for(const auto& eachImage : m_distortedImages)
    {
      Image img = eachImage.second;
      size_t sz = img.size();

      UInt8ArrayType::Pointer imageData = UInt8ArrayType::CreateArray(sz, m_dataAAName);
      Int64ArrayType::Pointer xPixels = Int64ArrayType::CreateArray(sz, m_XAAName);
      Int64ArrayType::Pointer yPixels = Int64ArrayType::CreateArray(sz, m_YAAName);
      size_t pixelIdx = 0;
      for(const auto& eachPixel : img)
      {
        imageData->setValue(pixelIdx, eachPixel.second);
        xPixels->setValue(pixelIdx, eachPixel.first.first);
        yPixels->setValue(pixelIdx, eachPixel.first.second);
        pixelIdx++;
      }

      // Set up the attribute matrix to hold the data arrays
      AttributeMatrixShPtr imageAM = AttributeMatrix::New({sz}, m_AMName, AttributeMatrix::Type::Cell);
      imageAM->addOrReplaceAttributeArray(imageData);
      imageAM->addOrReplaceAttributeArray(xPixels);
      imageAM->addOrReplaceAttributeArray(yPixels);

      // Set the up the geometry for the data container
      ImageGeom::Pointer imgGeo = ImageGeom::CreateGeometry(m_geoName);
      imgGeo->setOrigin(0.0f, 0.0f, 0.0f);
      imgGeo->setDimensions(m_resWidth, m_resHeight, 0.0f);

      // Set up the data container to hold the attribute matrix
      QString gridCoordsAsString = m_rowChar + QString::number(eachImage.first.first) + m_colChar + QString::number(eachImage.first.second);
      DataContainerShPtr imageDC = DataContainer::New(gridCoordsAsString);
      imageDC->setGeometry(imgGeo);
      imageDC->addOrReplaceAttributeMatrix(imageAM);

      chosenDataContainers.push_back(gridCoordsAsString);
      dca->addOrReplaceDataContainer(imageDC);
    }

    m_blendFilter->setProperty("ChosenDataContainers", QVariant(chosenDataContainers));
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

    // Assign the data container array to the Blend filter
    m_blendFilter->setDataContainerArray(dca);
  }

  int RunTest()
  {
    m_blendFilter->execute();

    DREAM3D_REQUIRE_EQUAL(m_blendFilter->getErrorCode(), 0)
    DoubleArrayType::Pointer transform = m_blendFilter->getDataContainerArray()->getDataContainer(m_outDCName)->getAttributeMatrix(m_outAMName)->getAttributeArrayAs<DoubleArrayType>(m_outAAName);
    bool outOfTolerance = CompareTransform(transform);
    DREAM3D_REQUIRE_EQUAL(outOfTolerance, false)

    return EXIT_SUCCESS;
  }

public:
  BlendTest(const BlendTest&) = delete;            // Copy Constructor
  BlendTest(BlendTest&&) = delete;                 // Move Constructor
  BlendTest& operator=(const BlendTest&) = delete; // Copy Assignment
  BlendTest& operator=(BlendTest&&) = delete;      // Move Assignment

  BlendTest()
  {
    IFilterFactory::Pointer blendFactory = FilterManager::Instance()->getFactoryFromClassName(m_filtName);
    DREAM3D_REQUIRE(blendFactory.get() != nullptr);

    m_blendFilter = blendFactory->create();
    DREAM3D_REQUIRE(m_blendFilter.get() != nullptr);
  }
  ~BlendTest() = default;

  void operator()()
  {
    int err = EXIT_SUCCESS;
    SetUp();
    DREAM3D_REGISTER_TEST(RunTest());
  }
};
