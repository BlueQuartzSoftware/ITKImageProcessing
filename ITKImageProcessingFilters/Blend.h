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
  SIMPL_SHARED_POINTERS(Blend)
  SIMPL_FILTER_NEW_MACRO(Blend)
  SIMPL_TYPE_MACRO_SUPER(Blend, AbstractFilter)

  Blend(const Blend&) = delete;            // Copy Constructor Not Implemented
  Blend& operator=(const Blend&) = delete; // Copy Assignment Not Implemented
  Blend(Blend&&) = delete;                 // Move Constructor Not Implemented
  Blend& operator=(Blend&&) = delete;      // Move Assignment Not Implemented
  ~Blend() override;

  SIMPL_FILTER_PARAMETER(QString, MontageName)
  Q_PROPERTY(QString MontageName READ getMontageName WRITE setMontageName)

  SIMPL_FILTER_PARAMETER(uint, MaxIterations)
  Q_PROPERTY(uint MaxIterations READ getMaxIterations WRITE setMaxIterations)

  SIMPL_FILTER_PARAMETER(float, OverlapPercentage)
  Q_PROPERTY(float OverlapPercentage READ getOverlapPercentage WRITE setOverlapPercentage)

  SIMPL_FILTER_PARAMETER(double, LowTolerance)
  Q_PROPERTY(double LowTolerance READ getLowTolerance WRITE setLowTolerance)

  SIMPL_FILTER_PARAMETER(double, HighTolerance)
  Q_PROPERTY(double HighTolerance READ getHighTolerance WRITE setHighTolerance)

  SIMPL_FILTER_PARAMETER(int, Degree)
  Q_PROPERTY(int Degree READ getDegree WRITE setDegree)

  SIMPL_FILTER_PARAMETER(QString, InitialSimplexGuess)
  Q_PROPERTY(QString InitialSimplexGuess READ getInitialSimplexGuess WRITE setInitialSimplexGuess)

  SIMPL_FILTER_PARAMETER(QString, AttributeMatrixName)
  Q_PROPERTY(QString AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, IPFColorsArrayName)
  Q_PROPERTY(QString IPFColorsArrayName READ getIPFColorsArrayName WRITE setIPFColorsArrayName)

  SIMPL_FILTER_PARAMETER(QString, BlendDCName)
  Q_PROPERTY(QString TransformMatrixName READ getTransformMatrixName WRITE setTransformMatrixName)

  SIMPL_FILTER_PARAMETER(QString, TransformMatrixName)
  Q_PROPERTY(QString TransformMatrixName READ getTransformMatrixName WRITE setTransformMatrixName)

  SIMPL_FILTER_PARAMETER(QString, TransformArrayName)
  Q_PROPERTY(QString TransformArrayName READ getTransformArrayName WRITE setTransformArrayName)

  //SIMPL_FILTER_PARAMETER(QString, NumIterationsArrayName)
  //Q_PROPERTY(QString NumIterationsArrayName READ getNumIterationsArrayName WRITE setNumIterationsArrayName)

  SIMPL_FILTER_PARAMETER(QString, ResidualArrayName)
  Q_PROPERTY(QString ResidualArrayName READ getResidualArrayName WRITE setResidualArrayName)

  bool GetConvergenceFromStopDescription(const QString&) const;

  uint GetIterationsFromStopDescription(const QString&) const;

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
  void warpDataContainers(const std::vector<double>& transformVector);

private:
  std::vector<double> m_InitialGuess;
};
