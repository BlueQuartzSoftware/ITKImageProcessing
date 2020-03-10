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
#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

// our PIMPL private class
class ITKImportRoboMetMontagePrivate;

/**
 * @brief The ITKImportRoboMetMontage class. See [Filter documentation](@ref ITKImportRoboMetMontage) for details.
 */
class ITKImageProcessing_EXPORT ITKImportRoboMetMontage : public AbstractFilter
{
  Q_OBJECT

// clang-format off
#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKImportRoboMetMontage SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ITKImportRoboMetMontage)
  PYB11_FILTER_NEW_MACRO(ITKImportRoboMetMontage)
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
  PYB11_FILTER_PARAMETER(int, SliceNumber)
  PYB11_FILTER_PARAMETER(QString, ImageFilePrefix)
  PYB11_FILTER_PARAMETER(QString, ImageFileExtension)
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

  PYB11_PROPERTY(int SliceNumber READ getSliceNumber WRITE setSliceNumber)
  PYB11_PROPERTY(QString ImageFilePrefix READ getImageFilePrefix WRITE setImageFilePrefix)
  PYB11_PROPERTY(QString ImageFileExtension READ getImageFileExtension WRITE setImageFileExtension)
  PYB11_PROPERTY(int32_t LengthUnit READ getLengthUnit WRITE setLengthUnit)
  PYB11_PROPERTY(IntVec2Type MontageStart READ getMontageStart WRITE setMontageStart)
  PYB11_PROPERTY(IntVec2Type MontageEnd READ getMontageEnd WRITE setMontageEnd)
#endif
  // clang-format on
  Q_DECLARE_PRIVATE(ITKImportRoboMetMontage)
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
    //   AttributeMatrix::Pointer MetaData;
  };

    using Self = ITKImportRoboMetMontage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKImportRoboMetMontage> New();

    /**
    * @brief Returns the name of the class for ITKImportRoboMetMontage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKImportRoboMetMontage
    */
    static QString ClassName();


  ~ITKImportRoboMetMontage() override;

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
    * @brief Setter property for SliceNumber
    */
    void setSliceNumber(int value); 
    /**
    * @brief Getter property for SliceNumber
    * @return Value of SliceNumber
    */
    int getSliceNumber() const;

  Q_PROPERTY(int SliceNumber READ getSliceNumber WRITE setSliceNumber)

    /**
    * @brief Setter property for ImageFilePrefix
    */
    void setImageFilePrefix(const QString& value); 
    /**
    * @brief Getter property for ImageFilePrefix
    * @return Value of ImageFilePrefix
    */
    QString getImageFilePrefix() const;

  Q_PROPERTY(QString ImageFilePrefix READ getImageFilePrefix WRITE setImageFilePrefix)

    /**
    * @brief Setter property for ImageFileExtension
    */
    void setImageFileExtension(const QString& value); 
    /**
    * @brief Getter property for ImageFileExtension
    * @return Value of ImageFileExtension
    */
    QString getImageFileExtension() const;

  Q_PROPERTY(QString ImageFileExtension READ getImageFileExtension WRITE setImageFileExtension)

  QString getMontageInformation();
  Q_PROPERTY(QString MontageInformation READ getMontageInformation)

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
  void setBoundsCache(const std::vector<ITKImportRoboMetMontage::BoundsType>& value);
  /**
   * @brief Getter property for BoundsCache
   * @return Value of BoundsCache
   */
  std::vector<BoundsType> getBoundsCache() const;




protected:
  ITKImportRoboMetMontage();

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
   * @brief Get the file path for an image file
   */
  QString getImageFilePath(const QString& filePath, int imageNumber, int row, int col);

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
    int m_SliceNumber = {};
    QString m_ImageFilePrefix = {};
    QString m_ImageFileExtension = {};
    bool m_FileWasRead = {};
    int32_t m_LengthUnit = {};

  QScopedPointer<ITKImportRoboMetMontagePrivate> const d_ptr;

  int32_t m_NumImages = -1;
  int m_RowCount = -1;
  int m_ColumnCount = -1;

public:
  ITKImportRoboMetMontage(const ITKImportRoboMetMontage&) = delete;            // Copy Constructor Not Implemented
  ITKImportRoboMetMontage(ITKImportRoboMetMontage&&) = delete;                 // Move Constructor Not Implemented
  ITKImportRoboMetMontage& operator=(const ITKImportRoboMetMontage&) = delete; // Copy Assignment Not Implemented
  ITKImportRoboMetMontage& operator=(ITKImportRoboMetMontage&&) = delete;      // Move Assignment Not Implemented
};
