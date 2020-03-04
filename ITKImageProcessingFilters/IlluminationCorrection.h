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

#include <QtCore/QMutex>
#include <QtCore/QString>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/IGeometryGrid.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "ITKImageProcessing/ZeissXml/ZeissTagsXmlSection.h"
#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @class IlluminationCorrection IlluminationCorrection.h ZeissImport/IlluminationCorrections/IlluminationCorrection.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ITKImageProcessing_EXPORT IlluminationCorrection : public AbstractFilter
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(IlluminationCorrection SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(IlluminationCorrection)
  PYB11_FILTER_NEW_MACRO(IlluminationCorrection)
  PYB11_FILTER_PARAMETER(QStringList, DataContainers)
  PYB11_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  PYB11_FILTER_PARAMETER(QString, ImageDataArrayName)
  PYB11_FILTER_PARAMETER(QString, CorrectedImageDataArrayName)
  PYB11_FILTER_PARAMETER(bool, ExportCorrectedImages)
  PYB11_FILTER_PARAMETER(QString, OutputPath)
  PYB11_FILTER_PARAMETER(QString, FileExtension)
  PYB11_FILTER_PARAMETER(DataArrayPath, BackgroundDataContainerPath)
  PYB11_FILTER_PARAMETER(DataArrayPath, BackgroundCellAttributeMatrixPath)
  PYB11_FILTER_PARAMETER(DataArrayPath, BackgroundImageArrayPath)
  PYB11_FILTER_PARAMETER(uint32_t, LowThreshold)
  PYB11_FILTER_PARAMETER(uint32_t, HighThreshold)
  PYB11_FILTER_PARAMETER(bool, ApplyCorrection)
  PYB11_FILTER_PARAMETER(bool, ApplyMedianFilter)
  PYB11_FILTER_PARAMETER(FloatVec3Type, MedianRadius)
  PYB11_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)
  PYB11_PROPERTY(QString CorrectedImageDataArrayName READ getCorrectedImageDataArrayName WRITE setCorrectedImageDataArrayName)
  PYB11_PROPERTY(bool ExportCorrectedImages READ getExportCorrectedImages WRITE setExportCorrectedImages)
  PYB11_PROPERTY(QString OutputPath READ getOutputPath WRITE setOutputPath)
  PYB11_PROPERTY(QString FileExtension READ getFileExtension WRITE setFileExtension)

  PYB11_PROPERTY(DataArrayPath BackgroundDataContainerPath READ getBackgroundDataContainerPath WRITE setBackgroundDataContainerPath)
  PYB11_PROPERTY(DataArrayPath BackgroundCellAttributeMatrixPath READ getBackgroundCellAttributeMatrixPath WRITE setBackgroundCellAttributeMatrixPath)
  PYB11_PROPERTY(DataArrayPath BackgroundImageArrayPath READ getBackgroundImageArrayPath WRITE setBackgroundImageArrayPath)

  PYB11_PROPERTY(uint32_t LowThreshold READ getLowThreshold WRITE setLowThreshold)
  PYB11_PROPERTY(uint32_t HighThreshold READ getHighThreshold WRITE setHighThreshold)
  PYB11_PROPERTY(bool ApplyCorrection READ getApplyCorrection WRITE setApplyCorrection)
  PYB11_PROPERTY(bool ApplyMedianFilter READ getApplyMedianFilter WRITE setApplyMedianFilter)
  PYB11_PROPERTY(FloatVec3Type MedianRadius READ getMedianRadius WRITE setMedianRadius)
#endif

