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

using MutexType = tbb::queuing_mutex;
using ScopedLockType = MutexType::scoped_lock;
#else
// included so that this can be compiled even without tbb
using MutexType = int;
using ScopedLockType = int;
#endif

#include "SIMPLib/Montages/GridMontage.h"
#include "SIMPLib/Utilities/ParallelData2DAlgorithm.h"
#include "SIMPLib/Utilities/ParallelTaskAlgorithm.h"


// ----------------------------------------------------------------------------
FFTHelper::PixelTypei FFTHelper::pixelType(int64_t x, int64_t y)
{
  return PixelTypei{x, y};
}

// ----------------------------------------------------------------------------
FFTHelper::PixelTypei FFTHelper::pixelType(size_t x, size_t y)
{
  return PixelTypei{static_cast<int64_t>(x), static_cast<int64_t>(y)};
}

// ----------------------------------------------------------------------------
FFTHelper::PixelTypei FFTHelper::pixelType(double x, double y)
{
  int64_t xi = static_cast<int64_t>(std::floor(x));
  int64_t yi = static_cast<int64_t>(std::floor(y));
  return PixelTypei{xi, yi};
}

// ----------------------------------------------------------------------------
FFTHelper::PixelTypei FFTHelper::getOldIndex(PixelTypei newCoords, PixelTypei offset, const ParametersType& parameters)
{
  PixelTypei oldCoords;
  oldCoords[0] = px(newCoords, offset, parameters);
  oldCoords[1] = py(newCoords, offset, parameters);
  return oldCoords;
}

// ----------------------------------------------------------------------------
int64_t FFTHelper::px(PixelTypei newCoords, PixelTypei offset, const ParametersType& parameters)
{
  const double newXPrime = newCoords[0] - offset[0];
  const double newYPrime = newCoords[1] - offset[1];

  double oldXPrime = 0.0;
  oldXPrime += parameters[0] * newXPrime;
  oldXPrime += parameters[1] * newYPrime;
  oldXPrime += parameters[2] * newXPrime * newXPrime;
  oldXPrime += parameters[3] * newYPrime * newYPrime;
  oldXPrime += parameters[4] * newXPrime * newYPrime;
  oldXPrime += parameters[5] * newXPrime * newXPrime * newYPrime;
  oldXPrime += parameters[6] * newXPrime * newYPrime * newYPrime;

  return static_cast<int64_t>(std::floor(oldXPrime + offset[0]));
}

// ----------------------------------------------------------------------------
int64_t FFTHelper::py(PixelTypei newCoords, PixelTypei offset, const ParametersType& parameters)
{
  const double newXPrime = newCoords[0] - offset[0];
  const double newYPrime = newCoords[1] - offset[1];

  double oldYPrime = 0.0;
  const size_t yOffset = getReqPartialParameterSize();
  oldYPrime += parameters[7] * newXPrime;
  oldYPrime += parameters[8] * newYPrime;
  oldYPrime += parameters[9] * newXPrime * newXPrime;
  oldYPrime += parameters[10] * newYPrime * newYPrime;
  oldYPrime += parameters[11] * newXPrime * newYPrime;
  oldYPrime += parameters[12] * newXPrime * newXPrime * newYPrime;
  oldYPrime += parameters[13] * newXPrime * newYPrime * newYPrime;

  return static_cast<int64_t>(std::floor(oldYPrime + offset[1]));
}

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
  FFTImageInitializer(const InputImage::Pointer& image, size_t width, size_t height, const DataArrayType::Pointer& dataArray)
  : m_Image(image)
  , m_Width(width)
  , m_Height(height)
  , m_DataArray(dataArray)
  , m_Comps(dataArray->getNumberOfComponents())
  {
    auto index = image->GetRequestedRegion().GetIndex();
    m_ImageIndex[0] = index[0];
    m_ImageIndex[1] = index[1];
  }

  /**
   * @brief Sets the image's pixel at the specified position based on the DataArray value.
   * @param pxlWidthIds
   * @param pxlHeightIdx
   */
  void setPixel(size_t pxlWidthIdx, size_t pxlHeightIdx) const
  {
    // Get the pixel index from the current pxlWidthIdx and pxlHeightIdx
    size_t pxlIdx = ((pxlWidthIdx) + (pxlHeightIdx)*m_Width) * m_Comps;
    PixelCoord idx;
    idx[0] = pxlWidthIdx + m_ImageIndex[0];
    idx[1] = pxlHeightIdx + m_ImageIndex[1];
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
  PixelCoord m_ImageIndex;
  DataArrayType::Pointer m_DataArray;
  size_t m_Comps;
};

