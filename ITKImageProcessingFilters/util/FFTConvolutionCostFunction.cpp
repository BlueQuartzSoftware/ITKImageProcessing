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

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include "tbb/queuing_mutex.h"
#endif

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
void FFTConvolutionCostFunction::Initialize(const QStringList& chosenDataContainers, const QString& rowChar, const QString& colChar, int degree, float overlapPercentage,
                                            const DataContainerArrayShPtr& dca, const QString& amName, const QString& daName)
{
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

  ParallelTaskAlgorithm taskAlg;
  // Populate and assign eachImage to m_imageGrid
  for(const auto& dc : dca->getDataContainers()) // Parallelize this
  {
    if(!chosenDataContainers.contains(dc->getName()))
    {
      continue;
    }
    std::function<void(void)> fn = std::bind(&FFTConvolutionCostFunction::InitializeDataContainer, this, dc, rowChar, colChar, amName, daName);
    taskAlg.execute(fn);
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
void FFTConvolutionCostFunction::InitializeDataContainer(const DataContainer::Pointer& dc, const QString& rowChar, const QString& colChar, const QString& amName, const QString& daName)
{
  static MutexType mutex;
  AttributeMatrix::Pointer am = dc->getAttributeMatrix(amName);
  DataArray<Grayscale_T>::Pointer da = am->getAttributeArrayAs<DataArray<Grayscale_T>>(daName);
  size_t comps = da->getNumberOfComponents();
  SizeVec3Type dims = dc->getGeometryAs<ImageGeom>()->getDimensions();
  size_t width = dims.getX();
  size_t height = dims.getY();

  InputImage::SizeType imageSize;
  imageSize[0] = width;
  imageSize[1] = height;

  PixelCoord imageOrigin;
  imageOrigin[0] = 0;
  imageOrigin[1] = 0;

  InputImage::Pointer eachImage = InputImage::New();
  eachImage->SetRegions(InputImage::RegionType(imageOrigin, imageSize));
  eachImage->Allocate();

  // A colored image could be used in a Fourier Transform as discussed in this paper:
  // https://ieeexplore.ieee.org/document/723451
  // NOTE Could this be parallelized?
  ParallelData2DAlgorithm dataAlg;
  dataAlg.setRange(0, 0, height, width);
  dataAlg.execute(FFTImageInitializer(eachImage, width, da));

  QString name = dc->getName();
  // NOTE For row/column string indicators with a length greater than one,
  // it would probably be more robust to 'lastIndexOf'
  int cLength = name.size() - name.indexOf(colChar) - 1;
  
  size_t row = name.midRef(name.indexOf(rowChar) + 1, name.size() - cLength - 2).toULong();
  size_t col = name.rightRef(cLength).toULong();
  GridKey imageKey = std::make_pair(row, col);
  ScopedLockType lock(mutex);
  m_ImageGrid[imageKey] = eachImage;
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
  return static_cast<uint32_t>(2 * (m_Degree * m_Degree + 2 * m_Degree + 1));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FFTConvolutionCostFunction::MeasureType FFTConvolutionCostFunction::GetValue(const ParametersType& parameters) const
{
  ImageGrid distortedGrid;
  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  // Apply the Transform to each image in the image grid
  for(const auto& eachImage : m_ImageGrid) // Parallelize this
  {
    fn = std::bind(&FFTConvolutionCostFunction::applyTransformation, this, parameters, eachImage, std::ref(distortedGrid));
    taskAlg.execute(fn);
  }
  taskAlg.wait();

  std::atomic<MeasureType> residual{0.0};
  // Find the FFT Convolution and accumulate the maximum value from each overlap
  for(const auto& eachOverlap : m_Overlaps) // Parallelize this
  {
    findFFTConvolutionAndMaxValue(eachOverlap, distortedGrid, residual);
    // std::function<void (void)> fn = std::bind(&FFTConvolutionCostFunction::findFFTConvolutionAndMaxValue, this, eachOverlap, distortedGrid, residual);
    // taskAlg.execute(fn);
  }

  // The value to minimize is the square of the sum of the maximum value of the fft convolution
  MeasureType result = sqrt(residual);
  return result;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::applyTransformation(const ParametersType& parameters, const ImageGrid::value_type& eachImage, ImageGrid& distortedGrid) const
{
  static MutexType mutex;
  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  const double tolerance = 0.05;

  InputImage::RegionType bufferedRegion = eachImage.second->GetBufferedRegion();

  typename InputImage::Pointer distortedImage = InputImage::New();
  distortedImage->SetRegions(bufferedRegion);
  distortedImage->Allocate();

  // Iterate through the pixels in eachImage and apply the transform
  itk::ImageRegionIterator<InputImage> it(eachImage.second, bufferedRegion);
  for(it.GoToBegin(); !it.IsAtEnd(); ++it) // Parallelize this
  {
    fn = std::bind(&FFTConvolutionCostFunction::applyTransformationPixel, this, tolerance, parameters, eachImage.second, distortedImage, bufferedRegion, it);
    taskAlg.execute(fn);
  }
  taskAlg.wait();

  GridKey distortedKey = eachImage.first;
  ScopedLockType lock(mutex);
  distortedGrid[distortedKey] = distortedImage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::applyTransformationPixel(double tolerance, const ParametersType& parameters, const InputImage::Pointer& inputImage, const InputImage::Pointer& distortedImage,
                                                          const InputImage::RegionType& bufferedRegion, const itk::ImageRegionIterator<InputImage>& iter) const
{
  static MutexType mutex;
  ParallelTaskAlgorithm taskAlg;
  std::function<void(void)> fn;

  const InputImage::SizeType dims = bufferedRegion.GetSize();
  const size_t width = dims[0];
  const size_t height = dims[1];
  const double lastXIndex = width - 1 + tolerance;
  const double lastYIndex = height - 1 + tolerance;

  const double x_trans = (width - 1) / 2.0;
  const double y_trans = (height - 1) / 2.0;

  PixelCoord pixel = iter.GetIndex();
  double x = x_trans;
  double y = y_trans;
  // Dave's method uses an m_IJ matrix described above
  // and so a different method of grabbing the appropriate index from the m_IJ
  // will be needed if using that static array
  for(size_t idx = 0; idx < parameters.size(); ++idx) // Parallelize this
  {
    fn = std::bind(&FFTConvolutionCostFunction::calculatePixelCoordinates, this, parameters, pixel, idx, x_trans, y_trans, x, y);
    taskAlg.execute(fn);
  }
  taskAlg.wait();

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
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::calculatePixelCoordinates(const ParametersType& parameters, const PixelCoord& pixel, size_t idx, double x_trans, double y_trans, double& x_ref, double& y_ref) const
{
  // eachIJ is a pair, the first index is the applied exponent to u
  // the second index is the applied exponent to v
  std::pair<int, int> eachIJ = m_IJ[idx - (idx >= m_IJ.size() ? m_IJ.size() : 0)];

  // The subtraction step here translates the coordinates so the center of the image
  // is at the origin
  // This allows for rotations to occur in a predictable way
  const double u_v = pow((pixel[0] - x_trans), eachIJ.first) * pow((pixel[1] - y_trans), eachIJ.second);

  const double term = u_v * parameters[idx];
  // This recorrects the centering translation to the appropriate x or y value
  idx < m_IJ.size() - 1 ? x_ref += term : y_ref += term;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FFTConvolutionCostFunction::findFFTConvolutionAndMaxValue(const OverlapPair& overlap, ImageGrid& distortedGrid, std::atomic<MeasureType>& residual) const
{
  static MutexType mutex;
  // Set the filter's image to the overlap region of the image
  auto imageKey = overlap.first.first;
  InputImage::Pointer image = distortedGrid.at(imageKey);
  Filter::Pointer filter = Filter::New();

  // NOTE It may be useful for debugging purposes to write the image to a file here

  auto kernelKey = overlap.first.second;
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
