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
 * @brief The ImportImageMontage class. See [Filter documentation](@ref importimagemontage) for details.
 */
class ITKImageProcessing_EXPORT ImportImageMontage : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ImportImageMontage SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(DataArrayPath DataContainerName READ getDataContainerName WRITE setDataContainerName)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)
  PYB11_PROPERTY(FloatVec3_t Origin READ getOrigin WRITE setOrigin)
  PYB11_PROPERTY(FloatVec3_t Resolution READ getResolution WRITE setResolution)
  PYB11_PROPERTY(FileListInfo_t InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)
public:
  SIMPL_SHARED_POINTERS(ImportImageMontage)
  SIMPL_FILTER_NEW_MACRO(ImportImageMontage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ImportImageMontage, AbstractFilter)

  ~ImportImageMontage() override;

  SIMPL_FILTER_PARAMETER(DataArrayPath, DataContainerName)
  Q_PROPERTY(DataArrayPath DataContainerName READ getDataContainerName WRITE setDataContainerName)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, MetaDataAttributeMatrixName)
  Q_PROPERTY(QString MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(FloatVec3_t, Origin)
  Q_PROPERTY(FloatVec3_t Origin READ getOrigin WRITE setOrigin)

  SIMPL_FILTER_PARAMETER(FloatVec3_t, Resolution)
  Q_PROPERTY(FloatVec3_t Resolution READ getResolution WRITE setResolution)

  SIMPL_FILTER_PARAMETER(FileListInfo_t, InputFileListInfo)
  Q_PROPERTY(FileListInfo_t InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  const QString getCompiledLibraryName() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  const QString getFilterVersion() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  const QString getBrandingString() const override;

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
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

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
  ImportImageMontage();

  /**
   * @brief parseRegistrationFile Parses the ASCII file with the registration coordinates
   * @param reader QFile to read
   * @return Integer error value
   */
  int32_t parseRegistrationFile(QFile& reader);

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  DEFINE_DATAARRAY_VARIABLE(float, RegistrationCoordinates)

  StringDataArray::WeakPointer m_AttributeArrayNamesPtr;
  QFile m_InStream;
  int32_t m_NumImages;
  QVector<QString> m_ArrayNames;
  std::vector<float> m_Coords;

  /**
   * @brief Include the declarations of the ITKImageReader helper functions that are common
   * to a few different filters across different plugins.
   */
  ITK_IMAGE_READER_HELPER_ImageDataArrayName() ITK_IMAGE_READER_HELPER_DECL()

public:
  ImportImageMontage(const ImportImageMontage&) = delete; // Copy Constructor Not Implemented
  ImportImageMontage(ImportImageMontage&&) = delete;                   // Move Constructor Not Implemented
  ImportImageMontage& operator=(const ImportImageMontage&) = delete;   // Copy Assignment Not Implemented
  ImportImageMontage& operator=(ImportImageMontage&&) = delete;        // Move Assignment Not Implemented
};

