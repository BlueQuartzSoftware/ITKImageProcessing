/* ============================================================================
 * Copyright (c) 2009-2018 BlueQuartz Software, LLC
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

#include "ImportVectorImageStack.h"

#include <cstring>

#include <limits>

#include <QtCore/QDir>
#include <QtCore/QString>

#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "ITKImageProcessing/FilterParameters/ImportVectorImageStackFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

namespace
{
const QString TempDCName("DC");
const QString TempAMName("AM");
const QString TempDAName("Image");
const DataArrayPath TempDAP(TempDCName, TempAMName, TempDAName);
} // namespace

enum createdPathID : RenameDataPath::DataID_t
{
  AttributeMatrixID21 = 21,

  DataArrayID31 = 31,
  DataArrayID32 = 32,

  DataContainerID = 1
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportVectorImageStack::ImportVectorImageStack()
: m_DataContainerName(SIMPL::Defaults::ImageDataContainerName)
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_VectorDataArrayName(SIMPL::CellData::VectorData)
, m_ConvertToGrayscale(true)
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportVectorImageStack::~ImportVectorImageStack() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportVectorImageStack::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_VECTORFILELISTINFO_FP("Input File List", InputFileListInfo, FilterParameter::Parameter, ImportVectorImageStack));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ImportVectorImageStack));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ImportVectorImageStack));
  parameters.push_back(SIMPL_NEW_BOOL_FP("Convert Color To Grayscale", ConvertToGrayscale, FilterParameter::Parameter, ImportVectorImageStack));

  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("Data Container Name", DataContainerName, FilterParameter::CreatedArray, ImportVectorImageStack));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));

  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix Name", CellAttributeMatrixName, FilterParameter::CreatedArray, ImportVectorImageStack));
  parameters.push_back(SIMPL_NEW_STRING_FP("Vector Data Array Name", VectorDataArrayName, FilterParameter::CreatedArray, ImportVectorImageStack));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportVectorImageStack::initialize()
{
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;
  m_Origin[2] = 0.0;

  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  m_Spacing[2] = 1.0;

  m_InputFileListInfo.FileExtension = QString("tif");
  m_InputFileListInfo.StartIndex = 0;
  m_InputFileListInfo.EndIndex = 0;
  m_InputFileListInfo.PaddingDigits = 0;
  m_InputFileListInfo.StartComponent = 0;
  m_InputFileListInfo.EndComponent = 1;
  m_InputFileListInfo.Separator = QString("-");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportVectorImageStack::dataCheck()
{
  DataArrayPath tempPath;
  clearErrorCondition();
  clearWarningCondition();
  QString ss;

  if(m_InputFileListInfo.InputPath.isEmpty())
  {
    ss = QObject::tr("The Input Directory must be set before executing this filter.");
    setErrorCondition(-13, ss);
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, getDataContainerName(), DataContainerID);
  if(getErrorCode() < 0)
  {
    return;
  }
  DataArrayPath dap(getDataContainerName().getDataContainerName(), getCellAttributeMatrixName(), QString(""));
  QVector<size_t> tDims = {0};
  AttributeMatrix::Pointer am = m->createNonPrereqAttributeMatrix(this, dap, tDims, AttributeMatrix::Type::Cell, AttributeMatrixID21);

  bool hasMissingFiles = false;
  bool orderAscending = false;

  if(m_InputFileListInfo.Ordering == SIMPL::RefFrameZDir::LowtoHigh)
  {
    orderAscending = true;
  }
  else if(m_InputFileListInfo.Ordering == SIMPL::RefFrameZDir::HightoLow)
  {
    orderAscending = false;
  }

  // Now generate all the file names the user is asking for and populate the table
  QVector<QString> fileList =
      FilePathGenerator::GenerateVectorFileList(m_InputFileListInfo.StartIndex, m_InputFileListInfo.EndIndex, m_InputFileListInfo.StartComponent, m_InputFileListInfo.EndComponent, hasMissingFiles,
                                                orderAscending, m_InputFileListInfo.InputPath, m_InputFileListInfo.FilePrefix, m_InputFileListInfo.Separator, m_InputFileListInfo.FileSuffix,
                                                m_InputFileListInfo.FileExtension, m_InputFileListInfo.PaddingDigits);

  if(fileList.isEmpty())
  {
    QString ss = QObject::tr("No files have been selected for import. Have you set the input directory?");
    setErrorCondition(-11, ss);
    return;
  }

  // Make Sure all the input files exist. Throw an error for each one that is missing
  // for(int i = 0; i < fileList.size(); i++)
  for(const auto fp : fileList)
  {

    QString filePath = QDir::toNativeSeparators(fp);
    QFileInfo fi(filePath);
    if(!fi.exists())
    {
      QString errorMessage = QString("File Not Found: %1.").arg(filePath);
      setErrorCondition(-40201, errorMessage);
    }
  }

  int totalIndex = m_InputFileListInfo.EndIndex - m_InputFileListInfo.StartIndex + 1;
  auto totalComp = static_cast<size_t>(m_InputFileListInfo.EndComponent - m_InputFileListInfo.StartComponent + 1);

  QVector<size_t> cDims = {totalComp};
  // for(int i = 0; i < totalIndex; ++i)
  int i = 0;
  {
    // for(int j = 0; j < totalComp; j++)
    {
      int index = 0;
      int index2 = 0;
      if(orderAscending)
      {
        index = m_InputFileListInfo.StartIndex + i;
      }
      else
      {
        index = m_InputFileListInfo.EndIndex - i;
      }

      index2 = m_InputFileListInfo.StartComponent /* +j */;

      QString filename = QString("%1%2%3%4%5.%6")
                             .arg(m_InputFileListInfo.FilePrefix)
                             .arg(QString::number(index), m_InputFileListInfo.PaddingDigits, '0')
                             .arg(m_InputFileListInfo.Separator)
                             .arg(QString::number(index2), m_InputFileListInfo.PaddingDigits, '0')
                             .arg(m_InputFileListInfo.FileSuffix)
                             .arg(m_InputFileListInfo.FileExtension);

      QString filePath = m_InputFileListInfo.InputPath + QDir::separator() + filename;
      filePath = QDir::toNativeSeparators(filePath);
      QFileInfo fi(filePath);
      if(!fi.exists())
      {
        QString errorMessage = QString("File Not Found: %1.").arg(filePath);
        setErrorCondition(-40200, errorMessage);
        return;
      }

      ITKImageReader::Pointer imageReader = ITKImageReader::New();
      imageReader->setDataContainerName(DataArrayPath(::TempDCName, "", ""));
      imageReader->setCellAttributeMatrixName(::TempAMName);
      imageReader->setImageDataArrayName(::TempDAName);
      imageReader->setFileName(filePath);
      imageReader->preflight();

      if (imageReader->getErrorCode() < 0)
      {
        setErrorCondition(imageReader->getErrorCode(), "Image Reader failed preflight.  Please contact the DREAM.3D developers for more information.");
      }

      // Get the DataContainer from the ITKReadImage filter
      DataContainer::Pointer imageReaderDC = imageReader->getDataContainerArray()->getDataContainer(::TempDCName);
      // First time through the loop, create a geometry object.
      if(i == 0)
      {
        // Get the geometry that was read from the ITKReadImage Filter
        ImageGeom::Pointer image = imageReaderDC->getGeometryAs<ImageGeom>();
        size_t xDim = 0, yDim = 0, zDim = 0;
        std::tie(xDim, yDim, zDim) = image->getDimensions();
        // Adjust the Z Dimension of the ImageGeometry and save it in our current Data Container.
        zDim = static_cast<size_t>(totalIndex);
        image->setDimensions(std::make_tuple(xDim, yDim, zDim));
        m->setGeometry(image);
        // Set the number of tuples in the Cell AttributeMatrix
        am->setTupleDimensions({xDim, yDim, zDim});
      }
      // Now figure out if the image was 8bit or 16 bit. The ONLY 2 types we are supporting
      // are 8 bit and 16 bit GrayScale images
      AttributeMatrix::Pointer imageReaderAM = imageReaderDC->getAttributeMatrix(::TempAMName);

      IDataArray::Pointer imageData = imageReaderAM->getAttributeArray(::TempDAName);
      UInt8ArrayType::Pointer bit8 = std::dynamic_pointer_cast<UInt8ArrayType>(imageData);
      if(nullptr != bit8)
      {
        am->createNonPrereqArray<UInt8ArrayType>(this, getVectorDataArrayName(), 0, cDims, DataArrayID31);
      }
      UInt16ArrayType::Pointer bit16 = std::dynamic_pointer_cast<UInt16ArrayType>(imageData);
      if(nullptr != bit16)
      {
        am->createNonPrereqArray<UInt16ArrayType>(this, getVectorDataArrayName(), 0, cDims, DataArrayID32);
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportVectorImageStack::preflight()
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
template <typename T> void importVectorData(ImportVectorImageStack* filter)
{

  using DataArrayType = DataArray<T>;
  using DataArrayPointerType = typename DataArrayType::Pointer;

  DataContainer::Pointer m = filter->getDataContainerArray()->getDataContainer(filter->getDataContainerName());
  AttributeMatrix::Pointer cellAttrMat = m->getAttributeMatrix(filter->getCellAttributeMatrixName());
  ImageGeom::Pointer geom = m->getGeometryAs<ImageGeom>();

  IDataArray::Pointer iVectorData = cellAttrMat->getAttributeArray(filter->getVectorDataArrayName());
  DataArrayPointerType vectorData = std::dynamic_pointer_cast<DataArrayType>(iVectorData);

  VectorFileListInfo_t m_InputFileListInfo = filter->getInputFileListInfo();
  bool orderAscending = false;

  if(m_InputFileListInfo.Ordering == SIMPL::RefFrameZDir::LowtoHigh)
  {
    orderAscending = true;
  }
  else if(m_InputFileListInfo.Ordering == SIMPL::RefFrameZDir::HightoLow)
  {
    orderAscending = false;
  }

  auto numSlices = static_cast<size_t>(m_InputFileListInfo.EndIndex - m_InputFileListInfo.StartIndex + 1);
  int totalComp = m_InputFileListInfo.EndComponent - m_InputFileListInfo.StartComponent + 1;

  for(size_t slice = 0; slice < numSlices; ++slice)
  {
    for(int j = 0; j < totalComp; j++)
    {
      int index = 0;
      int index2 = 0;
      if(orderAscending)
      {
        index = static_cast<size_t>(m_InputFileListInfo.StartIndex) + slice;
      }
      else
      {
        index = static_cast<size_t>(m_InputFileListInfo.EndIndex) - slice;
      }

      index2 = m_InputFileListInfo.StartComponent + j;

      QString filename = QString("%1%2%3%4%5.%6")
                             .arg(m_InputFileListInfo.FilePrefix)
                             .arg(QString::number(index), m_InputFileListInfo.PaddingDigits, '0')
                             .arg(m_InputFileListInfo.Separator)
                             .arg(QString::number(index2), m_InputFileListInfo.PaddingDigits, '0')
                             .arg(m_InputFileListInfo.FileSuffix)
                             .arg(m_InputFileListInfo.FileExtension);

      QString filePath = m_InputFileListInfo.InputPath + QDir::separator() + filename;
      filePath = QDir::toNativeSeparators(filePath);
      QFileInfo fi(filePath);
      if(!fi.exists())
      {
        QString errorMessage = QString("File Not Found: %1.").arg(filePath);
        filter->setErrorCondition(-40200, errorMessage);
        return;
      }

      QString progress = QString("Reading Slice %1/%2 Component %3/%4").arg(slice).arg(numSlices).arg(j).arg(totalComp);
      filter->notifyStatusMessage(progress);

      ITKImageReader::Pointer imageReader = ITKImageReader::New();
      imageReader->setDataContainerName(DataArrayPath(::TempDCName, "", ""));
      imageReader->setCellAttributeMatrixName(::TempAMName);
      imageReader->setImageDataArrayName(::TempDAName);
      imageReader->setFileName(filePath);
      imageReader->execute();
      if (imageReader->getErrorCode() < 0)
      {
        filter->setErrorCondition(imageReader->getErrorCode(), "Image Reader failed execution.  Please contact the DREAM.3D developers for more information.");
      }


      // Get the DataContainer from the ITKReadImage filter
      DataContainer::Pointer imageReaderDC = imageReader->getDataContainerArray()->getDataContainer(::TempDCName);
      AttributeMatrix::Pointer imageReaderAM = imageReaderDC->getAttributeMatrix(::TempAMName);
      IDataArray::Pointer iDataArrayPtr = imageReaderAM->getAttributeArray(::TempDAName);
      DataArrayPointerType data = std::dynamic_pointer_cast<DataArrayType>(iDataArrayPtr);
      int32_t importNumComp = data->getNumberOfComponents();
      size_t nTuples = data->getNumberOfTuples();
      size_t indexOffset = slice * nTuples;

      for(size_t t = 0; t < nTuples; t++)
      {
        // Get the value from the read in image
        T* value = data->getTuplePointer(t);
        if(importNumComp == 3 && filter->getConvertToGrayscale()) // The input image was RGB, so covert it to GrayScale
        {
          float x = 0.2125f;
          float y = 0.7154f;
          float z = 0.0721f;
          // Just store it in place since this is a throw away array
          value[0] = static_cast<uint16_t>((value[0] * x) + (value[1] * y) + (value[2] * z));
        }
        // Place that value into the VectorData Array at the correct location
        vectorData->setComponent(indexOffset + t, j, value[0]);
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportVectorImageStack::execute()
{
  clearErrorCondition();
  clearWarningCondition();

  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(getDataContainerName());
  AttributeMatrix::Pointer cellAttrMat = m->getAttributeMatrix(getCellAttributeMatrixName());
  IDataArray::Pointer vectorData = cellAttrMat->getAttributeArray(getVectorDataArrayName());

  UInt8ArrayType::Pointer bit8 = std::dynamic_pointer_cast<UInt8ArrayType>(vectorData);
  if(nullptr != bit8)
  {
    importVectorData<uint8_t>(this);
  }
  UInt16ArrayType::Pointer bit16 = std::dynamic_pointer_cast<UInt16ArrayType>(vectorData);
  if(nullptr != bit16)
  {
    importVectorData<uint16_t>(this);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ImportVectorImageStack::newFilterInstance(bool copyFilterParameters) const
{
  ImportVectorImageStack::Pointer filter = ImportVectorImageStack::New();
  if(copyFilterParameters)
  {
    filter->setFilterParameters(getFilterParameters());
    // We are going to hand copy all of the parameters because the other way of copying the parameters are going to
    // miss some of them because we are not enumerating all of them.
    SIMPL_COPY_INSTANCEVAR(DataContainerName)
    SIMPL_COPY_INSTANCEVAR(CellAttributeMatrixName)
    SIMPL_COPY_INSTANCEVAR(Spacing)
    SIMPL_COPY_INSTANCEVAR(Origin)
    SIMPL_COPY_INSTANCEVAR(InputFileListInfo)
    SIMPL_COPY_INSTANCEVAR(VectorDataArrayName)
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportVectorImageStack::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportVectorImageStack::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportVectorImageStack::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportVectorImageStack::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ImportVectorImageStack::getUuid()
{
  return QUuid("{c5474cd1-bea9-5a33-a0df-516e5735bab4}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportVectorImageStack::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportVectorImageStack::getHumanLabel() const
{
  return "Import Images (3D Vector Stack)";
}
