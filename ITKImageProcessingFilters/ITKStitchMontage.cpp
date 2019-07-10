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
#include "SIMPLib/ITK/itkDream3DImage.h"
#include "SIMPLib/ITK/itkDream3DTransformContainerToTransform.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"
#include "SIMPLib/ITK/itkProgressObserver.hpp"
#include "SIMPLib/ITK/itkTransformToDream3DITransformContainer.h"
#include "SIMPLib/ITK/itkTransformToDream3DTransformContainer.h"
#include "SIMPLib/SIMPLibVersion.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

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
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage Start (Col, Row) [Inclusive, Zero Based]", MontageStart, FilterParameter::Parameter, ITKStitchMontage));
  parameters.push_back(SIMPL_NEW_INT_VEC2_FP("Montage End (Col, Row) [Inclusive, Zero Based]", MontageEnd, FilterParameter::Parameter, ITKStitchMontage));

  {
    MultiDataContainerSelectionFilterParameter::RequirementType req =
        MultiDataContainerSelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_MDC_SELECTION_FP("Image Data Containers", ImageDataContainers, FilterParameter::RequiredArray, ITKStitchMontage, req));
  }
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

  QString ss;
  int err = 0;

  std::transform(m_MontageStart.begin(), m_MontageStart.end(), m_MontageEnd.begin(), m_MontageSize.begin(), [](int32_t a, int32_t b) -> int32_t { return a + b + 1; });
  int32_t rowCount = m_MontageSize[1];
  int32_t colCount = m_MontageSize[0];

  if(colCount <= 0 || rowCount <= 0)
  {
    QString ss = QObject::tr("The Montage Size x and y values must be greater than 0");
    setErrorCondition(-396, ss);
    return;
  }

  int selectedDCCount = getImageDataContainers().size();

  // Test to make sure at least one data container is selected
  if(selectedDCCount < 1)
  {
    QString ss = QObject::tr("At least one Data Container must be selected");
    setErrorCondition(-11001, ss);
    return;
  }

  int totalMontageSize = std::accumulate(m_MontageSize.begin(), m_MontageSize.end(), 1, std::multiplies<>());

  if(totalMontageSize != selectedDCCount)
  {
    QString ss = QObject::tr("The number of selected data containers (%1) does not match the number of data "
                             "containers expected by the montage size dimensions specified (%2)")
                     .arg(selectedDCCount)
                     .arg(totalMontageSize);
    setErrorCondition(-11002, ss);
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

  // Test to see that the image data containers are correct
  int dcCount = m_ImageDataContainers.size();

  QString dcName = m_ImageDataContainers[0];

  DataArrayPath testPath;
  testPath.setDataContainerName(dcName);
  testPath.setAttributeMatrixName(getCommonAttributeMatrixName());
  testPath.setDataArrayName(getCommonDataArrayName());

  AttributeMatrix::Pointer imageDataAM = getDataContainerArray()->getPrereqAttributeMatrixFromPath(this, testPath, err);
  if(getErrorCode() < 0 || err < 0)
  {
    return;
  }

  std::vector<size_t> imageDataTupleDims = imageDataAM->getTupleDimensions();
  if(imageDataTupleDims.size() < 2)
  {
    QString ss = QObject::tr("Image Data Array at path '%1' must have at least 2 tuple dimensions.").arg(testPath.serialize("/"));
    setErrorCondition(-11005, ss);
    return;
  }

  IDataArray::Pointer imagePtr = getDataContainerArray()->getPrereqIDataArrayFromPath<IDataArray, AbstractFilter>(this, testPath);
  if(getErrorCode() < 0)
  {
    return;
  }

  ImageGeom::Pointer tileImageGeom = ImageGeom::NullPointer();
  for(int i = 0; i < dcCount; i++)
  {
    QString dcName = m_ImageDataContainers[i];

    DataArrayPath testPath;
    testPath.setDataContainerName(dcName);
    testPath.setAttributeMatrixName(getCommonAttributeMatrixName());
    testPath.setDataArrayName(getCommonDataArrayName());

    ImageGeom::Pointer image = getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom, AbstractFilter>(this, testPath.getDataContainerName());
    if(getErrorCode() < 0)
    {
      return;
    }
    tileImageGeom = image; // Be sure we capture at least one of the input Image Geometries that is valid.. we need it later down the code...
  }

  if(getMontageDataContainerName().isEmpty())
  {
    QString ss = QObject::tr("Montage Data Container is empty.");
    setErrorCondition(-11007, ss);
    return;
  }

  if(getMontageAttributeMatrixName().isEmpty())
  {
    QString ss = QObject::tr("Montage Attribute Matrix is empty.");
    setErrorCondition(-11008, ss);
    return;
  }

  if(getMontageDataArrayName().isEmpty())
  {
    QString ss = QObject::tr("Montage Data Array is empty.");
    setErrorCondition(-11009, ss);
    return;
  }

  DataArrayPath dap(getMontageDataContainerName(), getMontageAttributeMatrixName(), getMontageDataArrayName());

  DataContainer::Pointer dc = getDataContainerArray()->createNonPrereqDataContainer(this, getMontageDataContainerName());
  if(getErrorCode() < 0)
  {
    return;
  }

  size_t montageArrayXSize = imageDataTupleDims[0] * colCount;
  size_t montageArrayYSize = imageDataTupleDims[1] * rowCount;

  ImageGeom::Pointer imageGeom = ImageGeom::New();
  imageGeom->setName("MontageGeometry");
  imageGeom->setDimensions(montageArrayXSize, montageArrayYSize, 1);
  imageGeom->setSpacing(tileImageGeom->getSpacing());

  dc->setGeometry(imageGeom);

  ss = QObject::tr("The image geometry dimensions of data container '%1' are projected to be (%2, %3, %4).  This is assuming "
                   "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
           .arg(dc->getName())
           .arg(montageArrayXSize)
           .arg(montageArrayYSize)
           .arg(1);
  setWarningCondition(-3001, ss);

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
  setWarningCondition(-3002, ss);

  if(getMontageDataArrayName().isEmpty())
  {
    QString ss = QObject::tr("The Montage Data Array Name field is empty.");
    setErrorCondition(-3003, ss);
    return;
  }

  IDataArray::Pointer da = imagePtr->createNewArray(montageArrayXSize * montageArrayYSize, std::vector<size_t>(1, 1), getMontageDataArrayName(), !getInPreflight());
  am->addOrReplaceAttributeArray(da);

  ss = QObject::tr("The number of elements of montage data array '%1' is projected to be %2.  This is assuming "
                   "0% overlap between tiles, so the actual geometry dimensions after executing the stitching algorithm may be smaller.")
           .arg(da->getName(), QLocale::system().toString(static_cast<int>(da->getNumberOfTuples())));
  setWarningCondition(-3004, ss);
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

  createFijiDataStructure();
  if(getErrorCode() < 0)
  {
    return;
  }

  if(!m_StageTiles.empty())
  {
    // Pass to ITK and generate montage
    // ITK returns a new Fiji data structure to DREAM3D
    // Store FIJI DS into SIMPL Transform DS inside the Geometry
    DataArrayPath dap(m_ImageDataContainers[0], getCommonAttributeMatrixName(), getCommonDataArrayName());
    DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(m_ImageDataContainers[0]);
    AttributeMatrix::Pointer am = dc->getAttributeMatrix(getCommonAttributeMatrixName());
    IDataArray::Pointer da = am->getAttributeArray(getCommonDataArrayName());

    EXECUTE_STITCH_FUNCTION_TEMPLATE(this, stitchMontage, da);
  }
  /* Let the GUI know we are done with this filter */
  notifyStatusMessage("Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKStitchMontage::createFijiDataStructure()
{
  DataContainerArray* dca = getDataContainerArray().get();
  // Loop over the data containers until we find the proper data container
  QMutableListIterator<QString> dcNameIter(m_ImageDataContainers);
  QStringList dcList;
  bool dataContainerPrefixChanged = false;

  int32_t rowCount = m_MontageSize[1];
  int32_t colCount = m_MontageSize[0];

  if(m_ImageDataContainers.size() != (colCount * rowCount))
  {
    return;
  }

  m_StageTiles.resize(rowCount);
  // for(unsigned i = 0; i < m_yMontageSize; i++)
  for(auto& stageTile : m_StageTiles)
  {
    stageTile.resize(colCount);
  }

  std::vector<size_t> cDims;
  while(dcNameIter.hasNext())
  {
    QString dcName = dcNameIter.next();
    dcList.push_back(dcName);
    DataContainer::Pointer dcItem = dca->getPrereqDataContainer(this, dcName);
    if(getErrorCode() < 0 || dcItem.get() == nullptr)
    {
      continue;
    }
    ImageGeom::Pointer image = dcItem->getGeometryAs<ImageGeom>();
    //   SizeVec3Type dimensions = image->getDimensions();
    FloatVec3Type origin = image->getOrigin();

    // Extract row and column data from the data container name
    //    QString filename = ""; // Need to find this?
    int indexOfUnderscore = dcName.lastIndexOf("_");
    if(!dataContainerPrefixChanged)
    {
      m_dataContainerPrefix = dcName.left(indexOfUnderscore);
      dataContainerPrefixChanged = true;
    }
    QString rowCol = dcName.right(dcName.size() - indexOfUnderscore - 1);
    rowCol = rowCol.right(rowCol.size() - 1);     // Remove 'r'
    QStringList rowCol_Split = rowCol.split("c"); // Split by 'c'
    int row = rowCol_Split[0].toInt();
    int col = rowCol_Split[1].toInt();
    itk::Tile2D tile;
    tile.Position[0] = origin[0];
    tile.Position[1] = origin[1];
    tile.FileName = ""; // This code gets its data from memory, not from a file

    m_StageTiles[row][col] = tile;

    int err = 0;
    AttributeMatrix::Pointer am = dcItem->getPrereqAttributeMatrix(this, getCommonAttributeMatrixName(), err);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename PixelType, typename AccumulatePixelType>
void ITKStitchMontage::stitchMontage(int peakMethodToUse, unsigned streamSubdivisions)
{
  using ScalarPixelType = typename itk::NumericTraits<PixelType>::ValueType;
  using ScalarImageType = itk::Dream3DImage<ScalarPixelType, Dimension>;
  using OriginalImageType = itk::Dream3DImage<PixelType, Dimension>;
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
  // using ScalarImageType = itk::Dream3DImage<ScalarPixelType, Dimension>;

  typename Resampler::Pointer resampler = Resampler::New();
  // resampleF->SetMontage(montage); // doesn't compile, because montage is expected
  // to be templated using itk::Image, not itk::Dream3DImage

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
  using OriginalImageType = itk::Dream3DImage<PixelType, Dimension>;
  using TransformType = itk::TranslationTransform<double, Dimension>;

  typename MontageType::TileIndexType ind;
  for(unsigned y = m_MontageStart[1]; y < m_MontageEnd[1]; y++)
  {
    ind[1] = y;
    for(unsigned x = m_MontageStart[0]; x < m_MontageEnd[0]; x++)
    {
      ind[0] = x;
      using toITKType = itk::InPlaceDream3DDataToImageFilter<PixelType, Dimension>;
      typename toITKType::Pointer toITK = toITKType::New();
      DataContainer::Pointer imageDC = GetImageDataContainer(y, x);
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
  using OriginalImageType = itk::Dream3DImage<PixelType, Dimension>;

  notifyStatusMessage("Resampling tiles into the stitched image");

  itk::ProgressObserver::Pointer progressObs = itk::ProgressObserver::New();
  progressObs->setFilter(this);
  progressObs->setMessagePrefix("Stitching Tiles Together");
  unsigned long progressObsTag = resampler->AddObserver(itk::ProgressEvent(), progressObs.get());

  using Dream3DImageType = itk::Dream3DImage<PixelType, Dimension>;
  using StreamingFilterType = itk::StreamingImageFilter<OriginalImageType, Dream3DImageType>;
  typename StreamingFilterType::Pointer streamingFilter = StreamingFilterType::New();
  streamingFilter->SetInput(resampler->GetOutput());
  streamingFilter->SetNumberOfStreamDivisions(streamSubdivisions);

  streamingFilter->Update();
  notifyStatusMessage("Finished resampling tiles");
  notifyStatusMessage("Converting into DREAM3D data structure");

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
DataContainer::Pointer ITKStitchMontage::GetImageDataContainer(int y, int x)
{
  DataContainerArray* dca = getDataContainerArray().get();
  // Loop over the data containers until we find the proper data container
  QMutableListIterator<QString> dcNameIter(m_ImageDataContainers);
  QStringList dcList;
  while(dcNameIter.hasNext())
  {
    QString dcName = dcNameIter.next();
    dcList.push_back(dcName);
    DataContainer::Pointer dcItem = dca->getPrereqDataContainer(this, dcName);
    if(getErrorCode() < 0 || dcItem.get() == nullptr)
    {
      continue;
    }

    QString rowCol = dcName.right(dcName.size() - dcName.lastIndexOf("_") - 1);
    rowCol = rowCol.right(rowCol.size() - 1);     // Remove 'r'
    QStringList rowCol_Split = rowCol.split("c"); // Split by 'c'
    int row = rowCol_Split[0].toInt();
    int col = rowCol_Split[1].toInt();
    if(row == y && col == x)
    {
      return dcItem;
    }
  }
  return nullptr;
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
const QString ITKStitchMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKStitchMontage::getUuid()
{
  return QUuid("{fa4efd40-f4a6-5524-9fc6-e1f8bbb2c42f}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::GenerationFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKStitchMontage::getHumanLabel() const
{
  return "ITK::Stitch Montage";
}
