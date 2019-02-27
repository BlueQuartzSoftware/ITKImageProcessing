/*
 * Your License or Copyright Information can go here
 */

#include "ITKImportRoboMetMontage.h"

#include <QtCore/QDir>
#include <QtCore/QDateTime>

#include <math.h>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/ITK/itkReadImageImpl.hpp"
#include "SIMPLib/Utilities/FilePathGenerator.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

 /* ############## Start Private Implementation ############################### */
 // -----------------------------------------------------------------------------
 //
 // -----------------------------------------------------------------------------
class ITKImportRoboMetMontagePrivate
{
	Q_DISABLE_COPY(ITKImportRoboMetMontagePrivate)
		Q_DECLARE_PUBLIC(ITKImportRoboMetMontage)
		ITKImportRoboMetMontage* const q_ptr;
	ITKImportRoboMetMontagePrivate(ITKImportRoboMetMontage* ptr);
	QString m_RoboMetConfigFilePathCache;
	QDateTime m_LastRead;
	ITKImportRoboMetMontage::ReaderMap m_ReaderCache;
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportRoboMetMontagePrivate::ITKImportRoboMetMontagePrivate(ITKImportRoboMetMontage* ptr)
	: q_ptr(ptr)
	, m_RoboMetConfigFilePathCache("")
{
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportRoboMetMontage::ITKImportRoboMetMontage()
: m_DataContainerPrefix(SIMPL::Defaults::ImageDataContainerName + "_")
, m_CellAttributeMatrixName(SIMPL::Defaults::CellAttributeMatrixName)
, m_RegistrationFile("")
, m_AttributeArrayName("ImageTile")
, d_ptr(new ITKImportRoboMetMontagePrivate(this))
{
  m_NumImages = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImportRoboMetMontage::~ITKImportRoboMetMontage() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SIMPL_PIMPL_PROPERTY_DEF(ITKImportRoboMetMontage, QString, RoboMetConfigFilePathCache)
SIMPL_PIMPL_PROPERTY_DEF(ITKImportRoboMetMontage, QDateTime, LastRead)
SIMPL_PIMPL_GET_PROPERTY_DEF(ITKImportRoboMetMontage, ITKImportRoboMetMontage::ReaderMap, ReaderCache)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setReaderCacheValue(const QString &filePath, const ITKImageReader::Pointer &reader)
{
	Q_D(ITKImportRoboMetMontage);
	d->m_ReaderCache.insert(filePath, reader);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::clearReaderCache()
{
	Q_D(ITKImportRoboMetMontage);
	d->m_ReaderCache.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::initialize()
{
  setErrorCondition(0);
  setWarningCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setupFilterParameters()
{
  QVector<FilterParameter::Pointer> parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Registration File (Mosaic Details)", RegistrationFile, FilterParameter::Parameter, ITKImportRoboMetMontage, "", "*.txt"));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Slice Number", SliceNumber, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image File Prefix", ImageFilePrefix, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image File Suffix", ImageFileSuffix, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image File Extension", ImageFileExtension, FilterParameter::Parameter, ITKImportRoboMetMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container Prefix", DataContainerPrefix, FilterParameter::CreatedArray, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Array Name", AttributeArrayName, FilterParameter::CreatedArray, ITKImportRoboMetMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  initialize();

  QString ss;

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

  int32_t err = 0;
  if (fi.completeSuffix() != "csv")
  {
    QString ss = QObject::tr("Config file is not in RoboMet format (*.csv).");
    setErrorCondition(-2003);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  if(m_ImageFilePrefix.isEmpty())
  {
	  QString ss = QObject::tr("The image file prefix must be set").arg(getHumanLabel());
	  setErrorCondition(-2004);
	  notifyErrorMessage(getHumanLabel(), ss, -1);
	  return;
  }

  if(m_ImageFileExtension.isEmpty())
  {
	  QString ss = QObject::tr("The image file extension must be set").arg(getHumanLabel());
	  setErrorCondition(-2005);
	  notifyErrorMessage(getHumanLabel(), ss, -1);
	  return;
  }

  if(m_DataContainerPrefix.isEmpty())
  {
	  QString ss = QObject::tr("The data container prefix must be set").arg(getHumanLabel());
	  setErrorCondition(-2006);
	  notifyErrorMessage(getHumanLabel(), ss, -1);
	  return;
  }

  if(m_CellAttributeMatrixName.isEmpty())
  {
	  QString ss = QObject::tr("The cell attribute matrix name must be set").arg(getHumanLabel());
	  setErrorCondition(-2007);
	  notifyErrorMessage(getHumanLabel(), ss, -1);
	  return;
  }

  if(m_AttributeArrayName.isEmpty())
  {
	  QString ss = QObject::tr("The attribute array name must be set").arg(getHumanLabel());
	  setErrorCondition(-2008);
	  notifyErrorMessage(getHumanLabel(), ss, -1);
	  return;
  }

  // Parse Fiji Config File
  QDateTime lastModified(fi.lastModified());
  size_t totalImageCount = m_RegisteredFilePaths.size();

  // Only parse the fiji config file again if the cache is outdated
  if (!getInPreflight() || getRegistrationFile() != getRoboMetConfigFilePathCache() || !getLastRead().isValid() || lastModified.msecsTo(getLastRead()) < 0)
  {
	  clearReaderCache();
	  clearParsingCache();

	  err = parseRoboMetConfigFile(getRegistrationFile());
	  if (err < 0)
	  {
		  return;
	  }

	  QVector<size_t> cDims(1, 1);

	  for (int i = 0; i < m_RegisteredFilePaths.size(); i++)
	  {
		  if (getCancel())
		  {
			  return;
		  }

		  QString registeredFilePath = m_RegisteredFilePaths[i];
		  QFileInfo imageFi(registeredFilePath);
		  notifyStatusMessage(getHumanLabel(), tr("[%1/%2]: Reading image '%3'").arg(i + 1)
			  .arg(totalImageCount).arg(imageFi.fileName()));

		  readImageFile(registeredFilePath);
	  }

	  // Set the new data into the cache
	  setLastRead(QDateTime::currentDateTime());
	  setRoboMetConfigFilePathCache(getRegistrationFile());
  }
  else
  {
	  ReaderMap readerCache = getReaderCache();
	  for (ReaderMap::iterator iter = readerCache.begin(); iter != readerCache.end(); iter++)
	  {
		  QString filePath = iter.key();

		  QString rowColIdString = m_RowColIdMap[filePath];
		  QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(rowColIdString);

		  ITKImageReader::Pointer reader = iter.value();
		  reader->setDataContainerName(dcName);
		  reader->setCellAttributeMatrixName(getCellAttributeMatrixName());
		  reader->setImageDataArrayName(getAttributeArrayName());
		  reader->setDataContainerArray(DataContainerArray::New());
		  if (getInPreflight())
		  {
			  reader->preflight();
		  }
		  else
		  {
			  reader->execute();
		  }

		  DataContainerArray::Pointer filterDca = reader->getDataContainerArray();
		  QList<DataContainer::Pointer> dcs = filterDca->getDataContainers();
		  for (DataContainer::Pointer dc : dcs)
		  {
			  getDataContainerArray()->addDataContainer(dc);

			  QPointF coords = m_CoordsMap[filePath];

			  ImageGeom::Pointer geom = dc->getGeometryAs<ImageGeom>();

			  geom->setOrigin(coords.x(), coords.y(), 1.0f);
		  }
	  }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::readImageFile(const QString &filePath)
{
	QFileInfo fi(filePath);
	QString rowColIdString = m_RowColIdMap[filePath];
	QPointF coords = m_CoordsMap[filePath];

	QString dcName = tr("%1_%2").arg(getDataContainerPrefix()).arg(rowColIdString);

	ITKImageReader::Pointer reader = ITKImageReader::New();
	reader->setFileName(fi.filePath());
	reader->setDataContainerName(dcName);
	reader->setCellAttributeMatrixName(getCellAttributeMatrixName());
	reader->setImageDataArrayName(getAttributeArrayName());

	if (getInPreflight())
	{
		reader->preflight();
	}
	else
	{
		reader->execute();
	}

	DataContainerArray::Pointer filterDca = reader->getDataContainerArray();
	QList<DataContainer::Pointer> dcs = filterDca->getDataContainers();
	for (DataContainer::Pointer dc : dcs)
	{
		getDataContainerArray()->addDataContainer(dc);
	}

	DataContainer::Pointer m = getDataContainerArray()->getPrereqDataContainer<AbstractFilter>(this, dcName);
	if (getErrorCondition() < 0)
	{
		return;
	}

	ImageGeom::Pointer geom = m->getGeometryAs<ImageGeom>();
	geom->setOrigin(coords.x(), coords.y(), 1.0f);

	setReaderCacheValue(filePath, reader);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ITKImportRoboMetMontage::parseRoboMetConfigFile(const QString &filePath)
{
  int32_t err = 0;
  setErrorCode(0);
  setErrorMessage("");

  bool topRowDone = false;
  QByteArray buf;
  QFile in(filePath);
  if (!in.open(QIODevice::ReadOnly | QIODevice::Text))
  {
	  QString msg = QString("Csv file could not be opened: ") + getFileName();
	  setErrorCode(-100);
	  setErrorMessage(msg);
	  return -100;
  }

  int imageNumberIndex = -1;
  int rowIndex = -1;
  int colIndex = -1;
  int xPosIndex = -1;
  int yPosIndex = -1;

  while (!in.atEnd())
  {
	  buf = in.readLine();
	  QList<QByteArray> line = buf.split(',');
	  if (!topRowDone)
	  {
		  for (auto columnName : line)
		  {
			  if (QString::compare(columnName.trimmed(), "row#", Qt::CaseInsensitive) == 0)
			  {
				  rowIndex = line.indexOf(columnName);
			  }
			  else if (QString::compare(columnName.trimmed(), "col#", Qt::CaseInsensitive) == 0)
			  {
				  colIndex = line.indexOf(columnName);
			  }
			  else if (QString::compare(columnName.trimmed(), "imageNumber", Qt::CaseInsensitive) == 0)
			  {
				  imageNumberIndex = line.indexOf(columnName);
			  }
			  else if (QString::compare(columnName.trimmed(), "Xposition", Qt::CaseInsensitive) == 0)
			  {
				  xPosIndex = line.indexOf(columnName);
			  }
			  else if (QString::compare(columnName.trimmed(), "Yposition", Qt::CaseInsensitive) == 0)
			  {
				  yPosIndex = line.indexOf(columnName);
			  }
		  }

		  if (imageNumberIndex < 0 || rowIndex < 0 || colIndex < 0 || xPosIndex < 0 || yPosIndex < 0)
		  {
			  setErrorCode(-101);
			  setErrorMessage("Required columns missing in CSV file.");
			  return -101;
		  }

		  topRowDone = true;
	  }
	  else
	  {
		  int imageNumber = line[imageNumberIndex].toInt();
		  if (imageNumber != getSliceNumber())
		  {
			  continue; // Skip irrelevant numbers
		  }
		  int row = line[rowIndex].toInt();
		  int col = line[colIndex].toInt();
		  		  
		  double xPos = line[xPosIndex].trimmed().toDouble();
		  double yPos = line[yPosIndex].trimmed().toDouble();

		  QString imageFilePath = getImageFilePath(filePath, imageNumber, row, col);

		  m_RegisteredFilePaths.push_back(imageFilePath);
		  m_CoordsMap.insert(imageFilePath, QPointF(xPos, yPos));
		  m_RowColIdMap.insert(imageFilePath, tr("r%1c%2").arg(row).arg(col));
		  m_NumImages++;
	  }

  }

  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ITKImportRoboMetMontage::getImageFilePath(const QString &filePath, int imageNumber,
	int row, int col)
{
	QFileInfo fi(filePath);
	int numberOfDigits = imageNumber == 0 ? 1 : ceil(log10(imageNumber));
	int numRowDigits = row <= 1 ? 1 : ceil(log10(row));
	int numColDigits = col <= 1 ? 1 : ceil(log10(col));
	QString sixDigitImageNumber;
	for (int digits = 6; digits > numberOfDigits; digits--)
	{
		sixDigitImageNumber.append("0");
	}
	sixDigitImageNumber.append(QString::number(imageNumber));

	QString twoDigitRowNumber;
	for (int digits = 2; digits > numRowDigits; digits--)
	{
		twoDigitRowNumber.append("0");
	} 
	twoDigitRowNumber.append(QString::number(row));

	QString twoDigitColNumber;
	for (int digits = 2; digits > numColDigits; digits--)
	{
		twoDigitColNumber.append("0");
	}
	twoDigitColNumber.append(QString::number(col));

	QString imageDir = getImageFilePrefix() + '_' + sixDigitImageNumber;
	QString imageFile = getImageFilePrefix() + '_' + sixDigitImageNumber + '_' +
		twoDigitColNumber + '_' + twoDigitRowNumber + '.' + getImageFileExtension();
	QString imageFilePath = fi.path() + QDir::separator() + imageDir + QDir::separator() + imageFile;

	return imageFilePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::preflight()
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
void ITKImportRoboMetMontage::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImportRoboMetMontage::newFilterInstance(bool copyFilterParameters) const
{
  ITKImportRoboMetMontage::Pointer filter = ITKImportRoboMetMontage::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getBrandingString() const
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getGroupName() const
{
  return SIMPL::FilterGroups::IOFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKImportRoboMetMontage::getUuid()
{
  return QUuid("{cdb130af-3616-57b1-be59-fe18113b2621}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::InputFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImportRoboMetMontage::getHumanLabel() const
{
  return "Import RoboMet Montage";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::clearParsingCache()
{
	m_NumImages = 0;
	m_RegisteredFilePaths.clear();
	m_CoordsMap.clear();
	m_RowColIdMap.clear();
}

#define ITK_IMAGE_READER_CLASS_NAME ITKImportRoboMetMontage

#include "SIMPLib/ITK/itkGetComponentsDimensions.h"
#include "SIMPLib/ITK/itkInPlaceImageToDream3DDataFilter.h"

#include "SIMPLib/ITK/itkImageReaderHelper.cpp"
