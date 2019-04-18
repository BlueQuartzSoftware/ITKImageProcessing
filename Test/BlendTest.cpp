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
#include <cmath>
#include <map>
#ifndef M_PI
#define M_PI (3.141592653f)
#endif

class BlendTest
{
  using Cell_T = size_t;
  using GrayScaleColor = uint8_t;
  using RGBColor = std::tuple<uint8_t, uint8_t, uint8_t>;
  using Image_T = GrayScaleColor;
  using PixelCoord = std::pair<size_t, size_t>;
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
  AbstractFilter::Pointer m_blendFilter;
  ImageGrid m_originalImages{};
  ImageGrid m_distortedImages{};

  const int m_data[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  static const int m_tileRows = 2;
  static const int m_tileColumns = 2;

  static const size_t m_res_width = 10;
  static const size_t m_res_height = 10;

  static const int m_d = 1;
  static const int m_max_iterations = 10;

  static const int m_theta_degrees = 90;
  static constexpr float m_n = 0.0f;
  static constexpr float m_m = 1.0f;
  static constexpr float m_theta = M_PI * m_theta_degrees / 180;

  static constexpr float m_overlap_percentage = 0.25f;
  const int m_x_overlapDimension = roundf(m_res_width * m_overlap_percentage);
  const int m_y_overlapDimension = roundf(m_res_height * m_overlap_percentage);
  const float m_a_stepSize = 0.1;

  static const int m_arrLength = 8;
  const std::vector<float> m_identity{0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0};
  const std::vector<float> m_translation{m_n, 1.0, 0.0, 0.0, m_m, 0.0, 1.0, 0.0};
  const std::vector<float> m_rotation{
      0.0, cos(m_theta), -sin(m_theta), 0.0, 0.0, sin(m_theta), cos(m_theta), 0.0,
  };
  const std::vector<float> m_rotation_uv{
      0, cos(m_theta), -sin(m_theta), 1.0, 0, sin(m_theta), cos(m_theta), 1.0,
  };
  const std::vector<float> m_trial{0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0};

  const std::vector<float> m_a{m_rotation};

  std::vector<int> GenData(int numElements) const
  {
    std::vector<int> data{};
    for(int eachIndex = 0; eachIndex < numElements; eachIndex++)
    {
      data.push_back(m_data[rand() % 10]);
    }
    return data;
  }

  template <class T> DataPair<T> make_product(size_t listOneSize, size_t listTwoSize) const
  {
    DataPair<T> product{};
    for(int listOneIndex = 0; listOneIndex < listOneSize; listOneIndex++)
    {
      for(int listTwoIndex = 0; listTwoIndex < listTwoSize; listTwoIndex++)
      {
        product.push_back(std::make_pair(listTwoIndex, listOneIndex));
      }
    }
    return product;
  }

  Image Transform(Image image, std::vector<float> a) const
  {
    const float tolerance = 0.05f;
    const float lastXIndex = m_res_width - 1 + tolerance;
    const float lastYIndex = m_res_height - 1 + tolerance;
    const int coeff_len = a.size() / 2;
    const int d = roundf(sqrt(coeff_len));
    const DataPair<int> i_j = make_product<int>(d, d);

    const float x_trans = (m_res_width - 1) / 2.0f;
    const float y_trans = (m_res_height - 1) / 2.0f;
    float x = 0;
    float y = 0;
    float u_v = 0;
    float term = 0;

    std::pair<int, int> eachIJ{};

    Image distortedImage{};
    for(const auto& pixel : image)
    {
      x = x_trans;
      y = y_trans;
      for(int idx = 0; idx < a.size(); ++idx)
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
        distortedImage[std::make_pair(roundf(x), roundf(y))] = image[pixel.first];
      }
    }
    return distortedImage;
  }

  template <class T> QString GridCoordsToName(std::pair<T, T> coords) const
  {
    return m_rowChar + QString::number(coords.first) + m_colChar + QString::number(coords.second);
  }

