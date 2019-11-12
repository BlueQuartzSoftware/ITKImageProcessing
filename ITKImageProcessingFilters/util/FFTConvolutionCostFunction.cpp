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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
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

#include "FFTConvolutionCostFunction.h"

#include <algorithm>
#include <limits>

#include "itkExtractImageFilter.h"

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include "tbb/queuing_mutex.h"
#endif

#include "SIMPLib/Montages/GridMontage.h"
#include "SIMPLib/Utilities/ParallelData2DAlgorithm.h"
#include "SIMPLib/Utilities/ParallelTaskAlgorithm.h"

using MutexType = tbb::queuing_mutex;
using ScopedLockType = MutexType::scoped_lock;

struct RegionBounds
{
  int64_t leftBound = 0;
  int64_t topBound = 0;
  int64_t rightBound = std::numeric_limits<int64_t>::max();
  int64_t bottomBound = std::numeric_limits<int64_t>::max();
};

/**
 * @class FFTConvolutionCostFunction FFTConvolutionCostFunction.h ITKImageProcessingFilters/util/FFTConvolutionCostFunction.h
 * @brief The FFTImageInitializer class is for running the FFTConvolutionCostFunction
 * in parallel on a target DataArray.
 */
class FFTImageInitializer
{
public:
  static const uint8_t IMAGE_DIMENSIONS = 2;
  using PixelCoord = itk::Index<IMAGE_DIMENSIONS>;
  using InputImage = itk::Image<PixelValue_T, IMAGE_DIMENSIONS>;
  using DataArrayType = DataArray<Grayscale_T>;

  /**
   * @brief Constructor
   * @param image
   * @param width
   * @param dataArray
   */
  FFTImageInitializer(const InputImage::Pointer& image, size_t width, size_t height, size_t xOffset, size_t yOffset, const DataArrayType::Pointer& dataArray)
  : m_Image(image)
  , m_Width(width)
  , m_Height(height)
  , m_XOffset(xOffset)
  , m_YOffset(yOffset)
  , m_DataArray(dataArray)
  , m_Comps(dataArray->getNumberOfComponents())
  {
  }

  /**
   * @brief Sets the image's pixel at the specified position based on the DataArray value.
   * @param pxlWidthIds
   * @param pxlHeightIdx
   */
  void setPixel(size_t pxlWidthIdx, size_t pxlHeightIdx) const
  {
    // Get the pixel index from the current pxlWidthIdx and pxlHeightIdx
    size_t pxlIdx = ((pxlWidthIdx + m_XOffset) + (pxlHeightIdx + m_YOffset) * m_Width) * m_Comps;
    PixelCoord idx;
    idx[0] = pxlWidthIdx;
    idx[1] = pxlHeightIdx;
    m_Image->SetPixel(idx, m_DataArray->getValue(pxlIdx));
  }

  /**
   * @brief Function operator to set the pixel value for items over a 2D range.
   * @param range
   */
  void operator()(const SIMPLRange2D& range) const
  {
    for(size_t y = range.minRow(); y < range.maxRow(); y++)
    {
      for(size_t x = range.minCol(); x < range.maxCol(); x++)
      {
        setPixel(x, y);
      }
    }
  }

private:
  InputImage::Pointer m_Image;
  size_t m_Width;
  size_t m_Height;
  size_t m_XOffset = 0;
  size_t m_YOffset = 0;
  DataArrayType::Pointer m_DataArray;
  size_t m_Comps;
};

/**
 * @brief
 */
class FFTImageOverlapGenerator
{
  static const uint8_t IMAGE_DIMENSIONS = 2;
  using PixelCoord = itk::Index<IMAGE_DIMENSIONS>;
  using InputImage = itk::Image<PixelValue_T, IMAGE_DIMENSIONS>;
  using ParametersType = itk::SingleValuedCostFunction::ParametersType;

public:
  /**
   * @brief Constructor
   * @param image
   * @param offset
   * @param parameters
   */
  FFTImageOverlapGenerator(const InputImage::Pointer& baseImg, const InputImage::Pointer& image, const PixelCoord& offset, size_t imageDim_x, size_t imageDim_y, size_t degree,
                           const ParametersType& parameters)
  : m_BaseImg(baseImg)
  , m_Image(image)
  , m_Offset(offset)
  , m_ImageDim_x(imageDim_x)
  , m_ImageDim_y(imageDim_y)
  , m_Degree(degree)
  , m_Parameters(parameters)
  {
  }

  /**
   * @brief Checks and returns if the base image contains the given PixelCoord.
   * @param index
   * @return
   */
  bool baseImageContainsIndex(const PixelCoord& index) const
  {
    const InputImage::RegionType& baseRegion = m_BaseImg->GetRequestedRegion();
    const PixelCoord& baseIndex = baseRegion.GetIndex();
    const auto& baseSize = baseRegion.GetSize();

    // Check edge cases for height / width
    for(size_t i = 0; i < 2; i++)
    {
      if(index[i] < baseIndex[i])
      {
        return false;
      }
      if(index[i] >= baseIndex[i] + baseSize[i])
      {
        return false;
      }
    }

    return true;
  }

