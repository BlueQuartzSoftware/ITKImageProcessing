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
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include <memory>

#include <QtCore/QPointF>

#include "SIMPLib/SIMPLib.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageReader.h"
#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

// our PIMPL private class
class ITKImportMontagePrivate;

struct ITKMontageCache
{
  ITKImageReader::Pointer imageReader;
  QString filePath;
  int row;
  int col;
  QPointF coords;
};

/**
 * @brief The ITKImportMontage class. See [Filter documentation](@ref ITKImportMontage) for details.
 */
class ITKImageProcessing_EXPORT ITKImportMontage : public AbstractFilter
{
  Q_OBJECT
#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKImportMontage SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ITKImportMontage)
  PYB11_FILTER_PARAMETER(QString, DataContainerPrefix)
  PYB11_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  PYB11_FILTER_PARAMETER(QString, AttributeArrayName)
  PYB11_FILTER_PARAMETER(bool, ChangeOrigin)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Origin)
  PYB11_FILTER_PARAMETER(bool, UsePixelCoordinates)
  PYB11_FILTER_PARAMETER(bool, ChangeSpacing)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Spacing)
  PYB11_FILTER_PARAMETER(int32_t, LengthUnit)
  PYB11_PROPERTY(QString DataContainerPrefix READ getDataContainerPrefix WRITE setDataContainerPrefix)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString AttributeArrayName READ getAttributeArrayName WRITE setAttributeArrayName)
  PYB11_PROPERTY(bool ChangeOrigin READ getChangeOrigin WRITE setChangeOrigin)
  PYB11_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)
  PYB11_PROPERTY(bool ChangeSpacing READ getChangeSpacing WRITE setChangeSpacing)
  PYB11_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)
  PYB11_PROPERTY(QString MontageInformation READ getMontageInformation)
  PYB11_PROPERTY(int RowCount READ getRowCount)
  PYB11_PROPERTY(int ColumnCount READ getColumnCount)
#endif
  Q_DECLARE_PRIVATE(ITKImportMontage)

