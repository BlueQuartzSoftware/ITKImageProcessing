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

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "SIMPLib/ITK/itkDream3DImage.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKImageProcessingFilter class. See [Filter documentation](@ref itkimageprocessingfilter) for details.
 */
class ITKImageProcessing_EXPORT ITKImageWriter : public AbstractFilter
{

  Q_OBJECT

  PYB11_CREATE_BINDINGS(ITKImageWriter SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(QString FileName READ getFileName WRITE setFileName)
  PYB11_PROPERTY(DataArrayPath ImageArrayPath READ getImageArrayPath WRITE setImageArrayPath)
  PYB11_PROPERTY(int Plane READ getPlane WRITE setPlane)
  PYB11_METHOD(void registerImageIOFactories)

public:
  SIMPL_SHARED_POINTERS(ITKImageWriter)
  SIMPL_FILTER_NEW_MACRO(ITKImageWriter)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKImageWriter, AbstractFilter)

  ~ITKImageWriter() override;

  static const int XYPlane = 0;
  static const int XZPlane = 1;
  static const int YZPlane = 2;
  
  SIMPL_FILTER_PARAMETER(QString, FileName)
  Q_PROPERTY(QString FileName READ getFileName WRITE setFileName)

  SIMPL_FILTER_PARAMETER(DataArrayPath, ImageArrayPath)
  Q_PROPERTY(DataArrayPath ImageArrayPath READ getImageArrayPath WRITE setImageArrayPath)

  SIMPL_FILTER_PARAMETER(int, Plane)
  Q_PROPERTY(int Plane READ getPlane WRITE setPlane)
  
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

  /**
  * @brief preflight Reimplemented from @see AbstractFilter class
  */
  void preflight() override;

  /* For Python use,
      register the ImageIOFactories
  */
  void registerImageIOFactories() const;

Q_SIGNALS:
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
  ITKImageWriter();
  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
  * @brief Converts Dream3D data to an ITK image and calls the actual writer.
  */
  template <typename TPixel, typename UnusedTPixel, unsigned int Dimension> void writeImage();

  /**
  * @brief writeAs2DStack Writes 3D images as a stack of 2D images.
  */
  template <typename TPixel, unsigned int Dimensions> 
  void writeAs2DStack(typename itk::Dream3DImage<TPixel, Dimensions>* image, unsigned long z_size);

  /**
  * @brief writeAsOneFile Writes images as one file.
  */
  template <typename TPixel, unsigned int Dimensions> 
  void writeAsOneFile(typename itk::Dream3DImage<TPixel, Dimensions>* image);
  
  private:
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
  void copyTuple(size_t index, size_t axisA, size_t dB, size_t axisB, size_t nComp,
                               IDataArray* currentData, IDataArray* sliceData);
  /**
   * @brief saveImageData
   * @param dca
   * @param slice
   * @param maxSlice
   */
  void saveImageData(DataContainerArray::Pointer dca, size_t slice, size_t maxSlice);                               

public:
  ITKImageWriter(const ITKImageWriter&) = delete; // Copy Constructor Not Implemented
  ITKImageWriter(ITKImageWriter&&) = delete;      // Move Constructor Not Implemented
  ITKImageWriter& operator=(const ITKImageWriter&) = delete; // Copy Assignment Not Implemented
  ITKImageWriter& operator=(ITKImageWriter&&) = delete;      // Move Assignment Not Implemented
};