  template <class T> std::pair<T, T> NameToGridCoords(QString name) const
  {
    int cLength = name.size() - name.indexOf(m_colChar) - 1;
    T r = static_cast<T>(name.midRef(name.indexOf(m_rowChar) + 1, name.size() - cLength - 2).toULong());
    T c = static_cast<T>(name.rightRef(cLength).toULong());

    return std::make_pair(r, c);
  }

  DataContainerArrayShPtr ImageGridToDataContainerArray(const ImageGrid& imageGrid) const
  {
    DataContainerArrayShPtr dca = DataContainerArray::New();
    for(const auto& eachImage : imageGrid)
    {
      Image img = eachImage.second;
      size_t sz = img.size();

      UInt8ArrayType::Pointer imageData = UInt8ArrayType::CreateArray(sz, m_dataAAName);
      FloatArrayType::Pointer xPixels = FloatArrayType::CreateArray(sz, m_XAAName);
      FloatArrayType::Pointer yPixels = FloatArrayType::CreateArray(sz, m_YAAName);
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
      imgGeo->setDimensions(m_res_width, m_res_height, 0.0f);

      // Set up the data container to hold the attribute matrix
      QString gridCoordsAsString = GridCoordsToName(eachImage.first);
      DataContainerShPtr imageDC = DataContainer::New(gridCoordsAsString);
      imageDC->setGeometry(imgGeo);
      imageDC->addOrReplaceAttributeMatrix(imageAM);

      // Add the DC to the data container array
      dca->addOrReplaceDataContainer(imageDC);
    }

    return dca;
  }

  ImageGrid DataContainerArrayToImageGrid(const DataContainerArrayShPtr dca) const
  {
    ImageGrid images{};
    for(const auto& eachDC : dca->getDataContainers())
    {
      AttributeMatrixShPtr am = eachDC->getAttributeMatrix(m_AMName);
      UInt8ArrayType::Pointer dataAA = am->getAttributeArrayAs<UInt8ArrayType>(m_dataAAName);
      FloatArrayType::Pointer xAA = am->getAttributeArrayAs<FloatArrayType>(m_XAAName);
      FloatArrayType::Pointer yAA = am->getAttributeArrayAs<FloatArrayType>(m_YAAName);

      Image img{};
      for(size_t pxlIdx = 0; pxlIdx < m_res_width * m_res_height; ++pxlIdx)
      {
        img[std::make_pair(xAA->getValue(pxlIdx), yAA->getValue(pxlIdx))] = dataAA->getValue(pxlIdx);
      }

      images[NameToGridCoords<Cell_T>(eachDC->getName())] = img;
    }
    return images;
  }

  ImageArray ImageToArray(const Image& image) const
  {
    ImageArray imgArray(m_res_height, std::vector<Image_T>(m_res_width, 0));

    for(const auto& eachPixel : image)
    {
      // Use nearest-neighbor sampling to convert raw x, y pixels into integers
      // Iterate through all of the pixels in the image
      // When converting to array, pixel origin comes from lower-left
      // corner - and need translated to origin in upper-left corner
      imgArray[m_res_height - roundf(eachPixel.first.second) - 1][roundf(eachPixel.first.first)] = eachPixel.second;
    }

    return imgArray;
  }

  void Print(const ImageGrid& imageGrid) const
  {
    QString stream;

    for(int tileRowIdx = 0; tileRowIdx < m_tileRows; ++tileRowIdx)
    {
      std::vector<ImageArray> imageRow{};
      for(const auto& eachImage : imageGrid)
      {
        if(eachImage.first.first == tileRowIdx)
        {
          imageRow.push_back(ImageToArray(eachImage.second));
        }
      }

      for(int row_idx = 0; row_idx < m_res_height; ++row_idx)
      {
        for(const auto& eachImage : imageRow)
        {
          for(const auto& eachCol : eachImage[row_idx])
          {
            stream += QString::number(eachCol) + " ";
          }
          stream += "  ";
        }
        stream += "\n";
      }
      stream += "\n";
    }
    qDebug().noquote() << stream;
  }

