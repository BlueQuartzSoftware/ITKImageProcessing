/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "IlluminationCorrection.h"

#include <cstring>
#include <numeric>
#include <set>
#include <thread>
#include <type_traits>

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>
#endif

#include <QtCore/QDir>
#include <QtCore/QMutexLocker>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QVector>

#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AttributeMatrixCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArrayCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/OutputPathFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Geometry/RectGridGeom.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageWriter.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKMedianImage.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagMapping.h"

#include <Eigen/Dense>

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  DataContainerID10 = 10,
  AttributeMatrixID20 = 20,
  DataArrayID30 = 30,
};

#define ZIF_PRINT_DBG_MSGS 0

namespace
{
const QString k_BackgroundDataContainerLabel("Created Data Container (Corrected)");
const QString k_BackgroundAttributeMatrixLabel("Created Attribute Matrix (Corrected)");
const QString k_BackgroundAttributeArrayLabel("Created Image Array Name (Corrected)");
const QString k_OutputProcessedImageLabel("Corrected Image Name");

template <typename OutArrayType, typename AccumType>
class ProcessInputImagesImpl
{
public:
  using AccumDataArrayType = DataArray<AccumType>;

  ProcessInputImagesImpl(IlluminationCorrection* filter, QString dcName, AccumType average, const AccumDataArrayType& accumArray)
  : m_Filter(filter)
  , m_DcName(std::move(dcName))
  , m_Average(average)
  , m_AccumArray(accumArray)
  {
  }
  ~ProcessInputImagesImpl() = default;
  ProcessInputImagesImpl(const ProcessInputImagesImpl&) = default;
  ProcessInputImagesImpl(ProcessInputImagesImpl&&) noexcept = default;
  ProcessInputImagesImpl& operator=(const ProcessInputImagesImpl&) = delete; // Copy Assignment Not Implemented
  ProcessInputImagesImpl& operator=(ProcessInputImagesImpl&&) = delete;      // Move Assignment Not Implemented

