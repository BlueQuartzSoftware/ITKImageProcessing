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

#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Geometry/IGeometryGrid.h"
#include "SIMPLib/SIMPLib.h"

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
  PYB11_CREATE_BINDINGS(IlluminationCorrection SUPERCLASS AbstractFilter)
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

public:
  SIMPL_SHARED_POINTERS(IlluminationCorrection)
  SIMPL_FILTER_NEW_MACRO(IlluminationCorrection)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(IlluminationCorrection, AbstractFilter)

  ~IlluminationCorrection() override;

  SIMPL_INSTANCE_STRING_PROPERTY(DataContainerName)

  SIMPL_FILTER_PARAMETER(QStringList, DataContainers)
  Q_PROPERTY(QStringList DataContainers READ getDataContainers WRITE setDataContainers)

  SIMPL_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  Q_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, ImageDataArrayName)
  Q_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)

  SIMPL_FILTER_PARAMETER(QString, CorrectedImageDataArrayName)
  Q_PROPERTY(QString CorrectedImageDataArrayName READ getCorrectedImageDataArrayName WRITE setCorrectedImageDataArrayName)

  SIMPL_FILTER_PARAMETER(bool, ExportCorrectedImages)
  Q_PROPERTY(bool ExportCorrectedImages READ getExportCorrectedImages WRITE setExportCorrectedImages)

  SIMPL_FILTER_PARAMETER(QString, OutputPath)
  Q_PROPERTY(QString OutputPath READ getOutputPath WRITE setOutputPath)

  SIMPL_FILTER_PARAMETER(QString, FileExtension)
  Q_PROPERTY(QString FileExtension READ getFileExtension WRITE setFileExtension)

  SIMPL_FILTER_PARAMETER(DataArrayPath, BackgroundDataContainerPath)
  Q_PROPERTY(DataArrayPath BackgroundDataContainerPath READ getBackgroundDataContainerPath WRITE setBackgroundDataContainerPath)

  SIMPL_FILTER_PARAMETER(DataArrayPath, BackgroundCellAttributeMatrixPath)
  Q_PROPERTY(DataArrayPath BackgroundCellAttributeMatrixPath READ getBackgroundCellAttributeMatrixPath WRITE setBackgroundCellAttributeMatrixPath)

  SIMPL_FILTER_PARAMETER(DataArrayPath, BackgroundImageArrayPath)
  Q_PROPERTY(DataArrayPath BackgroundImageArrayPath READ getBackgroundImageArrayPath WRITE setBackgroundImageArrayPath)

  SIMPL_FILTER_PARAMETER(uint32_t, LowThreshold)
  Q_PROPERTY(uint32_t LowThreshold READ getLowThreshold WRITE setLowThreshold)

  SIMPL_FILTER_PARAMETER(uint32_t, HighThreshold)
  Q_PROPERTY(uint32_t HighThreshold READ getHighThreshold WRITE setHighThreshold)

  SIMPL_FILTER_PARAMETER(bool, ApplyCorrection)
  Q_PROPERTY(int ApplyCorrection READ getApplyCorrection WRITE setApplyCorrection)

  SIMPL_FILTER_PARAMETER(bool, ApplyMedianFilter)
  Q_PROPERTY(bool ApplyMedianFilter READ getApplyMedianFilter WRITE setApplyMedianFilter)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, MedianRadius)
  Q_PROPERTY(FloatVec3Type MedianRadius READ getMedianRadius WRITE setMedianRadius)

  /**
   * @brief notifyFeatureCompleted
   * @return
   */
  void notifyFeatureCompleted(const QString& dcName);

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
   * @brief updateFilterParameters This is emitted when the filter requests all the latest Filter Parameters need to be
   * pushed from a user facing control such as the FilterParameter Widget
   * @param filter The filter to push the values into
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged This signal can be emitted when any of the filter parameters are changed internally.
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before the dataCheck() is called. This can change if needed.
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after the dataCheck() is called. Typically. This can change if needed.
   */
  void preflightExecuted();

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
    QVector<size_t> cDims = { 1 };
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
  QMutex m_NotifyMessage;

public:
  IlluminationCorrection(const IlluminationCorrection&) = delete;            // Copy Constructor Not Implemented
  IlluminationCorrection(IlluminationCorrection&&) = delete;                 // Move Constructor Not Implemented
  IlluminationCorrection& operator=(const IlluminationCorrection&) = delete; // Copy Assignment Not Implemented
  IlluminationCorrection& operator=(IlluminationCorrection&&) = delete;      // Move Assignment Not Implemented
};

