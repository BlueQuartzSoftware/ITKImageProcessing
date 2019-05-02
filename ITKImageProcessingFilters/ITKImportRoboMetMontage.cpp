/*
 * Your License or Copyright Information can go here
 */

#include "ITKImportRoboMetMontage.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Utilities/StringOperations.h"

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
: ITKImportMontage()
, m_RegistrationFile("")
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::initialize()
{
  clearErrorCode();
  clearWarningCode();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::setupFilterParameters()
{
  FilterParameterVectorType parameters;
  parameters.push_back(SIMPL_NEW_INPUT_FILE_FP("Registration File (Mosaic Details)", RegistrationFile, FilterParameter::Parameter, ITKImportRoboMetMontage, "", "*.txt"));
  parameters.push_back(SIMPL_NEW_INTEGER_FP("Slice Number", SliceNumber, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image File Prefix", ImageFilePrefix, FilterParameter::Parameter, ITKImportRoboMetMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image File Extension", ImageFileExtension, FilterParameter::Parameter, ITKImportRoboMetMontage));

  QStringList linkedProps("Origin");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Override Origin", ChangeOrigin, FilterParameter::Parameter, ITKImportMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Origin", Origin, FilterParameter::Parameter, ITKImportMontage));

  linkedProps.clear();
  linkedProps << "Spacing";
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Override Spacing", ChangeSpacing, FilterParameter::Parameter, ITKImportMontage, linkedProps));
  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Spacing", Spacing, FilterParameter::Parameter, ITKImportMontage));

  parameters.push_back(SIMPL_NEW_STRING_FP("Data Container Prefix", DataContainerPrefix, FilterParameter::CreatedArray, ITKImportMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Cell Attribute Matrix", CellAttributeMatrixName, FilterParameter::CreatedArray, ITKImportMontage));
  parameters.push_back(SIMPL_NEW_STRING_FP("Image Array Name", AttributeArrayName, FilterParameter::CreatedArray, ITKImportMontage));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImportRoboMetMontage::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  QString ss;

  QFileInfo fi(m_RegistrationFile);
  if(!fi.exists())
  {
    QString ss = QObject::tr("The registration file does not exist");
    setErrorCondition(-2001, ss);
    return;
  }

  if(m_RegistrationFile.isEmpty())
  {
    QString ss = QObject::tr("The registration file must be set").arg(getHumanLabel());
    setErrorCondition(-2002, ss);
    return;
  }

  int32_t err = 0;
  if (fi.completeSuffix() != "csv")
  {
    QString ss = QObject::tr("Config file is not in RoboMet format (*.csv).");
    setErrorCondition(-2003, ss);
    return;
  }

  if(m_ImageFilePrefix.isEmpty())
  {
	  QString ss = QObject::tr("The image file prefix must be set").arg(getHumanLabel());
    setErrorCondition(-2004, ss);
    return;
  }

  if(m_ImageFileExtension.isEmpty())
  {
	  QString ss = QObject::tr("The image file extension must be set").arg(getHumanLabel());
    setErrorCondition(-2005, ss);
    return;
  }

  if(getDataContainerPrefix().isEmpty())
  {
	  QString ss = QObject::tr("The data container prefix must be set").arg(getHumanLabel());
    setErrorCondition(-2006, ss);
    return;
  }

  if(getCellAttributeMatrixName().isEmpty())
  {
	  QString ss = QObject::tr("The cell attribute matrix name must be set").arg(getHumanLabel());
    setErrorCondition(-2007, ss);
    return;
  }

  if(getAttributeArrayName().isEmpty())
  {
	  QString ss = QObject::tr("The attribute array name must be set").arg(getHumanLabel());
    setErrorCondition(-2008, ss);
    return;
  }

  // Parse Robomet Config File
  QDateTime lastModified(fi.lastModified());
  size_t totalImageCount = m_RegisteredFilePaths.size();

  // Only parse the robomet config file again if the cache is outdated
  if (!getInPreflight() || getRegistrationFile() != getRoboMetConfigFilePathCache() || !getLastRead().isValid() || lastModified.msecsTo(getLastRead()) < 0)
  {
    setMontageCacheVector(MontageCacheVector());
	  clearParsingCache();

    err = parseRoboMetConfigFile();
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
      notifyStatusMessage(tr("[%1/%2]: Reading image '%3'").arg(i + 1).arg(totalImageCount).arg(imageFi.fileName()));

      readImageFile(registeredFilePath, m_CoordsMap[registeredFilePath], m_Rows[i], m_Columns[i]);
	  }

	  // Set the new data into the cache
	  setLastRead(QDateTime::currentDateTime());
	  setRoboMetConfigFilePathCache(getRegistrationFile());
  }
  else
  {
    readImagesFromCache();
  }

  if(getChangeOrigin() || getChangeSpacing())
  {
    adjustOriginAndSpacing();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ITKImportRoboMetMontage::parseRoboMetConfigFile()
{
  int32_t err = 0;
  setErrorCode(0);
  setErrorMessage("");

  bool topRowDone = false;
  QByteArray buf;
  QFile in(getRegistrationFile());
  if (!in.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString msg = QString("Csv file could not be opened: ") + getRegistrationFile();
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

      QString imageFilePath = getImageFilePath(getRegistrationFile(), imageNumber, row, col);

		  m_RegisteredFilePaths.push_back(imageFilePath);
		  m_CoordsMap.insert(imageFilePath, QPointF(xPos, yPos));
      m_Rows.push_back(row);
      m_Columns.push_back(col);
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
  QString sixDigitImageNumber = StringOperations::GeneratePaddedString(imageNumber, 6, '0');

  QString twoDigitRowNumber = StringOperations::GeneratePaddedString(row, 2, '0');

  QString twoDigitColNumber = StringOperations::GeneratePaddedString(col, 2, '0');

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
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage("Complete");
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
  m_Rows.clear();
  m_Columns.clear();
}