/**
 * @brief The FFTImageOverlapGenerator class is used for generating itk::Images
 * for the specified overlap region from a given itk::Image, offsets, and
 * dewarp parameters.
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
   * @param baseImg
   * @param image
   * @param offset
   * @param imageDim_x
   * @param imageDim_y
   * @param parameters
   */
  FFTImageOverlapGenerator(const InputImage::Pointer& baseImg, const InputImage::Pointer& image, const PixelCoord& offset, size_t imageDim_x, size_t imageDim_y, const ParametersType& parameters)
  : m_BaseImg(baseImg)
  , m_Image(image)
  , m_Offset(offset)
  , m_ImageDim_x(imageDim_x)
  , m_ImageDim_y(imageDim_y)
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
    const auto baseSize = baseRegion.GetSize();

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
    FFTHelper::PixelTypei index = FFTHelper::pixelType(x, y);
    FFTHelper::PixelTypei offset = FFTHelper::pixelType(x_trans - m_Offset[0], y_trans - m_Offset[1]);
    FFTHelper::PixelTypei oldPixel = FFTHelper::getOldIndex(index, offset, m_Parameters);
    return PixelCoord{oldPixel[0], oldPixel[1]};
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
        PixelCoord newIndex{static_cast<int64_t>(x), static_cast<int64_t>(y)};
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
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::Initialize(const GridMontageShPtr& montage, const DataContainerArrayShPtr& dca, const QString& amName, const QString& daName)
{
  m_Montage = montage;

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
      taskAlg.execute(std::bind(&FFTConvolutionCostFunction::initializeDataContainer, this, montage, row, col, amName, daName));
    }
  }
  taskAlg.wait();

  CropMap cropMap;
  for(const auto& image : m_ImageGrid)
  {
    precalcCropMap(image, cropMap);
  }

  m_Overlaps = createOverlapPairs(cropMap);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::calculateImageDim(const GridMontageShPtr& montage)
{
  size_t x = montage->getColumnCount() > 2 ? 1 : 0;
  size_t y = montage->getRowCount() > 2 ? 1 : 0;
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
FFTConvolutionCostFunction::PixelTypei FFTConvolutionCostFunction::calculateNew2OldPixel(int64_t x, int64_t y, const ParametersType& parameters, double x_trans, double y_trans) const
{
  FFTHelper::PixelTypei offset = FFTHelper::pixelType(x_trans, y_trans);
  return FFTHelper::getOldIndex({x, y}, offset, parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::initializeDataContainer(const GridMontageShPtr& montage, size_t row, size_t column, const QString& amName, const QString& daName)
{
  static MutexType mutex;

  GridTileIndex index = montage->getTileIndex(row, column);
  DataContainer::Pointer dc = montage->getDataContainer(index);
  AttributeMatrix::Pointer am = dc->getAttributeMatrix(amName);
  DataArray<Grayscale_T>::Pointer da = am->getAttributeArrayAs<DataArray<Grayscale_T>>(daName);
  size_t comps = da->getNumberOfComponents();
  ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
  FloatVec3Type spacing = imageGeom->getSpacing();
  SizeVec3Type dims = imageGeom->getDimensions();
  size_t width = dims.getX() / spacing.getX();
  size_t height = dims.getY() / spacing.getY();
  size_t xOrigin = imageGeom->getOrigin().getX() / spacing.getX();
  size_t yOrigin = imageGeom->getOrigin().getY() / spacing.getY();
  size_t offsetX = 0;
  size_t offsetY = 0;

  /////////////////////////////////////////////////////////////////////////////
  // This divided the dimensions and origins by the spacing to treat the     //
  // montage as if it has a spacing of size 1 in order to use itk::Index.    //
  /////////////////////////////////////////////////////////////////////////////

#if 1
  if(row == 0 && montage->getRowCount() > 2)
  {
    offsetY = height;
    const size_t yPrime = yOrigin + height;
    height = std::min(height, static_cast<size_t>(std::floor(m_ImageDim_y)));
    yOrigin = yPrime - height;
    offsetY -= height;
  }
  if(column == 0 && montage->getColumnCount() > 2)
  {
    offsetX = width;
    const size_t xPrime = xOrigin + width;
    width = std::min(width, static_cast<size_t>(std::floor(m_ImageDim_x)));
    xOrigin = xPrime - width;
    offsetX -= width;
  }
#endif

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
  dataAlg.setRange(offsetY, offsetX, height, width);
  dataAlg.execute(FFTImageInitializer(itkImage, width, height, da));

  GridKey imageKey = std::make_pair(column, row); // Flipped this to {x,y}
  ScopedLockType lock(mutex);
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
void FFTConvolutionCostFunction::precalcCropMap(const ImageGrid::value_type& inputImage, CropMap& cropMap)
{
  // Add bounds to the CropMap
  auto origin = inputImage.second->GetRequestedRegion().GetIndex();
  auto size = inputImage.second->GetRequestedRegion().GetSize();
  RegionBounds bounds;
  bounds.leftBound = origin[0];
  bounds.topBound = origin[1];
  bounds.rightBound = bounds.leftBound + size[0];
  bounds.bottomBound = bounds.topBound + size[1];
  cropMap[inputImage.first] = bounds;
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
  return FFTHelper::getReqParameterSize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::MeasureType FFTConvolutionCostFunction::GetValue(const ParametersType& parameters) const
{
  MeasureType residual = 0.0;
  // Find the FFT Convolution and accumulate the maximum value from each overlap
  for(const auto& overlap : m_Overlaps) // Parallelize this
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
FFTConvolutionCostFunction::OverlapPairs FFTConvolutionCostFunction::createOverlapPairs(const CropMap& cropMap) const
{
  OverlapPairs overlaps;

  for(const auto& iter : cropMap)
  {
    GridKey key = iter.first;
    RegionBounds bounds = iter.second;

    GridKey rightKey = std::make_pair(key.first + 1, key.second);
    const auto rightIter = cropMap.find(rightKey);
    if(rightIter != cropMap.end())
    {
      GridPair gridPair = std::make_pair(key, rightKey);
      RegionPair regions = createRightRegionPairs(bounds, rightIter->second);
      OverlapPair overlapPair = std::make_pair(gridPair, regions);
      overlaps.push_back(overlapPair);
    }

    GridKey botKey = std::make_pair(key.first, key.second + 1);
    const auto botIter = cropMap.find(botKey);
    if(botIter != cropMap.end())
    {
      GridPair gridPair = std::make_pair(key, botKey);
      RegionPair regions = createBottomRegionPairs(bounds, botIter->second);
      OverlapPair overlapPair = std::make_pair(gridPair, regions);
      overlaps.push_back(overlapPair);
    }
  }

  return overlaps;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::RegionPair FFTConvolutionCostFunction::createRightRegionPairs(const RegionBounds& left, const RegionBounds& right) const
{
  const int64_t topBound = std::max(left.topBound, right.topBound);
  const int64_t bottomBound = std::min(left.bottomBound, right.bottomBound);
  const int64_t width = std::abs(right.leftBound - left.rightBound);
  const int64_t height = std::abs(bottomBound - topBound);

  PixelCoord kernelOrigin;
  kernelOrigin[0] = left.rightBound;
  kernelOrigin[1] = topBound;

  InputImage::SizeType kernelSize;
  kernelSize[0] = width;
  kernelSize[1] = height;

  // With the changes in overlap / origins, there are no longer any differences in the RegionPair
  InputImage::RegionType rightRegion = InputImage::RegionType(kernelOrigin, kernelSize);
  return std::make_pair(rightRegion, rightRegion);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::RegionPair FFTConvolutionCostFunction::createBottomRegionPairs(const RegionBounds& top, const RegionBounds& bottom) const
{
  const int64_t leftBound = std::max(top.leftBound, bottom.leftBound);
  const int64_t rightBound = std::min(top.rightBound, bottom.rightBound);
  const int64_t width = std::abs(rightBound - leftBound);
  const int64_t height = std::abs(bottom.topBound - top.bottomBound);

  PixelCoord kernelOrigin;
  kernelOrigin[0] = leftBound;
  kernelOrigin[1] = top.bottomBound;

  InputImage::SizeType kernelSize;
  kernelSize[0] = width;
  kernelSize[1] = height;

  // With the changes in overlap / origins, there are no longer any differences in the RegionPair
  InputImage::RegionType botRegion = InputImage::RegionType(kernelOrigin, kernelSize);
  return std::make_pair(botRegion, botRegion);
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
  firstOverlapImg->SetRegions({firstRegion.GetIndex(), firstRegion.GetSize()});
  firstOverlapImg->Allocate();

  auto index = firstRegion.GetIndex();
  ParallelData2DAlgorithm dataAlg;
  dataAlg.setRange(index[1], index[0], firstRegion.GetSize()[1], firstRegion.GetSize()[0]);
  dataAlg.execute(FFTImageOverlapGenerator(firstBaseImg, firstOverlapImg, index, m_ImageDim_x, m_ImageDim_y, parameters));

  // Second image calculation
  InputImage::RegionType secondRegion = overlap.second.second;
  const InputImage::Pointer& secondBaseImg = m_ImageGrid.at(overlap.first.second);

  InputImage::Pointer secondOverlapImg = InputImage::New();
  secondOverlapImg->SetRegions({secondRegion.GetIndex(), secondRegion.GetSize()});
  secondOverlapImg->Allocate();

  index = secondRegion.GetIndex();
  dataAlg.setRange(index[1], index[0], secondRegion.GetSize()[1], secondRegion.GetSize()[0]);
  dataAlg.execute(FFTImageOverlapGenerator(secondBaseImg, secondOverlapImg, index, m_ImageDim_x, m_ImageDim_y, parameters));

  return std::make_pair(firstOverlapImg, secondOverlapImg);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::findFFTConvolutionAndMaxValue(const OverlapPair& overlap, const ParametersType& parameters, MeasureType& residual) const
{
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
