/* ============================================================================
 * Copyright (c) 2019-2019 BlueQuartz Software, LLC
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
 *    United States Air Force Prime Contract FA8650-15-D-5231
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include "SIMPLib/Filtering/AbstractFilter.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

// our PIMPL private class
class ITKImportFijiMontagePrivate;

/**
 * @brief The ITKImportFijiMontage class. See [Filter documentation](@ref ITKImportFijiMontage) for details.
 */
class ITKImageProcessing_EXPORT ITKImportFijiMontage : public AbstractFilter
{
  Q_OBJECT

  PYB11_CREATE_BINDINGS(ITKImportFijiMontage SUPERCLASS ITKImportMontage)

  PYB11_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)
  PYB11_PROPERTY(DataArrayPath DataContainerPath READ getDataContainerPath WRITE setDataContainerPath)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)
  PYB11_PROPERTY(bool ConvertToGrayScale READ getConvertToGrayScale WRITE setConvertToGrayScale)
  PYB11_PROPERTY(FloatVec3Type ColorWeights READ getColorWeights WRITE setColorWeights)
  PYB11_PROPERTY(bool ChangeOrigin READ getChangeOrigin WRITE setChangeOrigin)
  PYB11_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)
  PYB11_PROPERTY(bool ChangeSpacing READ getChangeSpacing WRITE setChangeSpacing)
  PYB11_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)
  PYB11_PROPERTY(int32_t LengthUnit READ getLengthUnit WRITE setLengthUnit)
  PYB11_PROPERTY(IntVec2Type MontageStart READ getMontageStart WRITE setMontageStart)
  PYB11_PROPERTY(IntVec2Type MontageEnd READ getMontageEnd WRITE setMontageEnd)

  Q_DECLARE_PRIVATE(ITKImportFijiMontage)

public:
  using BoundsType = struct
  {
    QString Filename;
    SizeVec3Type Dims;
    FloatVec3Type Origin;
    FloatVec3Type Spacing;
    int32_t Row;
    int32_t Col;
    IDataArray::Pointer ImageDataProxy;
  };

  SIMPL_SHARED_POINTERS(ITKImportFijiMontage)
  SIMPL_FILTER_NEW_MACRO(ITKImportFijiMontage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKImportFijiMontage, AbstractFilter)

  ~ITKImportFijiMontage() override;

  SIMPL_FILTER_PARAMETER(QString, InputFile)
  Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)

  SIMPL_FILTER_PARAMETER(IntVec2Type, MontageStart)
  Q_PROPERTY(IntVec2Type MontageStart READ getMontageStart WRITE setMontageStart)

  SIMPL_FILTER_PARAMETER(IntVec2Type, MontageEnd)
  Q_PROPERTY(IntVec2Type MontageEnd READ getMontageEnd WRITE setMontageEnd)

  SIMPL_FILTER_PARAMETER(DataArrayPath, DataContainerPath)
  Q_PROPERTY(DataArrayPath DataContainerPath READ getDataContainerPath WRITE setDataContainerPath)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, ImageDataArrayName)
  Q_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)

  SIMPL_FILTER_PARAMETER(bool, ConvertToGrayScale)
  Q_PROPERTY(bool ConvertToGrayScale READ getConvertToGrayScale WRITE setConvertToGrayScale)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, ColorWeights)
  Q_PROPERTY(FloatVec3Type ColorWeights READ getColorWeights WRITE setColorWeights)

  SIMPL_FILTER_PARAMETER(bool, ChangeOrigin)
  Q_PROPERTY(bool ChangeOrigin READ getChangeOrigin WRITE setChangeOrigin)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, Origin)
  Q_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)

  SIMPL_FILTER_PARAMETER(bool, ChangeSpacing)
  Q_PROPERTY(bool ChangeSpacing READ getChangeSpacing WRITE setChangeSpacing)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, Spacing)
  Q_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)

  SIMPL_GET_PROPERTY(int32_t, RowCount)
  Q_PROPERTY(int32_t RowCount READ getRowCount)

  SIMPL_GET_PROPERTY(int32_t, ColumnCount)
  Q_PROPERTY(int32_t ColumnCount READ getColumnCount)

  SIMPL_FILTER_PARAMETER(int32_t, LengthUnit)
  Q_PROPERTY(int32_t LengthUnit READ getLengthUnit WRITE setLengthUnit)

  QString getMontageInformation();
  Q_PROPERTY(QString MontageInformation READ getMontageInformation)

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

  SIMPL_PIMPL_PROPERTY_DECL(QString, InputFile_Cache)
  SIMPL_PIMPL_PROPERTY_DECL(QDateTime, TimeStamp_Cache)
  SIMPL_PIMPL_PROPERTY_DECL(std::vector<BoundsType>, BoundsCache)

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
   * @brief ITKImportFijiMontage::parseConfigFile
   * @param filePath
   * @return
   */
  std::vector<ITKImportFijiMontage::BoundsType> parseConfigFile(const QString& filePath);

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief flushCache
   */
  void flushCache();

  /**
   * @brief readMetaXml
   * @param device
   * @return
   */
  void generateCache();

  /**
   * @brief readImages
   */
  void readImages();

  /**
   * @brief generateDataStructure
   */
  void generateDataStructure();

  /**
   * @brief ITKImportFijiMontage::findTileIndices
   * @param tolerance
   * @param bounds
   */
  void findTileIndices(int32_t tolerance, std::vector<ITKImportFijiMontage::BoundsType>& bounds);

private:
  QScopedPointer<ITKImportFijiMontagePrivate> const d_ptr;
  bool m_FileWasRead = false;
  int m_RowCount = -1;
  int m_ColumnCount = -1;
  QStringList m_FilenameList;
  int32_t m_Tolerance = 100;

public:
  ITKImportFijiMontage(const ITKImportFijiMontage&) = delete;            // Copy Constructor Not Implemented
  ITKImportFijiMontage(ITKImportFijiMontage&&) = delete;                 // Move Constructor Not Implemented
  ITKImportFijiMontage& operator=(const ITKImportFijiMontage&) = delete; // Copy Assignment Not Implemented
  ITKImportFijiMontage& operator=(ITKImportFijiMontage&&) = delete;      // Move Assignment Not Implemented
};
