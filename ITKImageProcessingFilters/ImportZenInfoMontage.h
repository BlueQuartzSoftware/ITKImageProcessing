/* ============================================================================
 * Copyright (c) 2019-2019 BlueQuartz Software, LLC
 * All rights reserved.
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include <memory>

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

#include "ITKImageProcessing/ITKImageProcessingPlugin.h"
// our PIMPL private class
class ImportZenInfoMontagePrivate;

/**
 * @brief The ImportZenInfoMontage class. See [Filter documentation](@ref importzeninfomontage) for details.
 */
class ITKImageProcessing_EXPORT ImportZenInfoMontage : public AbstractFilter
{
  Q_OBJECT

// clang-format off
#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ImportZenInfoMontage SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ImportZenInfoMontage)
  PYB11_FILTER_NEW_MACRO(ImportZenInfoMontage)
  PYB11_FILTER_PARAMETER(QString, InputFile)
  PYB11_FILTER_PARAMETER(DataArrayPath, DataContainerPath)
  PYB11_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  PYB11_FILTER_PARAMETER(QString, ImageDataArrayName)
  PYB11_FILTER_PARAMETER(bool, ConvertToGrayScale)
  PYB11_FILTER_PARAMETER(FloatVec3Type, ColorWeights)
  PYB11_FILTER_PARAMETER(bool, ChangeOrigin)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Origin)
  PYB11_FILTER_PARAMETER(bool, ChangeSpacing)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Spacing)
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
  PYB11_PROPERTY(int32_t RowCount READ getRowCount)
  PYB11_PROPERTY(int32_t ColumnCount READ getColumnCount)
  PYB11_PROPERTY(IntVec2Type MontageStart READ getMontageStart WRITE setMontageStart)
  PYB11_PROPERTY(IntVec2Type MontageEnd READ getMontageEnd WRITE setMontageEnd)
#endif
  // clang-format on

  Q_DECLARE_PRIVATE(ImportZenInfoMontage)

public:
  using BoundsType = struct
  {
    QString Filename;
    SizeVec3Type Dims;
    FloatVec3Type Origin;
    FloatVec3Type Spacing;
    int32_t StartC;
    int32_t StartS;
    int32_t StartB;
    int32_t StartM;
    int32_t Row;
    int32_t Col;
    IDataArray::Pointer ImageDataProxy;
  };

  using Self = ImportZenInfoMontage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ImportZenInfoMontage> New();

  /**
   * @brief Returns the name of the class for ImportZenInfoMontage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ImportZenInfoMontage
   */
  static QString ClassName();

  ~ImportZenInfoMontage() override;

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
   * @brief Setter property for MontageStart
   */
  void setMontageStart(const IntVec2Type& value);
  /**
   * @brief Getter property for MontageStart
   * @return Value of MontageStart
   */
  IntVec2Type getMontageStart() const;

  Q_PROPERTY(IntVec2Type MontageStart READ getMontageStart WRITE setMontageStart)

  /**
   * @brief Setter property for MontageEnd
   */
  void setMontageEnd(const IntVec2Type& value);
  /**
   * @brief Getter property for MontageEnd
   * @return Value of MontageEnd
   */
  IntVec2Type getMontageEnd() const;

  Q_PROPERTY(IntVec2Type MontageEnd READ getMontageEnd WRITE setMontageEnd)

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
   * @brief Getter property for FilenameList
   * @return Value of FilenameList
   */
  QStringList getFilenameList() const;

  Q_PROPERTY(QStringList FilenameList READ getFilenameList)

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
   * @brief preflight Reimplemented from @see AbstractFilter class
   */
  void preflight() override;

  /**
   * @brief Setter property for Root
   */
  void setRoot(const QDomElement& value);
  /**
   * @brief Getter property for Root
   * @return Value of Root
   */
  QDomElement getRoot() const;

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
  void setBoundsCache(const std::vector<BoundsType>& value);
  /**
   * @brief Getter property for BoundsCache
   * @return Value of BoundsCache
   */
  std::vector<BoundsType> getBoundsCache() const;

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
  ImportZenInfoMontage();

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
   * @brief generateCache
   * @param exportDocument
   */
  void generateCache(QDomElement& exportDocument);

  /**
   * @brief parseImages
   * @param rootTags
   */
  void readImages();

  /**
   * @brief generateDataStructure
   */
  void generateDataStructure();

  /**
   * @brief findTileIndices
   * @param tolerance The tolerance value to group the coordinates by.
   * @param bounds The vector of BoundType objects representing one per tile.
   */
  void findTileIndices(int32_t tolerance, std::vector<BoundsType>& bounds);

private:
  QString m_InputFile = {};
  IntVec2Type m_MontageStart = {};
  IntVec2Type m_MontageEnd = {};
  DataArrayPath m_DataContainerPath = {};
  QString m_CellAttributeMatrixName = {};
  QString m_ImageDataArrayName = {};
  bool m_ConvertToGrayScale = {};
  FloatVec3Type m_ColorWeights = {};
  bool m_ChangeOrigin = {};
  FloatVec3Type m_Origin = {};
  bool m_ChangeSpacing = {};
  FloatVec3Type m_Spacing = {};

  QScopedPointer<ImportZenInfoMontagePrivate> const d_ptr;
  bool m_FileWasRead = false;
  int m_RowCount = -1;
  int m_ColumnCount = -1;
  QStringList m_FilenameList;
  int32_t m_Tolerance = 100;

public:
  /* Rule of 5: All special member functions should be defined if any are defined.
   * https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c21-if-you-define-or-delete-any-default-operation-define-or-delete-them-all
   */
  ImportZenInfoMontage(const ImportZenInfoMontage&) = delete;            // Copy Constructor Not Implemented
  ImportZenInfoMontage& operator=(const ImportZenInfoMontage&) = delete; // Copy Assignment Not Implemented
  ImportZenInfoMontage(ImportZenInfoMontage&&) = delete;                 // Move Constructor Not Implemented
  ImportZenInfoMontage& operator=(ImportZenInfoMontage&&) = delete;      // Move Assignment Not Implemented
};
