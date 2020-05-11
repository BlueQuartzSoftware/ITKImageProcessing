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
#include "ITKImageWriter.h"

#include <cstring>

#include <QtCore/QDir>

// ITK includes
#include <itkImageFileWriter.h>
#include <itkImageSeriesWriter.h>
#include <itkNumericSeriesFileNames.h>
#include <itksys/SystemTools.hxx>

/* For registering ImageIO Factories in Python*/
#include <itkBMPImageIOFactory.h>
#include <itkBioRadImageIOFactory.h>
#include <itkGE4ImageIOFactory.h>
#include <itkGE5ImageIOFactory.h>
#include <itkGiplImageIOFactory.h>
#include <itkJPEGImageIOFactory.h>
#include <itkMRCImageIOFactory.h>
#include <itkMetaImageIOFactory.h>
#include <itkNiftiImageIOFactory.h>
#include <itkNrrdImageIOFactory.h>
#include <itkPNGImageIOFactory.h>
#include <itkStimulateImageIOFactory.h>
#include <itkTIFFImageIOFactory.h>
#include <itkVTKImageIOFactory.h>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/ChoiceFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/OutputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Utilities/FileSystemPathHelper.h"
#include "SIMPLib/ITK/itkInPlaceDream3DDataToImageFilter.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#define DREAM3D_USE_RGB_RGBA 1
#define DREAM3D_USE_Vector 1
#include "SIMPLib/ITK/Dream3DTemplateAliasMacro.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessingPlugin.h"

