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
*    United States Air Force Prime Contract FA8650-10-D-5210
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include <memory>

#include <QtCore/QDateTime>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/ITK/itkImageReaderHelper.h"

#include <itkImageFileReader.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

// our PIMPL private class
class ITKImageReaderPrivate;

/**
 * @brief The ITKImageProcessingFilter class. See [Filter documentation](@ref itkimageprocessingfilter) for details.
 */
class ITKImageProcessing_EXPORT ITKImageReader : public AbstractFilter
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKImageReader SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ITKImageReader)
  PYB11_FILTER_NEW_MACRO(ITKImageReader)
  PYB11_FILTER_PARAMETER(QString, FileName)
  PYB11_FILTER_PARAMETER(DataArrayPath, DataContainerName)
  PYB11_FILTER_PARAMETER(QString, CellAttributeMatrixName)
  PYB11_FILTER_PARAMETER(QString, ImageDataArrayName)
  PYB11_PROPERTY(QString FileName READ getFileName WRITE setFileName)
  PYB11_PROPERTY(DataArrayPath DataContainerName READ getDataContainerName WRITE setDataContainerName)
  PYB11_PROPERTY(QString CellAttributeMatrixName READ getCellAttributeMatrixName WRITE setCellAttributeMatrixName)
  PYB11_PROPERTY(QString ImageDataArrayName READ getImageDataArrayName WRITE setImageDataArrayName)
#endif

  Q_DECLARE_PRIVATE(ITKImageReader)

public:
    using Self = ITKImageReader;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKImageReader> New();

    /**
    * @brief Returns the name of the class for ITKImageReader
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKImageReader
    */
    static QString ClassName();


  ~ITKImageReader() override;

    /**
    * @brief Setter property for FileName
    */
    void setFileName(const QString& value); 
    /**
    * @brief Getter property for FileName
    * @return Value of FileName
    */
    QString getFileName() const;

  Q_PROPERTY(QString FileName READ getFileName WRITE setFileName)

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
    * @brief Setter property for FileNameCache
    */
  void setFileNameCache(const QString& value);
  /**
   * @brief Getter property for FileNameCache
   * @return Value of FileNameCache
   */
  QString getFileNameCache() const;

  /**
   * @brief Setter property for LastRead
   */
  void setLastRead(const QDateTime& value);
  /**
   * @brief Getter property for LastRead
   * @return Value of LastRead
   */
  QDateTime getLastRead() const;

  /**
   * @brief Setter property for ImageArrayCache
   */
  void setImageArrayCache(const IDataArray::Pointer& value);
  /**
   * @brief Getter property for ImageArrayCache
   * @return Value of ImageArrayCache
   */
  IDataArray::Pointer getImageArrayCache() const;

  /**
   * @brief Setter property for DCGeometryCache
   */
  void setDCGeometryCache(const ImageGeom::Pointer& value);
  /**
   * @brief Getter property for DCGeometryCache
   * @return Value of DCGeometryCache
   */
  ImageGeom::Pointer getDCGeometryCache() const;


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
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

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
  ITKImageReader();
  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Include the declarations of the ITKImageReader helper functions that are common
   * to a few different filters across different plugins.
   */
  ITK_IMAGE_READER_HELPER_DECL()

private:
    QString m_FileName = {};
    DataArrayPath m_DataContainerName = {};
    QString m_CellAttributeMatrixName = {};
    QString m_ImageDataArrayName = {};

  QScopedPointer<ITKImageReaderPrivate> const d_ptr;

public:
  ITKImageReader(const ITKImageReader&) = delete; // Copy Constructor Not Implemented
  ITKImageReader(ITKImageReader&&) = delete;      // Move Constructor Not Implemented
  ITKImageReader& operator=(const ITKImageReader&) = delete; // Copy Assignment Not Implemented
  ITKImageReader& operator=(ITKImageReader&&) = delete;      // Move Assignment Not Implemented
};

