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

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ImportRegisteredImageMontage class. See [Filter documentation](@ref importregisteredimagemontage) for details.
 */
class ITKImageProcessing_EXPORT ImportRegisteredImageMontage : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ImportRegisteredImageMontage SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(QString DataContainerName READ getDataContainerName WRITE setDataContainerName)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)
  PYB11_PROPERTY(FloatVec3_t Origin READ getOrigin WRITE setOrigin)
  PYB11_PROPERTY(FloatVec3_t Resolution READ getResolution WRITE setResolution)
  PYB11_PROPERTY(QString RegistrationFile READ getRegistrationFile WRITE setRegistrationFile)
  PYB11_PROPERTY(FileListInfo_t InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)
  PYB11_PROPERTY(QString RegistrationCoordinatesArrayName READ getRegistrationCoordinatesArrayName WRITE setRegistrationCoordinatesArrayName)
  PYB11_PROPERTY(QString AttributeArrayNamesArrayName READ getAttributeArrayNamesArrayName WRITE setAttributeArrayNamesArrayName)
public:
  SIMPL_SHARED_POINTERS(ImportRegisteredImageMontage)
  SIMPL_FILTER_NEW_MACRO(ImportRegisteredImageMontage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ImportRegisteredImageMontage, AbstractFilter)

  ~ImportRegisteredImageMontage() override;

  SIMPL_FILTER_PARAMETER(QString, DataContainerPrefix)
  Q_PROPERTY(QString DataContainerPrefix READ getDataContainerPrefix WRITE setDataContainerPrefix)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(FloatVec3_t, Origin)
  Q_PROPERTY(FloatVec3_t Origin READ getOrigin WRITE setOrigin)

  SIMPL_FILTER_PARAMETER(FloatVec3_t, Resolution)
  Q_PROPERTY(FloatVec3_t Resolution READ getResolution WRITE setResolution)

  SIMPL_FILTER_PARAMETER(QString, RegistrationFile)
  Q_PROPERTY(QString RegistrationFile READ getRegistrationFile WRITE setRegistrationFile)

  SIMPL_FILTER_PARAMETER(FileListInfo_t, InputFileListInfo)
  Q_PROPERTY(FileListInfo_t InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)

  SIMPL_FILTER_PARAMETER(QString, AttributeArrayName)
  Q_PROPERTY(QString AttributeArrayName READ getAttributeArrayName WRITE setAttributeArrayName)

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
  ImportRegisteredImageMontage();

  /**
   * @brief parseFijiConfigFile Parses the Fiji file with the configuration coordinates
   * @param reader QFile to read
   * @return Integer error value
   */
  int32_t parseFijiConfigFile(QFile& reader);

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

private:
  QFile m_InStream;
  int32_t m_NumImages;
  std::vector<QString> m_RegisteredFileNames;
  std::vector<QPointF> m_Coords;
  std::vector<QString> m_RowColId;

  /**
   * @brief Include the declarations of the ITKImageReader helper functions that are common
   * to a few different filters across different plugins.
   */
  ITK_IMAGE_READER_HELPER_ImageDataArrayName() ITK_IMAGE_READER_HELPER_DECL()

public :
  ImportRegisteredImageMontage(const ImportRegisteredImageMontage&) = delete; // Copy Constructor Not Implemented
  ImportRegisteredImageMontage(ImportRegisteredImageMontage&&) = delete;                   // Move Constructor Not Implemented
  ImportRegisteredImageMontage& operator=(const ImportRegisteredImageMontage&) = delete;   // Copy Assignment Not Implemented
  ImportRegisteredImageMontage& operator=(ImportRegisteredImageMontage&&) = delete;        // Move Assignment Not Implemented
};

