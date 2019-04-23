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

#include "Blend.h"

#include "SIMPLib/Common/Constants.h"

#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/FilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
//#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "itkAmoebaOptimizer.h"
#include <itkFFTConvolutionImageFilter.h>

using namespace BlendFilter;

using Coord_T = size_t;
template <class DataType>
using DataPairs = std::vector<std::pair<DataType, DataType>>;

using GrayScaleColor = uint8_t;
using RGBColor = std::tuple<uint8_t, uint8_t, uint8_t>;
// TODO Set Image Type with filter parameter
using Image_T = GrayScaleColor;
using Pixel_T = size_t;
using PixelCoord = std::pair<Pixel_T, Pixel_T>;
using Image = std::map<PixelCoord, Image_T>;
using ImageArray = std::vector<std::vector<Image_T>>;

using OverlapPairs = std::set<std::pair<PixelCoord, PixelCoord>>;

class BlendFilter::ImageImpl
{
  Image m_image;
  size_t m_res_width;
  size_t m_res_height;

  static Image DataContainerToImage(DataContainerShPtr dc, const QString& amName, const QString& dataAAName, const QString& xAAName, const QString& yAAName)
  {
    Image img{};
    std::tuple<size_t, size_t, size_t> dims = dc->getGeometryAs<ImageGeom>()->getDimensions();
    AttributeMatrixShPtr am = dc->getAttributeMatrix(amName);
    UInt8ArrayType::Pointer dataAA = am->getAttributeArrayAs<UInt8ArrayType>(dataAAName);
    FloatArrayType::Pointer xAA = am->getAttributeArrayAs<FloatArrayType>(xAAName);
    FloatArrayType::Pointer yAA = am->getAttributeArrayAs<FloatArrayType>(yAAName);

    for(size_t pxlIdx = 0; pxlIdx < std::get<0>(dims) * std::get<1>(dims); ++pxlIdx)
    {
      auto x = xAA->getValue(pxlIdx);
      auto y = yAA->getValue(pxlIdx);
      auto v = dataAA->getValue(pxlIdx);
      img[std::make_pair(xAA->getValue(pxlIdx), yAA->getValue(pxlIdx))] = dataAA->getValue(pxlIdx);
    }
    return img;
  }

public:
  ImageImpl(const ImageImpl&) = default;
  ImageImpl(ImageImpl&&) = default;
  ImageImpl& operator=(const ImageImpl& other) = default;
  ImageImpl& operator=(ImageImpl&&) = default;

  ImageImpl(const Image& image)
  : m_image{image}
  {
    size_t maxX = 0;
    size_t maxY = 0;
    float x = 0;
    float y = 0;
    for(const auto& eachPixel : m_image)
    {
      x = eachPixel.first.first;
      y = eachPixel.first.second;
      maxX = roundf(x) > maxX ? roundf(x) : maxX;
      maxY = roundf(y) > maxY ? roundf(y) : maxY;
    }
    m_res_width = maxX + 1;
    m_res_height = maxY + 1;
  }

  ImageImpl(const DataContainerShPtr& dc, const QString& amName, const QString& dataAAName, const QString& xAAName, const QString& yAAName)
  : ImageImpl(DataContainerToImage(dc, amName, dataAAName, xAAName, yAAName))
  {
  }

  template <class T>
  static DataPairs<T> make_product(size_t listOneSize, size_t listOneStart, size_t listTwoSize, size_t listTwoStart)
  {
    DataPairs<T> product{};
    for(size_t listOneIndex = listOneStart; listOneIndex < listOneSize; listOneIndex++)
    {
      for(size_t listTwoIndex = listTwoStart; listTwoIndex < listTwoSize; listTwoIndex++)
      {
        product.push_back(std::make_pair(listTwoIndex, listOneIndex));
      }
    }
    return product;
  }

  template <class T>
  static DataPairs<T> make_product(size_t listOneSize, size_t listTwoSize)
  {
    return make_product<T>(listOneSize, 0, listTwoSize, 0);
  }

