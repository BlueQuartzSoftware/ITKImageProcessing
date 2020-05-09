/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
#include "ITKPCMTileRegistration.h"

#include <algorithm>
#include <type_traits>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/Common/TemplateHelpers.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"
#include "SIMPLib/ITK/itkDream3DFilterInterruption.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"
#include "SIMPLib/ITK/itkProgressObserver.hpp"
#include "SIMPLib/ITK/itkTransformToDream3DITransformContainer.h"
#include "SIMPLib/ITK/itkTransformToDream3DTransformContainer.h"
#include "SIMPLib/SIMPLibVersion.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/MetaXmlUtils.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/util/MontageImportHelper.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

#include "itkImageFileWriter.h"
#include "itkRGBToLuminanceImageFilter.h"
#include "itkStreamingImageFilter.h"
#include "itkTileMergeImageFilter.h"
#include "itkTileMontage.h"
#include "itkTxtTransformIOFactory.h"

#define EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(DATATYPE, filter, rgb_call, grayscale_call, inputData, ...)                                                                                          \
  int numOfComponents = inputData->getNumberOfComponents();                                                                                                                                            \
  if(numOfComponents == 3)                                                                                                                                                                             \
  {                                                                                                                                                                                                    \
    rgb_call<itk::RGBPixel<DATATYPE>>(__VA_ARGS__);                                                                                                                                                    \
  }                                                                                                                                                                                                    \
  else if(numOfComponents == 4)                                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    rgb_call<itk::RGBAPixel<DATATYPE>>(__VA_ARGS__);                                                                                                                                                   \
  }                                                                                                                                                                                                    \
  else if(numOfComponents == 1)                                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    grayscale_call<DATATYPE>(__VA_ARGS__);                                                                                                                                                             \
  }                                                                                                                                                                                                    \
  else                                                                                                                                                                                                 \
  {                                                                                                                                                                                                    \
    filter->setErrorCondition(TemplateHelpers::Errors::UnsupportedImageType, "The input array's image type is not recognized.  Supported image types"                                                  \
                                                                             " are grayscale (1-component), RGB (3-component), and RGBA (4-component)");                                               \
  }

#define EXECUTE_DATATYPE_FUNCTION_TEMPLATE(filter, rgb_call, grayscale_call, inputData, ...)                                                                                                           \
  if(TemplateHelpers::CanDynamicCast<FloatArrayType>()(inputData))                                                                                                                                     \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(float, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                        \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<DoubleArrayType>()(inputData))                                                                                                                               \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(double, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                       \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<Int8ArrayType>()(inputData))                                                                                                                                 \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(int8_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                       \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<UInt8ArrayType>()(inputData))                                                                                                                                \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(uint8_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                      \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<Int16ArrayType>()(inputData))                                                                                                                                \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(int16_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                      \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<UInt16ArrayType>()(inputData))                                                                                                                               \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(uint16_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                     \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<Int32ArrayType>()(inputData))                                                                                                                                \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(int32_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                      \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<UInt32ArrayType>()(inputData))                                                                                                                               \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(uint32_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                     \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<Int64ArrayType>()(inputData))                                                                                                                                \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(int64_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                      \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<UInt64ArrayType>()(inputData))                                                                                                                               \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(uint64_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                     \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<BoolArrayType>()(inputData))                                                                                                                                 \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(bool, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                         \
  }                                                                                                                                                                                                    \
  else if(TemplateHelpers::CanDynamicCast<DataArray<size_t>>()(inputData))                                                                                                                             \
  {                                                                                                                                                                                                    \
    EXECUTE_REGISTER_FUNCTION_TEMPLATE_HELPER(size_t, filter, rgb_call, grayscale_call, inputData, __VA_ARGS__);                                                                                       \
  }                                                                                                                                                                                                    \
  else                                                                                                                                                                                                 \
  {                                                                                                                                                                                                    \
    filter->setErrorCondition(TemplateHelpers::Errors::UnsupportedDataType, "The input array's data type is not supported");                                                                           \
  }

#define EXECUTE_REGISTER_FUNCTION_TEMPLATE(filter, rgb_call, grayscale_call, inputData, ...) EXECUTE_DATATYPE_FUNCTION_TEMPLATE(filter, rgb_call, grayscale_call, inputData, __VA_ARGS__)

