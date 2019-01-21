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
#include "SIMPLib/ITK/itkFijiConfigurationFileReader.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

// our PIMPL private class
class ITKImportFijiMontagePrivate;

/**
 * @brief The ITKImportFijiMontage class. See [Filter documentation](@ref ITKImportFijiMontage) for details.
 */
class ITKImageProcessing_EXPORT ITKImportFijiMontage : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKImportFijiMontage SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(QString DataContainerPrefix READ getDataContainerPrefix WRITE setDataContainerPrefix)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString FijiConfigFilePath READ getFijiConfigFilePath WRITE setFijiConfigFilePath)
  PYB11_PROPERTY(QString AttributeArrayName READ getAttributeArrayName WRITE setAttributeArrayName)
  Q_DECLARE_PRIVATE(ITKImportFijiMontage)
public:
  SIMPL_SHARED_POINTERS(ITKImportFijiMontage)
  SIMPL_FILTER_NEW_MACRO(ITKImportFijiMontage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKImportFijiMontage, AbstractFilter)

  ~ITKImportFijiMontage() override;

  SIMPL_FILTER_PARAMETER(QString, DataContainerPrefix)
  Q_PROPERTY(QString DataContainerPrefix READ getDataContainerPrefix WRITE setDataContainerPrefix)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, AttributeArrayName)
  Q_PROPERTY(QString AttributeArrayName READ getAttributeArrayName WRITE setAttributeArrayName)

  SIMPL_FILTER_PARAMETER(QString, FijiConfigFilePath)
  Q_PROPERTY(QString FijiConfigFilePath READ getFijiConfigFilePath WRITE setFijiConfigFilePath)

  typedef std::vector<ITKImageReader::Pointer> ImageReaderVector;
  typedef std::vector<itk::FijiImageTileData> TileDataVector;

  SIMPL_PIMPL_PROPERTY_DECL(QString, FijiConfigFilePathCache)
  SIMPL_PIMPL_PROPERTY_DECL(QDateTime, LastRead)
  SIMPL_PIMPL_PROPERTY_DECL(ImageReaderVector, ImageReaderCache)
  SIMPL_PIMPL_PROPERTY_DECL(TileDataVector, TileDataCache)

  /**
   * @brief appendImageReaderToCache
   * @param reader
   */
  void appendImageReaderToCache(const ITKImageReader::Pointer &reader);

  /**
   * @brief appendImageTileToCache
   * @param tileData
   */
  void appendImageTileToCache(const itk::FijiImageTileData &tileData);

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
  ITKImportFijiMontage();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  QScopedPointer<ITKImportFijiMontagePrivate> const d_ptr;

  /**
   * @brief readDataFile
   * @param imageTileData
   */
  void readImageFile(itk::FijiImageTileData imageTileData);

public :
  ITKImportFijiMontage(const ITKImportFijiMontage&) = delete; // Copy Constructor Not Implemented
  ITKImportFijiMontage(ITKImportFijiMontage&&) = delete;                   // Move Constructor Not Implemented
  ITKImportFijiMontage& operator=(const ITKImportFijiMontage&) = delete;   // Copy Assignment Not Implemented
  ITKImportFijiMontage& operator=(ITKImportFijiMontage&&) = delete;        // Move Assignment Not Implemented
};