  std::pair<size_t, size_t> GetDimensions() const
  {
    return std::make_pair(m_res_width, m_res_height);
  }

  template <class T> static std::pair<T, T> NameToGridCoords(QString name)
  {
    // NOTE Ideally, 'R' and 'C' would be parameterized and maybe obtained from
    // the Blend filter class itself, but this feels like a violation of SOLID
    // Regardless, these values should be parameterized SOMEWHERE
    int cLength = name.size() - name.indexOf('C') - 1;
    T r = static_cast<T>(name.midRef(name.indexOf('R') + 1, name.size() - cLength - 2).toULong());
    T c = static_cast<T>(name.rightRef(cLength).toULong());

    return std::make_pair(r, c);
  }

  size_t NumPixels() const
  {
    return m_image.size();
  }

  bool PixelExists(PixelCoord pixelCoord) const
  {
    return m_image.find(pixelCoord) != m_image.end();
  }

  Image_T GetValue(PixelCoord pixelCoord) const
  {
    return m_image.at(pixelCoord);
  }

  static ImageGrid DataContainerArrayToImageGrid(DataContainerArrayShPtr dca, const QString& amName, const QString& dataAAName, const QString& xAAName, const QString& yAAName)
  {
    ImageGrid images{};
    for(const auto& eachDC : dca->getDataContainers())
    {
      images.insert_or_assign(NameToGridCoords<Cell_T>(eachDC->getName()), DataContainerToImage(eachDC, amName, dataAAName, xAAName, yAAName));
    }
    return images;
  }

  ImageImpl Transform(std::vector<float> a) const
  {
    const float tolerance = 0.05f;
    const float lastXIndex = m_res_width - 1 + tolerance;
    const float lastYIndex = m_res_height - 1 + tolerance;
    const size_t coeff_len = a.size() / 2;
    const size_t d = roundf(sqrt(coeff_len));
    const DataPairs<size_t> i_j = make_product<size_t>(d, d);

    const float x_trans = (m_res_width - 1) / 2.0f;
    const float y_trans = (m_res_height - 1) / 2.0f;
    float x = 0;
    float y = 0;
    float u_v = 0;
    float term = 0;

    std::pair<int, int> eachIJ{};

    Image distortedImage{};
    for(const auto& pixel : m_image)
    {
      x = x_trans;
      y = y_trans;
      for(size_t idx = 0; idx < a.size(); ++idx)
      {
        eachIJ = i_j[idx - (idx >= i_j.size() ? i_j.size() : 0)];

        u_v = pow((pixel.first.first - x_trans), eachIJ.first) * pow((pixel.first.second - y_trans), eachIJ.second);

        term = u_v * a[idx];
        idx < coeff_len ? x += term : y += term;
      }

      // This check effectively "clips" data
      if(x >= -tolerance && x <= lastXIndex && y >= -tolerance && y <= lastYIndex)
      {
        distortedImage[std::make_pair(roundf(x), roundf(y))] = this->m_image.at(pixel.first);
      }
    }
    return ImageImpl(distortedImage);
  }