  void operator()() const
  {
    // std::cout << "Processing: " << m_DcName.toStdString() << std::endl;

    using OutputDataArrayType = DataArray<OutArrayType>;
    using OutputDataArrayPointerType = typename OutputDataArrayType::Pointer;

    DataContainer::Pointer dc = m_Filter->getDataContainerArray()->getDataContainer(m_DcName);
    AttributeMatrix::Pointer am = dc->getAttributeMatrix(m_Filter->getCellAttributeMatrixName());
    OutputDataArrayPointerType imageDataArrayPtr = am->getAttributeArrayAs<OutputDataArrayType>(m_Filter->getImageDataArrayName());
    OutputDataArrayPointerType correctedDataArrayPtr = am->getAttributeArrayAs<OutputDataArrayType>(m_Filter->getCorrectedImageDataArrayName());

    OutputDataArrayType& inputImage = *imageDataArrayPtr;
    OutputDataArrayType& correctedImage = *correctedDataArrayPtr;

    size_t totalPoints = inputImage.getNumberOfTuples();

    for(size_t t = 0; t < totalPoints; t++)
    {
      AccumType numerator = static_cast<AccumType>(inputImage[t]);
      AccumType denominator = m_AccumArray[t];

      AccumType temp = m_Average * numerator;
      if(denominator != 0)
      {
        temp = temp / denominator;
      }
      if(temp < 0)
      {
        temp = 0;
      }
      if(temp > std::numeric_limits<OutArrayType>::max())
      {
        temp = std::numeric_limits<OutArrayType>::max();
      }
      correctedImage[t] = temp;
    }
    if(m_Filter->getExportCorrectedImages())
    {
      ITKImageWriter::Pointer imageWriter = ITKImageWriter::New();
      imageWriter->setDataContainerArray(m_Filter->getDataContainerArray());
      QString outputPath = QString("%1/%2%3").arg(m_Filter->getOutputPath()).arg(m_DcName).arg(m_Filter->getFileExtension());
      imageWriter->setFileName(outputPath);
      DataArrayPath dap(m_DcName, m_Filter->getCellAttributeMatrixName(), m_Filter->getCorrectedImageDataArrayName());
      imageWriter->setImageArrayPath(dap);
      imageWriter->setPlane(0);

      imageWriter->execute();
      if(imageWriter->getErrorCode() < 0)
      {
        m_Filter->setErrorCondition(imageWriter->getErrorCode(), QString("%1 Filter could not write image to path '%2'").arg(imageWriter->ClassName()).arg(outputPath));
      }
    }
    m_Filter->notifyFeatureCompleted(m_DcName);
  }

private:
  IlluminationCorrection* m_Filter = nullptr;
  QString m_DcName;
  AccumType m_Average;
  const DataArray<AccumType>& m_AccumArray;
};

/**
 * @brief Calculates the output values using the templated output IDataArray output type
 */
template <typename OutArrayType, typename GeomType, typename AccumType>
void calculateOutputValues(IlluminationCorrection* filter)
{

  using OutputDataArrayType = DataArray<OutArrayType>;
  using OutputDataArrayPointerType = typename OutputDataArrayType::Pointer;

  using AccumDataArrayType = DataArray<AccumType>;

  DataContainerArray::Pointer dca = filter->getDataContainerArray();

  DataContainer::Pointer outputDc = dca->getDataContainer(filter->getBackgroundDataContainerPath());
  AttributeMatrix::Pointer outputAttrMat = outputDc->getAttributeMatrix(filter->getBackgroundCellAttributeMatrixPath());
  typename OutputDataArrayType::Pointer backgroundArrayPtr = outputAttrMat->getAttributeArrayAs<OutputDataArrayType>(filter->getBackgroundImageArrayPath().getDataArrayName());
  OutputDataArrayType& backgroundArray = *(backgroundArrayPtr);
  backgroundArray.initializeWithZeros();

  typename GeomType::Pointer outputGeom = outputDc->getGeometryAs<GeomType>();
  // SizeVec3Type dims = outputGeom->getDimensions();

  typename AccumDataArrayType::Pointer accumulateArrayPtr = AccumDataArrayType::CreateArray(backgroundArrayPtr->getNumberOfTuples(), "Accumulation Array", true);
  accumulateArrayPtr->initializeWithZeros();
  DataArray<AccumType>& accumArray = *accumulateArrayPtr;
  size_t numTuples = accumArray.getNumberOfTuples();

  typename SizeTArrayType::Pointer countArrayPtr = SizeTArrayType::CreateArray(backgroundArrayPtr->getNumberOfTuples(), "Count Array", true);
  countArrayPtr->initializeWithZeros();
  SizeTArrayType& counter = *(countArrayPtr);

  int32_t LowThreshold = filter->getLowThreshold();
  int32_t HighThreshold = filter->getHighThreshold();

  QString progressMessage = QString("Calculating Background Image...");
  filter->notifyStatusMessage(progressMessage);

  for(const auto& dcName : filter->getDataContainers())
  {
    DataArrayPath imageArrayPath(dcName, filter->getCellAttributeMatrixName(), filter->getImageDataArrayName());
    OutputDataArrayPointerType imageArrayPtr = dca->getAttributeMatrix(imageArrayPath)->getAttributeArrayAs<OutputDataArrayType>(imageArrayPath.getDataArrayName());
    OutputDataArrayType& imageArray = *imageArrayPtr;

    for(size_t t = 0; t < numTuples; t++)
    {
      if(imageArray[t] >= LowThreshold && imageArray[t] <= HighThreshold)
      {
        accumArray[t] += imageArray[t];
        counter[t]++;
      }
    }
  }

  // average the background values by the number of counts (counts will be the number of images unless the threshold
  // values do not include all the possible image values
  // (i.e. for an 8 bit image, if we only include values from 0 to 100, not every image value will be counted)
  for(int j = 0; j < numTuples; j++)
  {
    if(counter[j] > 0) // Guard against Divide by zero
    {
      accumArray[j] /= counter[j];
    }
  }

  // Median
  if(filter->getApplyMedianFilter())
  {
    QString progressMessage = QString("Applying Median Filter to Background Image...");
    filter->notifyStatusMessage(progressMessage);

    ITKMedianImage::Pointer imageProcessingFilter = ITKMedianImage::New();
    DataContainerArray::Pointer dca = filter->getDataContainerArray();
    DataArrayPath outPath = filter->getBackgroundImageArrayPath();
    DataContainer::Pointer outDc = dca->getDataContainer(outPath.getDataContainerName());
    AttributeMatrix::Pointer outAm = outDc->getAttributeMatrix(outPath.getAttributeMatrixName());
    IDataArray::Pointer outArray = outAm->removeAttributeArray(outPath.getDataArrayName());
    outAm->addOrReplaceAttributeArray(accumulateArrayPtr);

    imageProcessingFilter->setDataContainerArray(filter->getDataContainerArray());
    outPath.setDataArrayName(accumulateArrayPtr->getName());

    imageProcessingFilter->setSelectedCellArrayPath(outPath);
    imageProcessingFilter->setSaveAsNewArray(false);
    imageProcessingFilter->setRadius(filter->getMedianRadius());
    imageProcessingFilter->execute();
    outAm->addOrReplaceAttributeArray(outArray); // Put the original back into the Attr Mat

  } // Median

  // Assign output array values
  for(int i = 0; i < numTuples; ++i)
  {
    backgroundArray[i] = static_cast<OutArrayType>(accumArray[i]);
  }

  AccumType average = std::accumulate(accumArray.begin(), accumArray.end(), static_cast<AccumType>(0)) / accumArray.getNumberOfTuples();

  // Apply Correction
  if(filter->getApplyCorrection())
  {
    QString progressMessage = QString("Generating Corrected Images...");
    filter->notifyStatusMessage(progressMessage);

#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
    tbb::task_scheduler_init init;
    std::shared_ptr<tbb::task_group> g(new tbb::task_group);
    // C++11 RIGHT HERE....
    int32_t nthreads = static_cast<int32_t>(std::thread::hardware_concurrency()); // Returns ZERO if not defined on this platform
    int32_t threadCount = 0;
#endif

    for(const auto& dcName : filter->getDataContainers())
    {
#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
      g->run(ProcessInputImagesImpl<OutArrayType, AccumType>(filter, dcName, average, accumArray));
      threadCount++;
      if(threadCount == nthreads)
      {
        g->wait();
        threadCount = 0;
      }
#else
      ProcessInputImagesImpl<OutArrayType, AccumType> impl(filter, dcName, average, accumArray);
      impl();
#endif
    }
#ifdef SIMPL_USE_PARALLEL_ALGORITHMS
    // This will spill over if the number of files to process does not divide evenly by the number of threads.
    g->wait();
#endif

  } // Apply Correction
}

} // namespace

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IlluminationCorrection::IlluminationCorrection()
: m_DataContainers("")
, m_CellAttributeMatrixName(ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName)
, m_ImageDataArrayName(ITKImageProcessing::Montage::k_TileDataArrayDefaultName)
, m_CorrectedImageDataArrayName(ITKImageProcessing::Montage::k_TileCorrectedDefaultName)
, m_ExportCorrectedImages(false)
, m_OutputPath("")
, m_FileExtension(".tif")
, m_BackgroundDataContainerPath(ITKImageProcessing::Montage::k_BackgroundDataContainerDefaultName)
, m_BackgroundCellAttributeMatrixPath(ITKImageProcessing::Montage::k_BackgroundDataContainerDefaultName, ITKImageProcessing::Montage::k_BackgroundAttributeMatrixDefaultName, "")
, m_BackgroundImageArrayPath(ITKImageProcessing::Montage::k_BackgroundDataContainerDefaultName, ITKImageProcessing::Montage::k_BackgroundAttributeMatrixDefaultName,
                             ITKImageProcessing::Montage::k_BackgroundDataArrayDefaultName)