itk::NumericTraits<float> nmfloat;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKPCMTileRegistration::ITKPCMTileRegistration()
: m_DataContainerPrefix(ITKImageProcessing::Montage::k_DataContainerPrefixDefaultName)
, m_CommonAttributeMatrixName(ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName)
, m_CommonDataArrayName(ITKImageProcessing::Montage::k_TileDataArrayDefaultName)
{
  m_MontageStart = IntVec2Type(0, 0);
  m_MontageEnd = IntVec2Type(0, 0);
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKPCMTileRegistration::~ITKPCMTileRegistration() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);

  m_MontageStart[0] = m_ColumnMontageLimits[0];
  m_MontageStart[1] = m_RowMontageLimits[0];

  m_MontageEnd[0] = m_ColumnMontageLimits[1];
  m_MontageEnd[1] = m_RowMontageLimits[1];
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Column Start/End [Inclusive, Zero Based]", ColumnMontageLimits, FilterParameter::Parameter, ITKPCMTileRegistration));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Row Start/End [Inclusive, Zero Based]", RowMontageLimits, FilterParameter::Parameter, ITKPCMTileRegistration));

  parameters.push_back(SIMPL_NEW_INTEGER_FP("Padding Digits for DataContainer Names", DataContainerPaddingDigits, FilterParameter::Category::Parameter, ITKPCMTileRegistration));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container Prefix", DataContainerPrefix, FilterParameter::RequiredArray, ITKPCMTileRegistration));

  parameters.push_back(SIMPL_NEW_STRING_FP("Common Attribute Matrix", CommonAttributeMatrixName, FilterParameter::RequiredArray, ITKPCMTileRegistration));
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Data Array", CommonDataArrayName, FilterParameter::RequiredArray, ITKPCMTileRegistration));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::dataCheck()
{
  initialize();

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

  if(getDataContainerPrefix().isEmpty())
  {
    QString ss = QObject::tr("Data Container Prefix is empty.");
    setErrorCondition(-11003, ss);
    return;
  }

  if(getCommonAttributeMatrixName().isEmpty())
  {
    QString ss = QObject::tr("Common Attribute Matrix is empty.");
    setErrorCondition(-11003, ss);
    return;
  }

  if(getCommonDataArrayName().isEmpty())
  {
    QString ss = QObject::tr("Common Data Array is empty.");
    setErrorCondition(-11004, ss);
    return;
  }

  m_DataContainers.clear();

  DataContainerArray::Pointer dca = getDataContainerArray();
  // This is for the Tile Data Structure that we need to build up

  for(int32_t row = m_MontageStart[1]; row <= m_MontageEnd[1]; row++)
  {
    for(int32_t col = m_MontageStart[0]; col <= m_MontageEnd[0]; col++)
    {
      // Create our DataContainer Name using a Prefix and a rXXcYY format.
      QString dcName = MontageImportHelper::GenerateDataContainerName(getDataContainerPrefix(), m_DataContainerPaddingDigits, row, col);

      DataArrayPath testPath;
      testPath.setDataContainerName(dcName);
      testPath.setAttributeMatrixName(getCommonAttributeMatrixName());
      testPath.setDataArrayName(getCommonDataArrayName());

      DataContainer::Pointer dc = dca->getPrereqDataContainer(this, testPath);
      if(getErrorCode() < 0)
      {
        return;
      }

      IDataArray::Pointer imagePtr = dca->getPrereqIDataArrayFromPath(this, testPath);
      if(getErrorCode() < 0)
      {
        return;
      }

      ImageGeom::Pointer image = dca->getPrereqGeometryFromDataContainer<ImageGeom>(this, dcName);
      if(getErrorCode() < 0)
      {
        return;
      }

      typename ::TransformContainer::TransformParametersType dream3DParameters(12);
      typename ::TransformContainer::TransformFixedParametersType dream3DFixedParameters(3);

      TransformContainer::Pointer transformContainer = TransformContainer::New();
      transformContainer->setParameters(dream3DParameters);
      transformContainer->setFixedParameters(dream3DFixedParameters);
      transformContainer->setTransformTypeAsString("AffineTransform_double_3_3");

      image->setTransformContainer(transformContainer);
      m_DataContainers.push_back(dc);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  IDataArray::Pointer da = m_DataContainers[0]->getAttributeMatrix(getCommonAttributeMatrixName())->getAttributeArray(getCommonDataArrayName());

  EXECUTE_REGISTER_FUNCTION_TEMPLATE(this, registerRGBMontage, registerGrayscaleMontage, da)

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage("Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename MontageType>
typename MontageType::Pointer ITKPCMTileRegistration::createMontage(int peakMethodToUse)
{
  using ScalarPixelType = typename itk::NumericTraits<PixelType>::ValueType;
  using ScalarImageType = itk::Image<ScalarPixelType, Dimension>;
  using PCMType = itk::PhaseCorrelationImageRegistrationMethod<ScalarImageType, ScalarImageType>;

  IntVec3Type montageSize;
  std::transform(m_MontageStart.begin(), m_MontageStart.end(), m_MontageEnd.begin(), montageSize.begin(), [](int32_t a, int32_t b) -> int32_t { return b - a + 1; });
  int32_t rowCount = montageSize[1];
  int32_t colCount = montageSize[0];

  unsigned x1 = 1;
  unsigned y1 = 1;
  if(colCount < 2)
  {
    x1 = 0;
  }
  if(rowCount < 2)
  {
    y1 = 0;
  }

#if(ITK_VERSION_MAJOR == 5) && (ITK_VERSION_MINOR >= 1)
  using PaddingMethodEnum = typename PCMType::PaddingMethodEnum;
  using PeakInterpolationType = typename itk::MaxPhaseCorrelationOptimizer<PCMType>::PeakInterpolationMethodEnum;
#else
  using PaddingMethodEnum = typename PCMType::PaddingMethod;
  using PeakInterpolationType = typename itk::MaxPhaseCorrelationOptimizer<PCMType>::PeakInterpolationMethod;
#endif

  // Create tile montage
  using SizeValueType = itk::Size<2>::SizeValueType;
  typename MontageType::Pointer montage = MontageType::New();
  montage->SetMontageSize({static_cast<SizeValueType>(colCount), static_cast<SizeValueType>(rowCount)});
  montage->SetPaddingMethod(PaddingMethodEnum::MirrorWithExponentialDecay);
  //  montage->SetOriginAdjustment(originAdjustment);
  //  montage->SetForcedSpacing(sp);

  using PeakFinderUnderlying = typename std::underlying_type<PeakInterpolationType>::type;

  auto peakMethod = static_cast<PeakFinderUnderlying>(peakMethodToUse);
  montage->SetPeakInterpolationMethod(static_cast<PeakInterpolationType>(peakMethod));
  montage->Modified();

  return montage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename MontageType>
typename MontageType::Pointer ITKPCMTileRegistration::createGrayscaleMontage(int peakMethodToUse)
{
  using ScalarPixelType = typename itk::NumericTraits<PixelType>::ValueType;
  //	using PointType = itk::Point<double, Dimension>;
  using ScalarImageType = itk::Image<ScalarPixelType, Dimension>;

  typename MontageType::Pointer montage = createMontage<PixelType, MontageType>(peakMethodToUse);

  // Set tile image data from DREAM3D structure into tile montage
  for(int32_t row = m_MontageStart[1]; row <= m_MontageEnd[1]; row++)
  {
    typename MontageType::TileIndexType ind;
    ind[1] = static_cast<::itk::SizeValueType>(row - m_MontageStart[1]);
    for(int32_t col = m_MontageStart[0]; col <= m_MontageEnd[0]; col++)
    {
      ind[0] = static_cast<::itk::SizeValueType>(col - m_MontageStart[0]);

      // Get our DataContainer Name using a Prefix and a rXXcYY format.
      QString dcName = MontageImportHelper::GenerateDataContainerName(getDataContainerPrefix(), m_DataContainerPaddingDigits, row, col);
      DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dcName);

      using InPlaceDream3DToImageFileType = itk::InPlaceDream3DDataToImageFilter<ScalarPixelType, Dimension>;
      typename InPlaceDream3DToImageFileType::Pointer toITK = InPlaceDream3DToImageFileType::New();
      toITK->SetInput(dc);
      toITK->SetInPlace(true);
      toITK->SetAttributeMatrixArrayName(getCommonAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(getCommonDataArrayName().toStdString());
      toITK->Update();

      typename ScalarImageType::Pointer image = toITK->GetOutput();

      montage->SetInputTile(ind, image);
    }
  }

  return montage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename MontageType>
typename MontageType::Pointer ITKPCMTileRegistration::createRGBMontage(int peakMethodToUse)
{
  using ScalarPixelType = typename itk::NumericTraits<PixelType>::ValueType;
  //	using PointType = itk::Point<double, Dimension>;
  using ScalarImageType = itk::Image<ScalarPixelType, Dimension>;
  using OriginalImageType = itk::Image<PixelType, Dimension>;

  typename MontageType::Pointer montage = createMontage<PixelType, MontageType>(peakMethodToUse);

  // Set tile image data from DREAM3D structure into tile montage
  for(int32_t row = m_MontageStart[1]; row <= m_MontageEnd[1]; row++)
  {
    typename MontageType::TileIndexType ind;
    ind[1] = static_cast<::itk::SizeValueType>(row - m_MontageStart[1]);
    for(int32_t col = m_MontageStart[0]; col <= m_MontageEnd[0]; col++)
    {
      ind[0] = static_cast<::itk::SizeValueType>(col - m_MontageStart[0]);

      // Get our DataContainer Name using a Prefix and a rXXcYY format.
      QString dcName = MontageImportHelper::GenerateDataContainerName(getDataContainerPrefix(), m_DataContainerPaddingDigits, row, col);
      DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(dcName);

      using InPlaceDream3DToImageFileType = itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension>;
      typename InPlaceDream3DToImageFileType::Pointer toITK = InPlaceDream3DToImageFileType::New();

      toITK->SetInput(dc);
      toITK->SetInPlace(true);
      toITK->SetAttributeMatrixArrayName(getCommonAttributeMatrixName().toStdString());
      toITK->SetDataArrayName(getCommonDataArrayName().toStdString());
      toITK->Update();

      typename OriginalImageType::Pointer image = toITK->GetOutput();

      using FilterType = itk::RGBToLuminanceImageFilter<OriginalImageType, ScalarImageType>;
      typename FilterType::Pointer filter = FilterType::New();
      filter->SetInput(image);
      filter->Update();

      montage->SetInputTile(ind, filter->GetOutput());
    }
  }

  return montage;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType>
void ITKPCMTileRegistration::registerGrayscaleMontage(int peakMethodToUse, unsigned streamSubdivisions)
{
  using ScalarPixelType = typename itk::NumericTraits<PixelType>::ValueType;
  using ScalarImageType = itk::Image<ScalarPixelType, Dimension>;
  using MontageType = itk::TileMontage<ScalarImageType>;

  typename MontageType::Pointer montage = createGrayscaleMontage<PixelType, MontageType>(peakMethodToUse);

  // Execute the montage registration algorithm
  executeMontageRegistration<MontageType>(montage);

  // Store tile registration transforms in DREAM3D data containers
  storeMontageTransforms<MontageType>(montage);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType>
void ITKPCMTileRegistration::registerRGBMontage(int peakMethodToUse, unsigned streamSubdivisions)
{
  using ScalarPixelType = typename itk::NumericTraits<PixelType>::ValueType;
  using ScalarImageType = itk::Image<ScalarPixelType, Dimension>;
  using MontageType = itk::TileMontage<ScalarImageType>;

  typename MontageType::Pointer montage = createRGBMontage<PixelType, MontageType>(peakMethodToUse);

  // Execute the montage registration algorithm
  executeMontageRegistration<MontageType>(montage);

  // Store tile registration transforms in DREAM3D data containers
  storeMontageTransforms<MontageType>(montage);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename MontageType>
void ITKPCMTileRegistration::executeMontageRegistration(typename MontageType::Pointer montage)
{
  // Execute the tile registrations
  notifyStatusMessage("Doing the tile registrations");

  itk::ProgressObserver::Pointer progressObs = itk::ProgressObserver::New();
  progressObs->SetFilter(this);
  progressObs->SetMessagePrefix("Registering Tiles");
  montage->AddObserver(itk::ProgressEvent(), progressObs);
  montage->Update();
  notifyStatusMessage("Finished the tile registrations");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename MontageType>
void ITKPCMTileRegistration::storeMontageTransforms(typename MontageType::Pointer montage)
{
  using TransformType = itk::TranslationTransform<double, Dimension>;

  // Set tile image data from DREAM3D structure into tile montage
  for(int32_t row = m_MontageStart[1]; row <= m_MontageEnd[1]; row++)
  {
    typename MontageType::TileIndexType ind;
    ind[1] = static_cast<::itk::SizeValueType>(row - m_MontageStart[1]);
    for(int32_t col = m_MontageStart[0]; col <= m_MontageEnd[0]; col++)
    {
      ind[0] = static_cast<::itk::SizeValueType>(col - m_MontageStart[0]);

      const TransformType* regTr = montage->GetOutputTransform(ind);

      // Get our DataContainer Name using a Prefix and a rXXcYY format.
      QString dcName = MontageImportHelper::GenerateDataContainerName(getDataContainerPrefix(), m_DataContainerPaddingDigits, row, col);
      DataContainer::Pointer imageDC = getDataContainerArray()->getDataContainer(dcName);

      ImageGeom::Pointer image = imageDC->getGeometryAs<ImageGeom>();

      // Create an ITK affine transform as a reference
      AffineType::Pointer itkAffine = AffineType::New();
      AffineType::TranslationType t;
      t.Fill(0);
      for(unsigned i = 0; i < TransformType::SpaceDimension; i++)
      {
        t[i] = regTr->GetOffset()[i];
      }
      itkAffine->SetTranslation(t);

      TransformContainer::Pointer transformContainer = GetTransformContainerFromITKAffineTransform(itkAffine);
      auto containerParameters = transformContainer->getParameters();
      auto containerFixedParameters = transformContainer->getFixedParameters();
      using FilterType = itk::TransformToDream3DITransformContainer<double, 3>;

      FilterType::Pointer filter = FilterType::New();
      filter->SetInput(itkAffine);
      filter->Update();
      ::ITransformContainer::Pointer convertedITransformContainer = filter->GetOutput()->Get();
      ::TransformContainer::Pointer convertedTransformContainer = std::dynamic_pointer_cast<::TransformContainer>(convertedITransformContainer);

      image->setTransformContainer(convertedTransformContainer);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
typename TransformContainer::Pointer ITKPCMTileRegistration::GetTransformContainerFromITKAffineTransform(const AffineType::Pointer& itkAffine)
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
AbstractFilter::Pointer ITKPCMTileRegistration::newFilterInstance(bool copyFilterParameters) const
{
  ITKPCMTileRegistration::Pointer filter = ITKPCMTileRegistration::New();
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
QString ITKPCMTileRegistration::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKPCMTileRegistration::getUuid() const
{
  return QUuid("{4388723b-cc16-3477-ac6f-fe0107107e74}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::GenerationFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getHumanLabel() const
{
  return "ITK::Compute Tile Transformations (PCM Method)";
}

// -----------------------------------------------------------------------------
ITKPCMTileRegistration::Pointer ITKPCMTileRegistration::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKPCMTileRegistration> ITKPCMTileRegistration::New()
{
  struct make_shared_enabler : public ITKPCMTileRegistration
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getNameOfClass() const
{
  return QString("ITKPCMTileRegistration");
}

// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::ClassName()
{
  return QString("ITKPCMTileRegistration");
}

// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::setColumnMontageLimits(const IntVec2Type& value)
{
  m_ColumnMontageLimits = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ITKPCMTileRegistration::getColumnMontageLimits() const
{
  return m_ColumnMontageLimits;
}

// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::setRowMontageLimits(const IntVec2Type& value)
{
  m_RowMontageLimits = value;
}

// -----------------------------------------------------------------------------
IntVec2Type ITKPCMTileRegistration::getRowMontageLimits() const
{
  return m_RowMontageLimits;
}

// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::setDataContainerPrefix(const QString& value)
{
  m_DataContainerPrefix = value;
}

// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getDataContainerPrefix() const
{
  return m_DataContainerPrefix;
}

// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::setCommonAttributeMatrixName(const QString& value)
{
  m_CommonAttributeMatrixName = value;
}

// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getCommonAttributeMatrixName() const
{
  return m_CommonAttributeMatrixName;
}

// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::setCommonDataArrayName(const QString& value)
{
  m_CommonDataArrayName = value;
}

// -----------------------------------------------------------------------------
QString ITKPCMTileRegistration::getCommonDataArrayName() const
{
  return m_CommonDataArrayName;
}

// -----------------------------------------------------------------------------
void ITKPCMTileRegistration::setDataContainerPaddingDigits(int32_t value)
{
  m_DataContainerPaddingDigits = value;
}

// -----------------------------------------------------------------------------
int32_t ITKPCMTileRegistration::getDataContainerPaddingDigits() const
{
  return m_DataContainerPaddingDigits;
}
