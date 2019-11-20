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

#define USE_CROP_CHECK 0

struct RegionBounds
{
  int64_t leftBound = 0;
  int64_t topBound = 0;
  int64_t rightBound = std::numeric_limits<int64_t>::max();
  int64_t bottomBound = std::numeric_limits<int64_t>::max();
};

// ----------------------------------------------------------------------------
FFTHelper::PixelTypei FFTHelper::pixelType(int64_t x, int64_t y)
{
  return PixelTypei{ x, y };
}

// ----------------------------------------------------------------------------
FFTHelper::PixelTypei FFTHelper::pixelType(size_t x, size_t y)
{
  return PixelTypei{ static_cast<int64_t>(x), static_cast<int64_t>(y) };
}

// ----------------------------------------------------------------------------
FFTHelper::PixelTypei FFTHelper::pixelType(double x, double y)
{
  int64_t xi = static_cast<int64_t>(std::floor(x));
  int64_t yi = static_cast<int64_t>(std::floor(y));
  return PixelTypei{ xi, yi };
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
    size_t pxlIdx = ((pxlWidthIdx) + (pxlHeightIdx) * m_Width) * m_Comps;
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
  FFTImageOverlapGenerator(const InputImage::Pointer& baseImg, const InputImage::Pointer& image, const PixelCoord& offset, size_t imageDim_x, size_t imageDim_y,
                           const ParametersType& parameters)
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
    FFTHelper::PixelTypei offset = {static_cast<int64_t>(x_trans - m_Offset[0]), static_cast<int64_t>(y_trans - m_Offset[1])};
    FFTHelper::PixelTypei oldPixel = FFTHelper::getOldIndex(index, offset, m_Parameters);
    return PixelCoord{ oldPixel[0], oldPixel[1] };
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
void FFTConvolutionCostFunction::Initialize(const GridMontageShPtr& montage, const DataContainerArrayShPtr& dca, const QString& amName,
                                            const QString& daName)
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
      taskAlg.execute(std::bind(&FFTConvolutionCostFunction::InitializeDataContainer, this, montage, row, col, amName, daName));
    }
  }
  taskAlg.wait();

  // Populate m_overlaps
#if 0
  m_Overlaps.clear();
  for(const auto& image : m_ImageGrid)
  {
    taskAlg.execute(std::bind(&FFTConvolutionCostFunction::InitializeCellOverlaps, this, image));
  }
  taskAlg.wait();
#endif
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
  return FFTHelper::getOldIndex({ x, y }, offset, parameters);
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

#if 0
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
  CropMap cropMap;
  
  // Apply the Transform to each image in the image grid
  for(const auto& eachImage : m_ImageGrid) // Parallelize this
  {
    checkTransformation(parameters, eachImage, std::ref(cropMap));
  }

#if USE_CROP_CHECK
  if(!isCropMapValid(cropMap))
  {
    std::cout << "Invalid Parameters";
    // This is optimizing for the greatest return value, thus invalid inputs require a smallest return
    return std::numeric_limits<MeasureType>::min();
  }