public:
    using Self = IlluminationCorrection;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<IlluminationCorrection> New();

    /**
    * @brief Returns the name of the class for IlluminationCorrection
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for IlluminationCorrection
    */
    static QString ClassName();


  ~IlluminationCorrection() override;

    /**
    * @brief Setter property for DataContainerName
    */
    void setDataContainerName(const QString& value) ;
    /**
    * @brief Getter property for DataContainerName
    * @return Value of DataContainerName
    */
    QString getDataContainerName() const ;


    /**
    * @brief Setter property for DataContainers
    */
    void setDataContainers(const QStringList& value); 
    /**
    * @brief Getter property for DataContainers
    * @return Value of DataContainers
    */
    QStringList getDataContainers() const;

  Q_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)

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
    * @brief Setter property for CorrectedImageDataArrayName
    */
    void setCorrectedImageDataArrayName(const QString& value); 
    /**
    * @brief Getter property for CorrectedImageDataArrayName
    * @return Value of CorrectedImageDataArrayName
    */
    QString getCorrectedImageDataArrayName() const;

  Q_PROPERTY(QString CorrectedImageDataArrayName READ getCorrectedImageDataArrayName WRITE setCorrectedImageDataArrayName)

    /**
    * @brief Setter property for ExportCorrectedImages
    */
    void setExportCorrectedImages(bool value); 
    /**
    * @brief Getter property for ExportCorrectedImages
    * @return Value of ExportCorrectedImages
    */
    bool getExportCorrectedImages() const;

  Q_PROPERTY(bool ExportCorrectedImages READ getExportCorrectedImages WRITE setExportCorrectedImages)

    /**
    * @brief Setter property for OutputPath
    */
    void setOutputPath(const QString& value); 
    /**
    * @brief Getter property for OutputPath
    * @return Value of OutputPath
    */
    QString getOutputPath() const;

  Q_PROPERTY(QString OutputPath READ getOutputPath WRITE setOutputPath)

    /**
    * @brief Setter property for FileExtension
    */
    void setFileExtension(const QString& value); 
    /**
    * @brief Getter property for FileExtension
    * @return Value of FileExtension
    */
    QString getFileExtension() const;

  Q_PROPERTY(QString FileExtension READ getFileExtension WRITE setFileExtension)

    /**
    * @brief Setter property for BackgroundDataContainerPath
    */
    void setBackgroundDataContainerPath(const DataArrayPath& value); 
    /**
    * @brief Getter property for BackgroundDataContainerPath
    * @return Value of BackgroundDataContainerPath
    */
    DataArrayPath getBackgroundDataContainerPath() const;

  Q_PROPERTY(DataArrayPath BackgroundDataContainerPath READ getBackgroundDataContainerPath WRITE setBackgroundDataContainerPath)

    /**
    * @brief Setter property for BackgroundCellAttributeMatrixPath
    */
    void setBackgroundCellAttributeMatrixPath(const DataArrayPath& value); 
    /**
    * @brief Getter property for BackgroundCellAttributeMatrixPath
    * @return Value of BackgroundCellAttributeMatrixPath
    */
    DataArrayPath getBackgroundCellAttributeMatrixPath() const;

  Q_PROPERTY(DataArrayPath BackgroundCellAttributeMatrixPath READ getBackgroundCellAttributeMatrixPath WRITE setBackgroundCellAttributeMatrixPath)

    /**
    * @brief Setter property for BackgroundImageArrayPath
    */
    void setBackgroundImageArrayPath(const DataArrayPath& value); 
    /**
    * @brief Getter property for BackgroundImageArrayPath
    * @return Value of BackgroundImageArrayPath
    */
    DataArrayPath getBackgroundImageArrayPath() const;

  Q_PROPERTY(DataArrayPath BackgroundImageArrayPath READ getBackgroundImageArrayPath WRITE setBackgroundImageArrayPath)

    /**
    * @brief Setter property for LowThreshold
    */
    void setLowThreshold(uint32_t value); 
    /**
    * @brief Getter property for LowThreshold
    * @return Value of LowThreshold
    */
    uint32_t getLowThreshold() const;

  Q_PROPERTY(uint32_t LowThreshold READ getLowThreshold WRITE setLowThreshold)

    /**
    * @brief Setter property for HighThreshold
    */
    void setHighThreshold(uint32_t value); 
    /**
    * @brief Getter property for HighThreshold
    * @return Value of HighThreshold
    */
    uint32_t getHighThreshold() const;

  Q_PROPERTY(uint32_t HighThreshold READ getHighThreshold WRITE setHighThreshold)

    /**
    * @brief Setter property for ApplyCorrection
    */
    void setApplyCorrection(bool value); 
    /**
    * @brief Getter property for ApplyCorrection
    * @return Value of ApplyCorrection
    */
    bool getApplyCorrection() const;

  Q_PROPERTY(int ApplyCorrection READ getApplyCorrection WRITE setApplyCorrection)

    /**
    * @brief Setter property for ApplyMedianFilter
    */
    void setApplyMedianFilter(bool value); 
    /**
    * @brief Getter property for ApplyMedianFilter
    * @return Value of ApplyMedianFilter
    */
    bool getApplyMedianFilter() const;

  Q_PROPERTY(bool ApplyMedianFilter READ getApplyMedianFilter WRITE setApplyMedianFilter)

    /**
    * @brief Setter property for MedianRadius
    */
    void setMedianRadius(const FloatVec3Type& value); 
    /**
    * @brief Getter property for MedianRadius
    * @return Value of MedianRadius
    */
    FloatVec3Type getMedianRadius() const;

  Q_PROPERTY(FloatVec3Type MedianRadius READ getMedianRadius WRITE setMedianRadius)

  /**
   * @brief notifyFeatureCompleted
   * @return
   */
  void notifyFeatureCompleted(const QString& dcName);

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