#ifdef _WIN32
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageWriter::ITKImageWriter()
: m_FileName("")
, m_ImageArrayPath("", "", "")
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageWriter::~ITKImageWriter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::registerImageIOFactories() const
{
  itk::JPEGImageIOFactory::RegisterOneFactory();
  itk::NrrdImageIOFactory::RegisterOneFactory();
  itk::PNGImageIOFactory::RegisterOneFactory();
  itk::TIFFImageIOFactory::RegisterOneFactory();
  itk::JPEGImageIOFactory::RegisterOneFactory();
  itk::BMPImageIOFactory::RegisterOneFactory();
  itk::MetaImageIOFactory::RegisterOneFactory();
  itk::NiftiImageIOFactory::RegisterOneFactory();
  itk::GiplImageIOFactory::RegisterOneFactory();
  itk::VTKImageIOFactory::RegisterOneFactory();
  itk::StimulateImageIOFactory::RegisterOneFactory();
  itk::BioRadImageIOFactory::RegisterOneFactory();
  itk::GE4ImageIOFactory::RegisterOneFactory();
  itk::GE5ImageIOFactory::RegisterOneFactory();
  itk::MRCImageIOFactory::RegisterOneFactory();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  {
    ChoiceFilterParameter::Pointer parameter = ChoiceFilterParameter::New();
    parameter->setHumanLabel("Plane");
    parameter->setPropertyName("Plane");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKImageWriter, this, Plane));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKImageWriter, this, Plane));

    QVector<QString> choices;
    choices.push_back("XY");
    choices.push_back("XZ");
    choices.push_back("YZ");
    parameter->setChoices(choices);
    parameter->setCategory(FilterParameter::Parameter);
    parameters.push_back(parameter);
  }

  QString supportedExtensions = ITKImageProcessingPlugin::getListSupportedWriteExtensions();
  parameters.push_back(SIMPL_NEW_OUTPUT_FILE_FP("Output File", FileName, FilterParameter::Parameter, ITKImageWriter, supportedExtensions));

  parameters.push_back(SeparatorFilterParameter::New("Image Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req =
        DataArraySelectionFilterParameter::CreateRequirement(SIMPL::Defaults::AnyPrimitive, SIMPL::Defaults::AnyComponentSize, AttributeMatrix::Type::Cell, IGeometry::Type::Image);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Image", ImageArrayPath, FilterParameter::RequiredArray, ITKImageWriter, req));
  }
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setFileName(reader->readString("FileName", getFileName()));
  setImageArrayPath(reader->readDataArrayPath("ImageArrayPath", getImageArrayPath()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::dataCheck()
{
  clearErrorCode();
  clearWarningCode();

  FileSystemPathHelper::CheckOutputFile(this, "Output File Name", getFileName(), true);

  DataContainerArray::Pointer containerArray = getDataContainerArray();
  if(!containerArray)
  {
    setErrorCondition(-21002, "No container array.");
    return;
  }

  ImageGeom::Pointer imageGeometry = getDataContainerArray()->getPrereqGeometryFromDataContainer<ImageGeom>(this, getImageArrayPath().getDataContainerName());
  if(imageGeometry.get() == nullptr)
  {
    setErrorCondition(-21003, "No image geometry.");
    return;
  }

  IDataArray::Pointer imageDataArrayPtr = getDataContainerArray()->getPrereqIDataArrayFromPath(this, getImageArrayPath());
  if(getErrorCode() < 0)
  {
    QString ss = QObject::tr("The data array path '%1' was invalid for the property ImageArrayPath. Please check the input value.").arg(getImageArrayPath().serialize("/"));
    setErrorCondition(getErrorCode(), ss);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool ITKImageWriter::is2DFormat()
{
  QString Ext = itksys::SystemTools::LowerCase(itksys::SystemTools::GetFilenameExtension(getFileName().toStdString())).c_str();
  QStringList supported2DExtensions = ITKImageProcessingPlugin::getList2DSupportedFileExtensions();
  int index = supported2DExtensions.indexOf(QRegExp(".*" + Ext));
  return index != -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename TPixel, unsigned int Dimensions>
void ITKImageWriter::writeAs2DStack(typename itk::Image<TPixel, Dimensions>* image, unsigned long z_size)
{
  typedef itk::NumericSeriesFileNames NamesGeneratorType;
  NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
  std::string path = itksys::SystemTools::GetFilenamePath(getFileName().toStdString());
  std::string filename = itksys::SystemTools::GetFilenameWithoutExtension(getFileName().toStdString());
  std::string extension = itksys::SystemTools::GetFilenameExtension(getFileName().toStdString());
  std::string format = path + "/" + filename + "%03d" + extension;
  namesGenerator->SetSeriesFormat(format);
  namesGenerator->SetIncrementIndex(1);
  namesGenerator->SetStartIndex(0);
  namesGenerator->SetEndIndex(z_size - 1);
  typedef itk::Image<TPixel, Dimensions> InputImageType;
  typedef itk::Image<TPixel, Dimensions - 1> OutputImageType;
  typedef itk::ImageSeriesWriter<InputImageType, OutputImageType> SeriesWriterType;
  typename SeriesWriterType::Pointer writer = SeriesWriterType::New();
  writer->SetInput(image);
  writer->SetFileNames(namesGenerator->GetFileNames());
  writer->UseCompressionOn();
  writer->Update();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename TPixel, unsigned int Dimensions>
void ITKImageWriter::writeAsOneFile(typename itk::Image<TPixel, Dimensions>* image)
{
  typedef itk::Image<TPixel, Dimensions> ImageType;
  typedef itk::ImageFileWriter<ImageType> FileWriterType;
  typename FileWriterType::Pointer writer = FileWriterType::New();

  QString progress = QString("Saving %1").arg(getFileName());
  notifyStatusMessage(progress);

  writer->SetInput(image);
  writer->SetFileName(getFileName().toStdString().c_str());
  writer->UseCompressionOn();
  writer->Update();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template <typename TPixel, typename UnusedTPixel, unsigned int Dimensions>
void ITKImageWriter::writeImage()
{
  using ImageType = itk::Image<TPixel, Dimensions>;
  using ToITKType = itk::InPlaceDream3DDataToImageFilter<TPixel, Dimensions>;

  DataArrayPath path = getImageArrayPath();
  DataContainer::Pointer container = getDataContainerArray()->getDataContainer(path.getDataContainerName());

  try
  {
    typename ToITKType::Pointer toITK = ToITKType::New();
    toITK->SetInput(container);
    toITK->SetAttributeMatrixArrayName(path.getAttributeMatrixName().toStdString());
    toITK->SetDataArrayName(path.getDataArrayName().toStdString());
    toITK->SetInPlace(true);
    toITK->Update();
    if(this->is2DFormat() && Dimensions == 3)
    {
      typename ImageType::SizeType size = toITK->GetOutput()->GetLargestPossibleRegion().GetSize();
      if(size[2] < 2)
      {
        setErrorCondition(-21012, "Image is 2D, not 3D.");
        return;
      }
      this->writeAs2DStack<TPixel, Dimensions>(toITK->GetOutput(), size[2]);
    }
    else
    {
      this->writeAsOneFile<TPixel, Dimensions>(toITK->GetOutput());
    }
  } catch(itk::ExceptionObject& err)
  {
    QString errorMessage = "ITK exception was thrown while writing output file: %1";
    setErrorCondition(-21011, errorMessage.arg(err.GetDescription()));
    return;
  }
}

#define ITKIW_PREP_DC(dimA, dimB)                                                                                                                                                                      \
  size_t dA = dimA;                                                                                                                                                                                    \
  size_t dB = dimB;                                                                                                                                                                                    \
  size_t numElements = dA * dB;                                                                                                                                                                        \
  AttributeMatrix::Pointer am = AttributeMatrix::New(tDims, attributeMatrix->getName(), AttributeMatrix::Type::Cell);                                                                                  \
  dc->addOrReplaceAttributeMatrix(am);                                                                                                                                                                 \
  imageGeom->setDimensions(tDims.data());                                                                                                                                                              \
  IDataArray::Pointer sliceData = currentData->createNewArray(numElements, cDims, currentData->getName(), true);                                                                                       \
  am->insertOrAssign(sliceData);

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::execute()
{
  clearErrorCode();
  clearWarningCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  // Make sure any directory path is also available as the user may have just typed
  // in a path without actually creating the full path
  QFileInfo fi(getFileName());
  QString parentPath = fi.path();
  QDir dir;
  if(!dir.mkpath(parentPath))
  {
    QString ss = QObject::tr("Error creating parent path '%1'").arg(parentPath);
    setErrorCondition(-21009, ss);
    return;
  }

  DataArrayPath path = getImageArrayPath();
  if(!getDataContainerArray()->doesDataContainerExist(path.getDataContainerName()))
  {
    QString errorMessage = "Data container %1 does not exist.";
    setErrorCondition(-21006, errorMessage.arg(path.getDataContainerName()));
    return;
  }
  DataContainer::Pointer container = getDataContainerArray()->getDataContainer(path.getDataContainerName());
  if(!container->doesAttributeMatrixExist(path.getAttributeMatrixName()))
  {
    QString errorMessage = "Attribute matrix %1 does not exist.";
    setErrorCondition(-21007, errorMessage.arg(path.getAttributeMatrixName()));
    return;
  }
  AttributeMatrix::Pointer attributeMatrix = container->getAttributeMatrix(path.getAttributeMatrixName());
  if(!attributeMatrix->doesAttributeArrayExist(path.getDataArrayName()))
  {
    QString errorMessage = "Attribute array %1 does not exist.";
    setErrorCondition(-21008, errorMessage.arg(path.getDataArrayName()));
    return;
  }

  ImageGeom::Pointer currentGeom = container->getGeometryAs<ImageGeom>();
  SizeVec3Type dims = currentGeom->getDimensions();

  DataContainerArray::Pointer dca = DataContainerArray::New();
  DataContainer::Pointer dc = DataContainer::New(container->getName());
  dca->addOrReplaceDataContainer(dc);
  ImageGeom::Pointer imageGeom = ImageGeom::CreateGeometry(attributeMatrix->getName());
  dc->setGeometry(imageGeom);

  IDataArray::Pointer currentData = attributeMatrix->getAttributeArray(path.getDataArrayName());
  size_t nComp = currentData->getNumberOfComponents();
  std::vector<size_t> cDims = {static_cast<size_t>(currentData->getNumberOfComponents())};

  if(ITKImageWriter::XYPlane == m_Plane) // XY plane
  {
    std::vector<size_t> tDims = {dims[0], dims[1], 1};
    ITKIW_PREP_DC(dims[0], dims[1])

    for(size_t slice = 0; slice < dims[2]; ++slice)
    {
      for(size_t axisA = 0; axisA < dA; ++axisA)
      {
        for(size_t axisB = 0; axisB < dB; ++axisB)
        {
          size_t index = (slice * dA * dB) + (axisA * dB) + axisB;
          copyTuple(index, axisA, dB, axisB, nComp, currentData.get(), sliceData.get());
        }
      }
      saveImageData(dca, slice, dims[2]);
    }
  }
  else if(ITKImageWriter::XZPlane == m_Plane) // XZ plane
  {
    std::vector<size_t> tDims = {dims[0], dims[2], 1};
    ITKIW_PREP_DC(dims[2], dims[0])

    for(size_t slice = 0; slice < dims[1]; ++slice)
    {
      for(size_t axisA = 0; axisA < dA; ++axisA)
      {
        for(size_t axisB = 0; axisB < dB; ++axisB)
        {
          size_t index = (dims[1] * axisA * dB) + (slice * dB) + axisB;
          copyTuple(index, axisA, dB, axisB, nComp, currentData.get(), sliceData.get());
        }
      }
      saveImageData(dca, slice, dims[1]);
    }
  }
  else if(ITKImageWriter::YZPlane == m_Plane) // YZ plane
  {
    std::vector<size_t> tDims = {dims[1], dims[2], 1};
    ITKIW_PREP_DC(dims[2], dims[1])

    for(size_t slice = 0; slice < dims[0]; ++slice) // X
    {
      for(size_t axisA = 0; axisA < dA; ++axisA) // Z
      {
        for(size_t axisB = 0; axisB < dB; ++axisB) // Y
        {
          size_t index = (dims[0] * axisA * dB) + (axisB * dims[0]) + slice;
          copyTuple(index, axisA, dB, axisB, nComp, currentData.get(), sliceData.get());
        }
      }
      saveImageData(dca, slice, dims[0]);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::copyTuple(size_t index, size_t axisA, size_t dB, size_t axisB, size_t nComp, IDataArray* currentData, IDataArray* sliceData)
{
  size_t indexNew = (axisA * dB) + axisB;

  void* source = currentData->getVoidPointer((nComp * index));
  void* destination = sliceData->getVoidPointer(nComp * indexNew);
  ::memcpy(destination, source, currentData->getTypeSize() * nComp);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageWriter::saveImageData(DataContainerArray::Pointer dca, size_t slice, size_t maxSlice)
{
  QString originalFileName = getFileName();
  QFileInfo fi(originalFileName);

  QString adjustedFilePath;
  QTextStream out(&adjustedFilePath);
  out << fi.absolutePath() << "/" << fi.completeBaseName();
  if(maxSlice != 1)
  {
    out << "_" << slice;
  }
  out << "." << fi.suffix();

  setFileName(adjustedFilePath);
  DataContainerArray::Pointer originalDataContainerArray = getDataContainerArray();
  setDataContainerArray(dca);
  Dream3DArraySwitchMacro(this->writeImage, getImageArrayPath(), -21010);
  setDataContainerArray(originalDataContainerArray);
  setFileName(originalFileName);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImageWriter::newFilterInstance(bool copyFilterParameters) const
{
  ITKImageWriter::Pointer filter = ITKImageWriter::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageWriter::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageWriter::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageWriter::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageWriter::getGroupName() const
{
  return "IO";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid ITKImageWriter::getUuid() const
{
  return QUuid("{11473711-f94d-5d96-b749-ec36a81ad338}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageWriter::getSubGroupName() const
{
  return "Output";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImageWriter::getHumanLabel() const
{
  return "ITK::Image Export";
}

// -----------------------------------------------------------------------------
ITKImageWriter::Pointer ITKImageWriter::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKImageWriter> ITKImageWriter::New()
{
  struct make_shared_enabler : public ITKImageWriter
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKImageWriter::getNameOfClass() const
{
  return QString("ITKImageWriter");
}

// -----------------------------------------------------------------------------
QString ITKImageWriter::ClassName()
{
  return QString("ITKImageWriter");
}

// -----------------------------------------------------------------------------
void ITKImageWriter::setFileName(const QString& value)
{
  m_FileName = value;
}

// -----------------------------------------------------------------------------
QString ITKImageWriter::getFileName() const
{
  return m_FileName;
}

// -----------------------------------------------------------------------------
void ITKImageWriter::setImageArrayPath(const DataArrayPath& value)
{
  m_ImageArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath ITKImageWriter::getImageArrayPath() const
{
  return m_ImageArrayPath;
}

// -----------------------------------------------------------------------------
void ITKImageWriter::setPlane(int value)
{
  m_Plane = value;
}

// -----------------------------------------------------------------------------
int ITKImageWriter::getPlane() const
{
  return m_Plane;
}
