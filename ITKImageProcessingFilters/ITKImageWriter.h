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

#include "itkConfigure.h"
#if defined(ITK_VERSION_MAJOR) && ITK_VERSION_MAJOR == 4
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-field"
#endif
#endif

#include <itkImage.h>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKImageProcessingFilter class. See [Filter documentation](@ref itkimageprocessingfilter) for details.
 */
class ITKImageProcessing_EXPORT ITKImageWriter : public AbstractFilter
{

  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKImageWriter SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKImageWriter)
  PYB11_FILTER_NEW_MACRO(ITKImageWriter)
  PYB11_PROPERTY(QString FileName READ getFileName WRITE setFileName)
  PYB11_PROPERTY(DataArrayPath ImageArrayPath READ getImageArrayPath WRITE setImageArrayPath)
  PYB11_PROPERTY(int Plane READ getPlane WRITE setPlane)
  PYB11_PROPERTY(int IndexOffset READ getIndexOffset WRITE setIndexOffset)
  PYB11_METHOD(void registerImageIOFactories)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKImageWriter;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKImageWriter> New();

  /**
   * @brief Returns the name of the class for ITKImageWriter
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKImageWriter
   */
  static QString ClassName();

  ~ITKImageWriter() override;

  static const int XYPlane = 0;
  static const int XZPlane = 1;
  static const int YZPlane = 2;

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
   * @brief Setter property for ImageArrayPath
   */
  void setImageArrayPath(const DataArrayPath& value);
  /**
   * @brief Getter property for ImageArrayPath
   * @return Value of ImageArrayPath
   */
  DataArrayPath getImageArrayPath() const;
  Q_PROPERTY(DataArrayPath ImageArrayPath READ getImageArrayPath WRITE setImageArrayPath)

  /**
   * @brief Setter property for Plane
   */
  void setPlane(int value);
  /**
   * @brief Getter property for Plane
   * @return Value of Plane
   */
  int getPlane() const;
  Q_PROPERTY(int Plane READ getPlane WRITE setPlane)

  /**
   * @brief Setter property for IndexOffset
   */
  void setIndexOffset(int value);
  /**
   * @brief Getter property for IndexOffset
   * @return Value of IndexOffset
   */
  int getIndexOffset() const;
  Q_PROPERTY(int IndexOffset READ getIndexOffset WRITE setIndexOffset)

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
   * @brief is2DFormat returns true if file name extension corresponds to a 2D image format
   */
  bool is2DFormat();

  /* For Python use,
      register the ImageIOFactories
  */
  void registerImageIOFactories() const;

protected:
  ITKImageWriter();
  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Converts Dream3D data to an ITK image and calls the actual writer.
   */
  template <typename TPixel, typename UnusedTPixel, unsigned int Dimension>
  void writeImage();

  /**
   * @brief writeAs2DStack Writes 3D images as a stack of 2D images.
   */
  template <typename TPixel, unsigned int Dimensions>
  void writeAs2DStack(typename itk::Image<TPixel, Dimensions>* image, unsigned long z_size);

  /**
   * @brief writeAsOneFile Writes images as one file.
   */
  template <typename TPixel, unsigned int Dimensions>
  void writeAsOneFile(typename itk::Image<TPixel, Dimensions>* image);

private:
  QString m_FileName = {""};
  DataArrayPath m_ImageArrayPath = {"", "", ""};
  int m_Plane = {};
  int m_IndexOffset = {0};

  /**
   * @brief copyTuple
   * @param index
   * @param axisA
   * @param dB
   * @param axisB
   * @param nComp
   * @param currentData
   * @param sliceData
   */
  void copyTuple(size_t index, size_t axisA, size_t dB, size_t axisB, size_t nComp, IDataArray* currentData, IDataArray* sliceData);
  /**
   * @brief saveImageData
   * @param dca
   * @param slice
   * @param maxSlice
   */
  void saveImageData(DataContainerArray::Pointer dca, size_t slice, size_t maxSlice);

public:
  ITKImageWriter(const ITKImageWriter&) = delete;            // Copy Constructor Not Implemented
  ITKImageWriter(ITKImageWriter&&) = delete;                 // Move Constructor Not Implemented
  ITKImageWriter& operator=(const ITKImageWriter&) = delete; // Copy Assignment Not Implemented
  ITKImageWriter& operator=(ITKImageWriter&&) = delete;      // Move Assignment Not Implemented
};
