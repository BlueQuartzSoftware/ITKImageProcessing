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

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include "tbb/queuing_mutex.h"
#endif

#include "SIMPLib/Montages/GridMontage.h"
#include "SIMPLib/Utilities/ParallelData2DAlgorithm.h"
#include "SIMPLib/Utilities/ParallelTaskAlgorithm.h"

using MutexType = tbb::queuing_mutex;
using ScopedLockType = MutexType::scoped_lock;

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
  FFTImageInitializer(const InputImage::Pointer& image, size_t width, const DataArrayType::Pointer& dataArray)
  : m_Image(image)
  , m_Width(width)
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
    size_t pxlIdx = (pxlWidthIdx + pxlHeightIdx * m_Width) * m_Comps;
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
  for(const auto& image : m_ImageGrid)
  {
    std::function<void(void)> fn = std::bind(&FFTConvolutionCostFunction::InitializeOverlaps, this, image, overlapPercentage);
    taskAlg.execute(fn);
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
void FFTConvolutionCostFunction::calculateNew2OldPixel(size_t row, size_t col, const ParametersType& parameters, double x_trans, double y_trans) const
{
  static MutexType mutex;
  using PixelTyped = std::pair<double, double>;
  using PixelTypei = std::pair<int64_t, int64_t>;

  PixelTyped newPixel { row - y_trans, col - x_trans };

  const std::array<double, 2> newPrime = { std::get<0>(newPixel), std::get<1>(newPixel) };
  const double newXPrimeSqr = newPrime[0] * newPrime[0];
  const double newYPrimeSqr = newPrime[1] * newPrime[1];

  std::array<double, 2> oldPrime;
  oldPrime[0] = parameters[0] * newPrime[0] + parameters[1] * newPrime[1] + parameters[2] * newXPrimeSqr + parameters[3] * newYPrimeSqr + parameters[4] * newPrime[0] * newPrime[1] +
    parameters[5] * newXPrimeSqr * newPrime[1] + parameters[6] * newPrime[0] * newYPrimeSqr;
  oldPrime[1] = parameters[7] * newPrime[0] + parameters[8] * newPrime[1] + parameters[9] * newXPrimeSqr + parameters[10] * newYPrimeSqr + parameters[11] * newPrime[0] * newPrime[1] +
    parameters[12] * newXPrimeSqr * newPrime[1] + parameters[13] * newPrime[0] * newYPrimeSqr;

  const int64_t oldXUnbound = static_cast<int64_t>(round(oldPrime[0] + x_trans));
  const int64_t oldYUnbound = static_cast<int64_t>(round(oldPrime[1] + y_trans));
  PixelTypei oldPixel{ oldXUnbound, oldYUnbound };

  PixelTypei newPixeli{ static_cast<int64_t>(std::round(std::get<0>(newPixel))), static_cast<int64_t>(std::round(std::get<1>(newPixel))) };
  ScopedLockType lock(mutex);
  m_New2OldMap[newPixeli] = oldPixel;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::calculateNew2OldMap(const ParametersType& parameters) const
{
  m_New2OldMap.clear();

  int64_t startX = 0;
  int64_t startY = 0;

  GridTileIndex topLeftIndex = m_Montage->getTileIndex(0, 0);
  ImageGeom::Pointer geom = m_Montage->getDataContainer(topLeftIndex)->getGeometryAs<ImageGeom>();
  SizeVec3Type dims = geom->getDimensions();
  startX = -std::max(startX, static_cast<int64_t>(dims[0]) - static_cast<int64_t>(m_ImageDim_x));
  startY = -std::max(startY, static_cast<int64_t>(dims[1]) - static_cast<int64_t>(m_ImageDim_y));

  const double x_trans = (m_ImageDim_x - 1) / 2.0;
  const double y_trans = (m_ImageDim_y - 1) / 2.0;

  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  for(int64_t row = startY; row < m_ImageDim_y; row++)
  {
    for(int64_t col = startX; col < m_ImageDim_x; col++)
    {
      //calculateNew2OldPixel(row, col, parameters, x_trans, y_trans);
      fn = std::bind(&FFTConvolutionCostFunction::calculateNew2OldPixel, this, row, col, parameters, x_trans, y_trans);
      taskAlg.execute(fn);
    }
  }
  taskAlg.wait();
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
  SizeVec3Type dims = dc->getGeometryAs<ImageGeom>()->getDimensions();
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
  imageOrigin[0] = xOrigin;
  imageOrigin[1] = yOrigin;

  InputImage::Pointer itkImage = InputImage::New();
  itkImage->SetRegions(InputImage::RegionType(imageOrigin, imageSize));
  itkImage->Allocate();

  // A colored image could be used in a Fourier Transform as discussed in this paper:
  // https://ieeexplore.ieee.org/document/723451
  // NOTE Could this be parallelized?
  ParallelData2DAlgorithm dataAlg;
  dataAlg.setRange(yOrigin, xOrigin, height, width);
  dataAlg.execute(FFTImageInitializer(itkImage, width, da));

  GridKey imageKey = std::make_pair(row, column);
  ScopedLockType lock(mutex);
  m_ImageGrid[imageKey] = itkImage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::InitializeOverlaps(const ImageGrid::value_type& image, float overlapPercentage)
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
    size_t overlapDim = static_cast<size_t>(roundf(width * overlapPercentage));
    imageOrigin[0] = static_cast<itk::IndexValueType>(width - overlapDim);
    imageOrigin[1] = 0;
    imageSize[0] = overlapDim;
    imageSize[1] = height;

    kernelOrigin[0] = 0;
    kernelOrigin[1] = 0;
    kernelSize[0] = overlapDim;
    kernelSize[1] = height;

    GridPair position = std::make_pair(image.first, rightImage->first);
    RegionPair region = std::make_pair(InputImage::RegionType(imageOrigin, imageSize), InputImage::RegionType(kernelOrigin, kernelSize));
    OverlapPair overlap = std::make_pair(position, region);
    ScopedLockType lock(mutex);
    m_Overlaps.push_back(overlap);
  }
  if(bottomImage != m_ImageGrid.end())
  {
    // NOTE The width dimension for vertically overlapping images should be the same
    overlapDim = static_cast<size_t>(roundf(height * overlapPercentage));
    imageOrigin[0] = 0;
    imageOrigin[1] = 0;
    imageSize[0] = width;
    imageSize[1] = overlapDim;

    kernelOrigin[0] = static_cast<itk::IndexValueType>(height - overlapDim);
    kernelOrigin[1] = 0;
    kernelSize[0] = width;
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
  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  calculateNew2OldMap(parameters);

  // Apply the Transform to each image in the image grid
  for(const auto& eachImage : m_ImageGrid) // Parallelize this
  {
    fn = std::bind(&FFTConvolutionCostFunction::applyTransformation, this, parameters, eachImage, std::ref(distortedGrid));
    taskAlg.execute(fn);
  }
  taskAlg.wait();

  std::atomic<MeasureType> residual{0.0};
  // Find the FFT Convolution and accumulate the maximum value from each overlap
  for(const auto& overlap : m_Overlaps) // Parallelize this
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
void FFTConvolutionCostFunction::applyTransformation(const ParametersType& parameters, const ImageGrid::value_type& inputImage, ImageGrid& distortedGrid) const
{
  static MutexType mutex;
  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  const double tolerance = 0.05;

  InputImage::RegionType bufferedRegion = inputImage.second->GetBufferedRegion();

  typename InputImage::Pointer distortedImage = InputImage::New();
  distortedImage->SetRegions(bufferedRegion);
  distortedImage->Allocate();

  // Iterate through the pixels in eachImage and apply the transform
  itk::ImageRegionIterator<InputImage> it(inputImage.second, bufferedRegion);
  for(it.GoToBegin(); !it.IsAtEnd(); ++it) // Parallelize this
  {
    fn = std::bind(&FFTConvolutionCostFunction::applyTransformationPixel, this, tolerance, parameters, inputImage.second, distortedImage, bufferedRegion, it);
    taskAlg.execute(fn);
  }
  taskAlg.wait();

  GridKey distortedKey = inputImage.first;
  ScopedLockType lock(mutex);
  distortedGrid[distortedKey] = distortedImage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::applyTransformationPixel(double tolerance, const ParametersType& parameters, const InputImage::Pointer& inputImage, const InputImage::Pointer& distortedImage,
                                                          const InputImage::RegionType& bufferedRegion, itk::ImageRegionIterator<InputImage> iter) const
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
  calculatePixelCoordinates(parameters, inputImage, distortedImage, pixel, x_trans, y_trans, tolerance, lastXIndex, lastYIndex);

#if 0
  // This check effectively "clips" data and the cast and round
  // effectively perform a nearest neighbor sampling
  if(x >= -tolerance && x <= lastXIndex && y >= -tolerance && y <= lastYIndex)
  {
    PixelCoord eachPixel;
    eachPixel[0] = static_cast<int64_t>(round(x));
    eachPixel[1] = static_cast<int64_t>(round(y));
    // The value obtained with eachImage.second->GetPixel(eachPixel) could have
    // its "contrast" adjusted based on its radial location from the center of
    // the image at this step to compensate for error encountered from radial effects
    ScopedLockType lock(mutex);
    distortedImage->SetPixel(eachPixel, inputImage->GetPixel(eachPixel));
  }
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::calculatePixelCoordinates(const ParametersType& parameters, const InputImage::Pointer& inputImage, const InputImage::Pointer& distortedImage, const PixelCoord& newPixel, double x_trans, double y_trans, double tolerance, double lastXIndex, double lastYIndex) const
{
  static MutexType mutex;

  using PixelTypei = std::pair<int64_t, int64_t>;

  // eachIJ is a pair, the first index is the applied exponent to u
  // the second index is the applied exponent to v
  //std::pair<int, int> eachIJ = m_IJ[idx - (idx >= m_IJ.size() ? m_IJ.size() : 0)];

  //const std::array<double, 2> newPrime = { newPixel[0] - x_trans, newPixel[1] - y_trans };
  //const double newXPrimeSqr = newPrime[0] * newPrime[0];
  //const double newYPrimeSqr = newPrime[1] * newPrime[1];

  //std::array<double, 2> oldPrime;
  //oldPrime[0] = parameters[0] * newPrime[0] + parameters[1] * newPrime[1] + parameters[2] * newXPrimeSqr + parameters[3] * newYPrimeSqr + parameters[4] * newPrime[0] * newPrime[1] +
  //              parameters[5] * newXPrimeSqr * newPrime[1] + parameters[6] * newPrime[0] * newYPrimeSqr;
  //oldPrime[1] = parameters[7] * newPrime[0] + parameters[8] * newPrime[1] + parameters[9] * newXPrimeSqr + parameters[10] * newYPrimeSqr + parameters[11] * newPrime[0] * newPrime[1] +
  //              parameters[12] * newXPrimeSqr * newPrime[1] + parameters[13] * newPrime[0] * newYPrimeSqr;

  //auto inputSize = inputImage->GetBufferedRegion().GetSize();
  //const int64_t width = inputSize[0];
  //const int64_t height = inputSize[1];
  auto inputIndex = inputImage->GetBufferedRegion().GetIndex();
  const int64_t minX = inputIndex[0];
  const int64_t minY = inputIndex[1];

  PixelTypei newPixeli{ newPixel[0] - minX, newPixel[1] - minY };
  PixelTypei oldPixeli = m_New2OldMap[newPixeli];

  PixelCoord oldPixel;
  oldPixel[0] = std::get<0>(oldPixeli);
  oldPixel[1] = std::get<1>(oldPixeli);

  //const int64_t oldXUnbound = static_cast<int64_t>(round(oldPrime[0] + x_trans));
  //const int64_t oldYUnbound = static_cast<int64_t>(round(oldPrime[1] + y_trans));

  //PixelCoord oldPixel;
  //oldPixel[0] = std::min(std::max<int64_t>(oldXUnbound, 0), width - 1);
  //oldPixel[1] = std::min(std::max<int64_t>(oldYUnbound, 0), height - 1);

#if 0
  // The subtraction step here translates the coordinates so the center of the image
  // is at the origin
  // This allows for rotations to occur in a predictable way
  const double u_v = pow((pixel[0] - x_trans), eachIJ.first) * pow((pixel[1] - y_trans), eachIJ.second);

  const double term = u_v * parameters[idx];
  // This recorrects the centering translation to the appropriate x or y value
  idx < m_IJ.size() - 1 ? x_ref += term : y_ref += term;
#endif

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
void FFTConvolutionCostFunction::findFFTConvolutionAndMaxValue(const OverlapPair& overlap, ImageGrid& distortedGrid, std::atomic<MeasureType>& residual) const
{
  static MutexType mutex;
  // Set the filter's image to the overlap region of the image
  GridKey imageKey = overlap.first.first;
  InputImage::Pointer image = distortedGrid.at(imageKey);
  Filter::Pointer filter = Filter::New();

  // NOTE It may be useful for debugging purposes to write the image to a file here

  GridKey kernelKey = overlap.first.second;
  auto imageRegion = overlap.second.first;
  image->SetRequestedRegion(imageRegion);
  filter->SetInput(image);

  // Set the filter's kernel to the overlap region of the kernel
  InputImage::Pointer kernel = distortedGrid.at(kernelKey);

  // NOTE It may be useful for debugging purposes to write the kernel to a file here

  kernel->SetRequestedRegion(overlap.second.second);
  filter->SetKernelImage(kernel);

  // Run the filter
  filter->Update();
  OutputImage::Pointer fftConvolve = filter->GetOutput();

  // NOTE It may be useful for debugging purposes to write the output image to a file here

  // Increment by the maximum value of the output of the fftConvolve
  // NOTE This methodology of getting the max element from the fftConvolve
  // output might require a deeper look
  MeasureType maxValue = *std::max_element(fftConvolve->GetBufferPointer(), fftConvolve->GetBufferPointer() + fftConvolve->GetPixelContainer()->Size());
  ScopedLockType lock(mutex);
  residual = residual + maxValue;
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