protected:
  enum class ArrayType
  {
    Error = -1,
    UInt8,
    UInt16,
    Float32
  };
  enum class GeomType
  {
    Error = -1,
    ImageGeom,
    RectGridGeom
  };

  IlluminationCorrection();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Returns an enum value specifying which supported array type the incoming data falls into.
   * @return
   */
  ArrayType getArrayType();
  
  /**
   * @brief Returns an enum value specifying which supported geometry type the incoming data falls into.
   * @return
   */
  GeomType getGeomType();

  /**
   * @brief Calls the corresponding checkInputArrays based on the array and geometry type.
   * @param arrayType
   * @param geomType
   * @return
   */
  IGeometryGrid::Pointer checkInputArrays(ArrayType arrayType, GeomType geomType);

  /**
   * @brief Calls the corresponding calculateOutputValues based on the array and geometry type.
   * @param arrayType
   * @param geomType
   * @return
   */
  void calculateOutputValues(ArrayType arrayType, GeomType geomType);

  /**
   * @brief Checks the input arrays' type and geometry type and returns a deep copy of the geometry.
   * @return
   */
  template<typename DataArrayType, typename GeometryType>
  std::shared_ptr<GeometryType> checkInputArrays()
  {
    DataContainerArray::Pointer dca = getDataContainerArray();
    std::vector<size_t> cDims = {1};
    typename GeometryType::Pointer outputGridGeom = GeometryType::NullPointer();

    // Ensure each DataContainer has the proper path to the image data and the image data is grayscale
    for(const auto& dcName : m_DataContainers)
    {
      DataArrayPath imageArrayPath(dcName, m_CellAttributeMatrixName, m_ImageDataArrayName);
      typename DataArrayType::Pointer imageData = dca->getPrereqArrayFromPath<DataArrayType, AbstractFilter>(this, imageArrayPath, cDims);
      if(imageData.get() == nullptr)
      {
        QString msg;
        QTextStream out(&msg);
        out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not of the appropriate type and components{1} (Grayscale) data. Please select a pattern of AttributeArray Paths that are gray scale images";
        setErrorCondition(-53000, msg);
      }

      if(getErrorCode() >= 0)
      {
        typename GeometryType::Pointer gridGeom = dca->getDataContainer(dcName)->getGeometryAs<GeometryType>();
        if(gridGeom.get() != nullptr)
        {
          if(outputGridGeom.get() == nullptr)
          {
            outputGridGeom = std::dynamic_pointer_cast<GeometryType>(gridGeom->deepCopy());
          }
        }
        else
        {
          QString msg;
          QTextStream out(&msg);
          out << "DataContainer: " << dcName << " needs to have an ImageGeometry assigned. There is either no geometry assign to the Data Container or the Geometry is not of type ImageGeom.";
          setErrorCondition(-53001, msg);
        }
      }
    }
    return outputGridGeom;
  }

private:
    QString m_DataContainerName = {};
    QStringList m_DataContainers = {};
    QString m_CellAttributeMatrixName = {};
    QString m_ImageDataArrayName = {};
    QString m_CorrectedImageDataArrayName = {};
    bool m_ExportCorrectedImages = {};
    QString m_OutputPath = {};
    QString m_FileExtension = {};
    DataArrayPath m_BackgroundDataContainerPath = {};
    DataArrayPath m_BackgroundCellAttributeMatrixPath = {};
    DataArrayPath m_BackgroundImageArrayPath = {};
    uint32_t m_LowThreshold = {};
    uint32_t m_HighThreshold = {};
    bool m_ApplyCorrection = {};
    bool m_ApplyMedianFilter = {};
    FloatVec3Type m_MedianRadius = {};

  QMutex m_NotifyMessage;

public:
  IlluminationCorrection(const IlluminationCorrection&) = delete;            // Copy Constructor Not Implemented
  IlluminationCorrection(IlluminationCorrection&&) = delete;                 // Move Constructor Not Implemented
  IlluminationCorrection& operator=(const IlluminationCorrection&) = delete; // Copy Assignment Not Implemented
  IlluminationCorrection& operator=(IlluminationCorrection&&) = delete;      // Move Assignment Not Implemented
};

