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

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataContainers/DataContainerArrayProxy.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Geometry/CompositeTransformContainer.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Geometry/TransformContainer.h"
#include "SIMPLib/SIMPLib.h"

#include "itkAffineTransform.h"
#include "itkCompositeTransform.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

using AffineType = itk::AffineTransform<double, 3>;
using CompositeTransform = itk::CompositeTransform<double, 3>;

/**
 * @brief The
 */
class ITKImageProcessing_EXPORT ITKPCMTileRegistration : public AbstractFilter
{
  Q_OBJECT

  PYB11_CREATE_BINDINGS(ITKPCMTileRegistration SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(IntVec2Type MontageStart READ getMontageStart WRITE setMontageStart)
  PYB11_PROPERTY(IntVec2Type MontageEnd READ getMontageEnd WRITE setMontageEnd)
  PYB11_PROPERTY(QString DataContainerPrefix READ getDataContainerPrefix WRITE setDataContainerPrefix)
  PYB11_PROPERTY(QString CommonAttributeMatrixName READ getCommonAttributeMatrixName WRITE setCommonAttributeMatrixName)
  PYB11_PROPERTY(QString CommonDataArrayName READ getCommonDataArrayName WRITE setCommonDataArrayName)

public:
  SIMPL_SHARED_POINTERS(ITKPCMTileRegistration)
  SIMPL_FILTER_NEW_MACRO(ITKPCMTileRegistration)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKPCMTileRegistration, AbstractFilter)

  ~ITKPCMTileRegistration() override;

  SIMPL_FILTER_PARAMETER(IntVec2Type, MontageStart)
  Q_PROPERTY(IntVec2Type MontageStart READ getMontageStart WRITE setMontageStart)

  SIMPL_FILTER_PARAMETER(IntVec2Type, MontageEnd)
  Q_PROPERTY(IntVec2Type MontageEnd READ getMontageEnd WRITE setMontageEnd)

  SIMPL_FILTER_PARAMETER(QString, DataContainerPrefix)
  Q_PROPERTY(QString DataContainerPrefix READ getDataContainerPrefix WRITE setDataContainerPrefix)

  SIMPL_FILTER_PARAMETER(QString, CommonAttributeMatrixName)
  Q_PROPERTY(QString CommonAttributeMatrixName READ getCommonAttributeMatrixName WRITE setCommonAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, CommonDataArrayName)
  Q_PROPERTY(QString CommonDataArrayName READ getCommonDataArrayName WRITE setCommonDataArrayName)

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
  ITKPCMTileRegistration();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief registerGrayscaleMontage
   * @param peakMethodToUse
   * @param streamSubdivisions
   */
  template <typename PixelType>
  void registerGrayscaleMontage(int peakMethodToUse = 0, unsigned streamSubdivisions = 1);

  /**
   * @brief registerRGBMontage
   * @param peakMethodToUse
   * @param streamSubdivisions
   */
  template <typename PixelType>
  void registerRGBMontage(int peakMethodToUse = 0, unsigned streamSubdivisions = 1);

  /**
   * @brief Get the image from the appropriate data container
   * @param y
   * @param x
   */
  DataContainer::Pointer GetImageDataContainer(int y, int x);

  /**
   * @brief Get the transform container from an affine transform
   * @param itkAffine
   */
  typename TransformContainer::Pointer GetTransformContainerFromITKAffineTransform(const AffineType::Pointer& itkAffine);

private:
  static constexpr unsigned Dimension = 2;
  std::vector<DataContainer::Pointer> m_DataContainers;

  /**
   * @brief createMontage
   * @param peakMethodToUse
   * @param streamSubdivisions
   */
  template <typename PixelType, typename MontageType>
  typename MontageType::Pointer createMontage(int peakMethodToUse = 0);

  /**
   * @brief createGrayscaleMontage
   * @param peakMethodToUse
   * @param streamSubdivisions
   */
  template <typename PixelType, typename MontageType>
  typename MontageType::Pointer createGrayscaleMontage(int peakMethodToUse = 0);

  /**
   * @brief createRGBMontage
   * @param peakMethodToUse
   * @param streamSubdivisions
   */
  template <typename PixelType, typename MontageType>
  typename MontageType::Pointer createRGBMontage(int peakMethodToUse = 0);

  /**
   * @brief storeMontageTransforms
   */
  template <typename MontageType>
  void storeMontageTransforms(typename MontageType::Pointer montage);

  /**
   * @brief executeMontageRegistration
   * @param montage
   */
  template <typename MontageType>
  void executeMontageRegistration(typename MontageType::Pointer montage);

public:
  ITKPCMTileRegistration(const ITKPCMTileRegistration&) = delete;            // Copy Constructor Not Implemented
  ITKPCMTileRegistration(ITKPCMTileRegistration&&) = delete;                 // Move Constructor Not Implemented
  ITKPCMTileRegistration& operator=(const ITKPCMTileRegistration&) = delete; // Copy Assignment Not Implemented
  ITKPCMTileRegistration& operator=(ITKPCMTileRegistration&&) = delete;      // Move Assignment Not Implemented
};
