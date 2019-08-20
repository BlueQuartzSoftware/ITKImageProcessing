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
  FFTImageInitializer(const InputImage::Pointer& image, size_t width, size_t xOffset, size_t yOffset, const DataArrayType::Pointer& dataArray)
  : m_Image(image)
  , m_Width(width)
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
  size_t m_XOffset = 0;
  size_t m_YOffset = 0;
  DataArrayType::Pointer m_DataArray;
  size_t m_Comps;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MultiParamCostFunction::Initialize(std::vector<double> mins)
{
  m_Mins = mins;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void MultiParamCostFunction::GetDerivative(const ParametersType&, DerivativeType&) const
{
  throw std::exception();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
uint32_t MultiParamCostFunction::GetNumberOfParameters() const
{
  return static_cast<uint32_t>(m_Mins.size());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MultiParamCostFunction::MeasureType MultiParamCostFunction::GetValue(const ParametersType& parameters) const
{
  MeasureType residual = 0.0;
  size_t numParams = parameters.size();
  for(size_t idx = 0; idx < numParams; ++idx)
  {
    double minValue = m_Mins[idx];
    double paramValue = parameters[idx];
    residual += (idx == numParams - 1) ? minValue : pow(paramValue - minValue, 2);
  }
  return residual;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::Initialize(const GridMontageShPtr& montage, int degree, float overlapPercentage,
                                            const DataContainerArrayShPtr& dca, const QString& amName, const QString& daName)
{
  m_Montage = montage;
  m_Degree = degree;

  // Populate the m_IJ based on the degree selected
  for(int listOneIndex = 0; listOneIndex <= m_Degree; listOneIndex++)
  {
    for(int listTwoIndex = 0; listTwoIndex <= m_Degree; listTwoIndex++)
    {
      m_IJ.push_back(std::make_pair(listTwoIndex, listOneIndex));
    }
  }

  m_ImageGrid.clear();

  calculateImageDim(montage);
  m_OverlapXAmt = m_ImageDim_x * overlapPercentage;
  m_OverlapYAmt = m_ImageDim_y * overlapPercentage;

  const size_t numRows = montage->getRowCount();
  const size_t numCols = montage->getColumnCount();
  
  // Populate and assign each image to m_imageGrid
  ParallelTaskAlgorithm taskAlg;
  for(size_t row = 0; row < numRows; row++)
  {
    for(size_t col = 0; col < numCols; col++)
    {
      std::function<void(void)> fn = std::bind(&FFTConvolutionCostFunction::InitializeDataContainer, this, montage, row, col, amName, daName);
      taskAlg.execute(fn);
    }
  }
  taskAlg.wait();

  // Populate m_overlaps
  m_Overlaps.clear();
#if 1
  for(const auto& image : m_ImageGrid)
  {
    std::function<void(void)> fn = std::bind(&FFTConvolutionCostFunction::InitializePercentageOverlaps, this, image, overlapPercentage);
    taskAlg.execute(fn);
  }
#else
  for(size_t row = 0; row < numRows; row++)
  {
    for(size_t col = 0; col < numCols; col++)
    {
      std::function<void(void)> fn = std::bind(&FFTConvolutionCostFunction::InitializePositionalOverlaps, this, montage, row, col);
      taskAlg.execute(fn);
    }
  }
#endif
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

  const PixelTyped newPrime = { col - x_trans, row - y_trans };
  const double newXPrime = newPrime[0];
  const double newYPrime = newPrime[1];
  const double newXPrimeSqr = newPrime[0] * newPrime[0];
  const double newYPrimeSqr = newPrime[1] * newPrime[1];

  std::array<double, 2> oldPrime;
#if 0
  // old' = (a0 * x') + (a1 * y') + (a2 * x'Sqr) + (a3 * y'Sqr) + (a4 * x'y') + (a5 * x'Sqry') + (a6 * x'y'Sqr)
  oldPrime[0] = parameters[0] * newPrime[0] + parameters[1] * newPrime[1] + parameters[2] * newXPrimeSqr + parameters[3] * newYPrimeSqr + parameters[4] * newPrime[0] * newPrime[1] +
    parameters[5] * newXPrimeSqr * newPrime[1] + parameters[6] * newPrime[0] * newYPrimeSqr;
  oldPrime[1] = parameters[7] * newPrime[0] + parameters[8] * newPrime[1] + parameters[9] * newXPrimeSqr + parameters[10] * newYPrimeSqr + parameters[11] * newPrime[0] * newPrime[1] +
    parameters[12] * newXPrimeSqr * newPrime[1] + parameters[13] * newPrime[0] * newYPrimeSqr;
#else
  // old' = (a0 * y') + (a1 * y'Sqr) + (a2 * x') + (a3 * x'y) + (a4 * x'y'Sqr)+ (a5 * x'Sqr) + (a6 * x'Sqry')
  oldPrime[0] = parameters[0] * newYPrime + parameters[1] * newYPrimeSqr + parameters[2] * newXPrime + parameters[3] * newXPrime * newYPrime + parameters[4] * newXPrime * newYPrimeSqr +
    parameters[5] * newXPrimeSqr + parameters[6] * newXPrimeSqr * newYPrime;
  oldPrime[1] = parameters[7] * newYPrime + parameters[8] * newYPrimeSqr + parameters[9] * newXPrime + parameters[10] * newXPrime * newYPrime + parameters[11] * newXPrime * newYPrimeSqr +
    parameters[12] * newXPrimeSqr + parameters[13] * newXPrimeSqr * newYPrime;
#endif

  const int64_t oldXUnbound = static_cast<int64_t>(round(oldPrime[0] + x_trans));
  const int64_t oldYUnbound = static_cast<int64_t>(round(oldPrime[1] + y_trans));
  PixelTypei oldPixel{ oldXUnbound, oldYUnbound };
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
  dataAlg.execute(FFTImageInitializer(itkImage, dims.getX(), xOrigin, yOrigin, da));

  GridKey imageKey = std::make_pair(row, column);
  ScopedLockType lock(mutex);
  m_ImageGrid[imageKey] = itkImage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::InitializePercentageOverlaps(const ImageGrid::value_type& image, float overlapPercentage)
{
  static MutexType mutex;
  const size_t width = image.second->GetBufferedRegion().GetSize()[0];
  const size_t height = image.second->GetBufferedRegion().GetSize()[1];
  size_t overlapDim;

  PixelCoord imageOrigin;
  InputImage::SizeType imageSize;
  PixelCoord kernelOrigin;
  InputImage::SizeType kernelSize;

  auto rightImage{m_ImageGrid.find(std::make_pair(image.first.first, image.first.second + 1))};
  auto bottomImage{m_ImageGrid.find(std::make_pair(image.first.first + 1, image.first.second))};
  if(rightImage != m_ImageGrid.end())
  {
    // NOTE The height dimension for horizontally overlapping images should be the same
    size_t overlapHeight = std::min(height, static_cast<size_t>(m_ImageDim_y));
    //size_t originY = std::max(static_cast<size_t>(0), static_cast<size_t>(height - m_ImageDim_y));
    size_t overlapDim = static_cast<size_t>(roundf(m_ImageDim_x * overlapPercentage));
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
    //size_t originX = std::max(static_cast<size_t>(0), static_cast<size_t>(width - m_ImageDim_x));
    overlapDim = static_cast<size_t>(roundf(m_ImageDim_y * overlapPercentage));
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
void FFTConvolutionCostFunction::InitializePositionalOverlaps(const GridMontageShPtr& montage, size_t row, size_t column)
{
  static MutexType mutex;

  ImageGeom::Pointer imageGeom = getImageGeomFromMontage(montage, row, column);
  SizeVec3Type dims = imageGeom->getDimensions();
  FloatVec3Type origin = imageGeom->getOrigin();
  size_t width = dims.getX();
  size_t height = dims.getY();

  const bool hasNextColumn = column < montage->getColumnCount() - 1;
  const bool hasNextRow = row < montage->getRowCount() - 1;

  if(hasNextColumn)
  {
    ImageGeom::Pointer nextGeom = getImageGeomFromMontage(montage, row, column + 1);
    FloatVec3Type nextOrigin = nextGeom->getOrigin();
    SizeVec3Type nextDims = nextGeom->getDimensions();

    const itk::IndexValueType overlapAmt = static_cast<itk::IndexValueType>((origin[0] + width) - nextOrigin[0]);
    const double top = std::min(origin.getY() + dims.getY(), nextOrigin.getY() + nextDims.getY());
    const double bot = std::max(origin.getY(), nextOrigin.getY());

    size_t overlapHeight = std::min(static_cast<size_t>(top - bot), static_cast<size_t>(m_ImageDim_y));

    PixelCoord imageOrigin;
    InputImage::SizeType imageSize;
    imageOrigin[0] = static_cast<itk::IndexValueType>(width - overlapAmt);
    imageOrigin[1] = (top - overlapHeight) - origin[1]; // Push to the bottom when the tile is taller than m_ImageDim_y
    imageSize[0] = overlapAmt;
    imageSize[1] = overlapHeight;

    PixelCoord kernelOrigin;
    InputImage::SizeType kernelSize;
    kernelOrigin[0] = 0;
    kernelOrigin[1] = bot - nextOrigin[1];
    kernelSize[0] = overlapAmt;
    kernelSize[1] = overlapHeight;

    // Add overlap
    GridPair position = std::make_pair(GridKey{ column, row }, GridKey{ column + 1, row });
    RegionPair region = std::make_pair(InputImage::RegionType(imageOrigin, imageSize), InputImage::RegionType(kernelOrigin, kernelSize));
    OverlapPair overlap = std::make_pair(position, region);
    ScopedLockType lock(mutex);
    m_Overlaps.push_back(overlap);
  }
  if(hasNextRow)
  {
    ImageGeom::Pointer nextGeom = getImageGeomFromMontage(montage, row + 1, column);
    FloatVec3Type nextOrigin = nextGeom->getOrigin();
    SizeVec3Type nextDims = nextGeom->getDimensions();

    const itk::IndexValueType overlapAmt = static_cast<itk::IndexValueType>((nextOrigin[1] + nextDims[1]) - origin[1]);
    const double left = std::max(origin[0], nextOrigin[0]);
    const double right = std::min(origin[0] + width, nextOrigin[0] + nextDims[0]);
    const double overlapWidth = std::min(static_cast<itk::IndexValueType>(right - left), static_cast<itk::IndexValueType>(m_ImageDim_x));

    PixelCoord imageOrigin;
    InputImage::SizeType imageSize;
    imageOrigin[0] = (right - overlapWidth) - origin[1]; // Push to the right when the tile is wider than m_ImageDim_x
    imageOrigin[1] = static_cast<itk::IndexValueType>(height - overlapAmt);
    imageSize[0] = overlapWidth;
    imageSize[1] = overlapAmt;

    PixelCoord kernelOrigin;
    InputImage::SizeType kernelSize;
    kernelOrigin[0] = (right - overlapWidth) - nextOrigin[0];
    kernelOrigin[1] = 0;
    kernelSize[0] = overlapWidth;
    kernelSize[1] = overlapAmt;

    // Add overlap
    GridPair position = std::make_pair(GridKey(column, row), GridKey(column, row + 1));
    RegionPair region = std::make_pair(InputImage::RegionType(imageOrigin, imageSize), InputImage::RegionType(kernelOrigin, kernelSize));
    OverlapPair overlap = std::make_pair(position, region);
    ScopedLockType lock(mutex);
    m_Overlaps.push_back(overlap);
  }
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
  return 14; // Locked for Degree = 2.  Not all values have corresponding parameters.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::MeasureType FFTConvolutionCostFunction::GetValue(const ParametersType& parameters) const
{
  ImageGrid distortedGrid;
  CropMap cropMap;
  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  // Apply the Transform to each image in the image grid
  for(const auto& eachImage : m_ImageGrid) // Parallelize this
  {
    fn = std::bind(&FFTConvolutionCostFunction::applyTransformation, this, parameters, eachImage, std::ref(distortedGrid), std::ref(cropMap));
    taskAlg.execute(fn);
  }
  taskAlg.wait();

  // Crop overlap portions
  OverlapPairs copyOverlaps = m_Overlaps;
  for(auto& overlap : copyOverlaps)
  {
    cropOverlap(overlap, distortedGrid, cropMap);
  }

  MeasureType residual = 0.0;
  // Find the FFT Convolution and accumulate the maximum value from each overlap
  for(const auto& overlap : copyOverlaps) // Parallelize this
  {
    findFFTConvolutionAndMaxValue(overlap, distortedGrid, residual);
  }

  // The value to minimize is the square of the sum of the maximum value of the fft convolution
  MeasureType result = sqrt(residual);
  return result;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::applyTransformation(const ParametersType& parameters, const ImageGrid::value_type& inputImage, ImageGrid& distortedGrid, CropMap& cropMap) const
{
  static MutexType cropMutex;
  static MutexType distortedMutex;
  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  const double tolerance = 0.05;

  InputImage::RegionType bufferedRegion = inputImage.second->GetBufferedRegion();

  typename InputImage::Pointer distortedImage = InputImage::New();
  distortedImage->SetRegions(bufferedRegion);
  distortedImage->Allocate();

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
    fn = std::bind(&FFTConvolutionCostFunction::applyTransformationPixel, this, tolerance, parameters, inputImage.second, distortedImage, bufferedRegion, it, inputImage.first, std::ref(cropMap));
    taskAlg.execute(fn);
  }
  taskAlg.wait();

  updateCropMapBounds(inputImage.second, cropMap);

  GridKey distortedKey = inputImage.first;
  ScopedLockType lock(distortedMutex);
  distortedGrid[distortedKey] = distortedImage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::applyTransformationPixel(double tolerance, const ParametersType& parameters, const InputImage::Pointer& inputImage, const InputImage::Pointer& distortedImage,
                                                          const InputImage::RegionType& bufferedRegion, itk::ImageRegionIterator<InputImage> iter, const GridKey& gridKey, CropMap& cropMap) const
{
  static MutexType mutex;
  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  const double lastXIndex = bufferedRegion.GetSize()[0] - 1 + tolerance;
  const double lastYIndex = bufferedRegion.GetSize()[1] - 1 + tolerance;

  const double x_trans = (m_ImageDim_x - 1) / 2.0;
  const double y_trans = (m_ImageDim_y - 1) / 2.0;

  PixelCoord pixel = iter.GetIndex();
  // Dave's method uses an m_IJ matrix described above
  // and so a different method of grabbing the appropriate index from the m_IJ
  // will be needed if using that static array
  calculatePixelCoordinates(parameters, inputImage, distortedImage, gridKey, cropMap, pixel, x_trans, y_trans, tolerance, lastXIndex, lastYIndex);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::calculatePixelCoordinates(const ParametersType& parameters, const InputImage::Pointer& inputImage, const InputImage::Pointer& distortedImage, const GridKey& gridKey, CropMap& cropMap, const PixelCoord& newPixel, double x_trans, double y_trans, double tolerance, double lastXIndex, double lastYIndex) const
{
  static MutexType mutex;

  auto inputSize = inputImage->GetBufferedRegion().GetSize();
  const int64_t width = inputSize[0];
  const int64_t height = inputSize[1];
  auto inputIndex = inputImage->GetBufferedRegion().GetIndex();
  const int64_t minX = inputIndex[0];
  const int64_t minY = inputIndex[1];

  //PixelTypei newPixeli{ newPixel[0], newPixel[1] };
  PixelTypei oldPixeli = calculateNew2OldPixel(newPixel[0], newPixel[1], parameters, x_trans + minX, y_trans + minY);

  PixelCoord oldPixel;
  oldPixel[0] = oldPixeli[0];
  oldPixel[1] = oldPixeli[1];

  //PixelCoord oldPixel;
  bool isValidX = oldPixel[0] >= 0 && oldPixel[0] <= width - 1;
  bool isValidY = oldPixel[1] >= 0 && oldPixel[1] <= height - 1;
  bool isValid = isValidX && isValidY;

  // Do not set invalid pixels
  if(!isValid)
  {
    adjustCropMap(newPixel, inputImage, gridKey, cropMap);
    return;
  }

  //if(oldPixel[0] >= minX -tolerance && oldPixel[1] >= minY -tolerance)
  //if(oldPixel[0] >= -tolerance && oldPixel[0] <= lastXIndex && oldPixel[1] >= -tolerance && oldPixel[1] <= lastYIndex)
  if(oldPixel[0] >= minX -tolerance && oldPixel[0] <= lastXIndex && oldPixel[1] >= minY -tolerance && oldPixel[1] <= lastYIndex)
  {
    // The value obtained with eachImage.second->GetPixel(eachPixel) could have
    // its "contrast" adjusted based on its radial location from the center of
    // the image at this step to compensate for error encountered from radial effects
    ScopedLockType lock(mutex);
    auto oldPixelValue = inputImage->GetPixel(oldPixel);
    distortedImage->SetPixel(newPixel, oldPixelValue);
  }
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
    region.topBound = std::max(region.topBound, pixel[1]);
  }
  else if((distRight < distLeft) && (distRight < distTop) && (distRight < distBot))
  {
    region.rightBound = std::min(region.rightBound, pixel[0]);
  }
  else if((distLeft < distRight) && (distLeft < distTop) && (distLeft < distBot))
  {
    region.leftBound = std::max(region.leftBound, pixel[0]);
  }
  else if(distBot < distTop)
  {
    region.bottomBound = std::min(region.bottomBound, pixel[1]);
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
void FFTConvolutionCostFunction::cropOverlap(OverlapPair& overlap, ImageGrid& distortedGrid, CropMap& cropMap) const
{
  GridPair gridPair = overlap.first;
  RegionPair& regionPair = overlap.second;

  cropDistortedGrid(gridPair.first, distortedGrid, cropMap);
  cropDistortedGrid(gridPair.second, distortedGrid, cropMap);

  GridKey gridKey1 = gridPair.first;
  GridKey gridKey2 = gridPair.second;

  if(gridKey1.first == gridKey2.first)
  {
    cropOverlapHorizontal(overlap, distortedGrid, cropMap);
  }
  else
  {
    cropOverlapVertical(overlap, distortedGrid, cropMap);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::cropDistortedGrid(const GridKey& gridKey, ImageGrid& distortedGrid, CropMap& cropMap) const
{
  using CropFilter = itk::ExtractImageFilter<InputImage, InputImage>;

  RegionBounds bounds = cropMap[gridKey];
  PixelCoord origin;
  InputImage::SizeType size;
  origin[0] = bounds.leftBound;
  origin[1] = bounds.topBound;
  size[0] = bounds.rightBound - bounds.leftBound;
  size[1] = bounds.bottomBound - bounds.topBound;
  InputImage::Pointer distortedIn = distortedGrid[gridKey];

  InputImage::RegionType region(origin, size);
  CropFilter::Pointer cropFilter = CropFilter::New();
  cropFilter->SetExtractionRegion(region);
  cropFilter->SetInput(distortedIn);
  cropFilter->SetDirectionCollapseToIdentity();
  cropFilter->Update();
  distortedGrid[gridKey] = cropFilter->GetOutput();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::cropOverlapHorizontal(OverlapPair& overlap, const ImageGrid& distortedGrid, const CropMap& cropMap) const
{
  using Size = itk::Size<IMAGE_DIMENSIONS>;

  GridPair gridPair = overlap.first;
  RegionPair& regionPair = overlap.second;

  InputImage::RegionType distortedRegion1 = distortedGrid.at(gridPair.first)->GetRequestedRegion();
  InputImage::RegionType distortedRegion2 = distortedGrid.at(gridPair.second)->GetRequestedRegion();

  // Convenience constants
  const PixelCoord img1Origin = distortedRegion1.GetIndex();
  const Size img1Size = distortedRegion1.GetSize();

  const PixelCoord img2Origin = distortedRegion2.GetIndex();
  const Size img2Size = distortedRegion2.GetSize();

  const RegionBounds& region1 = cropMap.at(gridPair.first);
  const RegionBounds& region2 = cropMap.at(gridPair.second);

  // Begin calculations
  const size_t upperOverlapWidth = m_OverlapXAmt - (img1Size[0] - region1.rightBound);
  const size_t lowerOverlapWidth = m_OverlapXAmt - (region2.leftBound);

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
void FFTConvolutionCostFunction::cropOverlapVertical(OverlapPair& overlap, const ImageGrid& distortedGrid, const CropMap& cropMap) const
{
  using Size = itk::Size<IMAGE_DIMENSIONS>;

  GridPair gridPair = overlap.first;
  RegionPair& regionPair = overlap.second;

  InputImage::RegionType distortedRegion1 = distortedGrid.at(gridPair.first)->GetRequestedRegion();
  InputImage::RegionType distortedRegion2 = distortedGrid.at(gridPair.second)->GetRequestedRegion();

  // Convenience constants
  const PixelCoord img1Origin = distortedRegion1.GetIndex();
  const Size img1Size = distortedRegion1.GetSize();

  const PixelCoord img2Origin = distortedRegion2.GetIndex();
  const Size img2Size = distortedRegion2.GetSize();

  const RegionBounds& region1 = cropMap.at(gridPair.first);
  const RegionBounds& region2 = cropMap.at(gridPair.second);

  // Begin calculations
  const size_t upperOverlapHeight = m_OverlapYAmt - (img1Size[1] - region1.bottomBound);
  const size_t lowerOverlapHeight = m_OverlapYAmt - (region2.topBound);

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
FFTConvolutionCostFunction::KernelImages FFTConvolutionCostFunction::calculateKernelImages(const OverlapPair& overlap, ImageGrid& distortedGrid) const
{
  using CropFilter = itk::ExtractImageFilter<InputImage, InputImage>;

  GridKey imageKey = overlap.first.first;
  InputImage::Pointer image = distortedGrid.at(imageKey);

  GridKey kernelKey = overlap.first.second;
  auto imageRegion = overlap.second.first;
  image->SetRequestedRegion(imageRegion);

  

  RegionBounds bounds = cropMap[gridKey];
  PixelCoord origin;
  InputImage::SizeType size;
  origin[0] = bounds.leftBound;
  origin[1] = bounds.topBound;
  size[0] = bounds.rightBound - bounds.leftBound;
  size[1] = bounds.bottomBound - bounds.topBound;
  InputImage::Pointer distortedIn = distortedGrid[gridKey];

  // Crop filter
  InputImage::RegionType region(origin, size);
  CropFilter::Pointer cropFilter = CropFilter::New();
  cropFilter->SetExtractionRegion(region);
  cropFilter->SetInput(distortedIn);
  cropFilter->SetDirectionCollapseToIdentity();
  cropFilter->Update();


  KernelImages output;
  return output;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::findFFTConvolutionAndMaxValue(const OverlapPair& overlap, ImageGrid& distortedGrid, MeasureType& residual) const
{
  static MutexType mutex;
  // Set the filter's image to the overlap region of the image
  GridKey imageKey = overlap.first.first;
  InputImage::Pointer image = distortedGrid.at(imageKey);
  Filter::Pointer filter = Filter::New();

  GridKey kernelKey = overlap.first.second;
  auto imageRegion = overlap.second.first;
  image->SetRequestedRegion(imageRegion);
  filter->SetInput(image);

  // Set the filter's kernel to the overlap region of the kernel
  InputImage::Pointer kernel = distortedGrid.at(kernelKey);
  kernel->SetRequestedRegion(overlap.second.second);
  filter->SetKernelImage(kernel);

  // Run the filter
  filter->Update();
  OutputImage::Pointer fftConvolve = filter->GetOutput();

  // NOTE It may be useful for debugging purposes to write the output image to a file here

  // Increment by the maximum value of the output of the fftConvolve
  // NOTE This methodology of getting the max element from the fftConvolve
  // output might require a deeper look
  PixelValue_T* bufferPtr = fftConvolve->GetBufferPointer();
  itk::SizeValueType bufferSize = fftConvolve->GetPixelContainer()->Size();
  MeasureType maxValue = *std::max_element(bufferPtr, bufferPtr + bufferSize);
  ScopedLockType lock(mutex);
  residual += maxValue;
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