public:
  using Self = ITKImportMontage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  /**
   * @brief Returns the name of the class for ITKImportMontage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKImportMontage
   */
  static QString ClassName();

  ~ITKImportMontage() override;

  /**
   * @brief Setter property for DataContainerPrefix
   */
  void setDataContainerPrefix(const QString& value);
  /**
   * @brief Getter property for DataContainerPrefix
   * @return Value of DataContainerPrefix
   */
  QString getDataContainerPrefix() const;

  Q_PROPERTY(QString DataContainerPrefix READ getDataContainerPrefix WRITE setDataContainerPrefix)

  /**
   * @brief Setter property for CellAttributeMatrixName
   */
  void setCellAttributeMatrixName(const QString& value);
  /**
   * @brief Getter property for CellAttributeMatrixName
   * @return Value of CellAttributeMatrixName
   */
  QString getCellAttributeMatrixName() const;

  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  /**
   * @brief Setter property for AttributeArrayName
   */
  void setAttributeArrayName(const QString& value);
  /**
   * @brief Getter property for AttributeArrayName
   * @return Value of AttributeArrayName
   */
  QString getAttributeArrayName() const;

  Q_PROPERTY(QString AttributeArrayName READ getAttributeArrayName WRITE setAttributeArrayName)

  /**
   * @brief Setter property for ChangeOrigin
   */
  void setChangeOrigin(bool value);
  /**
   * @brief Getter property for ChangeOrigin
   * @return Value of ChangeOrigin
   */
  bool getChangeOrigin() const;

  Q_PROPERTY(bool ChangeOrigin READ getChangeOrigin WRITE setChangeOrigin)

  /**
   * @brief Setter property for Origin
   */
  void setOrigin(const FloatVec3Type& value);
  /**
   * @brief Getter property for Origin
   * @return Value of Origin
   */
  FloatVec3Type getOrigin() const;

  Q_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)

  /**
   * @brief Setter property for UsePixelCoordinates
   */
  void setUsePixelCoordinates(bool value);
  /**
   * @brief Getter property for UsePixelCoordinates
   * @return Value of UsePixelCoordinates
   */
  bool getUsePixelCoordinates() const;

  Q_PROPERTY(bool UsePixelCoordinates READ getUsePixelCoordinates WRITE setUsePixelCoordinates)

  /**
   * @brief Setter property for ChangeSpacing
   */
  void setChangeSpacing(bool value);
  /**
   * @brief Getter property for ChangeSpacing
   * @return Value of ChangeSpacing
   */
  bool getChangeSpacing() const;

  Q_PROPERTY(bool ChangeSpacing READ getChangeSpacing WRITE setChangeSpacing)

  /**
   * @brief Setter property for Spacing
   */
  void setSpacing(const FloatVec3Type& value);
  /**
   * @brief Getter property for Spacing
   * @return Value of Spacing
   */
  FloatVec3Type getSpacing() const;

  Q_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)

  /**
   * @brief Getter property for RowCount
   * @return Value of RowCount
   */
  int getRowCount() const;

  Q_PROPERTY(int RowCount READ getRowCount)

  /**
   * @brief Getter property for ColumnCount
   * @return Value of ColumnCount
   */
  int getColumnCount() const;

  Q_PROPERTY(int ColumnCount READ getColumnCount)

  /**
   * @brief Setter property for LengthUnit
   */
  void setLengthUnit(int32_t value);
  /**
   * @brief Getter property for LengthUnit
   * @return Value of LengthUnit
   */
  int32_t getLengthUnit() const;

  Q_PROPERTY(int32_t LengthUnit READ getLengthUnit WRITE setLengthUnit)

  /**
   * @brief Getter property for MontageInformation
   * @return Value of MontageInformation
   */
  QString getMontageInformation() const;

  Q_PROPERTY(QString MontageInformation READ getMontageInformation)

  using MontageCacheVector = std::vector<ITKMontageCache>;

  /**
   * @brief Setter property for MontageCacheVector
   */
  void setMontageCacheVector(const ITKImportMontage::MontageCacheVector& value);
  /**
   * @brief Getter property for MontageCacheVector
   * @return Value of MontageCacheVector
   */
  ITKImportMontage::MontageCacheVector getMontageCacheVector() const;

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
   */
  QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  QString getFilterVersion() const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  QUuid getUuid() const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

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



protected:
  void setRowCount(int32_t value);
  void setColumnCount(int32_t value);

  ITKImportMontage();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief appendToCache
   * @param reader
   * @param filePath
   * @param coords
   * @param row
   * @param col
   */
  void appendToCache(const ITKImageReader::Pointer& reader, const QString& filePath, QPointF coords, int row, int col, FloatVec3Type spacing);

  /**
   * @brief readImageFile
   * @param filePath
   * @param coords
   * @param row
   * @param col
   */
  void readImageFile(const QString& filePath, QPointF coords, int row, int col);

  /**
   * @brief readImagesFromCache
   */
  void readImagesFromCache();

  /**
   * @brief adjustOriginAndSpacing
   */
  void adjustOriginAndSpacing();

private:
  QString m_DataContainerPrefix = {};
  QString m_CellAttributeMatrixName = {};
  QString m_AttributeArrayName = {};
  bool m_ChangeOrigin = {};
  FloatVec3Type m_Origin = {};
  bool m_UsePixelCoordinates = {};
  bool m_ChangeSpacing = {};
  FloatVec3Type m_Spacing = {};
  int32_t m_LengthUnit = {};

  QScopedPointer<ITKImportMontagePrivate> const d_ptr;

  QString m_MontageInformation;

  int m_RowCount = 0;
  int m_ColumnCount = 0;

public:
  ITKImportMontage(const ITKImportMontage&) = delete;            // Copy Constructor Not Implemented
  ITKImportMontage(ITKImportMontage&&) = delete;                 // Move Constructor Not Implemented
  ITKImportMontage& operator=(const ITKImportMontage&) = delete; // Copy Assignment Not Implemented
  ITKImportMontage& operator=(ITKImportMontage&&) = delete;      // Move Assignment Not Implemented
};
