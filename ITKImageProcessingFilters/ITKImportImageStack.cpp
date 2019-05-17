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

#include "ITKImportImageStack.h"

#include <QtCore/QFileInfo>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/DataContainerCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessingPlugin.h"

enum createdPathID : RenameDataPath::DataID_t
{
  AttributeMatrixID21 = 21,

  DataArrayID31 = 31,

  DataContainerID = 1
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportImageStack::ITKImportImageStack()
: m_DataContainerName(SIMPL::Defaults::ImageDataContainerName)
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_ImageDataArrayName(SIMPL::CellData::ImageData)
{
  m_Origin[0] = 0.0f;
  m_Origin[1] = 0.0f;
  m_Origin[2] = 0.0f;

  m_Spacing[0] = 1.0f;
  m_Spacing[1] = 1.0f;
  m_Spacing[2] = 1.0f;

  m_InputFileListInfo.FileExtension = QString("tif");
  m_InputFileListInfo.StartIndex = 0;
  m_InputFileListInfo.EndIndex = 0;
  m_InputFileListInfo.PaddingDigits = 0;

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportImageStack::~ITKImportImageStack() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_FILELISTINFO_FP("Input File List", InputFileListInfo, FilterParameter::Parameter, ITKImportImageStack));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ITKImportImageStack, 0));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ITKImportImageStack, 0));
  parameters.push_back(SIMPL_NEW_DC_CREATION_FP("Data Container", DataContainerName, FilterParameter::CreatedArray, ITKImportImageStack));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_AM_WITH_LINKED_DC_FP("Cell Attribute Matrix", CellAttributeMatrixName, DataContainerName, FilterParameter::CreatedArray, ITKImportImageStack));
  parameters.push_back(SIMPL_NEW_DA_WITH_LINKED_AM_FP("Image Data", ImageDataArrayName, DataContainerName, CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImportImageStack));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setDataContainerName(reader->readDataArrayPath("DataContainerName", getDataContainerName()));
  setCellAttributeMatrixName(reader->readString("CellAttributeMatrixName", getCellAttributeMatrixName()));
  setImageDataArrayName(reader->readString("ImageDataArrayName", getImageDataArrayName()));
  setInputFileListInfo(reader->readFileListInfo("InputFileListInfo", getInputFileListInfo()));
  setOrigin(reader->readFloatVec3("Origin", getOrigin()));
  setSpacing(reader->readFloatVec3("Spacing", getSpacing()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVector<QString> ITKImportImageStack::getFileList()
{
  bool hasMissingFiles = false;
  bool orderAscending = false;

  if(m_InputFileListInfo.Ordering == 0)
  {
    orderAscending = true;
  }
  else if(m_InputFileListInfo.Ordering == 1)
  {
    orderAscending = false;
  }

  // Now generate all the file names the user is asking for and populate the table
  return FilePathGenerator::GenerateFileList(m_InputFileListInfo.StartIndex, m_InputFileListInfo.EndIndex, m_InputFileListInfo.IncrementIndex, hasMissingFiles, orderAscending,
                                             m_InputFileListInfo.InputPath, m_InputFileListInfo.FilePrefix, m_InputFileListInfo.FileSuffix, m_InputFileListInfo.FileExtension,
                                             m_InputFileListInfo.PaddingDigits);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  DataArrayPath tempPath;
  QString ss;

  if(m_InputFileListInfo.InputPath.isEmpty())
  {
    ss = QObject::tr("The input directory must be set");
    setErrorCondition(-64500, ss);
  }

  QFileInfo fi(m_InputFileListInfo.InputPath);
  if(!fi.exists())
  {
    QString ss = QObject::tr("The input directory does not exist: %1").arg(m_InputFileListInfo.InputPath);
    setErrorCondition(-64501, ss);
  }

  DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, getDataContainerName(), DataContainerID);
  if(getErrorCode() < 0 || nullptr == m.get())
  {
    return;
  }

  bool orderAscending = false;

  if(m_InputFileListInfo.Ordering == 0)
  {
    orderAscending = true;
  }
  else if(m_InputFileListInfo.Ordering == 1)
  {
    orderAscending = false;
  }

  // Now generate all the file names the user is asking for and populate the table
  const QVector<QString> fileList = this->getFileList();
  if(fileList.empty())
  {
    ss.clear();
    QTextStream out(&ss);
    out << " No files have been selected for import. Have you set the input directory and other values so that input files will be generated?\n";
    out << "InputPath: " << m_InputFileListInfo.InputPath << "\n";
    out << "FilePrefix: " << m_InputFileListInfo.FilePrefix << "\n";
    out << "FileSuffix: " << m_InputFileListInfo.FileSuffix << "\n";
    out << "FileExtension: " << m_InputFileListInfo.FileExtension << "\n";
    out << "PaddingDigits: " << m_InputFileListInfo.PaddingDigits << "\n";
    out << "StartIndex: " << m_InputFileListInfo.StartIndex << "\n";
    out << "EndIndex: " << m_InputFileListInfo.EndIndex << "\n";
    setErrorCondition(-64501, ss);
    return;
  }

  // Validate all the files in the list. Throw an error for each one if it does not exist
  for(const auto& filePath : fileList)
  {
    QFileInfo fi(filePath);
    if(!fi.exists())
    {
      QString errorMessage = QString("File does not exist: %1").arg(filePath);
      setErrorCondition(-64502, errorMessage);
    }
  }
  if(getErrorCode() < 0)
  {
    return;
  }

  // Create a subfilter to read each image, although for preflight we are going to read the first image in the
  // list and hope the rest are correct.
  ITKImageReader::Pointer imageReader = ITKImageReader::New();
  DataContainerArray::Pointer dca = DataContainerArray::New();
  imageReader->setDataContainerArray(dca);
  imageReader->setDataContainerName(getDataContainerName());
  imageReader->setCellAttributeMatrixName(getCellAttributeMatrixName());
  imageReader->setImageDataArrayName(getImageDataArrayName());
  imageReader->setFileName(fileList[0]);
  imageReader->preflight();
  if(imageReader->getErrorCode() < 0)
  {
    setErrorCondition(imageReader->getErrorCode(), "Error Reading Input Image.");
    return;
  }
  // Extract the Geometry and update the geometry
  DataContainer::Pointer dc = dca->getDataContainer(getDataContainerName());
  ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
  SizeVec3Type dims = imageGeom->getDimensions();
  dims[2] = fileList.size();
  imageGeom->setDimensions(dims);
  imageGeom->setOrigin(m_Origin);
  imageGeom->setSpacing(m_Spacing);
  m->setGeometry(imageGeom);
  dc->setGeometry(IGeometry::NullPointer());

  // Extract the Cell Attribute Matrix that was created and resize it for the number of Tuples that we have
  // and add it to the actual DataContainer that this filter created.
  AttributeMatrix::Pointer cellAttrMat = dc->getAttributeMatrix(getCellAttributeMatrixName());
  cellAttrMat->resizeAttributeArrays(dims.toContainer<QVector<size_t>>());
  m->insertOrAssign(cellAttrMat);
  // The data array that was embedded in the Cell Attribute Matrix just got resized so we should not have to do
  // anything else at this point.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::preflight()
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
template <typename TPixel>
void readImageStack(ITKImportImageStack* filter, const QVector<QString>& fileList)
{

  DataArrayPath dcName = filter->getDataContainerName();
  QString attrMatName = filter->getCellAttributeMatrixName();
  QString arrayName = filter->getImageDataArrayName();

  DataContainer::Pointer dc = filter->getDataContainerArray()->getDataContainer(dcName);
  ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
  SizeVec3Type dims = imageGeom->getDimensions();
  size_t tuplesPerSlice = dims[0] * dims[1];
  AttributeMatrix::Pointer cellAttrMatr = dc->getAttributeMatrix(attrMatName);
  using DataArrayType = DataArray<TPixel>;
  using DataArrayPointerType = typename DataArrayType::Pointer;
  IDataArray::Pointer iDataArray = cellAttrMatr->getAttributeArray(arrayName);
  DataArrayPointerType outputData = std::dynamic_pointer_cast<DataArrayType>(iDataArray);
  if(nullptr == outputData.get())
  {
    std::cout << "Could not dynamic_pointer_cast<> from IDataArray to " << iDataArray->getInfoString(SIMPL::InfoStringFormat::HtmlFormat).toStdString() << std::endl;
    return;
  }

  outputData->allocate(); // This most likely didn't happen during the preflight because how we are doing things is a bit unorthodox so make sure the data array is allocated.

  // Variables for the progress Reporting
  size_t slice = 0;
  int32_t progress = 0;
  int64_t zStartIndex = 0;
  int64_t z = zStartIndex;
  float total = static_cast<float>(fileList.size() - zStartIndex);
  // Loop over all the files importing them one by one and copying the data into the data array
  for(const auto& filePath : fileList)
  {
    // Do some progress message
    progress = static_cast<int32_t>(z - zStartIndex);
    progress = (int32_t)(100.0f * (float)(progress) / total);
    QString msg = "Importing: " + filePath;
    filter->notifyStatusMessage(msg.toLatin1().data());

    // Create a subfilter to read each image, although for preflight we are going to read the first image in the
    // list and hope the rest are correct.
    ITKImageReader::Pointer imageReader = ITKImageReader::New();
    DataContainerArray::Pointer dca = DataContainerArray::New();
    imageReader->setDataContainerArray(dca);
    imageReader->setDataContainerName(dcName);
    imageReader->setCellAttributeMatrixName(attrMatName);
    imageReader->setImageDataArrayName(arrayName);
    imageReader->setFileName(filePath);
    imageReader->execute();
    if(imageReader->getErrorCode() < 0)
    {
      QString msg = QString("Error reading image %1").arg(filePath);
      filter->setErrorCondition(imageReader->getErrorCode(), msg);
      return;
    }

    // Compute the Tuple Index we are at:
    size_t tupleIndex = (slice * dims[0] * dims[1]);
    // get the current Slice data...
    DataArrayPointerType tempData = dca->getDataContainer(dcName)->getAttributeMatrix(attrMatName)->getAttributeArrayAs<DataArrayType>(arrayName);
    // Copy that into the output array...
    outputData->copyFromArray(tupleIndex, tempData, 0, tuplesPerSlice);

    slice++;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::execute()
{
  clearErrorCode();
  clearWarningCode();

  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  const QVector<QString> fileList = this->getFileList();
  // we just need to figure out what kind of data the images are in, the readImageStack() will do the rest.
  itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(fileList[0].toLatin1(), itk::ImageIOFactory::ReadMode);
  imageIO->SetFileName(fileList[0].toLatin1());
  imageIO->ReadImageInformation();

  using ComponentType = itk::ImageIOBase::IOComponentType;
  const ComponentType component = imageIO->GetComponentType();
  switch(component)
  {
  case itk::ImageIOBase::UCHAR:
    readImageStack<unsigned char>(this, fileList);
    break;
  case itk::ImageIOBase::CHAR:
    readImageStack<char>(this, fileList);
    break;
  case itk::ImageIOBase::USHORT:
    readImageStack<uint16_t>(this, fileList);
    break;
  case itk::ImageIOBase::SHORT:
    readImageStack<int16_t>(this, fileList);
    break;
  case itk::ImageIOBase::UINT:
    readImageStack<uint32_t>(this, fileList);
    break;
  case itk::ImageIOBase::INT:
    readImageStack<int32_t>(this, fileList);
    break;
  case itk::ImageIOBase::ULONG:
    readImageStack<uint64_t>(this, fileList);
    break;
  case itk::ImageIOBase::LONG:
    readImageStack<int64_t>(this, fileList);
    break;
  case itk::ImageIOBase::FLOAT:
    readImageStack<float>(this, fileList);
    break;
  case itk::ImageIOBase::DOUBLE:
    readImageStack<double>(this, fileList);
    break;
  default:
    QString errorMessage = QString("Unsupported pixel component: %1.").arg(imageIO->GetComponentTypeAsString(component).c_str());
    setErrorCondition(-64504, errorMessage);
    break;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImportImageStack::newFilterInstance(bool copyFilterParameters) const
{
  ITKImportImageStack::Pointer filter = ITKImportImageStack::New();
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
    SIMPL_COPY_INSTANCEVAR(ImageStack)
    SIMPL_COPY_INSTANCEVAR(ImageDataArrayName)
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportImageStack::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportImageStack::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportImageStack::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportImageStack::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKImportImageStack::getUuid()
{
  return QUuid("{cf7d7497-9573-5102-bedd-38f86a6cdfd4}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportImageStack::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportImageStack::getHumanLabel() const
{
  return "ITK::Import Images (3D Stack)";
}
