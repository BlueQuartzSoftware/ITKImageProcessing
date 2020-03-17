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

#include <QtCore/QString>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/ITK/itkImageReaderHelper.h"
#include "SIMPLib/Common/SIMPLArray.hpp"

#include "ITKImageProcessing/FilterParameters/ImportVectorImageStackFilterParameter.h"
#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @class ImportVectorImageStack ImportVectorImageStack.h ImageIO/Code/ImageIOFilters/ImportVectorImageStack.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ITKImageProcessing_EXPORT ImportVectorImageStack : public AbstractFilter
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ImportVectorImageStack SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ImportVectorImageStack)
  PYB11_FILTER_NEW_MACRO(ImportVectorImageStack)
  PYB11_FILTER_PARAMETER(DataArrayPath, DataContainerName)
  PYB11_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Origin)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Spacing)
  PYB11_FILTER_PARAMETER(VectorFileListInfo_t, InputFileListInfo)
  PYB11_FILTER_PARAMETER(QString, VectorDataArrayName)
  PYB11_FILTER_PARAMETER(bool, ConvertToGrayscale)
  PYB11_PROPERTY(DataArrayPath DataContainerName READ getDataContainerName WRITE setDataContainerName)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(FloatVec3Type Origin READ getOrigin WRITE setOrigin)
  PYB11_PROPERTY(FloatVec3Type Spacing READ getSpacing WRITE setSpacing)
  PYB11_PROPERTY(VectorFileListInfo_t InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)
  PYB11_PROPERTY(QString VectorDataArrayName READ getVectorDataArrayName WRITE setVectorDataArrayName)
  PYB11_PROPERTY(bool ConvertToGrayscale READ getConvertToGrayscale WRITE setConvertToGrayscale)
#endif

public:
    using Self = ImportVectorImageStack;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ImportVectorImageStack> New();

    /**
    * @brief Returns the name of the class for ImportVectorImageStack
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ImportVectorImageStack
    */
    static QString ClassName();


  ~ImportVectorImageStack() override;

    /**
    * @brief Setter property for DataContainerName
    */
    void setDataContainerName(const DataArrayPath& value); 
    /**
    * @brief Getter property for DataContainerName
    * @return Value of DataContainerName
    */
    DataArrayPath getDataContainerName() const;

  Q_PROPERTY(DataArrayPath DataContainerName READ getDataContainerName WRITE setDataContainerName)

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
    * @brief Setter property for InputFileListInfo
    */
    void setInputFileListInfo(const VectorFileListInfo_t& value); 
    /**
    * @brief Getter property for InputFileListInfo
    * @return Value of InputFileListInfo
    */
    VectorFileListInfo_t getInputFileListInfo() const;

  Q_PROPERTY(VectorFileListInfo_t InputFileListInfo READ getInputFileListInfo WRITE setInputFileListInfo)

    /**
    * @brief Setter property for VectorDataArrayName
    */
    void setVectorDataArrayName(const QString& value); 
    /**
    * @brief Getter property for VectorDataArrayName
    * @return Value of VectorDataArrayName
    */
    QString getVectorDataArrayName() const;

  Q_PROPERTY(QString VectorDataArrayName READ getVectorDataArrayName WRITE setVectorDataArrayName)

    /**
    * @brief Setter property for ConvertToGrayscale
    */
    void setConvertToGrayscale(bool value); 
    /**
    * @brief Getter property for ConvertToGrayscale
    * @return Value of ConvertToGrayscale
    */
    bool getConvertToGrayscale() const;

  Q_PROPERTY(bool ConvertToGrayscale READ getConvertToGrayscale WRITE setConvertToGrayscale)

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



protected:
  ImportVectorImageStack();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief generateFileList
   */
  void generateFileList();

public:
  ImportVectorImageStack(const ImportVectorImageStack&) = delete;            // Copy Constructor Not Implemented
  ImportVectorImageStack(ImportVectorImageStack&&) = delete;                 // Move Constructor Not Implemented
  ImportVectorImageStack& operator=(const ImportVectorImageStack&) = delete; // Copy Assignment Not Implemented
  ImportVectorImageStack& operator=(ImportVectorImageStack&&) = delete;      // Move Assignment Not Implemented

  private:
    DataArrayPath m_DataContainerName = {};
    QString m_CellAttributeMatrixName = {};
    FloatVec3Type m_Origin = {};
    FloatVec3Type m_Spacing = {};
    VectorFileListInfo_t m_InputFileListInfo = {};
    QString m_VectorDataArrayName = {};
    bool m_ConvertToGrayscale = {};

};

