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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
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

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Utilities/MontageSelection.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The Blend class. See [Filter documentation](@ref blend) for details.
 */
class ITKImageProcessing_EXPORT Blend : public AbstractFilter
{
  Q_OBJECT

public:
  using Self = Blend;
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

  Blend(const Blend&) = delete;            // Copy Constructor Not Implemented
  Blend& operator=(const Blend&) = delete; // Copy Assignment Not Implemented
  Blend(Blend&&) = delete;                 // Move Constructor Not Implemented
  Blend& operator=(Blend&&) = delete;      // Move Assignment Not Implemented
  ~Blend() override;

  QString getMontageName() const;
  void setMontageName(const QString& value);
  Q_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)

  uint getMaxIterations() const;
  void setMaxIterations(uint value);
  Q_PROPERTY(uint MaxIterations READ getMaxIterations WRITE setMaxIterations)

  IntVec2Type getOverlapAmt() const;
  void setOverlapAmt(const IntVec2Type& value);
  Q_PROPERTY(IntVec2Type OverlapAmt READ getOverlapAmt WRITE setOverlapAmt)

  double getLowTolerance() const;
  void setLowTolerance(double value);
  Q_PROPERTY(double LowTolerance READ getLowTolerance WRITE setLowTolerance)

  double getHighTolerance() const;
  void setHighTolerance(double value);
  Q_PROPERTY(double HighTolerance READ getHighTolerance WRITE setHighTolerance)

  int getDegree() const;
  void setDegree(int value);
  Q_PROPERTY(int Degree READ getDegree WRITE setDegree)

  bool getUseAmoebaOptimizer() const;
  void setUseAmoebaOptimizer(bool value);
  Q_PROPERTY(bool UseAmoebaOptimizer READ getUseAmoebaOptimizer WRITE setUseAmoebaOptimizer)

  bool getSpecifyInitialSimplex() const;
  void setSpecifyInitialSimplex(bool value);
  Q_PROPERTY(bool SpecifyInitialSimplex READ getSpecifyInitialSimplex WRITE setSpecifyInitialSimplex)

  QString getPxStr() const;
  void setPxStr(const QString& value);
  Q_PROPERTY(QString PxStr READ getPxStr WRITE setPxStr)

  QString getPyStr() const;
  void setPyStr(const QString& value);
  Q_PROPERTY(QString PyStr READ getPyStr WRITE setPyStr)

  QString getAttributeMatrixName() const;
  void setAttributeMatrixName(const QString& value);
  Q_PROPERTY(QString AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)

  QString getIPFColorsArrayName() const;
  void setIPFColorsArrayName(const QString& value);
  Q_PROPERTY(QString IPFColorsArrayName READ getIPFColorsArrayName WRITE setIPFColorsArrayName)

  bool getCreateTransformContainer() const;
  void setCreateTransformContainer(bool value);
  Q_PROPERTY(bool CreateTransformContainer READ getCreateTransformContainer WRITE setCreateTransformContainer)

  QString getBlendDCName() const;
  void setBlendDCName(const QString& value);
  Q_PROPERTY(QString BlendDCName READ getBlendDCName WRITE setBlendDCName)

  QString getTransformMatrixName() const;
  void setTransformMatrixName(const QString& value);
  Q_PROPERTY(QString TransformMatrixName READ getTransformMatrixName WRITE setTransformMatrixName)

  QString getTransformArrayName() const;
  void setTransformArrayName(const QString& value);
  Q_PROPERTY(QString TransformArrayName READ getTransformArrayName WRITE setTransformArrayName)

  QString getResidualArrayName() const;
  void setResidualArrayName(const QString& value);
  Q_PROPERTY(QString ResidualArrayName READ getResidualArrayName WRITE setResidualArrayName)

  QString getTransformPrefix() const;
  void setTransformPrefix(const QString& value);
  Q_PROPERTY(QString TransformPrefix READ getTransformPrefix WRITE setTransformPrefix)

  bool GetConvergenceFromStopDescription(const QString&) const;

  uint GetIterationsFromStopDescription(const QString&) const;

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
  Blend();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

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
   * @brief Creates new DataContainers and warps the data by the FFT Convolution kernel generated.
   * @param transformVector
   */
  void warpDataContainers(const std::vector<double>& transformVector, double imageDimX, double imageDimY);

  std::pair<double, double> getImageDims() const;

  /**
   * @brief Returns the required length for either X or Y parameters.
   * Combined parameter length should be 2x this value.
   * @return
   */
  size_t getSingleParamCount() const;

  /**
   * @brief Returns the default parameter scaling / step size.
   * This is a combination of getDefaultScalingX() and getDefaultScalingY().
   * @return
   */
  std::vector<double> getDefaultScaling() const;

  /**
   * @brief Gets the default Px  scaling / step size
   * @return
   */
  std::vector<double> getDefaultScalingX() const;
  
  /**
   * @brief Gets the default Py  scaling / step size
   * @return
   */
  std::vector<double> getDefaultScalingY() const;

private:
  QString m_MontageName;
  uint m_MaxIterations = 1000;
  IntVec2Type m_OverlapAmt = {10, 10};
  double m_LowTolerance = 1E-2;
  double m_HighTolerance = 1E-2;
  int m_Degree = 2;
  bool m_UseAmoebaOptimizer = false;
  bool m_SpecifyInitialSimplex = true;
  QString m_PxStr = "0.0;0.0;0.0;1.0;0.0;0.0;0.0";
  QString m_PyStr = "0.0;1.0;0.0;0.0;0.0;0.0;0.0";
  QString m_AttributeMatrixName;
  QString m_IPFColorsArrayName;
  bool m_CreateTransformContainer;
  QString m_BlendDCName = "Blend Data";
  QString m_TransformMatrixName= "Transform Matrix";
  QString m_TransformArrayName = "Transform";
  QString m_ResidualArrayName = "Residual";
  QString m_TransformPrefix = "Transformed_";

  std::vector<double> m_PxVec;
  std::vector<double> m_PyVec;
};
