/*
 * Your License or Copyright Information can go here
 */

#pragma once

#include <QtCore/QFile>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "SIMPLib/ITK/itkImageReaderHelper.h"

#include <itkImageFileReader.h>

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

 // our PIMPL private class
class ITKImportRoboMetMontagePrivate;

/**
 * @brief The ITKImportRoboMetMontage class. See [Filter documentation](@ref ITKImportRoboMetMontage) for details.
 */
class ITKImageProcessing_EXPORT ITKImportRoboMetMontage : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKImportRoboMetMontage SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(int SliceNumber READ getSliceNumber WRITE setSliceNumber)
  PYB11_PROPERTY(DataArrayPath DataContainerName READ getDataContainerName WRITE setDataContainerName)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)
  PYB11_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)
  PYB11_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)
  PYB11_PROPERTY(QString RegistrationFile READ getRegistrationFile WRITE setRegistrationFile)
  PYB11_PROPERTY(FileListInfo_t InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)
  PYB11_PROPERTY(QString RegistrationCoordinatesArrayName READ getRegistrationCoordinatesArrayName WRITE setRegistrationCoordinatesArrayName)
  PYB11_PROPERTY(QString AttributeArrayName READ getAttributeArrayName WRITE setAttributeArrayName)
  PYB11_PROPERTY(QString ImageFilePrefix READ getImageFilePrefix WRITE setImageFilePrefix)
  PYB11_PROPERTY(QString ImageFileSuffix READ getImageFileSuffix WRITE setImageFileSuffix)
  PYB11_PROPERTY(QString ImageFileExtension READ getImageFileExtension WRITE setImageFileExtension)
  Q_DECLARE_PRIVATE(ITKImportRoboMetMontage)
public:
  SIMPL_SHARED_POINTERS(ITKImportRoboMetMontage)
  SIMPL_FILTER_NEW_MACRO(ITKImportRoboMetMontage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKImportRoboMetMontage, AbstractFilter)

  ~ITKImportRoboMetMontage() override;
  /**
   * @brief These get filled out if there are errors. Negative values are error codes
   */
  SIMPL_INSTANCE_PROPERTY(int, ErrorCode)

  SIMPL_INSTANCE_STRING_PROPERTY(ErrorMessage)

  SIMPL_FILTER_PARAMETER(QString, DataContainerName)
  Q_PROPERTY(QString DataContainerName READ getDataContainerName WRITE setDataContainerName)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(int, SliceNumber)
  Q_PROPERTY(int SliceNumber READ getSliceNumber WRITE setSliceNumber)

  SIMPL_FILTER_PARAMETER(QString, MetaDataAttributeMatrixName)
  Q_PROPERTY(QString MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, Origin)
  Q_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, Spacing)
  Q_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)

  SIMPL_FILTER_PARAMETER(QString, RegistrationFile)
  Q_PROPERTY(QString RegistrationFile READ getRegistrationFile WRITE setRegistrationFile)

  SIMPL_FILTER_PARAMETER(QString, AttributeArrayName)
  Q_PROPERTY(QString AttributeArrayName READ getAttributeArrayName WRITE setAttributeArrayName)

  SIMPL_FILTER_PARAMETER(QString, ImageFilePrefix)
  Q_PROPERTY(QString ImageFilePrefix READ getImageFilePrefix WRITE setImageFilePrefix)

  SIMPL_FILTER_PARAMETER(QString, ImageFileSuffix)
  Q_PROPERTY(QString ImageFileSuffix READ getImageFileSuffix WRITE setImageFileSuffix)

  SIMPL_FILTER_PARAMETER(QString, ImageFileExtension)
  Q_PROPERTY(QString ImageFileExtension READ getImageFileExtension WRITE setImageFileExtension)

  typedef QMap<QString, ITKImageReader::Pointer> ReaderMap;

  SIMPL_PIMPL_PROPERTY_DECL(QString, RoboMetConfigFilePathCache)
  SIMPL_PIMPL_PROPERTY_DECL(QDateTime, LastRead)
  SIMPL_PIMPL_GET_PROPERTY_DECL(ReaderMap, ReaderCache)

  /**
   * @brief setReaderCacheValue
   * @param filePath
   * @param reader
   */
  void setReaderCacheValue(const QString &filePath, const ITKImageReader::Pointer &reader);

  /**
   * @brief clearReaderCache
   */
  void clearReaderCache();

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  const QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  const QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  const QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  const QUuid getUuid() override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  const QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
   * @brief preflight Reimplemented from @see AbstractFilter class
   */
  void preflight() override;

signals:
  /**
   * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
   * be pushed from a user-facing control (such as a widget)
   * @param filter Filter instance pointer
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged Emitted when any Filter parameter is changed internally
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before calling dataCheck()
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after calling dataCheck()
   */
  void preflightExecuted();

protected:
  ITKImportRoboMetMontage();

  /**
   * @brief parseFijiConfigFile Parses the Fiji file with the configuration coordinates
   * @param reader QFile to read
   * @return Integer error value
   */
  int32_t parseRoboMetConfigFile(const QString &filePath);

  /**
   * @brief parseRobometConfigFile Parses the Robomet file with the configuration coordinates
   * @param reader QFile to read
   * @return Integer error value
   */
  int32_t parseRobometConfigFile(QFile& reader);

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Get the file path for an image file
   */
  QString getImageFilePath(const QString &filePath, int imageNumber, int row, int col);

private:
  QScopedPointer<ITKImportRoboMetMontagePrivate> const d_ptr;
	
  int32_t m_NumImages;
  std::vector<QString> m_RegisteredFilePaths;
  QMap<QString, QPointF> m_CoordsMap;
  QMap<QString, QString> m_RowColIdMap;

  /**
   * @brief readDataFile
   * @param filePath
   */
  void readImageFile(const QString &filePath);

  /**
   * @brief clearParsingCache
   */
  void clearParsingCache();

  /**
   * @brief Include the declarations of the ITKImageReader helper functions that are common
   * to a few different filters across different plugins.
   */
  ITK_IMAGE_READER_HELPER_ImageDataArrayName() ITK_IMAGE_READER_HELPER_DECL()

public :
  ITKImportRoboMetMontage(const ITKImportRoboMetMontage&) = delete; // Copy Constructor Not Implemented
  ITKImportRoboMetMontage(ITKImportRoboMetMontage&&) = delete;                   // Move Constructor Not Implemented
  ITKImportRoboMetMontage& operator=(const ITKImportRoboMetMontage&) = delete;   // Copy Assignment Not Implemented
  ITKImportRoboMetMontage& operator=(ITKImportRoboMetMontage&&) = delete;        // Move Assignment Not Implemented
};