  /**
   * @brief Calculate the old pixel position from the given x and y values.
   * @param x
   * @param y
   * @return
   */
  PixelCoord calculateOldPixelIndex(size_t x, size_t y) const
  {
    const double x_trans = (m_ImageDim_x - 1) / 2.0;
    const double y_trans = (m_ImageDim_y - 1) / 2.0;
    std::array<double, 2> newPrime{x + m_Offset[0] - x_trans, y + m_Offset[1] - y_trans};
    const double newXPrime = newPrime[0];
    const double newYPrime = newPrime[1];

    // old' = Ei(Ej(aij * x^i * y^j)
    double oldXPrime = 0.0;
    double oldYPrime = 0.0;
    const size_t yInit = (m_Degree * m_Degree) + (2 * m_Degree + 1);
    for(size_t i = 0; i <= m_Degree; i++)
    {
      for(size_t j = 0; j <= m_Degree; j++)
      {
        const double xyParam = std::pow(newXPrime, i) * std::pow(newYPrime, j);
        const size_t xOffset = i * (m_Degree + 1) + j;
        const size_t yOffset = xOffset + yInit;
        oldXPrime += m_Parameters[xOffset] * xyParam;
        oldYPrime += m_Parameters[yOffset] * xyParam;
      }
    }

    const int64_t oldXUnbound = static_cast<int64_t>(round(oldXPrime + x_trans));
    const int64_t oldYUnbound = static_cast<int64_t>(round(oldYPrime + y_trans));
    PixelCoord oldPixel{oldXUnbound, oldYUnbound};
    return oldPixel;
  }

