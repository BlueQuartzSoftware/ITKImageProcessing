/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include <memory>

#include <itkVectorRescaleIntensityImageFilter.h>

#include "SIMPLib/SIMPLib.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageProcessingBase.h"
#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKVectorRescaleIntensityImage class. See [Filter documentation](@ref ITKVectorRescaleIntensityImage) for details.
 */
class ITKImageProcessing_EXPORT ITKVectorRescaleIntensityImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKVectorRescaleIntensityImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKVectorRescaleIntensityImage)
  PYB11_FILTER_NEW_MACRO(ITKVectorRescaleIntensityImage)
  PYB11_PROPERTY(double OutputMaximumMagnitude READ getOutputMaximumMagnitude WRITE setOutputMaximumMagnitude)
  PYB11_PROPERTY(int OutputType READ getOutputType WRITE setOutputType)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKVectorRescaleIntensityImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKVectorRescaleIntensityImage> New();

    /**
    * @brief Returns the name of the class for ITKVectorRescaleIntensityImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKVectorRescaleIntensityImage
    */
    static QString ClassName();


  ~ITKVectorRescaleIntensityImage() override;

    /**
    * @brief Setter property for OutputMaximumMagnitude
    */
    void setOutputMaximumMagnitude(double value); 
    /**
    * @brief Getter property for OutputMaximumMagnitude
    * @return Value of OutputMaximumMagnitude
    */
    double getOutputMaximumMagnitude() const;
  Q_PROPERTY(double OutputMaximumMagnitude READ getOutputMaximumMagnitude WRITE setOutputMaximumMagnitude)

    /**
    * @brief Setter property for OutputType
    */
    void setOutputType(int value); 
    /**
    * @brief Getter property for OutputType
    * @return Value of OutputType
    */
    int getOutputType() const;
  Q_PROPERTY(int OutputType READ getOutputType WRITE setOutputType)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

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
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKVectorRescaleIntensityImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheck() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheckImpl();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

  /**
  * @brief Checks 'value' can be casted to OutputPixelType.
  */
  template <typename OutputPixelType> void CheckEntryBounds(double value, QString name);

public:
  ITKVectorRescaleIntensityImage(const ITKVectorRescaleIntensityImage&) = delete; // Copy Constructor Not Implemented
  ITKVectorRescaleIntensityImage(ITKVectorRescaleIntensityImage&&) = delete;      // Move Constructor Not Implemented
  ITKVectorRescaleIntensityImage& operator=(const ITKVectorRescaleIntensityImage&) = delete; // Copy Assignment Not Implemented
  ITKVectorRescaleIntensityImage& operator=(ITKVectorRescaleIntensityImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_OutputMaximumMagnitude = StaticCastScalar<double, double, double>(255);
    int m_OutputType = {0};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