#endif

  // Crop overlap portions
  OverlapPairs overlaps = createOverlapPairs(cropMap);

  MeasureType residual = 0.0;
  // Find the FFT Convolution and accumulate the maximum value from each overlap
  for(const auto& overlap : overlaps) // Parallelize this
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
    if(!isRegionValid(index, bounds, cropMap))
    {
      return false;
    }
  }
  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool FFTConvolutionCostFunction::isRegionValid(const GridKey& index, const RegionBounds& bounds, const CropMap& cropMap) const
{
  const GridKey rightKey = std::make_pair(index.first + 1, index.second);
  const GridKey botKey = std::make_pair(index.first, index.second + 1);

  const auto rightBoundsIter = cropMap.find(rightKey);
  const auto botBoundsIter = cropMap.find(botKey);

  if(rightBoundsIter != cropMap.end())
  {
    const RegionBounds rightBounds = rightBoundsIter->second;
    if(bounds.rightBound <= rightBounds.leftBound)
    {
      return false;
    }
  }
  if(botBoundsIter != cropMap.end())
  {
    const RegionBounds botBounds = botBoundsIter->second;
    if(bounds.bottomBound <= botBounds.topBound)
    {
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::checkTransformation(const ParametersType& parameters, const ImageGrid::value_type& inputImage, CropMap& cropMap) const
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

  ParallelTaskAlgorithm taskAlg;
  // Iterate through the pixels in eachImage and apply the transform
  for(int64_t y = bounds.topBound; y < bounds.bottomBound; y++)
  {
    for(int64_t x = bounds.leftBound; x < bounds.rightBound; x++)
    {
      PixelCoord index = { x, y };
      taskAlg.execute(std::bind(&FFTConvolutionCostFunction::applyTransformationPixel, this, parameters, inputImage.second, index, inputImage.first, std::ref(cropMap)));
    }
  }
  taskAlg.wait();

  //updateCropMapBounds(inputImage.second, cropMap);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::applyTransformationPixel(const ParametersType& parameters, const InputImage::Pointer& inputImage,
                                                          PixelCoord index, const GridKey& gridKey, CropMap& cropMap) const
{
  const double x_trans = (m_ImageDim_x - 1) / 2.0;
  const double y_trans = (m_ImageDim_y - 1) / 2.0;

  // Dave's method uses an m_IJ matrix described above
  // and so a different method of grabbing the appropriate index from the m_IJ
  // will be needed if using that static array
  calculatePixelCoordinates(parameters, inputImage, gridKey, cropMap, index, x_trans, y_trans);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::calculatePixelCoordinates(const ParametersType& parameters, const InputImage::Pointer& inputImage, const GridKey& gridKey, CropMap& cropMap,
                                                           const PixelCoord& newPixel, double x_trans, double y_trans) const
{
  //static MutexType mutex;

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

#if USE_CROP_CHECK
  bool isValidX = oldPixel[0] >= minX && oldPixel[0] < minX + width;
  bool isValidY = oldPixel[1] >= minY && oldPixel[1] < minY + height;
  bool isValid = isValidX && isValidY;

  // Do not set invalid pixels
  if(!isValid)
  {
    adjustCropMap(newPixel, inputImage, gridKey, cropMap);
    return;
  }
#endif
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::adjustCropMap(const PixelCoord& pixel, const InputImage::Pointer& inputImage, const GridKey& gridKey, CropMap& cropMap) const
{
  static MutexType mutex;

  auto inputSize = inputImage->GetBufferedRegion().GetSize();
  const int64_t width = inputSize[0];
  const int64_t height = inputSize[1];
  auto inputIndex = inputImage->GetBufferedRegion().GetIndex();
  const int64_t minX = inputIndex[0];
  const int64_t minY = inputIndex[1];

  const int64_t maxX = minX + width;
  const int64_t maxY = minY + height;

  const int64_t x = static_cast<int64_t>(pixel[0]);
  const int64_t y = static_cast<int64_t>(pixel[1]);

  const int64_t distTop = y - minY;
  const int64_t distBot = maxY - y;
  const int64_t distLeft = x - minX;
  const int64_t distRight = maxX - x;

  // Use a reference so we only override the target value instead of all four
  // Use min/max value in case the bounds might already be closer than the current pixel
  ScopedLockType lock(mutex);
  RegionBounds region = cropMap[gridKey];
  if((distTop < distBot) && (distTop < distLeft) && (distTop < distRight))
  {
    region.topBound = std::max(region.topBound, y);
  }
  else if((distRight < distLeft) && (distRight < distTop) && (distRight < distBot))
  {
    region.rightBound = std::min(region.rightBound, x);
  }
  else if((distLeft < distRight) && (distLeft < distTop) && (distLeft < distBot))
  {
    region.leftBound = std::max(region.leftBound, x);
  }
  else if(distBot < distTop)
  {
    region.bottomBound = std::min(region.bottomBound, y);
  }

  cropMap[gridKey] = region;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::updateCropMapBounds(const InputImage::Pointer& inputImage, CropMap& cropMap) const
{
  static MutexType mutex;

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
    ScopedLockType lock(mutex);
    cropMap[key] = region;
  }
}

#if 0
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
#endif

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
  // TODO: update values for globalization. FirstRegion.GetSize() has max height
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
  //static MutexType mutex;
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

  //std::cout << "Max Value: " << maxValue;
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
