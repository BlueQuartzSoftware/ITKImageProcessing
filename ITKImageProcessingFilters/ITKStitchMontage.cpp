/* ============================================================================
 * Copyright (c) 2009-2016 BlueQuartz Software, LLC
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
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
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
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <memory>

#include "ITKStitchMontage.h"

#include <sstream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/Common/TemplateHelpers.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/itkDream3DFilterInterruption.h"
#include "SIMPLib/ITK/itkDream3DITransformContainerToTransform.h"
#include "SIMPLib/ITK/itkDream3DTransformContainerToTransform.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"
#include "SIMPLib/ITK/itkProgressObserver.hpp"
#include "SIMPLib/ITK/itkTransformToDream3DITransformContainer.h"
#include "SIMPLib/ITK/itkTransformToDream3DTransformContainer.h"
#include "SIMPLib/SIMPLibVersion.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/MontageImportHelper.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

#include "util/MontageImportHelper.h"

#include "itkImageFileWriter.h"
#include "itkStreamingImageFilter.h"
#include "itkTileMergeImageFilter.h"
#include "itkTileMontage.h"
#include "itkTxtTransformIOFactory.h"

#define EXECUTE_STITCH_FUNCTION_TEMPLATE_HELPER(DATATYPE, filter, call, inputData, ...)                                                                                                                \
  int numOfComponents = inputData->getNumberOfComponents();                                                                                                                                            \
  if(numOfComponents == 3)                                                                                                                                                                             \
  {                                                                                                                                                                                                    \
    call<itk::RGBPixel<DATATYPE>, itk::RGBPixel<uint64_t>>(__VA_ARGS__);                                                                                                                               \
  }                                                                                                                                                                                                    \
  else if(numOfComponents == 4)                                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    call<itk::RGBAPixel<DATATYPE>, itk::RGBAPixel<uint64_t>>(__VA_ARGS__);                                                                                                                             \
  }                                                                                                                                                                                                    \
  else if(numOfComponents == 1)                                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    call<DATATYPE, uint64_t>(__VA_ARGS__);                                                                                                                                                             \
  }                                                                                                                                                                                                    \
  else                                                                                                                                                                                                 \
  {                                                                                                                                                                                                    \
    filter->setErrorCondition(TemplateHelpers::Errors::UnsupportedImageType, "The input array's image type is not recognized.  Supported image types"                                                  \
                                                                             " are grayscale (1-component), RGB (3-component), and RGBA (4-component)");                                               \
  }

#define EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(DATATYPE, filter, call, inputData, ...)                                                                                                               \
  int numOfComponents = inputData->getNumberOfComponents();                                                                                                                                            \
  if(numOfComponents == 3)                                                                                                                                                                             \
  {                                                                                                                                                                                                    \
    EXECUTE_STITCH_FUNCTION_TEMPLATE_HELPER(DATATYPE, filter, call, inputData, __VA_ARGS__);                                                                                                           \
  }                                                                                                                                                                                                    \
  else if(numOfComponents == 4)                                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    EXECUTE_STITCH_FUNCTION_TEMPLATE_HELPER(DATATYPE, filter, call, inputData, __VA_ARGS__);                                                                                                           \
  }                                                                                                                                                                                                    \
  else if(numOfComponents == 1)                                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    EXECUTE_STITCH_FUNCTION_TEMPLATE_HELPER(DATATYPE, filter, call, inputData, __VA_ARGS__);                                                                                                           \
  }                                                                                                                                                                                                    \
  else                                                                                                                                                                                                 \
  {                                                                                                                                                                                                    \
    filter->setErrorCondition(TemplateHelpers::Errors::UnsupportedImageType, "The input array's image type is not recognized.  Supported image types"                                                  \
                                                                             " are grayscale (1-component), RGB (3-component), and RGBA (4-component)");                                               \
  }

#define EXECUTE_DATATYPE_FUNCTION_TEMPLATE(filter, call, inputData, ...)                                                                                                                               \
  if(TemplateHelpers::CanDynamicCast<FloatArrayType>()(inputData))                                                                                                                                     \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(float, filter, call, inputData, __VA_ARGS__);                                                                                                             \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<DoubleArrayType>()(inputData))                                                                                                                               \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(double, filter, call, inputData, __VA_ARGS__);                                                                                                            \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<Int8ArrayType>()(inputData))                                                                                                                                 \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(int8_t, filter, call, inputData, __VA_ARGS__);                                                                                                            \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<UInt8ArrayType>()(inputData))                                                                                                                                \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(uint8_t, filter, call, inputData, __VA_ARGS__);                                                                                                           \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<Int16ArrayType>()(inputData))                                                                                                                                \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(int16_t, filter, call, inputData, __VA_ARGS__);                                                                                                           \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<UInt16ArrayType>()(inputData))                                                                                                                               \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(uint16_t, filter, call, inputData, __VA_ARGS__);                                                                                                          \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<Int32ArrayType>()(inputData))                                                                                                                                \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(int32_t, filter, call, inputData, __VA_ARGS__);                                                                                                           \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<UInt32ArrayType>()(inputData))                                                                                                                               \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(uint32_t, filter, call, inputData, __VA_ARGS__);                                                                                                          \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<Int64ArrayType>()(inputData))                                                                                                                                \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(int64_t, filter, call, inputData, __VA_ARGS__);                                                                                                           \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<UInt64ArrayType>()(inputData))                                                                                                                               \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(uint64_t, filter, call, inputData, __VA_ARGS__);                                                                                                          \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<BoolArrayType>()(inputData))                                                                                                                                 \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(bool, filter, call, inputData, __VA_ARGS__);                                                                                                              \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<DataArray<size_t>>()(inputData))                                                                                                                             \
  {                                                                                                                                                                                                    \
    EXECUTE_ACCUMULATETYPE_FUNCTION_TEMPLATE(size_t, filter, call, inputData, __VA_ARGS__);                                                                                                            \
  }                                                                                                                                                                                                    \
  else                                                                                                                                                                                                 \
  {                                                                                                                                                                                                    \
    filter->setErrorCondition(TemplateHelpers::Errors::UnsupportedDataType, "The input array's data type is not supported");                                                                           \
  }

#define EXECUTE_STITCH_FUNCTION_TEMPLATE(filter, call, inputData, ...) EXECUTE_DATATYPE_FUNCTION_TEMPLATE(filter, call, inputData, __VA_ARGS__)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKStitchMontage::ITKStitchMontage()
: m_MontageStart(IntVec2Type(0, 0))
, m_MontageEnd(IntVec2Type(0, 0))
, m_DataContainerPrefix(ITKImageProcessing::Montage::k_DataContainerPrefixDefaultName)
, m_CommonAttributeMatrixName(ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName)
, m_CommonDataArrayName(ITKImageProcessing::Montage::k_TileDataArrayDefaultName)
, m_MontageDataContainerName(ITKImageProcessing::Montage::k_MontageDataContainerDefaultName)
, m_MontageAttributeMatrixName(ITKImageProcessing::Montage::k_MontageAttributeMatrixDefaultName)
, m_MontageDataArrayName(ITKImageProcessing::Montage::k_MontageDataArrayDefaultName)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKStitchMontage::~ITKStitchMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
  m_ImageDataContainers.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Start (Col, Row) [Inclusive, Zero Based]", MontageStart, FilterParameter::Parameter, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage End (Col, Row) [Inclusive, Zero Based]", MontageEnd, FilterParameter::Parameter, ITKStitchMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container Prefix", DataContainerPrefix, FilterParameter::RequiredArray, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Attribute Matrix", CommonAttributeMatrixName, FilterParameter::RequiredArray, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Data Array", CommonDataArrayName, FilterParameter::RequiredArray, ITKStitchMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Data Container Name", MontageDataContainerName, FilterParameter::CreatedArray, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Attribute Matrix Name", MontageAttributeMatrixName, FilterParameter::CreatedArray, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Montage Data Array Name", MontageDataArrayName, FilterParameter::CreatedArray, ITKStitchMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  if(getDataContainerPrefix().isEmpty())
  {
    QString ss = QObject::tr("Data Container Prefix is empty.");
    setErrorCondition(-11000, ss);
    return;
  }

  if(getCommonAttributeMatrixName().isEmpty())
  {
    QString ss = QObject::tr("Common Attribute Matrix is empty.");
    setErrorCondition(-11001, ss);
    return;
  }

  if(getCommonDataArrayName().isEmpty())
  {
    QString ss = QObject::tr("Common Data Array is empty.");
    setErrorCondition(-11002, ss);
    return;
  }

  if(m_MontageStart[0] > m_MontageEnd[0])
  {
    QString ss = QObject::tr("Montage Start Column (%1) must be equal or less than Montage End Column(%2)").arg(m_MontageStart[0]).arg(m_MontageEnd[0]);
    setErrorCondition(-11003, ss);
    return;
  }
  if(m_MontageStart[1] > m_MontageEnd[1])
  {
    QString ss = QObject::tr("Montage Start Row (%1) must be equal or less than Montage End Row(%2)").arg(m_MontageStart[1]).arg(m_MontageEnd[1]);
    setErrorCondition(-11004, ss);
    return;
  }
  if(m_MontageStart[0] < 0 || m_MontageEnd[0] < 0)
  {
    QString ss = QObject::tr("Montage Start Column (%1) and Montage End Column(%2) must be greater than Zero (0)").arg(m_MontageStart[0]).arg(m_MontageEnd[0]);
    setErrorCondition(-11005, ss);
    return;
  }
  if(m_MontageStart[1] < 0 || m_MontageEnd[1] < 0)
  {
    QString ss = QObject::tr("Montage Start Row (%1) and Montage End Row(%2) must be greater than Zero (0)").arg(m_MontageStart[1]).arg(m_MontageEnd[1]);
    setErrorCondition(-11006, ss);
    return;
  }

  m_ImageDataContainers.clear();

  DataContainerArray::Pointer dca = getDataContainerArray();

  IDataArray::Pointer tilePtr = IDataArray::NullPointer();
  ImageGeom::Pointer tileGeom = ImageGeom::NullPointer();
  std::vector<size_t> tileTupleDims;
  for(int32_t row = m_MontageStart[1]; row <= m_MontageEnd[1]; row++)
  {
    for(int32_t col = m_MontageStart[0]; col <= m_MontageEnd[0]; col++)
    {
      // Create our DataContainer Name using a Prefix and a rXXcYY format.
      QString dcName = MontageImportHelper::GenerateDataContainerName(getDataContainerPrefix(), m_MontageEnd, row, col);

      DataArrayPath testPath;
      testPath.setDataContainerName(dcName);
      testPath.setAttributeMatrixName(getCommonAttributeMatrixName());
      testPath.setDataArrayName(getCommonDataArrayName());

      DataContainer::Pointer dc = dca->getPrereqDataContainer(this, testPath);
      if(getErrorCode() < 0)
      {
        return;
      }
      int err = -11020;
      AttributeMatrix::Pointer am = getDataContainerArray()->getPrereqAttributeMatrixFromPath(this, testPath, err);
      if(getErrorCode() < 0)
      {
        return;
      }

      tileTupleDims = am->getTupleDimensions(); // Be sure we capture at least one of the input tile dimensions.. we need it later down the code...
      if(tileTupleDims.size() < 2)
      {
        QString ss = QObject::tr("Image Data Array at path '%1' must have at least 2 tuple dimensions.").arg(testPath.serialize("/"));
        setErrorCondition(-11007, ss);
        return;
      }

      tilePtr = dca->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, testPath);
      if(getErrorCode() < 0)
      {
        return;
      }

      tileGeom = dca->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(this, dcName);
      if(getErrorCode() < 0)
      {
        return;
      }

      m_ImageDataContainers.push_back(dc);
    }
  }

  if(getMontageDataContainerName().isEmpty())
  {
    QString ss = QObject::tr("Montage Data Container is empty.");
    setErrorCondition(-11008, ss);
    return;
  }

  if(getMontageAttributeMatrixName().isEmpty())
  {
    QString ss = QObject::tr("Montage Attribute Matrix is empty.");
    setErrorCondition(-11009, ss);
    return;
  }

  if(getMontageDataArrayName().isEmpty())
  {
    QString ss = QObject::tr("Montage Data Array is empty.");
    setErrorCondition(-11010, ss);
    return;
  }

  // Create the montage structure
  DataArrayPath dap(getMontageDataContainerName(), getMontageAttributeMatrixName(), getMontageDataArrayName());

  DataContainer::Pointer dc = getDataContainerArray()->createNonPrereqDataContainer(this, getMontageDataContainerName());
  if(getErrorCode() < 0)
  {
    return;
  }

  std::transform(m_MontageStart.begin(), m_MontageStart.end(), m_MontageEnd.begin(), m_MontageSize.begin(), [](int32_t a, int32_t b) -> int32_t { return b - a + 1; });
  size_t rowCount = m_MontageSize[1];
  size_t colCount = m_MontageSize[0];

  size_t montageArrayXSize = tileTupleDims[0] * colCount;
  size_t montageArrayYSize = tileTupleDims[1] * rowCount;

  ImageGeom::Pointer montageGeom = ImageGeom::New();
  montageGeom->setName("MontageGeometry");
  montageGeom->setDimensions(montageArrayXSize, montageArrayYSize, 1);
  montageGeom->setSpacing(tileGeom->getSpacing());

  dc->setGeometry(montageGeom);

  QString ss = QObject::tr("The image geometry dimensions of data container '%1' are projected to be (%2, %3, %4).  This is assuming "
                           "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
                   .arg(dc->getName())
                   .arg(montageArrayXSize)
                   .arg(montageArrayYSize)
                   .arg(1);
  setWarningCondition(-11011, ss);

  std::vector<size_t> tDims = {montageArrayXSize, montageArrayYSize, 1};

  AttributeMatrix::Pointer am = dc->createNonPrereqAttributeMatrix(this, dap.getAttributeMatrixName(), tDims, AttributeMatrix::Type::Cell);
  if(getErrorCode() < 0)
  {
    return;
  }

  ss = QObject::tr("The tuple dimensions of attribute matrix '%1' are projected to be (%2, %3, %4).  This is assuming "
                   "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
           .arg(am->getName())
           .arg(montageArrayXSize)
           .arg(montageArrayYSize)
           .arg(1);
  setWarningCondition(-11012, ss);

  if(getMontageDataArrayName().isEmpty())
  {
    QString ss = QObject::tr("The Montage Data Array Name field is empty.");
    setErrorCondition(-11013, ss);
    return;
  }

  IDataArray::Pointer da = tilePtr->createNewArray(montageArrayXSize * montageArrayYSize, std::vector<size_t>(1, 1), getMontageDataArrayName(), !getInPreflight());
  am->addOrReplaceAttributeArray(da);

  ss = QObject::tr("The number of elements of montage data array '%1' is projected to be %2.  This is assuming "
                   "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
           .arg(da->getName(), QLocale::system().toString(static_cast<int>(da->getNumberOfTuples())));
  setWarningCondition(-11014, ss);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  // Pass to ITK and generate montage
  // ITK returns a new Fiji data structure to DREAM3D
  // Store FIJI DS into SIMPL Transform DS inside the Geometry
  DataArrayPath dap(m_ImageDataContainers[0]->getName(), getCommonAttributeMatrixName(), getCommonDataArrayName());
  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(m_ImageDataContainers[0]->getName());
  AttributeMatrix::Pointer am = dc->getAttributeMatrix(getCommonAttributeMatrixName());
  IDataArray::Pointer da = am->getAttributeArray(getCommonDataArrayName());

  EXECUTE_STITCH_FUNCTION_TEMPLATE(this, stitchMontage, da);

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage("Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename AccumulatePixelType>
void ITKStitchMontage::stitchMontage(int peakMethodToUse, unsigned streamSubdivisions)
{
  using ScalarPixelType = typename itk::NumericTraits<PixelType>::ValueType;
  using ScalarImageType = itk::Image<ScalarPixelType, Dimension>;
  using OriginalImageType = itk::Image<PixelType, Dimension>;
  using MontageType = itk::TileMontage<ScalarImageType>;

  // Create the resampler
  using Resampler = itk::TileMergeImageFilter<OriginalImageType, AccumulatePixelType>;
  typename Resampler::Pointer resampler = createResampler<PixelType, Resampler>();

  // Initialize the resampler
  initializeResampler<PixelType, MontageType, Resampler>(resampler);

  // Execute the stitching algorithm
  executeStitching<PixelType, Resampler>(resampler, streamSubdivisions);

  // Convert montaged image into DREAM3D data structure
  convertMontageToD3D<PixelType, OriginalImageType>(resampler->GetOutput());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename Resampler>
typename Resampler::Pointer ITKStitchMontage::createResampler()
{
  // using ScalarPixelType = typename itk::NumericTraits<PixelType>::ValueType;
  // using ScalarImageType = itk::Image<ScalarPixelType, Dimension>;

  typename Resampler::Pointer resampler = Resampler::New();
  // resampleF->SetMontage(montage); // doesn't compile, because montage is expected
  // to be templated using itk::Image, not itk::Image

  //  typename ScalarImageType::SpacingType sp;
  //  sp.Fill(1.0); // assume unit spacing

  unsigned int rowCount = m_MontageSize[1];
  unsigned int colCount = m_MontageSize[0];

  resampler->SetMontageSize({colCount, rowCount});
  //  resampler->SetForcedSpacing(sp);

  return resampler;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename MontageType, typename Resampler>
void ITKStitchMontage::initializeResampler(typename Resampler::Pointer resampler)
{
  using OriginalImageType = itk::Image<PixelType, Dimension>;
  using TransformType = itk::TranslationTransform<double, Dimension>;

  typename MontageType::TileIndexType ind;
  for(unsigned y = m_MontageStart[1]; y <= m_MontageEnd[1]; y++)
  {
    ind[1] = y - m_MontageStart[1];
    for(unsigned x = m_MontageStart[0]; x <= m_MontageEnd[0]; x++)
    {
      ind[0] = x - m_MontageStart[0];
      using toITKType = itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension>;
      typename toITKType::Pointer toITK = toITKType::New();
      QString dcName = MontageImportHelper::GenerateDataContainerName(getDataContainerPrefix(), m_MontageEnd, y, x);
      DataContainer::Pointer imageDC = getDataContainerArray()->getDataContainer(dcName);
      // Check the resolution and fix if necessary
      ImageGeom::Pointer geom = imageDC->getGeometryAs<ImageGeom>();

      toITK->SetInput(imageDC);
      toITK->SetInPlace(true);
      toITK->SetAttributeMatrixArrayName(getCommonAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(getCommonDataArrayName().toStdString());
      toITK->Update();

      typename OriginalImageType::Pointer image = toITK->GetOutput();

      resampler->SetInputTile(ind, image);

      typename MontageType::TransformPointer regTr = MontageType::TransformType::New();
      ::ITransformContainer::Pointer transformContainer = geom->getTransformContainer();
      if(transformContainer.get() != nullptr)
      {
        using FilterType = itk::Dream3DITransformContainerToTransform<double, 3>;
        FilterType::Pointer filter = FilterType::New();
        filter->SetInput(transformContainer);
        filter->Update();

        AffineType::Pointer itkAffine = dynamic_cast<AffineType*>(filter->GetOutput()->Get().GetPointer());
        AffineType::TranslationType t = itkAffine->GetTranslation();
        auto offset = regTr->GetOffset();
        for(unsigned i = 0; i < TransformType::SpaceDimension; i++)
        {
          offset[i] = t[i];
        }

        regTr->SetOffset(offset);
      }

      resampler->SetTileTransform(ind, regTr);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename Resampler>
void ITKStitchMontage::executeStitching(typename Resampler::Pointer resampler, unsigned streamSubdivisions)
{
  using OriginalImageType = itk::Image<PixelType, Dimension>;

  notifyStatusMessage("Resampling tiles into the stitched image");

  itk::ProgressObserver::Pointer progressObs = itk::ProgressObserver::New();
  progressObs->SetFilter(this);
  progressObs->SetMessagePrefix("Stitching Tiles Together");
  unsigned long progressObsTag = resampler->AddObserver(itk::ProgressEvent(), progressObs);

  using Dream3DImageType = itk::Image<PixelType, Dimension>;
  using StreamingFilterType = itk::StreamingImageFilter<OriginalImageType, Dream3DImageType>;
  typename StreamingFilterType::Pointer streamingFilter = StreamingFilterType::New();
  streamingFilter->SetInput(resampler->GetOutput());
  streamingFilter->SetNumberOfStreamDivisions(streamSubdivisions);

  streamingFilter->Update();
  notifyStatusMessage("Finished resampling tiles");

  resampler->RemoveObserver(progressObsTag);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename OriginalImageType>
void ITKStitchMontage::convertMontageToD3D(OriginalImageType* image)
{
  DataArrayPath dataArrayPath(getMontageDataContainerName(), getMontageAttributeMatrixName(), getMontageDataArrayName());
  DataContainer::Pointer container = getDataContainerArray()->getDataContainer(dataArrayPath.getDataContainerName());

  using ToDream3DType = itk::InPlaceImageToDream3DDataFilter<PixelType, Dimension>;
  typename ToDream3DType::Pointer toDream3DFilter = ToDream3DType::New();
  toDream3DFilter->SetInput(image);
  toDream3DFilter->SetInPlace(true);
  toDream3DFilter->SetAttributeMatrixArrayName(dataArrayPath.getAttributeMatrixName().toStdString());
  toDream3DFilter->SetDataArrayName(dataArrayPath.getDataArrayName().toStdString());
  toDream3DFilter->SetDataContainer(container);
  toDream3DFilter->Update();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
typename TransformContainer::Pointer ITKStitchMontage::GetTransformContainerFromITKAffineTransform(const AffineType::Pointer& itkAffine)
{
  auto parameters = itkAffine->GetParameters();
  auto fixedParameters = itkAffine->GetFixedParameters();
  auto itkAffineName = itkAffine->GetTransformTypeAsString();

  std::vector<::TransformContainer::ParametersValueType> dream3DParameters(parameters.GetSize());
  std::vector<::TransformContainer::ParametersValueType> dream3DFixedParameters(fixedParameters.GetSize());
  for(size_t p = 0; p < dream3DParameters.size(); p++)
  {
    dream3DParameters[p] = parameters[p];
  }
  for(size_t p = 0; p < dream3DFixedParameters.size(); p++)
  {
    dream3DFixedParameters[p] = fixedParameters[p];
  }
  // Create a SIMPL transform container and manually convert the ITK transform to
  // a SIMPL transform container.
  TransformContainer::Pointer transformContainer = TransformContainer::New();
  transformContainer->setTransformTypeAsString(itkAffineName);
  transformContainer->setMovingName("");
  transformContainer->setReferenceName("World");
  transformContainer->setFixedParameters(dream3DFixedParameters);
  transformContainer->setParameters(dream3DParameters);
  return transformContainer;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKStitchMontage::newFilterInstance(bool copyFilterParameters) const
{
  ITKStitchMontage::Pointer filter = ITKStitchMontage::New();
  if(copyFilterParameters)
  {
    filter->setFilterParameters(getFilterParameters());
    // We are going to hand copy all of the parameters because the other way of copying the parameters are going to
    // miss some of them because we are not enumerating all of them.
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKStitchMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKStitchMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKStitchMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKStitchMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKStitchMontage::getUuid() const
{
  return QUuid("{fa4efd40-f4a6-5524-9fc6-e1f8bbb2c42f}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKStitchMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::GenerationFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKStitchMontage::getHumanLabel() const
{
  return "ITK::Stitch Montage";
}

// -----------------------------------------------------------------------------
ITKStitchMontage::Pointer ITKStitchMontage::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKStitchMontage> ITKStitchMontage::New()
{
  struct make_shared_enabler : public ITKStitchMontage  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKStitchMontage::getNameOfClass() const
{
  return QString("ITKStitchMontage");
}

// -----------------------------------------------------------------------------
QString ITKStitchMontage::ClassName()
{
  return QString("ITKStitchMontage");
}

// -----------------------------------------------------------------------------
void ITKStitchMontage::setMontageStart(const IntVec2Type& value)
{
  m_MontageStart = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ITKStitchMontage::getMontageStart() const
{
  return m_MontageStart;
}

// -----------------------------------------------------------------------------
void ITKStitchMontage::setMontageEnd(const IntVec2Type& value)
{
  m_MontageEnd = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ITKStitchMontage::getMontageEnd() const
{
  return m_MontageEnd;
}

// -----------------------------------------------------------------------------
void ITKStitchMontage::setDataContainerPrefix(const QString& value)
{
  m_DataContainerPrefix = value;
}

// -----------------------------------------------------------------------------
QString ITKStitchMontage::getDataContainerPrefix() const
{
  return m_DataContainerPrefix;
}

// -----------------------------------------------------------------------------
void ITKStitchMontage::setCommonAttributeMatrixName(const QString& value)
{
  m_CommonAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ITKStitchMontage::getCommonAttributeMatrixName() const
{
  return m_CommonAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ITKStitchMontage::setCommonDataArrayName(const QString& value)
{
  m_CommonDataArrayName = value;
}

// -----------------------------------------------------------------------------
QString ITKStitchMontage::getCommonDataArrayName() const
{
  return m_CommonDataArrayName;
}

// -----------------------------------------------------------------------------
void ITKStitchMontage::setMontageDataContainerName(const QString& value)
{
  m_MontageDataContainerName = value;
}

// -----------------------------------------------------------------------------
QString ITKStitchMontage::getMontageDataContainerName() const
{
  return m_MontageDataContainerName;
}

// -----------------------------------------------------------------------------
void ITKStitchMontage::setMontageAttributeMatrixName(const QString& value)
{
  m_MontageAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ITKStitchMontage::getMontageAttributeMatrixName() const
{
  return m_MontageAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ITKStitchMontage::setMontageDataArrayName(const QString& value)
{
  m_MontageDataArrayName = value;
}

// -----------------------------------------------------------------------------
QString ITKStitchMontage::getMontageDataArrayName() const
{
  return m_MontageDataArrayName;
}


