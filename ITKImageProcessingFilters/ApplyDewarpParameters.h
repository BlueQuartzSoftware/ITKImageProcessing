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

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Utilities/MontageSelection.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

#include <itkConfigure.h>
#define SIMPL_ITK_VERSION_CHECK (ITK_VERSION_MAJOR == 4)
#if SIMPL_ITK_VERSION_CHECK
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-field"
#endif
#endif
#include "itkSingleValuedCostFunction.h"

/**
 * @brief The ApplyDewarpParameters class. See [Filter documentation](@ref ApplyDewarpParameters) for details.
 */
class ITKImageProcessing_EXPORT ApplyDewarpParameters : public AbstractFilter
{
  Q_OBJECT

  // clang-format off
  PYB11_BEGIN_BINDINGS(ApplyDewarpParameters SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ApplyDewarpParameters)
  PYB11_FILTER_NEW_MACRO(ApplyDewarpParameters)
  PYB11_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)
  PYB11_PROPERTY(bool UseDataArray READ getUseDataArray WRITE setUseDataArray)
  PYB11_PROPERTY(QString AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)
  PYB11_PROPERTY(QString MaskName READ getMaskName WRITE setMaskName)
  PYB11_PROPERTY(DataArrayPath TransformPath READ getTransformPath WRITE setTransformPath)
  PYB11_PROPERTY(QString TransformPrefix READ getTransformPrefix WRITE setTransformPrefix)
  PYB11_END_BINDINGS()
  // clang-format on

public:
  using Self = ApplyDewarpParameters;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<Self>;
  static Pointer NullPointer();

  static Pointer New();

  /**
   * @brief Returns the name of the class for DataContainerReader
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for DataContainerReader
   */
  static QString ClassName();

  ApplyDewarpParameters(const ApplyDewarpParameters&) = delete;            // Copy Constructor Not Implemented
  ApplyDewarpParameters& operator=(const ApplyDewarpParameters&) = delete; // Copy Assignment Not Implemented
  ApplyDewarpParameters(ApplyDewarpParameters&&) = delete;                 // Move Constructor Not Implemented
  ApplyDewarpParameters& operator=(ApplyDewarpParameters&&) = delete;      // Move Assignment Not Implemented
  ~ApplyDewarpParameters() override;

  QString getMontageName() const;
  void setMontageName(const QString& value);
  Q_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)

  bool getUseDataArray() const;
  void setUseDataArray(bool value);
  Q_PROPERTY(bool UseDataArray READ getUseDataArray WRITE setUseDataArray)

  // FloatVec7Type getXFactors() const;
  // void setXFactors(const FloatVec7Type& value);
  // Q_PROPERTY(FloatVec7Type XFactors READ getXFactors WRITE setXFactors)

  // FloatVec7Type getYFactors() const;
  // void setYFactors(const FloatVec7Type& value);
  // Q_PROPERTY(FloatVec7Type YFactors READ getYFactors WRITE setYFactors)

  QString getAttributeMatrixName() const;
  void setAttributeMatrixName(const QString& value);
  Q_PROPERTY(QString AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)

  QString getMaskName() const;
  void setMaskName(const QString& value);
  Q_PROPERTY(QString MaskName READ getMaskName WRITE setMaskName)

  DataArrayPath getTransformPath() const;
  void setTransformPath(const DataArrayPath& value);
  Q_PROPERTY(DataArrayPath TransformPath READ getTransformPath WRITE setTransformPath)

  QString getTransformPrefix() const;
  void setTransformPrefix(const QString& value);
  Q_PROPERTY(QString TransformPrefix READ getTransformPrefix WRITE setTransformPrefix)

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
  ApplyDewarpParameters();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Returns a vector the combined Px and Py values
   * @return
   */
  std::vector<double> getPxyVec();

  /**
   * @brief Checks the montage requirements and emits the required error conditions.
   * Returns true if all requirements pass. Returns false otherwise.
   * @return
   */
  bool checkMontageRequirements();

  /**
   * @brief Creates new DataContainers and warps the data by the FFT Convolution kernel generated.
   * @param parameters
   * @param imageDimX
   * @param imageDimY
   */
  void warpDataContainers(const itk::SingleValuedCostFunction::ParametersType& parameters, double imageDimX, double imageDimY);

  /**
   * @brief Returns the target image dimensions for the montage
   * @return
   */
  std::pair<double, double> getImageDims() const;

  /**
   * @brief Returns the required length for either X or Y parameters.
   * Combined parameter length should be 2x this value.
   * @return
   */
  size_t getSingleParamCount() const;

private:
  QString m_MontageName;
  QString m_AttributeMatrixName;
  QString m_TransformPrefix = "Transformed_";
  QString m_MaskName = "Mask";
  bool m_UseDataArray = true;
  DataArrayPath m_TransformPath;

  // FloatVec7Type m_XFactors = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  // FloatVec7Type m_YFactors = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
};

#if SIMPL_ITK_VERSION_CHECK
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif
#undef SIMPL_ITK_VERSION_CHECK