, m_LowThreshold(20000)
, m_HighThreshold(65535)
, m_ApplyCorrection(false)
{
  m_ApplyMedianFilter = true;
  m_MedianRadius = {10.0f, 10.0f, 1.0f};
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IlluminationCorrection::~IlluminationCorrection() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void IlluminationCorrection::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  MultiDataContainerSelectionFilterParameter::RequirementType req;
  req.dcGeometryTypes.push_back(IGeometry::Type::Image);
  req.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  parameters.push_back(SIMPL_NEW_MDC_SELECTION_FP("Select Image Data Containers", DataContainers, FilterParameter::Parameter, IlluminationCorrection, req));
  parameters.push_back(SIMPL_NEW_STRING_FP("Input Attribute Matrix Name", CellAttributeMatrixName, FilterParameter::RequiredArray, IlluminationCorrection));
  parameters.push_back(SIMPL_NEW_STRING_FP("Input Image Array Name", ImageDataArrayName, FilterParameter::RequiredArray, IlluminationCorrection));
  parameters.push_back(SIMPL_NEW_STRING_FP(::k_OutputProcessedImageLabel, CorrectedImageDataArrayName, FilterParameter::CreatedArray, IlluminationCorrection));

  parameters.push_back(SeparatorFilterParameter::New("Created Background Image Name", FilterParameter::CreatedArray));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP(::k_BackgroundDataContainerLabel, BackgroundDataContainerPath, FilterParameter::CreatedArray, IlluminationCorrection));
  AttributeMatrixCreationFilterParameter::RequirementType cellAmReq;
  cellAmReq.dcGeometryTypes.push_back(IGeometry::Type::Image);
  cellAmReq.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  parameters.push_back(SIMPL_NEW_AM_CREATION_FP(::k_BackgroundAttributeMatrixLabel, BackgroundCellAttributeMatrixPath, FilterParameter::CreatedArray, IlluminationCorrection, cellAmReq));

  DataArrayCreationFilterParameter::RequirementType imageReq;
  imageReq.dcGeometryTypes.push_back(IGeometry::Type::Image);
  imageReq.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  imageReq.amTypes.push_back(AttributeMatrix::Type::Cell);
  parameters.push_back(SIMPL_NEW_DA_CREATION_FP(::k_BackgroundAttributeArrayLabel, BackgroundImageArrayPath, FilterParameter::CreatedArray, IlluminationCorrection, imageReq));

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Lowest allowed Image value (Image Value)", LowThreshold, FilterParameter::Parameter, IlluminationCorrection));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Highest allowed Image value (Image Value)", HighThreshold, FilterParameter::Parameter, IlluminationCorrection));

  QStringList linkedProps;

  parameters.push_back(SeparatorFilterParameter::New("Background Image Processing", FilterParameter::Parameter));
  linkedProps.clear();
  linkedProps << "MedianRadius";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Apply median filter to background image", ApplyMedianFilter, FilterParameter::Parameter, IlluminationCorrection, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("MedianRadius", MedianRadius, FilterParameter::Parameter, IlluminationCorrection));

  parameters.push_back(SeparatorFilterParameter::New("Process Input Images", FilterParameter::Parameter));
  linkedProps.clear();
  linkedProps << "CorrectedImageDataArrayName"
              << "ExportCorrectedImages";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Apply Background Correction to Input Images", ApplyCorrection, FilterParameter::Parameter, IlluminationCorrection, linkedProps));

  linkedProps.clear();
  linkedProps << "OutputPath"
              << "FileExtension";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Export Corrected Images", ExportCorrectedImages, FilterParameter::Parameter, IlluminationCorrection, linkedProps));
  parameters.push_back(SIMPL_NEW_OUTPUT_PATH_FP("Output Path", OutputPath, FilterParameter::Parameter, IlluminationCorrection, "*", "*", 0));
  parameters.push_back(SIMPL_NEW_STRING_FP("File Extension", FileExtension, FilterParameter::Parameter, IlluminationCorrection, 0));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void IlluminationCorrection::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IlluminationCorrection::ArrayType IlluminationCorrection::getArrayType()
{
  DataContainerArray::Pointer dca = getDataContainerArray();
  const std::vector<size_t> cDims = {1};

  for(const auto& dcName : m_DataContainers)
  {
    DataArrayPath imageArrayPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);
    if(!getDataContainerArray()->doesAttributeArrayExist(imageArrayPath))
    {
      QString msg = QString("The Attribute Array path that was generated does not point to an existing Attribute Array. Please set the 'Input Attribute Matrix Name' and 'Input Image Array Name' to "
                            "values that will generate a valid path. Current path generated is %2")
                        .arg(imageArrayPath.serialize());
      setErrorCondition(-53000, msg);
      continue;
    }
    AttributeMatrix::Pointer am = dca->getAttributeMatrix(imageArrayPath);

    IDataArray::Pointer da = am->getChildByName(m_ImageDataArrayName);
    if(da->getComponentDimensions() != cDims)
    {
      QString msg;
      QTextStream out(&msg);
      out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not single-component (Grayscale) data. Please select a pattern of AttributeArray Paths that are gray scale images";
      setErrorCondition(-53000, msg);
      return ArrayType::Error;
    }
    QString typeString = da->getTypeAsString();
    if("uint8_t" == typeString)
    {
      return ArrayType::UInt8;
    }
    if("uint16_t" == typeString)
    {
      return ArrayType::UInt16;
    }
    if("float" == typeString)
    {
      return ArrayType::Float32;
    }

    QString msg;
    QTextStream out(&msg);
    out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not of the appropriate type. Please select a pattern of AttributeArray Paths that are gray scale images";
    setErrorCondition(-53004, msg);
    return ArrayType::Error;
  }
  return ArrayType::Error;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IlluminationCorrection::GeomType IlluminationCorrection::getGeomType()
{
  DataContainerArray::Pointer dca = getDataContainerArray();
  for(const auto& dcName : m_DataContainers)
  {
    IGeometryGrid::Pointer gridGeom = dca->getDataContainer(dcName)->getGeometryAs<IGeometryGrid>();
    if(nullptr == gridGeom)
    {
      QString msg;
      QTextStream out(&msg);
      out << "DataContainer: " << dcName << " needs to have an IGeometryGrid assigned. There is either no geometry assign to the Data Container or the Geometry is not of type IGeometryGrid.";
      setErrorCondition(-53001, msg);
      return GeomType::Error;
    }
    switch(gridGeom->getGeometryType())
    {
    case IGeometry::Type::Image:
      return GeomType::ImageGeom;
    case IGeometry::Type::RectGrid:
      return GeomType::RectGridGeom;
    default:
      setErrorCondition(-53002, "Invalid Geometry type detected.  An ImageGeometry or RectGridGeometry is required for incoming data.");
      return GeomType::Error;
    }
  }
  return GeomType::Error;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void IlluminationCorrection::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  // Check for empty list. If list is empty then the OutputGeometry was never formed and it wont help to go on..
  if(m_DataContainers.isEmpty())
  {
    setErrorCondition(-53006, "At least one DataContainer must be selected.");
    return;
  }

  if(m_CellAttributeMatrixName.isEmpty())
  {
    setErrorCondition(-53007, "The 'Input Attribute Matrix Name' must contain a valid name. Each Data Container should contain an AttributeMatrix of type Cell with this name.");
    return;
  }

  if(m_ImageDataArrayName.isEmpty())
  {
    setErrorCondition(-53008, "The 'Input Image Array Name' must contain a valid name. The Attribute Matrix should have an Attribute Array of the given name.");
    return;
  }

  if(m_CorrectedImageDataArrayName.isEmpty())
  {
    setErrorCondition(-53009, "The 'Corrected Output Array Name' must contain a valid name. The Attribute Matrix should have an Attribute Array of the given name.");
    return;
  }

  DataContainerArray::Pointer dca = getDataContainerArray();
  std::vector<size_t> cDims = {1};

  // CheckInputArrays() templated on array and geometry types.
  ArrayType arrayType = getArrayType();
  if(arrayType == IlluminationCorrection::ArrayType::Error)
  {
    return;
  }
  GeomType geomType = getGeomType();
  if(geomType == IlluminationCorrection::GeomType::Error)
  {
    return;
  }

  if(arrayType == ArrayType::UInt8 && (m_LowThreshold > std::numeric_limits<uint8_t>::max() || m_HighThreshold > std::numeric_limits<uint8_t>::max()))
  {
    setWarningCondition(53000, "Either the Lower or Upper threshold values are larger than the larges possibly value for UInt8. Valid values are between [0, 255]");
  }
  if(arrayType == ArrayType::UInt16 && (m_LowThreshold > std::numeric_limits<uint16_t>::max() || m_HighThreshold > std::numeric_limits<uint16_t>::max()))
  {
    setWarningCondition(53001, "Either the Lower or Upper threshold values are larger than the larges possibly value for UInt8. Valid values are between [0, 65535]");
  }
  if(m_LowThreshold > m_HighThreshold)
  {
    setErrorCondition(-53030, "The lower threshold is greater than the upper threshold.");
  }

  // Create all the 'Corrected Input Images'
  if(getApplyCorrection())
  {
    for(const auto& dcName : m_DataContainers)
    {
      DataContainer::Pointer dc = dca->getDataContainer(dcName);
      if(nullptr != dc)
      {
        AttributeMatrix::Pointer am = dc->getAttributeMatrix(getCellAttributeMatrixName());
        if(nullptr != am)
        {
          switch(arrayType)
          {
          case ArrayType::UInt8:
            am->createNonPrereqArray<UInt8ArrayType>(this, getCorrectedImageDataArrayName(), 0, {1});
            break;
          case ArrayType::UInt16:
            am->createNonPrereqArray<UInt16ArrayType>(this, getCorrectedImageDataArrayName(), 0, {1});
            break;
          case ArrayType::Float32:
            am->createNonPrereqArray<FloatArrayType>(this, getCorrectedImageDataArrayName(), 0, {1});
            break;
          default:
            setErrorCondition(-53010, "Could create the corrected image Attribute Array because the Input array type is not UInt8, UInt16 or Float");
            break;
          }
        }
      }
    }
  }

  if(m_ExportCorrectedImages)
  {
    if(m_OutputPath.isEmpty())
    {
      QString ss = QObject::tr("The output path must be set");
      setErrorCondition(-53011, ss);
      return;
    }
    if(m_OutputPath.endsWith("/"))
    {
      m_OutputPath.chop(1);
    }

    if(m_FileExtension.isEmpty())
    {
      QString ss = QObject::tr("The file extension must be set.");
      setErrorCondition(-53012, ss);
      return;
    }
  }
  IGeometryGrid::Pointer outputGridGeom = checkInputArrays(arrayType, geomType);

  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainer::Pointer outputDc = dca->createNonPrereqDataContainer(this, getBackgroundDataContainerPath(), DataContainerID10);
  if(getErrorCode() < 0)
  {
    return;
  }

  if(m_BackgroundDataContainerPath.getDataContainerName() != m_BackgroundCellAttributeMatrixPath.getDataContainerName())
  {
    setErrorCondition(
        -53009,
        QString("The '%1', '%2' and '%3' must all have the same Data Container.").arg(::k_BackgroundDataContainerLabel).arg(::k_BackgroundAttributeMatrixLabel).arg(::k_BackgroundAttributeArrayLabel));
    return;
  }

  outputDc->setGeometry(outputGridGeom);

  std::vector<size_t> tDims = outputGridGeom->getDimensions().toContainer<std::vector<size_t>>();
  AttributeMatrix::Pointer outputAttrMat = outputDc->createNonPrereqAttributeMatrix(this, getBackgroundCellAttributeMatrixPath(), tDims, AttributeMatrix::Type::Cell, AttributeMatrixID20);

  if(m_BackgroundCellAttributeMatrixPath.getAttributeMatrixName() != m_BackgroundImageArrayPath.getAttributeMatrixName())
  {
    setErrorCondition(-53009, QString("The '%1' and '%2' must all have the same Attribute Matrix.").arg(::k_BackgroundAttributeMatrixLabel).arg(::k_BackgroundAttributeArrayLabel));
    return;
  }

  switch(arrayType)
  {
  case ArrayType::UInt8:
    outputAttrMat->createNonPrereqArray<UInt8ArrayType>(this, m_BackgroundImageArrayPath.getDataArrayName(), 0, cDims, DataArrayID30);
    break;
  case ArrayType::UInt16:
    outputAttrMat->createNonPrereqArray<UInt16ArrayType>(this, m_BackgroundImageArrayPath.getDataArrayName(), 0, cDims, DataArrayID30);
    break;
  case ArrayType::Float32:
    outputAttrMat->createNonPrereqArray<FloatArrayType>(this, m_BackgroundImageArrayPath.getDataArrayName(), 0, cDims, DataArrayID30);
    break;
  default:
    setErrorCondition(-53006, "A valid Attribute Array type (UInt8, UInt16, or Float) is required for this filter.");
    break;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void IlluminationCorrection::preflight()
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
void IlluminationCorrection::execute()
{
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCode() < 0)
  {
    return;
  }
  if(m_ExportCorrectedImages)
  {
    QString ss;
    QDir dir;
    if(!dir.mkpath(m_OutputPath))
    {
      ss = QObject::tr("Error creating output path '%1'").arg(m_OutputPath);
      setErrorCondition(-53014, ss);
      return;
    }

    if(!m_FileExtension.startsWith(".")) // if no '.', add '.' to file extension
    {
      m_FileExtension = "." + m_FileExtension;
    }
  }

  ArrayType arrayType = getArrayType();
  GeomType geomType = getGeomType();
  calculateOutputValues(arrayType, geomType);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IGeometryGrid::Pointer IlluminationCorrection::checkInputArrays(ArrayType arrayType, GeomType geomType)
{
  if(geomType != GeomType::ImageGeom && geomType != GeomType::RectGridGeom)
  {
    setErrorCondition(-53005, "A valid geometry type (ImageGeom or RectGridGeom) is required for this filter.");
    return IGeometryGrid::NullPointer();
  }

  if(arrayType == ArrayType::UInt8 && geomType == GeomType::ImageGeom)
  {
    return checkInputArrays<UInt8ArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::UInt16 && geomType == GeomType::ImageGeom)
  {
    return checkInputArrays<UInt16ArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::Float32 && geomType == GeomType::ImageGeom)
  {
    return checkInputArrays<FloatArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::UInt8 && geomType == GeomType::RectGridGeom)
  {
    return checkInputArrays<UInt8ArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::UInt16 && geomType == GeomType::RectGridGeom)
  {
    return checkInputArrays<UInt16ArrayType, ImageGeom>();
  }
  if(arrayType == ArrayType::Float32 && geomType == GeomType::RectGridGeom)
  {
    return checkInputArrays<FloatArrayType, RectGridGeom>();
  }

  return IGeometryGrid::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void IlluminationCorrection::calculateOutputValues(ArrayType arrayType, GeomType geomType)
{

  if(geomType != GeomType::ImageGeom && geomType != GeomType::RectGridGeom)
  {
    setErrorCondition(-53005, "A valid geometry type (ImageGeom or RectGridGeom) is required for this filter.");
    return;
  }

  if(arrayType == ArrayType::UInt8 && geomType == GeomType::ImageGeom)
  {
    ::calculateOutputValues<uint8_t, ImageGeom, uint64_t>(this);
  }
  if(arrayType == ArrayType::UInt16 && geomType == GeomType::ImageGeom)
  {
    ::calculateOutputValues<uint16_t, ImageGeom, uint64_t>(this);
  }
  if(arrayType == ArrayType::Float32 && geomType == GeomType::ImageGeom)
  {
    ::calculateOutputValues<float, ImageGeom, double>(this);
  }
  if(arrayType == ArrayType::UInt8 && geomType == GeomType::RectGridGeom)
  {
    ::calculateOutputValues<uint8_t, RectGridGeom, uint64_t>(this);
  }
  if(arrayType == ArrayType::UInt16 && geomType == GeomType::RectGridGeom)
  {
    ::calculateOutputValues<uint16_t, RectGridGeom, uint64_t>(this);
  }
  if(arrayType == ArrayType::Float32 && geomType == GeomType::RectGridGeom)
  {
    ::calculateOutputValues<float, RectGridGeom, double>(this);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void IlluminationCorrection::notifyFeatureCompleted(const QString& dcName)
{
  QMutexLocker locker(&m_NotifyMessage);
  QString ss = QObject::tr("%1 Correction Completed").arg(dcName);
  notifyStatusMessage(ss);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer IlluminationCorrection::newFilterInstance(bool copyFilterParameters) const
{
  IlluminationCorrection::Pointer filter = IlluminationCorrection::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString IlluminationCorrection::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString IlluminationCorrection::getBrandingString() const
{
  return ITKImageProcessingConstants::ITKImageProcessingPluginDisplayName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString IlluminationCorrection::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString IlluminationCorrection::getGroupName() const
{
  return SIMPL::FilterGroups::ProcessingFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString IlluminationCorrection::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::ImageFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString IlluminationCorrection::getHumanLabel() const
{
  return "ITK::Illumination Correction";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid IlluminationCorrection::getUuid()
{
  return QUuid("{a48f7a51-0ca9-584f-a0ca-4bfebdc41d7c}");
}
