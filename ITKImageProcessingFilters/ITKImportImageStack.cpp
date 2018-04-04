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

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessingPlugin.h"

#include "itksys/SystemTools.hxx"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportImageStack::ITKImportImageStack()
: m_DataContainerName(SIMPL::Defaults::ImageDataContainerName)
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_BoundsFile("")
, m_ImageDataArrayName(SIMPL::CellData::ImageData)
{
  m_Origin.x = 0.0f;
  m_Origin.y = 0.0f;
  m_Origin.z = 0.0f;

  m_Resolution.x = 1.0f;
  m_Resolution.y = 1.0f;
  m_Resolution.z = 1.0f;

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
  QVector<FilterParameter::Pointer> parameters;
  parameters.push_back(SIMPL_NEW_FILELISTINFO_FP("Input File List", InputFileListInfo, FilterParameter::Parameter, ITKImportImageStack));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ITKImportImageStack, 0));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Resolution", Resolution, FilterParameter::Parameter, ITKImportImageStack, 0));
  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container", DataContainerName, FilterParameter::CreatedArray, ITKImportImageStack));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImportImageStack));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Data", ImageDataArrayName, FilterParameter::CreatedArray, ITKImportImageStack));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setDataContainerName(reader->readString("DataContainerName", getDataContainerName()));
  setCellAttributeMatrixName(reader->readString("CellAttributeMatrixName", getCellAttributeMatrixName()));
  setImageDataArrayName(reader->readString("ImageDataArrayName", getImageDataArrayName()));
  setInputFileListInfo(reader->readFileListInfo("InputFileListInfo", getInputFileListInfo()));
  setOrigin(reader->readFloatVec3("Origin", getOrigin()));
  setResolution(reader->readFloatVec3("Resolution", getResolution()));
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
  return FilePathGenerator::GenerateFileList(m_InputFileListInfo.StartIndex, m_InputFileListInfo.EndIndex, hasMissingFiles, orderAscending, m_InputFileListInfo.InputPath,
                                             m_InputFileListInfo.FilePrefix, m_InputFileListInfo.FileSuffix, m_InputFileListInfo.FileExtension, m_InputFileListInfo.PaddingDigits);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setWarningCondition(0);

  DataArrayPath tempPath;
  QString ss;

  if(m_InputFileListInfo.InputPath.isEmpty() == true)
  {
    ss = QObject::tr("The input directory must be set");
    setErrorCondition(-13);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, getDataContainerName());
  if(getErrorCondition() < 0 || nullptr == m.get())
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
  if(fileList.size() == 0)
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

    setErrorCondition(-11);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  const bool dataCheck = true;
  readImage(fileList, dataCheck);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportImageStack::readImage(const QVector<QString>& fileList, bool dataCheck)
{
  try
  {
    for(size_t fileIndex = 0; fileIndex < fileList.size(); ++fileIndex)
    {
      const std::string fileName = fileList[fileIndex].toStdString();
      if(!itksys::SystemTools::FileExists(fileName))
      {
        setErrorCondition(-7);
        QString errorMessage = "File does not exist: %1";
        notifyErrorMessage(getHumanLabel(), errorMessage.arg(fileName.c_str()), getErrorCondition());
        return;
      }
    }
    const QString filename = fileList[0];
    itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(filename.toLatin1(), itk::ImageIOFactory::ReadMode);
    if(nullptr == imageIO)
    {
      setErrorCondition(-5);
      QString errorMessage = "ITK could not read the given file \"%1\". Format is likely unsupported.";
      notifyErrorMessage(getHumanLabel(), errorMessage.arg(filename), getErrorCondition());
      return;
    }
    imageIO->SetFileName(filename.toLatin1());
    imageIO->ReadImageInformation();

    typedef itk::ImageIOBase::IOComponentType ComponentType;
    const ComponentType type = imageIO->GetComponentType();
    const unsigned int dimensions = imageIO->GetNumberOfDimensions();
    if(dimensions != 2)
    {
      setErrorCondition(-1);
      const QString errorMessage = QString("Slice image dimensions do not equal 2. Dimension of current image is %1").arg(dimensions);
      notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());
      return;
    }

    switch(type)
    {
    case itk::ImageIOBase::UCHAR:
      readImageWithPixelType<unsigned char>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::CHAR:
      readImageWithPixelType<char>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::USHORT:
      readImageWithPixelType<unsigned short>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::SHORT:
      readImageWithPixelType<short>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::UINT:
      readImageWithPixelType<unsigned int>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::INT:
      readImageWithPixelType<int>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::ULONG:
      readImageWithPixelType<unsigned long>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::LONG:
      readImageWithPixelType<long>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::FLOAT:
      readImageWithPixelType<float>(fileList, dataCheck);
      break;
    case itk::ImageIOBase::DOUBLE:
      readImageWithPixelType<double>(fileList, dataCheck);
      break;
    default:
      setErrorCondition(-4);
      QString errorMessage = QString("Unsupported pixel type: %1.").arg(imageIO->GetComponentTypeAsString(type).c_str());
      notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());
      break;
    }
  } catch(itk::ExceptionObject& err)
  {
    setErrorCondition(-55559);
    QString errorMessage = "ITK exception was thrown while processing input file: %1";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(err.what()), getErrorCondition());
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename TPixel> void ITKImportImageStack::readImageWithPixelType(const QVector<QString>& fileList, bool dataCheck)
{
  DataContainer::Pointer container = getDataContainerArray()->getDataContainer(getDataContainerName());
  if(nullptr == container.get())
  {
    setErrorCondition(-4);
    notifyErrorMessage(getHumanLabel(), "Container not found.", getErrorCondition());
    return;
  }

  const unsigned int Dimension = 3;
  typedef itk::Dream3DImage<TPixel, Dimension> ImageType;
  typedef itk::ImageSeriesReader<ImageType> ReaderType;
  typedef itk::InPlaceImageToDream3DDataFilter<TPixel, Dimension> ToDream3DType;

  typename ReaderType::Pointer reader = ReaderType::New();
  typename ReaderType::FileNamesContainer fileNames(fileList.size());
  for(size_t fileIndex = 0; fileIndex < fileList.size(); ++fileIndex)
  {
    fileNames[fileIndex] = fileList[fileIndex].toStdString();
  }
  reader->SetFileNames(fileNames);

  if(dataCheck)
  {
    readImageOutputInformation<TPixel>(reader, container);
  }
  else
  {
    typename ToDream3DType::Pointer toDream3DFilter = ToDream3DType::New();
    toDream3DFilter->SetInput(reader->GetOutput());
    toDream3DFilter->SetInPlace(true);
    toDream3DFilter->SetAttributeMatrixArrayName(m_CellAttributeMatrixName.toStdString());
    toDream3DFilter->SetDataArrayName(m_ImageDataArrayName.toStdString());
    toDream3DFilter->SetDataContainer(container);
    toDream3DFilter->Update();
  }

  ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
  image->setResolution(m_Resolution.x, m_Resolution.y, m_Resolution.z);
  image->setOrigin(m_Origin.x, m_Origin.y, m_Origin.z);
  const typename ImageType::SizeType size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  QVector<size_t> tDims(Dimension, 1);
  for(unsigned int i = 0; i < Dimension; i++)
  {
    tDims[i] = size[i];
  }
  image->setDimensions(tDims[0], tDims[1], tDims[2]);
  container->setGeometry(image);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename TPixel> void ITKImportImageStack::readImageOutputInformation(typename itk::ImageSeriesReader<itk::Dream3DImage<TPixel, 3>>::Pointer& reader, DataContainer::Pointer& container)
{
  const unsigned int Dimension = 3;
  typedef itk::Dream3DImage<TPixel, Dimension> ImageType;

  reader->UpdateOutputInformation();
  const typename ImageType::SizeType size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  QVector<size_t> tDims(Dimension, 1);
  for(unsigned int i = 0; i < Dimension; i++)
  {
    tDims[i] = size[i];
  }

  QVector<size_t> cDims(1, 1);
  AttributeMatrix::Pointer cellAttrMat = container->createNonPrereqAttributeMatrix(this, m_CellAttributeMatrixName, tDims, AttributeMatrix::Type::Cell);
  if(getErrorCondition() < 0)
  {
    return;
  }
  DataArrayPath path;
  path.update(getDataContainerName(), getCellAttributeMatrixName(), getImageDataArrayName());
  getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<TPixel>, AbstractFilter, TPixel>(this, path, 0, cDims);
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
void ITKImportImageStack::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  const QVector<QString> fileList = this->getFileList();
  const bool dataCheck = false;
  readImage(fileList, dataCheck);

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImportImageStack::newFilterInstance(bool copyFilterParameters) const
{
  ITKImportImageStack::Pointer filter = ITKImportImageStack::New();
  if(true == copyFilterParameters)
  {
    filter->setFilterParameters(getFilterParameters());
    // We are going to hand copy all of the parameters because the other way of copying the parameters are going to
    // miss some of them because we are not enumerating all of them.
    SIMPL_COPY_INSTANCEVAR(DataContainerName)
    SIMPL_COPY_INSTANCEVAR(CellAttributeMatrixName)
    SIMPL_COPY_INSTANCEVAR(Resolution)
    SIMPL_COPY_INSTANCEVAR(Origin)
#if 0
    SIMPL_COPY_INSTANCEVAR(ZStartIndex)
    SIMPL_COPY_INSTANCEVAR(ZEndIndex)
    SIMPL_COPY_INSTANCEVAR(InputPath)
    SIMPL_COPY_INSTANCEVAR(FilePrefix)
    SIMPL_COPY_INSTANCEVAR(FileSuffix)
    SIMPL_COPY_INSTANCEVAR(FileExtension)
    SIMPL_COPY_INSTANCEVAR(PaddingDigits)
    SIMPL_COPY_INSTANCEVAR(RefFrameZDir)
#endif
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
  return "IO";
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
  return "Input";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportImageStack::getHumanLabel() const
{
  return "ITK::Import Images (3D Stack)";
}