  // TODO
  int CompareImages(const ImageGrid& blendedImages) const
  {
    int failedImages = 0;
    return failedImages;
  }

  void SetUp()
  {
    DataPair<int> tiles{make_product<int>(m_tileRows, m_tileColumns)};
    DataPair<Image_T> pixels{make_product<Image_T>(m_res_width, m_res_height)};

    qDebug() << "Generating images...";
    for(const auto& eachTile : tiles)
    {
      std::vector<int> data{GenData(pixels.size())};
      int pixelIndex = 0;
      Image img{};
      for(const auto& eachPixel : pixels)
      {
        img[eachPixel] = data[pixelIndex++];
      }
      m_originalImages[eachTile] = img;
    }

    qDebug() << "Generating overlaps...";
    for(auto& eachImage : m_originalImages)
    {
      // Skip the top-left image
      if(eachImage.first.first == 0 && eachImage.first.second == 0)
      {
        continue;
      }

      int r = eachImage.first.first;
      int c = eachImage.first.second;
      auto upperImage{m_originalImages.find(std::make_pair(r - 1, c))};
      auto lefterImage{m_originalImages.find(std::make_pair(r, c - 1))};

      // Overlap Data
      for(auto& eachPixel : eachImage.second)
      {
        int x = eachPixel.first.first;
        int y = eachPixel.first.second;
        // Overlap the data if inside the horizontal overlap region
        if(x < m_x_overlapDimension && lefterImage != m_originalImages.end())
        {
          eachPixel.second = lefterImage->second[std::make_pair(m_res_width - m_x_overlapDimension + x, y)];
        }
        // Overlap the data if inside the vertical overlap region
        if(y >= m_res_height - m_y_overlapDimension && upperImage != m_originalImages.end())
        {
          eachPixel.second = upperImage->second[std::make_pair(x, m_y_overlapDimension + y - m_res_height)];
        }
      }
    }

    qDebug() << "Distorting original images...";
    for(const auto& eachImage : m_originalImages)
    {
      m_distortedImages[eachImage.first] = Transform(eachImage.second, m_a);
    }

    qDebug() << "Setting up Blend filter properties...";
    m_blendFilter->setProperty("MaxIterations", QVariant(m_max_iterations));
    m_blendFilter->setProperty("Degree", QVariant(m_d));
    m_blendFilter->setProperty("OverlapMethod", QVariant(0));
    m_blendFilter->setProperty("OverlapPercentage", QVariant(m_overlap_percentage));
    m_blendFilter->setProperty("DataAttributeArrayName", QVariant(m_dataAAName));
    m_blendFilter->setProperty("AttributeMatrixName", QVariant(m_AMName));
    m_blendFilter->setProperty("XAttributeArrayName", QVariant(m_XAAName));
    m_blendFilter->setProperty("YAttributeArrayName", QVariant(m_YAAName));

    // Assign the data container array to the Blend filter
    qDebug() << "Assigning images to the Blend filter...";
    m_blendFilter->setDataContainerArray(ImageGridToDataContainerArray(m_distortedImages));
  }

  int RunTest()
  {
    qDebug() << "Attempting to blend...\n";
    // Run the filter's execute method
    m_blendFilter->execute();

    // Get the blended images from the data container array of the filter
    ImageGrid blendedImages{DataContainerArrayToImageGrid(m_blendFilter->getDataContainerArray())};

    // Check that the algorithm converged and the compare the
    // reverted image data to the original images
    int failedImages = CompareImages(blendedImages);
    int errorCode = m_blendFilter->getErrorCode();

    DREAM3D_REQUIRE_EQUAL(failedImages + errorCode, 0)

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
