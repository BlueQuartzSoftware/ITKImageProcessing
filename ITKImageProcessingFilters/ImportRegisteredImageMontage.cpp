/*
 * Your License or Copyright Information can go here
 */

#include "ImportRegisteredImageMontage.h"

#include <QtCore/QDir>

#include "itkParseTileConfiguration.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/itkReadImageImpl.hpp"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportRegisteredImageMontage::ImportRegisteredImageMontage()
: m_DataContainerPrefix(SIMPL::Defaults::ImageDataContainerName + "_")
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_RegistrationFile("")
, m_AttributeArrayName("ImageTile")
{
  m_Origin.x = 0.0;
  m_Origin.y = 0.0;
  m_Origin.z = 0.0;

  m_Resolution.x = 1.0;
  m_Resolution.y = 1.0;
  m_Resolution.z = 1.0;

  m_InputFileListInfo.FileExtension = QString("tif");
  m_InputFileListInfo.StartIndex = 0;
  m_InputFileListInfo.EndIndex = 0;
  m_InputFileListInfo.PaddingDigits = 0;

  m_NumImages = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportRegisteredImageMontage::~ImportRegisteredImageMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRegisteredImageMontage::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);

  if(m_InStream.isOpen())
  {
    m_InStream.close();
  }
  m_NumImages = 0;
  m_RegisteredFileNames.clear();
  m_Coords.clear();
  m_RowColId.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRegisteredImageMontage::setupFilterParameters()
{
  QVector<FilterParameter::Pointer> parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Registration File", RegistrationFile, FilterParameter::Parameter, ImportRegisteredImageMontage, "", "*.txt"));
  parameters.push_back(SIMPL_NEW_FILELISTINFO_FP("Input File List", InputFileListInfo, FilterParameter::Parameter, ImportRegisteredImageMontage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ImportRegisteredImageMontage));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Resolution", Resolution, FilterParameter::Parameter, ImportRegisteredImageMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container Prefix", DataContainerPrefix, FilterParameter::CreatedArray, ImportRegisteredImageMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ImportRegisteredImageMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Array Name", AttributeArrayName, FilterParameter::CreatedArray, ImportRegisteredImageMontage));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRegisteredImageMontage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  initialize();

  QString ss;
  int32_t err = 0;

  if(m_InputFileListInfo.InputPath.isEmpty())
  {
    ss = QObject::tr("The input directory must be set");
    setErrorCondition(-2000);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  QFileInfo fi(m_RegistrationFile);
  if(!fi.exists())
  {
    QString ss = QObject::tr("The registration file does not exist");
    setErrorCondition(-2001);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if(m_RegistrationFile.isEmpty())
  {
    QString ss = QObject::tr("The registration file must be set").arg(getHumanLabel());
    setErrorCondition(-2002);
    notifyErrorMessage(getHumanLabel(), ss, -1);
    return;
  }

  m_InStream.setFileName(getRegistrationFile());
  if(!m_InStream.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString ss = QObject::tr("Registration file could not be opened");
    setErrorCondition(-2003);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if (fi.completeSuffix() == "csv")
  {
    // Parse Robomet Config File
    err = parseRobometConfigFile(m_InStream);
    if(err < 0)
    {
      return;
    }
  }
  else if (fi.completeSuffix() == "txt")
  {
    // Parse Fiji Config File
    err = parseFijiConfigFile(m_InStream);
    if(err < 0)
    {
      return;
    }
  }
  else
  {
    QString ss = QObject::tr("Registration file is not in a recognized format.  Registration files must either be Fiji (*.txt) or Robomet (*.csv) configuration files.");
    setErrorCondition(-2005);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if(getInPreflight())
  {
    m_InStream.close();
  }

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
  QVector<QString> fileList = FilePathGenerator::GenerateFileList(m_InputFileListInfo.StartIndex, m_InputFileListInfo.EndIndex, m_InputFileListInfo.IncrementIndex, hasMissingFiles, orderAscending,
                                                                  m_InputFileListInfo.InputPath, m_InputFileListInfo.FilePrefix, m_InputFileListInfo.FileSuffix, m_InputFileListInfo.FileExtension,
                                                                  m_InputFileListInfo.PaddingDigits);
  if(fileList.empty())
  {
    QString ss = QObject::tr("No files have been selected for import. Have you set the input directory?");
    setErrorCondition(-2006);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
  else
  {
    size_t availableFileCount = 0;
    for(const auto& imageFName : fileList)
    {
      QFileInfo fi(imageFName);
      if(fi.exists())
      {
        availableFileCount++;
      }
    }
    if(availableFileCount != m_NumImages)
    {
      QString ss = QObject::tr("The number of files in selected folder (%1) does not match the number in the registration file (%2)").arg(fileList.size()).arg(m_NumImages);
      setErrorCondition(-2007);
      notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
      return;
    }

    QVector<size_t> cDims(1, 1);

    for(int i = 0; i < fileList.size(); i++)
    {
      if(getCancel())
      {
        return;
      }

      QString imageFName = fileList[i];

      QFileInfo fi(imageFName);
      if(!fi.exists())
      {
        continue;
      }

      int arrayNamesPos = find(m_RegisteredFileNames.begin(), m_RegisteredFileNames.end(), fi.fileName()) - m_RegisteredFileNames.begin();
      if (arrayNamesPos >= m_RegisteredFileNames.size())
      {
        QString ss = QObject::tr("File '%1' in the selected files list does not have a corresponding entry in the registration file '%2'").arg(fi.fileName()).arg(m_RegistrationFile);
        setErrorCondition(-2008);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        return;
      }

      QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(m_RowColId[arrayNamesPos]);

      DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, dcName);
      if(getErrorCondition() < 0)
      {
        return;
      }
      ImageGeom::Pointer geom = ImageGeom::New();
      geom->initializeWithZeros();
      m->setGeometry(geom);

      setFileName(imageFName);
      DataArrayPath dap(dcName, getCellAttributeMatrixName(), getAttributeArrayName());
      readImage(dap, getInPreflight());
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ImportRegisteredImageMontage::parseFijiConfigFile(QFile& reader)
{
  int32_t err = 0;

  itk::TileLayout2D stageTiles = itk::ParseTileConfiguration2D(getRegistrationFile().toStdString());
  for (int i = 0; i < stageTiles.size(); i++)
  {
    auto tile2D = stageTiles[i];
    for (int j = 0; j < tile2D.size(); j++)
    {
      auto tile = tile2D[j];
      m_RegisteredFileNames.push_back(QString::fromStdString(tile.FileName));

      uint32_t coordsSize = tile.Position.GetPointDimension();
      if (coordsSize != 2)
      {
        QString ss = QObject::tr("The dimension size of all tiles in the fiji config file must be equal to 2.");
        setErrorCondition(-2004);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        m_RegisteredFileNames.clear();
        m_Coords.clear();
        m_RowColId.clear();
        m_NumImages = 0;
        return -1;
      }

      m_Coords.push_back(QPointF(tile.Position[0], tile.Position[1]));
      m_RowColId.push_back(tr("r%1c%2").arg(i).arg(j));
      m_NumImages++;
    }
  }

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ImportRegisteredImageMontage::parseRobometConfigFile(QFile& reader)
{
  reader.reset();
  int32_t err = 0;
  QByteArray buf;
  QList<QByteArray> tokens;
  QList<QByteArray> files;
  bool ok = false;
  QString word("");
  QString xCoord;
  QString yCoord;
  int32_t lineNum = 0;

  // TODO: modify this reader to use caching
  while(!reader.atEnd())
  {
    buf = reader.readLine();
    buf = buf.trimmed();
    buf = buf.simplified();
    tokens = buf.split(' ');

    // If the buffer is split by a ';' and ends in a ')', then
    // we are probable actually reading a file name and its coordinates
    if(tokens.size() == 4)
    {
      if(tokens[1][0] == ';' && tokens[tokens.size() - 1][tokens[tokens.size() - 1].size() - 1] == ')')
      {
        lineNum++;
      }

      files = tokens[0].split('.');
      word = files[0];
      m_RegisteredFileNames.push_back(word);

      yCoord = tokens[tokens.size() - 1];
      xCoord = tokens[tokens.size() - 2];
      yCoord.remove(')');
      xCoord.remove(',');
      xCoord.remove('(');
      m_Coords.push_back(QPointF(xCoord.toFloat(&ok), yCoord.toFloat(&ok)));
    }
  }

  m_NumImages = lineNum;

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRegisteredImageMontage::preflight()
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
void ImportRegisteredImageMontage::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  int fileCount = m_RegisteredFileNames.size();
  for(int i = 0; i < fileCount; i++)
  {
    QString imageFName = m_RegisteredFileNames[i];
    QFileInfo fi(imageFName);
    if(!fi.exists())
    {
      continue;
    }

    QString dcName = getDataContainerPrefix() + QString::number(i);

    DataContainer::Pointer m = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, dcName);
    if(getErrorCondition() < 0)
    {
      return;
    }

    {
      ImageGeom::Pointer imageGeom = m->getGeometryAs<ImageGeom>();
      size_t dims[3] = {0, 0, 0};
      std::tie(dims[0], dims[1], dims[2]) = imageGeom->getDimensions();
      dims[2] = fileCount;
      imageGeom->setDimensions(dims);
    }

    if(getCancel())
    {
      return;
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ImportRegisteredImageMontage::newFilterInstance(bool copyFilterParameters) const
{
  ImportRegisteredImageMontage::Pointer filter = ImportRegisteredImageMontage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ImportRegisteredImageMontage::getUuid()
{
  return QUuid("{5878723b-cc16-5486-ac5f-ff0107107e74}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ImportRegisteredImageMontage::getHumanLabel() const
{
  return "ITK::Import Registered Image Montage";
}

#define ITK_IMAGE_READER_CLASS_NAME ImportRegisteredImageMontage

#include "SIMPLib/ITK/itkGetComponentsDimensions.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

#include "SIMPLib/ITK/itkImageReaderHelper.cpp"
