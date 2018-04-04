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
*    United States Air Force Prime Contract FA8650-10-D-5210
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "ITKImageReader.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkGetComponentsDimensions.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/itkInPlaceImageToDream3DDataFilter.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessingPlugin.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageReader::ITKImageReader()
: m_FileName("")
, m_DataContainerName(SIMPL::Defaults::ImageDataContainerName)
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_ImageDataArrayName(SIMPL::CellData::ImageData)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageReader::~ITKImageReader() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::setupFilterParameters()
{
  FilterParameterVector parameters;
  QString supportedExtensions = ITKImageProcessingPlugin::getListSupportedReadExtensions();
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("File", FileName, FilterParameter::Parameter, ITKImageReader, supportedExtensions, "Image"));
  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container", DataContainerName, FilterParameter::CreatedArray, ITKImageReader));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImageReader));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Data", ImageDataArrayName, FilterParameter::CreatedArray, ITKImageReader));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setFileName(reader->readString("FileName", getFileName()));
  setDataContainerName(reader->readString("DataContainerName", getDataContainerName()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::dataCheck()
{
  // check file name exists
  QString filename = getFileName();
  if(filename.isEmpty())
  {
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), "Invalid filename.", getErrorCondition());
    return;
  }

  if(getDataContainerName().isEmpty())
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(), "No container name.", getErrorCondition());
    return;
  }

  DataContainer::Pointer container = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, getDataContainerName());
  if(!container.get())
  {
    setErrorCondition(-3);
    notifyErrorMessage(getHumanLabel(), "No container.", getErrorCondition());
    return;
  }
  readImage(true);
  // If we got here, that means that there is no error
  setErrorCondition(0);
  setWarningCondition(0);
  setWarningCondition(0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::preflight()
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
template <typename TComponent> void ITKImageReader::readImage(const itk::ImageIOBase::Pointer& imageIO, const QString& filename, bool dataCheck)
{
  const unsigned int dimensions = imageIO->GetNumberOfDimensions();
  switch(dimensions)
  {
  case 1:
  {
    readImage<TComponent, 1>(imageIO, filename, dataCheck);
    break;
  }
  case 2:
  {
    readImage<TComponent, 2>(imageIO, filename, dataCheck);
    break;
  }
  default:
  {
    readImage<TComponent, 3>(imageIO, filename, dataCheck);
    break;
  }
  }
}

template <typename TComponent, unsigned int dimensions> void ITKImageReader::readImage(const itk::ImageIOBase::Pointer& imageIO, const QString& filename, bool dataCheck)
{
  typedef itk::ImageIOBase::IOPixelType PixelTypeType;
  PixelTypeType pixel = imageIO->GetPixelType();
  const unsigned int nbComponents = imageIO->GetNumberOfComponents();
  switch(pixel)
  {
  case itk::ImageIOBase::SCALAR:
    readImage<TComponent, dimensions>(filename, dataCheck);
    break;
  case itk::ImageIOBase::RGBA:
    readImage<itk::RGBAPixel<TComponent>, dimensions>(filename, dataCheck);
    break;
  case itk::ImageIOBase::RGB:
    readImage<itk::RGBPixel<TComponent>, dimensions>(filename, dataCheck);
    break;
  case itk::ImageIOBase::VECTOR:
    if(nbComponents == 2)
    {
      readImage<itk::Vector<TComponent, 2>, dimensions>(filename, dataCheck);
    }
    else if(nbComponents == 3)
    {
      readImage<itk::Vector<TComponent, 3>, dimensions>(filename, dataCheck);
    }
    else if(nbComponents == 36)
    {
      readImage<itk::Vector<TComponent, 36>, dimensions>(filename, dataCheck);
    }
    else
    {
      setErrorCondition(-4);
      QString errorMessage = QString("Unsupported number of components: %1.").arg(nbComponents);
      notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());
      break;
    }
    break;
  case itk::ImageIOBase::UNKNOWNPIXELTYPE:
  case itk::ImageIOBase::POINT:
  case itk::ImageIOBase::COVARIANTVECTOR:
  case itk::ImageIOBase::SYMMETRICSECONDRANKTENSOR:
  case itk::ImageIOBase::DIFFUSIONTENSOR3D:
  case itk::ImageIOBase::COMPLEX:
  case itk::ImageIOBase::FIXEDARRAY:
  case itk::ImageIOBase::MATRIX:
  default:
    setErrorCondition(-4);
    QString errorMessage = QString("Unsupported pixel type: %1.").arg(itk::ImageIOBase::GetPixelTypeAsString(pixel).c_str());
    notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());
    break;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename TPixel, unsigned int dimensions>
void ITKImageReader::readImageOutputInformation(typename itk::ImageFileReader<itk::Dream3DImage<TPixel, dimensions>>::Pointer& reader, DataContainer::Pointer& container)
{
  typedef itk::Dream3DImage<TPixel, dimensions> ImageType;
  typedef typename itk::NumericTraits<TPixel>::ValueType ValueType;
  reader->UpdateOutputInformation();
  const typename ImageType::PointType origin = reader->GetOutput()->GetOrigin();
  const typename ImageType::SizeType size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  const typename ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  QVector<float> torigin(3, 0);
  QVector<float> tspacing(3, 0);
  QVector<size_t> tDims(3, 1);
  // Initialize torigin/tspacing/tDims since arrays are always of size 3 and ITK image may have a different size.
  for(size_t i = 0; i < dimensions; i++)
  {
    torigin[i] = origin[i];
    tspacing[i] = spacing[i];
    tDims[i] = size[i];
  }
  ImageGeom::Pointer image = ImageGeom::CreateGeometry(SIMPL::Geometry::ImageGeometry);
  image->setDimensions(tDims[0], tDims[1], tDims[2]);
  image->setOrigin(torigin[0], torigin[1], torigin[2]);
  image->setResolution(tspacing[0], tspacing[1], tspacing[2]);
  container->setGeometry(image);

  QVector<size_t> cDims = ITKDream3DHelper::GetComponentsDimensions<TPixel>();
  AttributeMatrix::Pointer cellAttrMat = container->createNonPrereqAttributeMatrix(this, m_CellAttributeMatrixName, tDims, AttributeMatrix::Type::Cell);
  if(getErrorCondition() < 0)
  {
    return;
  }
  DataArrayPath path;
  path.update(getDataContainerName(), getCellAttributeMatrixName(), getImageDataArrayName());
  getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<ValueType>, AbstractFilter, ValueType>(this, path, 0, cDims);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename TPixel, unsigned int dimensions> void ITKImageReader::readImage(const QString& filename, bool dataCheck)
{
  DataContainer::Pointer container = getDataContainerArray()->getDataContainer(getDataContainerName());
  if(nullptr == container.get())
  {
    setErrorCondition(-4);
    notifyErrorMessage(getHumanLabel(), "Container not found.", getErrorCondition());
    return;
  }

  typedef itk::Dream3DImage<TPixel, dimensions> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::InPlaceImageToDream3DDataFilter<TPixel, dimensions> ToDream3DType;

  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename.toStdString().c_str());

  if(dataCheck)
  {
    readImageOutputInformation<TPixel, dimensions>(reader, container);
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
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::readImage(bool dataCheck)
{
  try
  {
    QString filename = getFileName();
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
    const ComponentType component = imageIO->GetComponentType();
    switch(component)
    {
    case itk::ImageIOBase::UCHAR:
      readImage<unsigned char>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::CHAR:
      readImage<char>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::USHORT:
      readImage<unsigned short>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::SHORT:
      readImage<short>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::UINT:
      readImage<unsigned int>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::INT:
      readImage<int>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::ULONG:
      readImage<unsigned long>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::LONG:
      readImage<long>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::FLOAT:
      readImage<float>(imageIO, filename, dataCheck);
      break;
    case itk::ImageIOBase::DOUBLE:
      readImage<double>(imageIO, filename, dataCheck);
      break;
    default:
      setErrorCondition(-4);
      QString errorMessage = QString("Unsupported pixel component: %1.").arg(imageIO->GetComponentTypeAsString(component).c_str());
      notifyErrorMessage(getHumanLabel(), errorMessage, getErrorCondition());
      break;
    }
  } catch(itk::ExceptionObject& err)
  {
    setErrorCondition(-55557);
    QString errorMessage = "ITK exception was thrown while processing input file: %1";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(err.what()), getErrorCondition());
    return;
  }
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }
  readImage(false);
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImageReader::newFilterInstance(bool copyFilterParameters) const
{
  ITKImageReader::Pointer filter = ITKImageReader::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getGroupName() const
{
  return "IO";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKImageReader::getUuid()
{
  return QUuid("{653b7b5c-03cb-5b32-8c3e-3637745e5ff6}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getSubGroupName() const
{
  return "Input";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getHumanLabel() const
{
  return "ITK::Image Reader";
}
