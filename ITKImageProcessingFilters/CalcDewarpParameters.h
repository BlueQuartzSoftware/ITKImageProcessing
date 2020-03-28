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

#include "itkSingleValuedCostFunction.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/Utilities/MontageSelection.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/util/FFTAmoebaOptimizer.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The CalcDewarpParameters class. See [Filter documentation](@ref CalcDewarpParameters) for details.
 */
class ITKImageProcessing_EXPORT CalcDewarpParameters : public AbstractFilter
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(CalcDewarpParameters SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(CalcDewarpParameters)
  PYB11_FILTER_NEW_MACRO(CalcDewarpParameters)

  PYB11_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)
  PYB11_PROPERTY(uint32_t MaxIterations READ getMaxIterations WRITE setMaxIterations)
  PYB11_PROPERTY(double FractionalTolerance READ getFractionalTolerance WRITE setFractionalTolerance)
  PYB11_PROPERTY(int Delta READ getDelta WRITE setDelta)
  PYB11_PROPERTY(bool SpecifyInitialSimplex READ getSpecifyInitialSimplex WRITE setSpecifyInitialSimplex)
  PYB11_PROPERTY(FloatVec7Type XFactors READ getXFactors WRITE setXFactors)
  PYB11_PROPERTY(FloatVec7Type YFactors READ getYFactors WRITE setYFactors)
  PYB11_PROPERTY(QString AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)
  PYB11_PROPERTY(QString IPFColorsArrayName READ getIPFColorsArrayName WRITE setIPFColorsArrayName)
  PYB11_PROPERTY(QString TransformDCName READ getTransformDCName WRITE setTransformDCName)
  PYB11_PROPERTY(QString TransformMatrixName READ getTransformMatrixName WRITE setTransformMatrixName)
  PYB11_PROPERTY(QString TransformArrayName READ getTransformArrayName WRITE setTransformArrayName)

#endif

  using AmoebaOptimizer = itk::FFTAmoebaOptimizer;

public:
  using Self = CalcDewarpParameters;
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

  CalcDewarpParameters(const CalcDewarpParameters&) = delete;            // Copy Constructor Not Implemented
  CalcDewarpParameters& operator=(const CalcDewarpParameters&) = delete; // Copy Assignment Not Implemented
  CalcDewarpParameters(CalcDewarpParameters&&) = delete;                 // Move Constructor Not Implemented
  CalcDewarpParameters& operator=(CalcDewarpParameters&&) = delete;      // Move Assignment Not Implemented
  ~CalcDewarpParameters() override;

  QString getMontageName() const;
  void setMontageName(const QString& value);
  Q_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)

  uint32_t getMaxIterations() const;
  void setMaxIterations(uint32_t value);
  Q_PROPERTY(uint32_t MaxIterations READ getMaxIterations WRITE setMaxIterations)

  double getFractionalTolerance() const;
  void setFractionalTolerance(double value);
  Q_PROPERTY(double FractionalTolerance READ getFractionalTolerance WRITE setFractionalTolerance)

  int getDelta() const;
  void setDelta(int value);
  Q_PROPERTY(int Delta READ getDelta WRITE setDelta)

  bool getSpecifyInitialSimplex() const;
  void setSpecifyInitialSimplex(bool value);
  Q_PROPERTY(bool SpecifyInitialSimplex READ getSpecifyInitialSimplex WRITE setSpecifyInitialSimplex)

  FloatVec7Type getXFactors() const;
  void setXFactors(const FloatVec7Type& value);
  Q_PROPERTY(FloatVec7Type XFactors READ getXFactors WRITE setXFactors)

  FloatVec7Type getYFactors() const;
  void setYFactors(const FloatVec7Type& value);
  Q_PROPERTY(FloatVec7Type YFactors READ getYFactors WRITE setYFactors)

  QString getAttributeMatrixName() const;
  void setAttributeMatrixName(const QString& value);
  Q_PROPERTY(QString AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)

  QString getIPFColorsArrayName() const;
  void setIPFColorsArrayName(const QString& value);
  Q_PROPERTY(QString IPFColorsArrayName READ getIPFColorsArrayName WRITE setIPFColorsArrayName)

  QString getTransformDCName() const;
  void setTransformDCName(const QString& value);
  Q_PROPERTY(QString TransformDCName READ getTransformDCName WRITE setTransformDCName)

  QString getTransformMatrixName() const;
  void setTransformMatrixName(const QString& value);
  Q_PROPERTY(QString TransformMatrixName READ getTransformMatrixName WRITE setTransformMatrixName)

  QString getTransformArrayName() const;
  void setTransformArrayName(const QString& value);
  Q_PROPERTY(QString TransformArrayName READ getTransformArrayName WRITE setTransformArrayName)

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

public slots:
  /**
   * @brief Cancel the operation
   */
  void setCancel(bool value) override;

protected:
  CalcDewarpParameters();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Returns a vector of the default Px values
   * @return
   */
  std::vector<double> getDefaultPx() const;

  /**
   * @brief Returns a vector of the default Py values
   * @return
   */
  std::vector<double> getDefaultPy() const;

  /**
   * @brief Returns a vector the combined Px and Py values
   * @return
   */
  std::vector<double> getPxyVec() const;

  /**
   * @brief Checks the montage requirements and emits the required error conditions.
   * Returns true if all requirements pass. Returns false otherwise.
   * @return
   */
  bool checkMontageRequirements();

  /**
   * @brief Returns the name used for the internal grayscale array.
   * @return
   */
  QString getGrayscaleArrayName() const;

  /**
   * @brief Generates internal grayscale array.
   */
  void generateGrayscaleIPF();

  /**
   * @brief Removes internal grayscale array.
   */
  void deleteGrayscaleIPF();

  /**
   * @brief Returns the required length for either X or Y parameters.
   * Combined parameter length should be 2x this value.
   * @return
   */
  size_t getSingleParamCount() const;

  /**
   * @brief Returns the default parameter step size.
   * @param params
   * @param imgDimX
   * @param imgDimY
   * @return
   */
  std::vector<double> getStepSizes(const std::vector<double>& params, size_t imgDimX, size_t imgDimY) const;

private:
  AmoebaOptimizer::Pointer m_Optimizer = nullptr;
  QString m_MontageName;
  uint m_MaxIterations = 1000;
  double m_FractionalTolerance = 1E-5;
  int m_StepDelta = 5;
  bool m_SpecifyInitialSimplex = true;
  QString m_AttributeMatrixName;
  QString m_IPFColorsArrayName = "IPFColor";
  QString m_TransformDCName = "Dewarp Data";
  QString m_TransformMatrixName = "Transform Matrix";
  QString m_TransformArrayName = "Transform";
  QString m_TransformPrefix = "Transformed_";

  FloatVec7Type m_XFactors = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  FloatVec7Type m_YFactors = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
};