  /**
   * @brief Function operator to set the pixel value for items over a 2D range.
   * @param range
   */
  void operator()(const SIMPLRange2D& range) const
  {
    for(size_t y = range.minRow(); y < range.maxRow(); y++)
    {
      for(size_t x = range.minCol(); x < range.maxCol(); x++)
      {
        PixelCoord newIndex;
        newIndex[0] = x;
        newIndex[1] = y;
        PixelValue_T pixel{0};
        const PixelCoord& oldIndex = calculateOldPixelIndex(x, y);
        if(baseImageContainsIndex(oldIndex))
        {
          pixel = m_BaseImg->GetPixel(oldIndex);
        }
        m_Image->SetPixel(newIndex, pixel);
      }
    }
  }

private:
  InputImage::Pointer m_BaseImg;
  InputImage::Pointer m_Image;
  PixelCoord m_Offset;
  size_t m_ImageDim_x;
  size_t m_ImageDim_y;
  ParametersType m_Parameters;
  size_t m_Degree;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::Initialize(const GridMontageShPtr& montage, int degree, const IntVec2Type& overlapAmt, const DataContainerArrayShPtr& dca, const QString& amName,
                                            const QString& daName)
{
  m_Montage = montage;
  m_Degree = degree;

  // Populate the m_IJ based on the degree selected
#if 0
  for(int listOneIndex = 0; listOneIndex <= m_Degree; listOneIndex++)
  {
    for(int listTwoIndex = 0; listTwoIndex <= m_Degree; listTwoIndex++)
    {
      m_IJ.push_back(std::make_pair(listTwoIndex, listOneIndex));
    }
  }
#endif

  m_ImageGrid.clear();

  calculateImageDim(montage);
  m_OverlapXAmt = static_cast<size_t>(overlapAmt[0]);
  m_OverlapYAmt = static_cast<size_t>(overlapAmt[1]);

  const size_t numRows = montage->getRowCount();
  const size_t numCols = montage->getColumnCount();

  // Populate and assign each image to m_imageGrid
  ParallelTaskAlgorithm taskAlg;
  for(size_t row = 0; row < numRows; row++)
  {
    for(size_t col = 0; col < numCols; col++)
    {
      taskAlg.execute(std::bind(&FFTConvolutionCostFunction::InitializeDataContainer, this, montage, row, col, amName, daName));
    }
  }
  taskAlg.wait();

  // Populate crop overlaps
  //initializeCropOverlaps(montage, amName, daName);

  // Populate m_overlaps
  m_Overlaps.clear();
  for(const auto& image : m_ImageGrid)
  {
    taskAlg.execute(std::bind(&FFTConvolutionCostFunction::InitializeCellOverlaps, this, image));
  }
  taskAlg.wait();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::calculateImageDim(const GridMontageShPtr& montage)
{
  size_t x = montage->getColumnCount() > 1 ? 1 : 0;
  size_t y = montage->getRowCount() > 1 ? 1 : 0;
  {
    GridTileIndex xIndex = montage->getTileIndex(0, x);
    DataContainer::Pointer dc = montage->getDataContainer(xIndex);
    ImageGeom::Pointer geom = dc->getGeometryAs<ImageGeom>();
    m_ImageDim_x = geom->getDimensions()[0];
  }
  {
    GridTileIndex yIndex = montage->getTileIndex(y, 0);
    DataContainer::Pointer dc = montage->getDataContainer(yIndex);
    ImageGeom::Pointer geom = dc->getGeometryAs<ImageGeom>();
    m_ImageDim_y = geom->getDimensions()[1];
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::PixelTypei FFTConvolutionCostFunction::calculateNew2OldPixel(int64_t row, int64_t col, const ParametersType& parameters, double x_trans, double y_trans) const
{
  using PixelTyped = std::array<double, 2>;

  const PixelTyped newPrime = {col - x_trans, row - y_trans};
  const double newXPrime = newPrime[0];
  const double newYPrime = newPrime[1];
  const double newXPrimeSqr = newPrime[0] * newPrime[0];
  const double newYPrimeSqr = newPrime[1] * newPrime[1];

  // old' = Ei(Ej(aij * x^i * y^j)
  std::array<double, 2> oldPrime = {0.0, 0.0};
  const size_t yInit = (m_Degree * m_Degree) + (2 * m_Degree + 1);
  for(size_t i = 0; i <= m_Degree; i++)
  {
    for(size_t j = 0; j <= m_Degree; j++)
    {
      const double xyParam = std::pow(newXPrime, i) * std::pow(newYPrime, j);
      const size_t xOffset = i * (m_Degree + 1) + j;
      const size_t yOffset = xOffset + yInit;
      oldPrime[0] = oldPrime[0] + parameters[xOffset] * xyParam;
      oldPrime[1] = oldPrime[1] + parameters[yOffset] * xyParam;
    }
  }

  const int64_t oldXUnbound = static_cast<int64_t>(round(oldPrime[0] + x_trans));
  const int64_t oldYUnbound = static_cast<int64_t>(round(oldPrime[1] + y_trans));
  PixelTypei oldPixel{oldXUnbound, oldYUnbound};
  return oldPixel;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::InitializeDataContainer(const GridMontageShPtr& montage, size_t row, size_t column, const QString& amName, const QString& daName)
{
  static MutexType mutex;

  GridTileIndex index = montage->getTileIndex(row, column);
  DataContainer::Pointer dc = montage->getDataContainer(index);
  AttributeMatrix::Pointer am = dc->getAttributeMatrix(amName);
  DataArray<Grayscale_T>::Pointer da = am->getAttributeArrayAs<DataArray<Grayscale_T>>(daName);
  size_t comps = da->getNumberOfComponents();
  ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
  SizeVec3Type dims = imageGeom->getDimensions();
  size_t width = dims.getX();
  size_t height = dims.getY();
  size_t xOrigin = 0;
  size_t yOrigin = 0;

  if(row == 0)
  {
    height = std::min(height, static_cast<size_t>(std::round(m_ImageDim_y)));
    yOrigin = dims.getY() - height;
  }
  if(column == 0)
  {
    width = std::min(width, static_cast<size_t>(std::round(m_ImageDim_x)));
    xOrigin = dims.getX() - width;
  }

  InputImage::SizeType imageSize;
  imageSize[0] = width;
  imageSize[1] = height;

  PixelCoord imageOrigin;
  imageOrigin[0] = 0;
  imageOrigin[1] = 0;

  InputImage::Pointer itkImage = InputImage::New();
  itkImage->SetRegions(InputImage::RegionType(imageOrigin, imageSize));
  itkImage->Allocate();

  // A colored image could be used in a Fourier Transform as discussed in this paper:
  // https://ieeexplore.ieee.org/document/723451
  // NOTE Could this be parallelized?
  ParallelData2DAlgorithm dataAlg;
  dataAlg.setRange(0, 0, height, width);
  //dataAlg.execute(FFTImageInitializer(itkImage, dims.getX(), dims.getY(), xOrigin, yOrigin, da));
  dataAlg.execute(FFTImageInitializer(itkImage, width, height, xOrigin, yOrigin, da));

  GridKey imageKey = std::make_pair(row, column);
  ScopedLockType lock(mutex);
  m_ImageGrid[imageKey] = itkImage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::InitializeCellOverlaps(const ImageGrid::value_type& image)
{
  static MutexType mutex;
  const size_t width = image.second->GetBufferedRegion().GetSize()[0];
  const size_t height = image.second->GetBufferedRegion().GetSize()[1];

  PixelCoord imageOrigin;
  InputImage::SizeType imageSize;
  PixelCoord kernelOrigin;
  InputImage::SizeType kernelSize;

  //GridKey gridKey = image.first;
  auto rightImage{m_ImageGrid.find(std::make_pair(image.first.first, image.first.second + 1))};
  auto bottomImage{m_ImageGrid.find(std::make_pair(image.first.first + 1, image.first.second))};
  if(rightImage != m_ImageGrid.end())
  {
    // NOTE The height dimension for horizontally overlapping images should be the same
    size_t overlapHeight = std::min(height, static_cast<size_t>(m_ImageDim_y));
    const size_t& overlapDim = m_OverlapXAmt;
    imageOrigin[0] = static_cast<itk::IndexValueType>(width - overlapDim);
    imageOrigin[1] = 0; // Push to the bottom when the tile is taller than m_ImageDim_y
    imageSize[0] = overlapDim;
    imageSize[1] = overlapHeight;

    kernelOrigin[0] = 0;
    kernelOrigin[1] = 0;
    kernelSize[0] = overlapDim;
    kernelSize[1] = overlapHeight;

    GridPair position = std::make_pair(image.first, rightImage->first);
    RegionPair region = std::make_pair(InputImage::RegionType(imageOrigin, imageSize), InputImage::RegionType(kernelOrigin, kernelSize));
    OverlapPair overlap = std::make_pair(position, region);
    ScopedLockType lock(mutex);
    m_Overlaps.push_back(overlap);
  }
  if(bottomImage != m_ImageGrid.end())
  {
    // NOTE The width dimension for vertically overlapping images should be the same
    size_t overlapWidth = std::min(width, static_cast<size_t>(m_ImageDim_x));
    const size_t& overlapDim = m_OverlapYAmt;
    imageOrigin[0] = 0; // Push to the right when the tile is wider than m_ImageDim_x
    imageOrigin[1] = static_cast<itk::IndexValueType>(height - overlapDim);
    imageSize[0] = overlapWidth;
    imageSize[1] = overlapDim;

    kernelOrigin[0] = 0;
    kernelOrigin[1] = 0;
    kernelSize[0] = overlapWidth;
    kernelSize[1] = overlapDim;

    GridPair position = std::make_pair(image.first, bottomImage->first);
    RegionPair region = std::make_pair(InputImage::RegionType(imageOrigin, imageSize), InputImage::RegionType(kernelOrigin, kernelSize));
    OverlapPair overlap = std::make_pair(position, region);
    ScopedLockType lock(mutex);
    m_Overlaps.push_back(overlap);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::initializeCropOverlaps(const GridMontageShPtr& montage, const QString& amName, const QString& daName)
{
  const size_t numCols = montage->getColumnCount();
  const size_t numRows = montage->getRowCount();

  for(size_t col = 0; col < numCols; col++)
  {
    initializeCropRowImg(montage, col, amName, daName);
  }
  for(size_t row = 0; row < numRows; row++)
  {
    initializeCropColImg(montage, row, amName, daName);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::initializeCropColImg(const GridMontageShPtr& montage, size_t row, const QString& amName, const QString& daName)
{
  GridTileIndex index = montage->getTileIndex(row, 0);
  DataContainer::Pointer dc = montage->getDataContainer(index);
  AttributeMatrix::Pointer am = dc->getAttributeMatrix(amName);
  DataArray<Grayscale_T>::Pointer da = am->getAttributeArrayAs<DataArray<Grayscale_T>>(daName);
  size_t comps = da->getNumberOfComponents();
  ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
  SizeVec3Type dims = imageGeom->getDimensions();
  size_t xOrigin = 0;
  size_t yOrigin = 0;

  size_t height = std::min(dims.getY(), static_cast<size_t>(std::round(m_ImageDim_y)));
  size_t minWidth = static_cast<size_t>(std::round(m_ImageDim_x));
  size_t width = std::max(dims.getX() - minWidth, m_OverlapXAmt);

  InputImage::SizeType imageSize;
  imageSize[0] = width;
  imageSize[1] = height;

  PixelCoord imageOrigin;
  imageOrigin[0] = 0;
  imageOrigin[1] = 0;

  InputImage::Pointer itkImage = InputImage::New();
  itkImage->SetRegions(InputImage::RegionType(imageOrigin, imageSize));
  itkImage->Allocate();

  // A colored image could be used in a Fourier Transform as discussed in this paper:
  // https://ieeexplore.ieee.org/document/723451
  // NOTE Could this be parallelized?
  ParallelData2DAlgorithm dataAlg;
  dataAlg.setRange(0, 0, height, width);
  //dataAlg.execute(FFTImageInitializer(itkImage, dims.getX(), dims.getY(), xOrigin, yOrigin, da));
  dataAlg.execute(FFTImageInitializer(itkImage, width, height, xOrigin, yOrigin, da));

  //GridKey imageKey = std::make_pair(row, montage->getColumnCount());
  GridKey imageKey = std::make_pair(montage->getColumnCount(), row);
  m_ImageGrid[imageKey] = itkImage;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::initializeCropRowImg(const GridMontageShPtr& montage, size_t col, const QString& amName, const QString& daName)
{
  GridTileIndex index = montage->getTileIndex(0, col);
  DataContainer::Pointer dc = montage->getDataContainer(index);
  AttributeMatrix::Pointer am = dc->getAttributeMatrix(amName);
  DataArray<Grayscale_T>::Pointer da = am->getAttributeArrayAs<DataArray<Grayscale_T>>(daName);
  size_t comps = da->getNumberOfComponents();
  ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
  SizeVec3Type dims = imageGeom->getDimensions();
  size_t xOrigin = 0;
  size_t yOrigin = 0;

  size_t minHeight = static_cast<size_t>(std::round(m_ImageDim_y));
  size_t height = std::max(dims.getY() - minHeight, m_OverlapYAmt);
  
  size_t width = std::min(dims.getX(), static_cast<size_t>(std::round(m_ImageDim_x)));

  InputImage::SizeType imageSize;
  imageSize[0] = width;
  imageSize[1] = height;

  PixelCoord imageOrigin;
  imageOrigin[0] = 0;
  imageOrigin[1] = 0;

  InputImage::Pointer itkImage = InputImage::New();
  itkImage->SetRegions(InputImage::RegionType(imageOrigin, imageSize));
  itkImage->Allocate();

  // A colored image could be used in a Fourier Transform as discussed in this paper:
  // https://ieeexplore.ieee.org/document/723451
  // NOTE Could this be parallelized?
  ParallelData2DAlgorithm dataAlg;
  dataAlg.setRange(0, 0, height, width);
  //dataAlg.execute(FFTImageInitializer(itkImage, dims.getX(), dims.getY(), xOrigin, yOrigin, da));
  dataAlg.execute(FFTImageInitializer(itkImage, width, height, xOrigin, yOrigin, da));

  //GridKey imageKey = std::make_pair(montage->getRowCount(), col);
  GridKey imageKey = std::make_pair(col, montage->getRowCount());
  m_ImageGrid[imageKey] = itkImage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImageGeom::Pointer getImageGeomFromMontage(const GridMontageShPtr& montage, size_t row, size_t column)
{
  if(nullptr == montage)
  {
    return nullptr;
  }
  if(row >= montage->getRowCount() || column >= montage->getColumnCount())
  {
    return nullptr;
  }

  GridTileIndex index = montage->getTileIndex(row, column);
  DataContainer::Pointer dc = montage->getDataContainer(index);
  return dc->getGeometryAs<ImageGeom>();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::GetDerivative(const ParametersType&, DerivativeType&) const
{
  throw std::exception();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint32_t FFTConvolutionCostFunction::GetNumberOfParameters() const
{
  return static_cast<uint32_t>(2 * (m_Degree * m_Degree + 2 * m_Degree + 1));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::MeasureType FFTConvolutionCostFunction::GetValue(const ParametersType& parameters) const
{
  CropMap cropMap;
  ParallelTaskAlgorithm taskAlg;

  // Apply the Transform to each image in the image grid
  for(const auto& eachImage : m_ImageGrid) // Parallelize this
  {
    taskAlg.execute(std::bind(&FFTConvolutionCostFunction::checkTransformation, this, parameters, eachImage, std::ref(cropMap)));
  }
  taskAlg.wait();

  if(!isCropMapValid(cropMap))
  {
    std::cout << "Invalid Parameters";
    // This is optimizing for the greatest return value, thus invalid inputs require a smallest return
    return std::numeric_limits<MeasureType>::min();
  }

  // Crop overlap portions
  OverlapPairs copyOverlaps = m_Overlaps;
  for(auto& overlap : copyOverlaps)
  {
    cropOverlap(overlap, cropMap);
  }

  MeasureType residual = 0.0;
  // Find the FFT Convolution and accumulate the maximum value from each overlap
  for(const auto& overlap : copyOverlaps) // Parallelize this
  {
    findFFTConvolutionAndMaxValue(overlap, parameters, residual);
  }

  std::cout << "Parameters: " << parameters;
  std::cout << "Result " << residual;

  // The value to maximize is the square of the sum of the maximum value of the fft convolution
  MeasureType result = residual * residual;
  return result;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool FFTConvolutionCostFunction::isCropMapValid(const CropMap& cropMap) const
{
  for(auto cropValue : cropMap)
  {
    const GridKey& index = cropValue.first;
    const RegionBounds& bounds = cropValue.second;
    if(!isRegionValid(index, bounds))
    {
      return false;
    }
  }
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool FFTConvolutionCostFunction::isRegionValid(const GridKey& index, const RegionBounds& bounds) const
{
  size_t minWidth = m_ImageDim_x - m_OverlapXAmt;
  size_t minHeight = m_ImageDim_y - m_OverlapYAmt;

  size_t xOffset = 0;
  size_t yOffset = 0;

  // Calculate offsets
  bool isLeft = (index.first == 0);
  bool isTop = (index.second == 0);
  if(isLeft)
  {
    const auto& img = m_ImageGrid.at(index);
    const auto& size = img->GetRequestedRegion().GetSize();
    xOffset = std::max(0.0, size[0] - m_ImageDim_x);
  }
  if(isTop)
  {
    const auto& img = m_ImageGrid.at(index);
    const auto& size = img->GetRequestedRegion().GetSize();
    yOffset = std::max(0.0, size[1] - m_ImageDim_y);
  }

  // Check bounds
  if(bounds.topBound >= (m_OverlapYAmt + yOffset) || bounds.topBound < 0)
  {
    return false;
  }
  if(bounds.bottomBound <= (minHeight + yOffset) || bounds.bottomBound >= (m_ImageDim_y + yOffset))
  {
    return false;
  }
  if(bounds.leftBound >= (m_OverlapXAmt + xOffset) || bounds.leftBound < 0)
  {
    return false;
  }
  if(bounds.rightBound <= (minWidth + xOffset) || bounds.rightBound >= (m_ImageDim_x + xOffset))
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::checkTransformation(const ParametersType& parameters, const ImageGrid::value_type& inputImage, CropMap& cropMap) const
{
  static MutexType cropMutex;
  static MutexType distortedMutex;
  ParallelTaskAlgorithm taskAlg;

  const double tolerance = 0.05;

  InputImage::RegionType bufferedRegion = inputImage.second->GetBufferedRegion();

  // Add bounds to the CropMap
  {
    auto origin = inputImage.second->GetOrigin();
    auto size = inputImage.second->GetRequestedRegion().GetSize();
    RegionBounds bounds;
    bounds.leftBound = origin[0];
    bounds.topBound = origin[1];
    bounds.rightBound = bounds.leftBound + size[0];
    bounds.bottomBound = bounds.topBound + size[1];
    ScopedLockType lock(cropMutex);
    cropMap[inputImage.first] = bounds;
  }

  // Iterate through the pixels in eachImage and apply the transform
  itk::ImageRegionIterator<InputImage> it(inputImage.second, bufferedRegion);
  for(it.GoToBegin(); !it.IsAtEnd(); ++it) // Parallelize this
  {
    taskAlg.execute(std::bind(&FFTConvolutionCostFunction::applyTransformationPixel, this, tolerance, parameters, inputImage.second, bufferedRegion, it, inputImage.first, std::ref(cropMap)));
  }
  taskAlg.wait();

  updateCropMapBounds(inputImage.second, cropMap);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::applyTransformationPixel(double tolerance, const ParametersType& parameters, const InputImage::Pointer& inputImage, const InputImage::RegionType& bufferedRegion,
                                                          itk::ImageRegionIterator<InputImage> iter, const GridKey& gridKey, CropMap& cropMap) const
{
  const double lastXIndex = bufferedRegion.GetSize()[0] - 1 + tolerance;
  const double lastYIndex = bufferedRegion.GetSize()[1] - 1 + tolerance;

  const double x_trans = (m_ImageDim_x - 1) / 2.0;
  const double y_trans = (m_ImageDim_y - 1) / 2.0;

  PixelCoord pixel = iter.GetIndex();
  // Dave's method uses an m_IJ matrix described above
  // and so a different method of grabbing the appropriate index from the m_IJ
  // will be needed if using that static array
  calculatePixelCoordinates(parameters, inputImage, gridKey, cropMap, pixel, x_trans, y_trans, tolerance, lastXIndex, lastYIndex);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::calculatePixelCoordinates(const ParametersType& parameters, const InputImage::Pointer& inputImage, const GridKey& gridKey, CropMap& cropMap,
                                                           const PixelCoord& newPixel, double x_trans, double y_trans, double tolerance, double lastXIndex, double lastYIndex) const
{
  static MutexType mutex;

  auto inputSize = inputImage->GetBufferedRegion().GetSize();
  const int64_t width = inputSize[0];
  const int64_t height = inputSize[1];
  auto inputIndex = inputImage->GetBufferedRegion().GetIndex();
  const int64_t minX = inputIndex[0];
  const int64_t minY = inputIndex[1];

  PixelTypei oldPixeli = calculateNew2OldPixel(newPixel[0], newPixel[1], parameters, x_trans + minX, y_trans + minY);

  PixelCoord oldPixel;
  oldPixel[0] = oldPixeli[0];
  oldPixel[1] = oldPixeli[1];

#if 1
  bool isValidX = oldPixel[0] >= 0 && oldPixel[0] <= width - 1;
  bool isValidY = oldPixel[1] >= 0 && oldPixel[1] <= height - 1;
  bool isValid = isValidX && isValidY;

  // Do not set invalid pixels
  if(!isValid)
  {
    adjustCropMap(newPixel, inputImage, gridKey, cropMap);
    return;
  }
#else
  if(oldPixel[0] < 0)
  {
    oldPixel[0] += width;
  }
  else if(oldPixel[0] >= width)
  {
    oldPixel[0] -= width;
  }
  if(oldPixel[1] < 0)
  {
    oldPixel[1] += height;
  }
  else if(oldPixel[1] >= height)
  {
    oldPixel[1] -= height;
  }
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::adjustCropMap(const PixelCoord& pixel, const InputImage::Pointer& inputImage, const GridKey& gridKey, CropMap& cropMap) const
{
  auto inputSize = inputImage->GetBufferedRegion().GetSize();
  const int64_t width = inputSize[0];
  const int64_t height = inputSize[1];
  auto inputIndex = inputImage->GetBufferedRegion().GetIndex();
  const int64_t minX = inputIndex[0];
  const int64_t minY = inputIndex[1];

  const int64_t maxX = minX + width;
  const int64_t maxY = minY + height;

  const int64_t distTop = pixel[1] - minY;
  const int64_t distBot = maxY - pixel[1];
  const int64_t distLeft = pixel[0] - minX;
  const int64_t distRight = maxX - pixel[0];

  // Use a reference so we only override the target value instead of all four
  // Use min/max value in case the bounds might already be closer than the current pixel
  RegionBounds region = cropMap[gridKey];
  if((distTop < distBot) && (distTop < distLeft) && (distTop < distRight))
  {
    region.topBound = std::max(region.topBound, static_cast<int64_t>(pixel[1]));
  }
  else if((distRight < distLeft) && (distRight < distTop) && (distRight < distBot))
  {
    region.rightBound = std::min(region.rightBound, static_cast<int64_t>(pixel[0]));
  }
  else if((distLeft < distRight) && (distLeft < distTop) && (distLeft < distBot))
  {
    region.leftBound = std::max(region.leftBound, static_cast<int64_t>(pixel[0]));
  }
  else if(distBot < distTop)
  {
    region.bottomBound = std::min(region.bottomBound, static_cast<int64_t>(pixel[1]));
  }

  cropMap[gridKey] = region;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::updateCropMapBounds(const InputImage::Pointer& inputImage, CropMap& cropMap) const
{
  auto inputSize = inputImage->GetBufferedRegion().GetSize();
  const int64_t width = inputSize[0];
  const int64_t height = inputSize[1];
  auto inputIndex = inputImage->GetBufferedRegion().GetIndex();
  const int64_t initX = inputIndex[0];
  const int64_t initY = inputIndex[1];

  for(auto& mapIter : cropMap)
  {
    auto key = mapIter.first;
    RegionBounds region = mapIter.second;
    region.rightBound = std::min(region.rightBound, initX + width);
    region.bottomBound = std::min(region.bottomBound, initY + height);
    cropMap[key] = region;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::cropOverlap(OverlapPair& overlap, CropMap& cropMap) const
{
  const GridPair& gridPair = overlap.first;

  const GridKey& gridKey1 = gridPair.first;
  const GridKey& gridKey2 = gridPair.second;

  // cropDistortedGrid(gridKey1, distortedGrid, cropMap);
  // cropDistortedGrid(gridKey2, distortedGrid, cropMap);

  if(gridKey1.first == gridKey2.first)
  {
    cropOverlapHorizontal(overlap, cropMap);
  }
  else
  {
    cropOverlapVertical(overlap, cropMap);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::cropOverlapHorizontal(OverlapPair& overlap, const CropMap& cropMap) const
{
  using Size = itk::Size<IMAGE_DIMENSIONS>;

  GridPair gridPair = overlap.first;
  RegionPair& regionPair = overlap.second;

  const InputImage::RegionType& distortedRegion1 = m_ImageGrid.at(gridPair.first)->GetRequestedRegion();
  const InputImage::RegionType& distortedRegion2 = m_ImageGrid.at(gridPair.second)->GetRequestedRegion();

  // Convenience constants
  const PixelCoord& img1Origin = distortedRegion1.GetIndex();
  const Size& img1Size = distortedRegion1.GetSize();

  const PixelCoord& img2Origin = distortedRegion2.GetIndex();
  const Size& img2Size = distortedRegion2.GetSize();

  const RegionBounds& region1 = cropMap.at(gridPair.first);
  const RegionBounds& region2 = cropMap.at(gridPair.second);

  // Begin calculations
  const size_t upperOverlapWidth = std::max(static_cast<int64_t>(0), static_cast<int64_t>(m_OverlapXAmt) - static_cast<int64_t>((img1Size[0] - region1.rightBound)));
  const size_t lowerOverlapWidth = std::max(static_cast<int64_t>(0), static_cast<int64_t>(m_OverlapXAmt) - static_cast<int64_t>((region2.leftBound)));

  InputImage::RegionType leftRegion = regionPair.first;
  InputImage::RegionType rightRegion = regionPair.second;

  // Update first region
  {
    PixelCoord kernelOrigin = leftRegion.GetIndex();
    InputImage::SizeType kernelSize = leftRegion.GetSize();

    kernelOrigin[1] = region1.topBound;
    kernelSize[0] = upperOverlapWidth;
    kernelSize[1] = region1.bottomBound - region1.topBound;

    leftRegion = InputImage::RegionType(kernelOrigin, kernelSize);
  }
  // Update second region
  {
    PixelCoord kernelOrigin = rightRegion.GetIndex();
    InputImage::SizeType kernelSize = rightRegion.GetSize();

    kernelOrigin[0] = region2.leftBound;
    kernelOrigin[1] = region2.topBound;
    kernelSize[0] = lowerOverlapWidth;
    kernelSize[1] = region2.bottomBound - region2.topBound;

    rightRegion = InputImage::RegionType(kernelOrigin, kernelSize);
  }

  regionPair = std::make_pair(leftRegion, rightRegion);
  overlap = std::make_pair(gridPair, regionPair);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::cropOverlapVertical(OverlapPair& overlap, const CropMap& cropMap) const
{
  using Size = itk::Size<IMAGE_DIMENSIONS>;

  GridPair gridPair = overlap.first;
  RegionPair& regionPair = overlap.second;

  const InputImage::RegionType& distortedRegion1 = m_ImageGrid.at(gridPair.first)->GetRequestedRegion();
  const InputImage::RegionType& distortedRegion2 = m_ImageGrid.at(gridPair.second)->GetRequestedRegion();

  // Convenience constants
  const PixelCoord& img1Origin = distortedRegion1.GetIndex();
  const Size& img1Size = distortedRegion1.GetSize();

  const PixelCoord& img2Origin = distortedRegion2.GetIndex();
  const Size& img2Size = distortedRegion2.GetSize();

  const RegionBounds& region1 = cropMap.at(gridPair.first);
  const RegionBounds& region2 = cropMap.at(gridPair.second);

  // Begin calculations
  const size_t upperOverlapHeight = std::max(static_cast<int64_t>(0), static_cast<int64_t>(m_OverlapYAmt) - static_cast<int64_t>((img1Size[1] - region1.bottomBound)));
  const size_t lowerOverlapHeight = std::max(static_cast<int64_t>(0), static_cast<int64_t>(m_OverlapYAmt) - (region2.topBound));

  InputImage::RegionType topRegion = regionPair.first;
  InputImage::RegionType botRegion = regionPair.second;

  // Update first region
  {
    PixelCoord kernelOrigin = topRegion.GetIndex();
    InputImage::SizeType kernelSize = topRegion.GetSize();

    kernelOrigin[0] = region1.leftBound;
    kernelSize[0] = region1.rightBound - region1.leftBound;
    kernelSize[1] = upperOverlapHeight;

    topRegion = InputImage::RegionType(kernelOrigin, kernelSize);
  }
  // Update second region
  {
    PixelCoord kernelOrigin = botRegion.GetIndex();
    InputImage::SizeType kernelSize = botRegion.GetSize();

    kernelOrigin[0] = region2.leftBound;
    kernelOrigin[1] = region2.topBound;
    kernelSize[0] = region2.rightBound - region2.leftBound;
    kernelSize[1] = lowerOverlapHeight;

    botRegion = InputImage::RegionType(kernelOrigin, kernelSize);
  }

  regionPair = std::make_pair(topRegion, botRegion);
  overlap = std::make_pair(gridPair, regionPair);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::ImagePair FFTConvolutionCostFunction::createOverlapImages(const OverlapPair& overlap, const ParametersType& parameters) const
{
  // First image calculation
  InputImage::RegionType firstRegion = overlap.second.first;
  const InputImage::Pointer& firstBaseImg = m_ImageGrid.at(overlap.first.first);

  InputImage::Pointer firstOverlapImg = InputImage::New();
  firstOverlapImg->SetRegions({{0, 0}, firstRegion.GetSize()});
  firstOverlapImg->Allocate();

  auto index = firstRegion.GetIndex();
  ParallelData2DAlgorithm dataAlg;
  dataAlg.setRange(0, 0, firstRegion.GetSize()[1], firstRegion.GetSize()[0]);
  dataAlg.execute(FFTImageOverlapGenerator(firstBaseImg, firstOverlapImg, index, m_ImageDim_x, m_ImageDim_y, m_Degree, parameters));

  // Second image calculation
  InputImage::RegionType secondRegion = overlap.second.second;
  const InputImage::Pointer& secondBaseImg = m_ImageGrid.at(overlap.first.second);

  InputImage::Pointer secondOverlapImg = InputImage::New();
  secondOverlapImg->SetRegions({{0, 0}, secondRegion.GetSize()});
  secondOverlapImg->Allocate();

  index = secondRegion.GetIndex();
  dataAlg.setRange(0, 0, secondRegion.GetSize()[1], secondRegion.GetSize()[0]);
  dataAlg.execute(FFTImageOverlapGenerator(secondBaseImg, secondOverlapImg, index, m_ImageDim_x, m_ImageDim_y, m_Degree, parameters));

  return std::make_pair(firstOverlapImg, secondOverlapImg);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::findFFTConvolutionAndMaxValue(const OverlapPair& overlap, const ParametersType& parameters, MeasureType& residual) const
{
  static MutexType mutex;
  ImagePair overlapImgs = createOverlapImages(overlap, parameters);

  ConvolutionFilter::Pointer filter = ConvolutionFilter::New();
  filter->SetInput(overlapImgs.first);
  filter->SetKernelImage(overlapImgs.second);
  filter->Update();
  OutputImage::Pointer fftConvolve = filter->GetOutput();

  // Increment by the maximum value of the output of the fftConvolve
  // NOTE This methodology of getting the max element from the fftConvolve
  // output might require a deeper look
  PixelValue_T* bufferPtr = fftConvolve->GetBufferPointer();
  itk::SizeValueType bufferSize = fftConvolve->GetPixelContainer()->Size();
  MeasureType maxValue = *std::max_element(bufferPtr, bufferPtr + bufferSize);
  //ScopedLockType lock(mutex);
  residual += maxValue;

  std::cout << "Max Value: " << maxValue;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::ImageGrid FFTConvolutionCostFunction::getImageGrid() const
{
  return m_ImageGrid;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double FFTConvolutionCostFunction::getImageDimX() const
{
  return m_ImageDim_x;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double FFTConvolutionCostFunction::getImageDimY() const
{
  return m_ImageDim_y;
}