  static void Print(const ImageGrid& imageGrid)
  {
    QString stream;
    // Get the number of rows in the grid - this should work as long as a map is used (highest R value will be last)
    Cell_T tileRows = imageGrid.rbegin()->first.first + 1;

    for(Cell_T tileRowIdx = 0; tileRowIdx < tileRows; ++tileRowIdx)
    {
      // Get the maximum height
      size_t max_height = 0;

      // Populate the images in this row
      std::vector<ImageArray> imageRow{};
      for(const auto& eachImage : imageGrid)
      {
        // Get the dimensions of the image
        size_t width = 0, height = 0;
        std::tie(width, height) = eachImage.second.GetDimensions();
        max_height = height > max_height ? height : max_height;
        if(eachImage.first.first == tileRowIdx)
        {
          ImageArray imgArray(height, std::vector<Image_T>(width, 0));
          for(const auto& eachPixel : eachImage.second.m_image)
          {
            // Use nearest-neighbor sampling to convert raw x, y pixels into integers
            // Iterate through all of the pixels in the image
            // When converting to array, pixel origin comes from lower-left
            // corner - and need translated to origin in upper-left corner
            imgArray[height - roundf(eachPixel.first.second) - 1][roundf(eachPixel.first.first)] = eachPixel.second;
          }
          imageRow.push_back(imgArray);
        }
      }

      for(size_t row_idx = 0; row_idx < max_height; ++row_idx)
      {
        for(const auto& eachImage : imageRow)
        {
          // This could result in misaligned images if all heights are not the same
          // Or in a worst-case, throw an out-of-index exception
          // FIXME Should instead have a more robust way of lining up rows
          // And skipping that image if it does not have the desired row_idx
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
    qDebug().noquote() << stream.trimmed();
  }
};

class BlendFilter::OverlapImpl
{
  std::pair<Cell_T, Cell_T> m_image1_key;
  std::pair<Cell_T, Cell_T> m_image2_key;
  OverlapPairs m_pairs;

public:
  OverlapImpl() = delete;
  OverlapImpl(const OverlapImpl&) = delete;
  OverlapImpl(OverlapImpl&&) = default;
  OverlapImpl& operator=(const OverlapImpl&) = delete;
  OverlapImpl& operator=(OverlapImpl&&) = delete;

  OverlapImpl(std::pair<Cell_T, Cell_T> image1Key, std::pair<Cell_T, Cell_T> image2Key, OverlapPairs pairs)
  {
    m_image1_key = image1Key;
    m_image2_key = image2Key;
    m_pairs = pairs;
  }

  OverlapPairs GetOverlapPairs() const
  {
    return m_pairs;
  }

  std::pair<Cell_T, Cell_T> Image1Key() const
  {
    return m_image1_key;
  }

  std::pair<Cell_T, Cell_T> Image2Key() const
  {
    return m_image2_key;
  }

  void Print(ImageGrid imageGrid) const
  {
    Image img1{}, img2{};
    Pixel_T lowestImage1X = m_pairs.begin()->first.first;
    Pixel_T lowestImage1Y = m_pairs.begin()->first.second;
    Pixel_T lowestImage2X = m_pairs.begin()->second.first;
    Pixel_T lowestImage2Y = m_pairs.begin()->second.second;
    for(const auto& eachPixel : m_pairs)
    {
      lowestImage1X = eachPixel.first.first < lowestImage1X ? eachPixel.first.first : lowestImage1X;
      lowestImage1Y = eachPixel.first.second < lowestImage1Y ? eachPixel.first.second : lowestImage1Y;
      lowestImage2X = eachPixel.second.first < lowestImage2X ? eachPixel.second.first : lowestImage2X;
      lowestImage2Y = eachPixel.second.second < lowestImage2Y ? eachPixel.second.second : lowestImage2Y;
    }

    for(const auto& eachPixel : m_pairs)
    {
      img1.insert_or_assign({eachPixel.first.first - lowestImage1X, eachPixel.first.second - lowestImage1Y}, imageGrid.at(m_image1_key).GetValue(eachPixel.first));
      img2.insert_or_assign({eachPixel.second.first - lowestImage2X, eachPixel.second.second - lowestImage2Y}, imageGrid.at(m_image2_key).GetValue(eachPixel.second));
    }

    ImageGrid overlapGrid;
    overlapGrid.insert_or_assign(this->m_image1_key, ImageImpl{img1});
    overlapGrid.insert_or_assign(this->m_image2_key, ImageImpl{img2});
    ImageImpl::Print(overlapGrid);
  }

  static std::vector<OverlapImpl> DetermineOverlaps(const ImageGrid& imageGrid,  const float overlapPercentage)
  {
    std::vector<OverlapImpl> overlaps{};
    size_t width, height, overlapDim, r, c;

    for(const auto& eachImage : imageGrid)
    {
      std::tie(width, height) = eachImage.second.GetDimensions();

      r = eachImage.first.first;
      c = eachImage.first.second;
      auto rightImage{imageGrid.find(std::make_pair(r, c + 1))};
      auto bottomImage{imageGrid.find(std::make_pair(r + 1, c))};

      if(rightImage != imageGrid.end())
      {
        OverlapPairs coords;
        overlapDim = static_cast<size_t>(width * overlapPercentage);
        for(size_t eachX = width - overlapDim; eachX < width; ++eachX)
        {
          for(size_t eachY = 0; eachY < height; ++eachY)
          {
            coords.insert({std::make_pair(eachX, eachY), std::make_pair(eachX + overlapDim - width, eachY)});
          }
        }
        overlaps.push_back(OverlapImpl(std::make_pair(r, c), std::make_pair(r, c + 1), coords));
      }
      if(bottomImage != imageGrid.end())
      {
        OverlapPairs coords;
        overlapDim = static_cast<size_t>(height * overlapPercentage);
        for(size_t eachX = 0; eachX < width; ++eachX)
        {
          for(size_t eachY = height - overlapDim; eachY < height; ++eachY)
          {
            coords.insert({std::make_pair(eachX, eachY + overlapDim - height), std::make_pair(eachX, eachY)});
          }
        }
        overlaps.push_back(OverlapImpl(std::make_pair(r, c), std::make_pair(r + 1, c), coords));
      }
    }
    return overlaps;
  }
};

class SimpleCostFunction : public itk::SingleValuedCostFunction
{
  // A simple cost function that whose minimum should be at (-B/2, C)
  static const int8_t m_StartValue = -10;
  static const int8_t m_EndValue = 10;
  static const uint8_t m_NumValues = m_EndValue - m_StartValue;
  static const int8_t m_B = -2;
  static const int8_t m_C = 1;
  static constexpr double m_initialX = 7.0;

public:
  itkNewMacro(SimpleCostFunction);

  void GetDerivative(const ParametersType&, DerivativeType&) const override
  {
    throw std::exception("Derivatives are not implemented for the optimization type");
  }

  uint32_t GetNumberOfParameters() const override { return 1; }

  MeasureType GetValue(const ParametersType& parameters) const override
  {
    double x = parameters[0];
    MeasureType value = x * x + x * m_B + m_C;
    return value;
  }

  ParametersType GetInitialValues()
  {
    ParametersType initParams(GetNumberOfParameters());
    initParams[0] = m_initialX;
    return initParams;
  }
};

class FFTConvolutionCostFunction : public itk::SingleValuedCostFunction
{
  size_t m_degree = 2;
  std::vector<OverlapImpl> m_overlaps;
  ImageGrid m_imageGrid;

public:
  itkNewMacro(FFTConvolutionCostFunction);

  // TODO Set up constructors that will set class instances of the imageGrid,
  // the number of coefficients, and the overlaps
  // Or...just initialize everything with an initialize method

  void SetPolynomialDegree(size_t degree) { m_degree = degree; }

  void SetImageGrid(ImageGrid imageGrid) { m_imageGrid = imageGrid; }

  // TODO
  void SetOverlaps(std::vector<OverlapImpl> overlaps) { /*m_overlaps = overlaps;*/ }

  // TODO
  void GetDerivative(const ParametersType&, DerivativeType&) const override
  {
    // THROW AN EXCEPTION
  }

  // TODO
  uint32_t GetNumberOfParameters() const override { }

  ParametersType GetInitialValues()
  {
    const size_t ind_len = static_cast<size_t>(pow(m_degree, 2) + 2 * m_degree + 1);
    ParametersType params{2 * ind_len};
    float value = 0.0f;
    float increment = 0.1f;
    for(size_t x = 0; x < 2 * ind_len; ++x)
    {
      value = 0.0f;
      if(x == 1 || x == ind_len + 2)
      {
        value = increment;
      }
      if(x == 2)
      {
        value = increment;
      }
      if(x == ind_len + 1)
      {
        value = -increment;
      }
      params[x] = value;
    }
    return params;
  }

  // TODO Make this work as the call back function for the Amoeba Optimizer
  MeasureType GetValue(const ParametersType& parameters) const override
  {
    // Should be initialized to a very large number?
    MeasureType residual = 0.0f;
    // Convert the parameters into a vector that can be passed into an Image.Transform()
    Transform transform(parameters.size());
    for (const auto& eachParameter : parameters) {
      transform.push_back(eachParameter);
    }
    
    // Perform the distortion on all images in the imageGrid
    ImageGrid distortedGrid;
    for(const auto& eachImage : m_imageGrid) // TODO Parallelize this
    {
      distortedGrid.insert_or_assign(eachImage.first, eachImage.second.Transform(transform));
    }

    for(const auto& eachOverlap : m_overlaps) // TODO Parallelize this
    {
      auto found1 = distortedGrid.find(eachOverlap.Image1Key());
      auto found2 = distortedGrid.find(eachOverlap.Image2Key());

      // NOTE This should effectively be replaced by obtaining the
      // convolution of the FFT of two overlap regions
      // NOTE Or use this area to call a callback that's passed in as a parameter
      // to the Optimize function

      // TODO Assign the residual to some calculation done to the output
      // of the FFTConvolution filter
    }
    return residual;
  }
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Blend::Blend()
: m_OverlapMethod(0)
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Blend::~Blend() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::initialize()
{
  setErrorCondition(0, "");
  setWarningCondition(0, "");
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  //  MultiDataContainerSelectionFilterParameter::Pointer dcs{
  //    MultiDataContainerSelectionFilterParameter::New(
  //      "Chosen Data Containers", "ChosenDataContainers",
  //      getChosenDataContainers(), FilterParameter::Category::RequiredArray,
  //      std::bind(&Blend::setChosenDataContainers, this, std::placeholders::_1),
  //      std::bind(&Blend::getChosenDataContainers, this), {}
  //  )};

  IntFilterParameter::Pointer maxIterations{IntFilterParameter::New("Max Iterations", "MaxIterations", 1000, FilterParameter::Category::Parameter,
                                                                    std::bind(&Blend::setMaxIterations, this, std::placeholders::_1), std::bind(&Blend::getMaxIterations, this))};

  IntFilterParameter::Pointer degree{
      IntFilterParameter::New("Degree", "Degree", 1, FilterParameter::Category::Parameter, std::bind(&Blend::setDegree, this, std::placeholders::_1), std::bind(&Blend::getDegree, this))};

  LinkedChoicesFilterParameter::Pointer overlapMethod{LinkedChoicesFilterParameter::New("Overlap Method", "OverlapMethod", 0, FilterParameter::Parameter,
                                                                                        std::bind(&Blend::setOverlapMethod, this, std::placeholders::_1), std::bind(&Blend::getOverlapMethod, this),
                                                                                        {"Percent", "Origins"}, {"OverlapPercentage"})};

  FloatFilterParameter::Pointer overlapPercentage{FloatFilterParameter::New("Overlap Percentage", "OverlapPercentage", 0.0f, FilterParameter::Category::Parameter,
                                                                            std::bind(&Blend::setOverlapPercentage, this, std::placeholders::_1), std::bind(&Blend::getOverlapPercentage, this), 0)};

  StringFilterParameter::Pointer amName{StringFilterParameter::New("Attribute Matrix Name", "AttributeMatrixName", {}, FilterParameter::Category::Parameter,
                                                                   std::bind(&Blend::setAttributeMatrixName, this, std::placeholders::_1), std::bind(&Blend::getAttributeMatrixName, this), {})};

  StringFilterParameter::Pointer xAAName{StringFilterParameter::New("Attribute Array Name", "AttributeArrayName", {}, FilterParameter::Category::Parameter,
                                                                    std::bind(&Blend::setXAttributeArrayName, this, std::placeholders::_1), std::bind(&Blend::getXAttributeArrayName, this), {})};

  StringFilterParameter::Pointer yAAName{StringFilterParameter::New("Attribute Array Name", "AttributeArrayName", {}, FilterParameter::Category::Parameter,
                                                                    std::bind(&Blend::setYAttributeArrayName, this, std::placeholders::_1), std::bind(&Blend::getYAttributeArrayName, this), {})};

  StringFilterParameter::Pointer dataAAName{StringFilterParameter::New("Attribute Array Name", "AttributeArrayName", {}, FilterParameter::Category::Parameter,
                                                                       std::bind(&Blend::setDataAttributeArrayName, this, std::placeholders::_1), std::bind(&Blend::getDataAttributeArrayName, this),
                                                                       {})};

  //  parameters.push_back(dcs);
  parameters.push_back(maxIterations);
  parameters.push_back(degree);
  parameters.push_back(overlapMethod);
  parameters.push_back(overlapPercentage);
  parameters.push_back(amName);
  parameters.push_back(xAAName);
  parameters.push_back(yAAName);
  parameters.push_back(dataAAName);

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::dataCheck()
{
  setErrorCondition(0, "");
  setWarningCondition(0, "");
  //  OverlapMethod overlapMethod = static_cast<OverlapMethod>(m_OverlapMethod);
  // Check bounds of m_overlapPercent
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck();                       // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted();          // We are done preflighting this filter
  setInPreflight(false);             // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void Blend::execute()
{
  initialize();
  dataCheck();
  if(getCancel())
  {
    return;
  }

  if(getWarningCode() < 0)
  {
    QString ss = QObject::tr("Some warning message");
    setWarningCondition(-88888888, ss);
    notifyStatusMessage(ss);
  }

  if(getErrorCode() < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999, ss);
    notifyStatusMessage(ss);
    return;
  }

  // These should probably be parametized
  const double lowTolerance = 0.01f;
  const double highTolerance = 0.01f;

  itk::AmoebaOptimizer::Pointer optimizer = itk::AmoebaOptimizer::New();
  optimizer->SetMaximumNumberOfIterations(m_MaxIterations);
  optimizer->SetFunctionConvergenceTolerance(lowTolerance);
  optimizer->SetParametersConvergenceTolerance(highTolerance);

  // This information will eventually reside in the cost function
  // i.e. The information used as arguments to these contructors should belong
  // in the constructor for the FFTConvolutionCostFunction
//  ImageGrid imageGrid{ImageImpl::DataContainerArrayToImageGrid(this->getDataContainerArray(), m_AttributeMatrixName, m_DataAttributeArrayName, m_XAttributeArrayName, m_YAttributeArrayName)};
//  std::vector<OverlapImpl> overlaps{OverlapImpl::DetermineOverlaps(imageGrid, m_OverlapPercentage)};

  // For now, the cost function is just going to be a simple 2nd order polynomial
  // And it skirts around any iterative transforms being done, saving on computation time
//  using CostFunctionType = FFTConvolutionCostFunction;
  using CostFunctionType = SimpleCostFunction;
  CostFunctionType implementation;
  optimizer->SetInitialPosition(implementation.GetInitialValues());
  optimizer->SetCostFunction(&implementation);

  // Start the optimization!
  optimizer->StartOptimization();

  // TODO Determine whether the solution truly converged

  // Optimization complete!
  // Get the transform and value following the optimization
  itk::AmoebaOptimizer::ParametersType a = optimizer->GetCurrentPosition();
  Transform out(a.Size());
  for (const auto& eachCoeff : a)
  {
    out.push_back(eachCoeff);
  }
  itk::AmoebaOptimizer::MeasureType value = optimizer->GetValue();

  // TODO Get number of iterations the optimizer executed

  // TODO
  // Set up new Attribute matrix with results of filter
  // Should contain the transformation matrix (a)
  // And a data array with a single element for the iterations run
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer Blend::newFilterInstance(bool copyFilterParameters) const
{
  Blend::Pointer filter = Blend::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return std::move(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getSubGroupName() const
{
  return "Processing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString Blend::getHumanLabel() const
{
  return "Blend";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid Blend::getUuid()
{
  return QUuid("{f2c81aa6-8edf-51d8-8c30-81ad983815e9}");
}
