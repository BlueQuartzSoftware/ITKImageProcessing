/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagsXmlSection.h"

// our PIMPL private class
class ImportAxioVisionV4MontagePrivate;

/**
 * @class ImportAxioVisionV4Montage ImportAxioVisionV4Montage.h ZeissImport/ImportAxioVisionV4Montages/ImportAxioVisionV4Montage.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ITKImageProcessing_EXPORT ImportAxioVisionV4Montage : public AbstractFilter
{
  Q_OBJECT

  // Start Python bindings declarations
  // clang-format off
  PYB11_BEGIN_BINDINGS(ImportAxioVisionV4Montage SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ImportAxioVisionV4Montage)
  PYB11_FILTER_NEW_MACRO(ImportAxioVisionV4Montage)
  PYB11_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)
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
  PYB11_PROPERTY(bool ImportAllMetaData READ getImportAllMetaData WRITE setImportAllMetaData)
  PYB11_PROPERTY(QString MetaDataAttributeMatrixName getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)
  PYB11_PROPERTY(IntVec2Type ColumnMontageLimits READ getColumnMontageLimits WRITE setColumnMontageLimits)
  PYB11_PROPERTY(IntVec2Type RowMontageLimits READ getRowMontageLimits WRITE setRowMontageLimits)
  PYB11_END_BINDINGS()
  // clang-format on
  // End Python bindings declarations

  Q_DECLARE_PRIVATE(ImportAxioVisionV4Montage)

public:
  struct BoundsType
  {
    QString Filename;
    SizeVec3Type Dims;
    FloatVec3Type Origin;
    FloatVec3Type Spacing;
    int32_t Row;
    int32_t Col;
    IDataArray::Pointer ImageDataProxy;
    AttributeMatrix::Pointer MetaData;
    IGeometry::LengthUnit LengthUnit;
  };

  using Self = ImportAxioVisionV4Montage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ImportAxioVisionV4Montage> New();

  /**
   * @brief Returns the name of the class for ImportAxioVisionV4Montage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ImportAxioVisionV4Montage
   */
  static QString ClassName();

  ~ImportAxioVisionV4Montage() override;

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
   * @brief Setter property for MetaDataAttributeMatrixName
   */
  void setMetaDataAttributeMatrixName(const QString& value);
  /**
   * @brief Getter property for MetaDataAttributeMatrixName
   * @return Value of MetaDataAttributeMatrixName
   */
  QString getMetaDataAttributeMatrixName() const;
  Q_PROPERTY(QString MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)

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
   * @brief Setter property for ImportAllMetaData
   */
  void setImportAllMetaData(bool value);
  /**
   * @brief Getter property for ImportAllMetaData
   * @return Value of ImportAllMetaData
   */
  bool getImportAllMetaData() const;
  Q_PROPERTY(bool ImportAllMetaData READ getImportAllMetaData WRITE setImportAllMetaData)

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
   * @brief Setter property for FileWasRead
   */
  void setFileWasRead(bool value);
  /**
   * @brief Getter property for FileWasRead
   * @return Value of FileWasRead
   */
  bool getFileWasRead() const;
  Q_PROPERTY(bool FileWasRead READ getFileWasRead)

  /**
   * @brief Setter property for GeneratedFileList
   */
  void setGeneratedFileList(const QStringList& value);
  /**
   * @brief Getter property for GeneratedFileList
   * @return Value of GeneratedFileList
   */
  QStringList getGeneratedFileList() const;
  Q_PROPERTY(QStringList GeneratedFileList READ getGeneratedFileList)

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

protected:
  ImportAxioVisionV4Montage();

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
  void generateCache(QDomElement& root);

  /**
   * @brief readImages
   */
  void readImages();

  /**
   * @brief generateDataStructure
   */
  void generateDataStructure();

  /**
   * @brief getImageScaling
   * @param scalingTagsSection
   * @return
   */
  ImageGeom::Pointer initializeImageGeom(const QDomElement& root, const ZeissTagsXmlSection::Pointer& photoTagsSection);

  /**
   * @brief generateMetaDataAttributeMatrix
   * @param photoTagsSection
   * @param imageCount
   * @return
   */
  void addMetaData(const AttributeMatrix::Pointer& metaAm, const ZeissTagsXmlSection::Pointer& photoTagsSection, int index);

  /**
   * @brief generateMetaDataAttributeMatrix -root data
   * @param rootTagsSection
   * @param imageCount
   * @return
   */

  void addRootMetaData(const AttributeMatrix::Pointer& metaAm, const ZeissTagsXmlSection::Pointer& rootTagsSection, int index);

private:
  QString m_MontageName = QString("AxioVision Montage");
  QString m_InputFile = {};
  IntVec2Type m_ColumnMontageLimits = {0, 0};
  IntVec2Type m_RowMontageLimits = {0, 0};
  IntVec2Type m_MontageStart = {0, 0};
  IntVec2Type m_MontageEnd = {0, 0};
  DataArrayPath m_DataContainerPath = {};
  QString m_CellAttributeMatrixName = {};
  QString m_ImageDataArrayName = {};
  QString m_MetaDataAttributeMatrixName = {};
  bool m_ConvertToGrayScale = false;
  bool m_ImportAllMetaData = false;
  FloatVec3Type m_ColorWeights = {};
  bool m_ChangeOrigin = false;
  FloatVec3Type m_Origin = {};
  bool m_ChangeSpacing = false;
  FloatVec3Type m_Spacing = {};
  bool m_FileWasRead = {};
  QStringList m_GeneratedFileList = {};

  QScopedPointer<ImportAxioVisionV4MontagePrivate> const d_ptr;

  int m_RowCount = -1;
  int m_ColumnCount = -1;
  QStringList m_FilenameList;

public:
  ImportAxioVisionV4Montage(const ImportAxioVisionV4Montage&) = delete;            // Copy Constructor Not Implemented
  ImportAxioVisionV4Montage(ImportAxioVisionV4Montage&&) = delete;                 // Move Constructor Not Implemented
  ImportAxioVisionV4Montage& operator=(const ImportAxioVisionV4Montage&) = delete; // Copy Assignment Not Implemented
  ImportAxioVisionV4Montage& operator=(ImportAxioVisionV4Montage&&) = delete;      // Move Assignment Not Implemented
};
