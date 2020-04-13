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

#include <memory>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/Geometry/IGeometry.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

// our PIMPL private class
class ITKImportFijiMontagePrivate;

/**
 * @brief The ITKImportFijiMontage class. See [Filter documentation](@ref ITKImportFijiMontage) for details.
 */
class ITKImageProcessing_EXPORT ITKImportFijiMontage : public AbstractFilter
{
  Q_OBJECT
  // clang-format off
  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKImportFijiMontage SUPERCLASS ITKImportMontage)
  PYB11_SHARED_POINTERS(ITKImportFijiMontage)
  PYB11_FILTER_NEW_MACRO(ITKImportFijiMontage)

  PYB11_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)
  PYB11_PROPERTY(int32_t LengthUnit READ getLengthUnit WRITE setLengthUnit)
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
  PYB11_PROPERTY(IntVec2Type ColumnMontageLimits READ getColumnMontageLimits WRITE setColumnMontageLimits)
  PYB11_PROPERTY(IntVec2Type RowMontageLimits READ getRowMontageLimits WRITE setRowMontageLimits)
  PYB11_END_BINDINGS()
  // End Python bindings declarations
  // clang-format on

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
    IGeometry::LengthUnit LengthUnit;
  };

  using Self = ITKImportFijiMontage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKImportFijiMontage> New();

  /**
   * @brief Returns the name of the class for ITKImportFijiMontage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKImportFijiMontage
   */
  static QString ClassName();

  ~ITKImportFijiMontage() override;

  /**
   * @brief Setter property for MontageName
   */
  void setMontageName(const QString& value);
  /**
   * @brief Getter property for MontageName
   * @return Value of MontageName
   */
  QString getMontageName() const;
  Q_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)

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
   * @brief Setter property for InputFile
   */
  void setInputFile(const QString& value);
  /**
   * @brief Getter property for InputFile
   * @return Value of InputFile
   */
  QString getInputFile() const;
  Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)

  /**
   * @brief Setter property for ColumnMontageLimits
   */
  void setColumnMontageLimits(const IntVec2Type& value);
  /**
   * @brief Getter property for ColumnMontageLimits
   * @return Value of ColumnMontageLimits
   */
  IntVec2Type getColumnMontageLimits() const;
  Q_PROPERTY(IntVec2Type ColumnMontageLimits READ getColumnMontageLimits WRITE setColumnMontageLimits)

  /**
   * @brief Setter property for RowMontageLimits
   */
  void setRowMontageLimits(const IntVec2Type& value);
  /**
   * @brief Getter property for RowMontageLimits
   * @return Value of RowMontageLimits
   */
  IntVec2Type getRowMontageLimits() const;
  Q_PROPERTY(IntVec2Type RowMontageLimits READ getRowMontageLimits WRITE setRowMontageLimits)

  /**
   * @brief Setter property for DataContainerPath
   */
  void setDataContainerPath(const DataArrayPath& value);
  /**
   * @brief Getter property for DataContainerPath
   * @return Value of DataContainerPath
   */
  DataArrayPath getDataContainerPath() const;
  Q_PROPERTY(DataArrayPath DataContainerPath READ getDataContainerPath WRITE setDataContainerPath)

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
   * @brief Setter property for ImageDataArrayName
   */
  void setImageDataArrayName(const QString& value);
  /**
   * @brief Getter property for ImageDataArrayName
   * @return Value of ImageDataArrayName
   */
  QString getImageDataArrayName() const;
  Q_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)

  /**
   * @brief Setter property for ConvertToGrayScale
   */
  void setConvertToGrayScale(bool value);
  /**
   * @brief Getter property for ConvertToGrayScale
   * @return Value of ConvertToGrayScale
   */
  bool getConvertToGrayScale() const;
  Q_PROPERTY(bool ConvertToGrayScale READ getConvertToGrayScale WRITE setConvertToGrayScale)

  /**
   * @brief Setter property for ColorWeights
   */
  void setColorWeights(const FloatVec3Type& value);
  /**
   * @brief Getter property for ColorWeights
   * @return Value of ColorWeights
   */
  FloatVec3Type getColorWeights() const;
  Q_PROPERTY(FloatVec3Type ColorWeights READ getColorWeights WRITE setColorWeights)

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
  int32_t getRowCount() const;
  Q_PROPERTY(int32_t RowCount READ getRowCount)

  /**
   * @brief Getter property for ColumnCount
   * @return Value of ColumnCount
   */
  int32_t getColumnCount() const;
  Q_PROPERTY(int32_t ColumnCount READ getColumnCount)

  QString getMontageInformation();
  Q_PROPERTY(QString MontageInformation READ getMontageInformation)

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
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

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
   * @brief Setter property for InputFile_Cache
   */
  void setInputFile_Cache(const QString& value);
  /**
   * @brief Getter property for InputFile_Cache
   * @return Value of InputFile_Cache
   */
  QString getInputFile_Cache() const;

  /**
   * @brief Setter property for TimeStamp_Cache
   */
  void setTimeStamp_Cache(const QDateTime& value);
  /**
   * @brief Getter property for TimeStamp_Cache
   * @return Value of TimeStamp_Cache
   */
  QDateTime getTimeStamp_Cache() const;

  /**
   * @brief Setter property for BoundsCache
   */
  void setBoundsCache(const std::vector<ITKImportFijiMontage::BoundsType>& value);
  /**
   * @brief Getter property for BoundsCache
   * @return Value of BoundsCache
   */
  std::vector<BoundsType> getBoundsCache() const;




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
  void dataCheck() override;

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
  QString m_MontageName = QString("Zen Montage");
  int32_t m_LengthUnit = 6;
  QString m_InputFile = {};
  IntVec2Type m_ColumnMontageLimits = {0, 0};
  IntVec2Type m_RowMontageLimits = {0, 0};
  IntVec2Type m_MontageStart = {0, 0};
  IntVec2Type m_MontageEnd = {0, 0};
  DataArrayPath m_DataContainerPath = {};
  QString m_CellAttributeMatrixName = {};
  QString m_ImageDataArrayName = {};
  bool m_ConvertToGrayScale = {};
  FloatVec3Type m_ColorWeights = {};
  bool m_ChangeOrigin = {};
  FloatVec3Type m_Origin = {};
  bool m_ChangeSpacing = {};
  FloatVec3Type m_Spacing = {};

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
