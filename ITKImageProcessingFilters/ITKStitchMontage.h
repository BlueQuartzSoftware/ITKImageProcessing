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

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataContainers/DataContainerArrayProxy.h"
#include "SIMPLib/FilterParameters/IntVec3FilterParameter.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Geometry/CompositeTransformContainer.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/Geometry/TransformContainer.h"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/Utilities/MontageSelection.h"

#include "itkAffineTransform.h"
#include "itkCompositeTransform.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"
#include "ITKImageProcessing/ITKImageProcessingConstants.h"

using AffineType = itk::AffineTransform<double, 3>;
using CompositeTransform = itk::CompositeTransform<double, 3>;

/**
 * @brief The
 */
class ITKImageProcessing_EXPORT ITKStitchMontage : public AbstractFilter
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKStitchMontage SUPERCLASS AbstractFilter)
  PYB11_SHARED_POINTERS(ITKStitchMontage)
  PYB11_FILTER_NEW_MACRO(ITKStitchMontage)

  PYB11_PROPERTY(MontageSelection MontageSelection READ getMontageSelection WRITE setMontageSelection)

  PYB11_PROPERTY(QString CommonAttributeMatrixName READ getCommonAttributeMatrixName WRITE setCommonAttributeMatrixName)
  PYB11_PROPERTY(QString CommonDataArrayName READ getCommonDataArrayName WRITE setCommonDataArrayName)
  PYB11_PROPERTY(QString MontageDataContainerName READ getMontageDataContainerName WRITE setMontageDataContainerName)
  PYB11_PROPERTY(QString MontageAttributeMatrixName READ getMontageAttributeMatrixName WRITE setMontageAttributeMatrixName)
  PYB11_PROPERTY(QString MontageDataArrayName READ getMontageDataArrayName WRITE setMontageDataArrayName)

#endif

public:
  using Self = ITKStitchMontage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKStitchMontage> New();

  /**
   * @brief Returns the name of the class for ITKStitchMontage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKStitchMontage
   */
  static QString ClassName();

  ~ITKStitchMontage() override;

  /**
   * @brief Getter property for MontageSelection
   * @return
   */
  MontageSelection getMontageSelection() const;

  /**
   * @brief Setter property for MontageSelection
   * @param value
   */
  void setMontageSelection(const MontageSelection& value);

  Q_PROPERTY(MontageSelection MontageSelection READ getMontageSelection WRITE setMontageSelection)
  /**
   * @brief Setter property for CommonAttributeMatrixName
   */
  void setCommonAttributeMatrixName(const QString& value);
  /**
   * @brief Getter property for CommonAttributeMatrixName
   * @return Value of CommonAttributeMatrixName
   */
  QString getCommonAttributeMatrixName() const;

  Q_PROPERTY(QString CommonAttributeMatrixName READ getCommonAttributeMatrixName WRITE setCommonAttributeMatrixName)

  /**
   * @brief Setter property for CommonDataArrayName
   */
  void setCommonDataArrayName(const QString& value);
  /**
   * @brief Getter property for CommonDataArrayName
   * @return Value of CommonDataArrayName
   */
  QString getCommonDataArrayName() const;

  Q_PROPERTY(QString CommonDataArrayName READ getCommonDataArrayName WRITE setCommonDataArrayName)

  /**
   * @brief Setter property for MontageDataContainerName
   */
  void setMontageDataContainerName(const QString& value);
  /**
   * @brief Getter property for MontageDataContainerName
   * @return Value of MontageDataContainerName
   */
  QString getMontageDataContainerName() const;

  Q_PROPERTY(QString MontageDataContainerName READ getMontageDataContainerName WRITE setMontageDataContainerName)

  /**
   * @brief Setter property for MontageAttributeMatrixName
   */
  void setMontageAttributeMatrixName(const QString& value);
  /**
   * @brief Getter property for MontageAttributeMatrixName
   * @return Value of MontageAttributeMatrixName
   */
  QString getMontageAttributeMatrixName() const;

  Q_PROPERTY(QString MontageAttributeMatrixName READ getMontageAttributeMatrixName WRITE setMontageAttributeMatrixName)

  /**
   * @brief Setter property for MontageDataArrayName
   */
  void setMontageDataArrayName(const QString& value);
  /**
   * @brief Getter property for MontageDataArrayName
   * @return Value of MontageDataArrayName
   */
  QString getMontageDataArrayName() const;
  Q_PROPERTY(QString MontageDataArrayName READ getMontageDataArrayName WRITE setMontageDataArrayName)

  /**
   * @brief getMontageInformation
   * @return
   */
  QString getDataContainerInfo();
  Q_PROPERTY(QString DataContainerInfo READ getDataContainerInfo)

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
  ITKStitchMontage();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck() override;

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief Stitch the montage
   */
  template <typename PixelType, typename AccumulatePixelType>
  void stitchMontage(int peakMethodToUse = 0, unsigned streamSubdivisions = 1);

  /**
   * @brief Get the transform container from an affine transform
   * @param itkAffine
   */
  typename TransformContainer::Pointer GetTransformContainerFromITKAffineTransform(const AffineType::Pointer& itkAffine);

private:
  MontageSelection m_MontageSelection = {};

  QString m_CommonAttributeMatrixName = {ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName};
  QString m_CommonDataArrayName = {ITKImageProcessing::Montage::k_TileDataArrayDefaultName};
  QString m_MontageDataContainerName = {ITKImageProcessing::Montage::k_MontageDataContainerDefaultName};
  QString m_MontageAttributeMatrixName = {ITKImageProcessing::Montage::k_MontageAttributeMatrixDefaultName};
  QString m_MontageDataArrayName = {ITKImageProcessing::Montage::k_MontageDataArrayDefaultName};

  // QString m_DataContainerList;

  static constexpr unsigned Dimension = 2;
  IntVec2Type m_MontageSize;
  std::vector<DataContainer::Pointer> m_ImageDataContainers;

  /**
   * @brief createResampler
   */
  template <typename PixelType, typename Resampler>
  typename Resampler::Pointer createResampler();

  /**
   * @brief initializeResampler
   */
  template <typename PixelType, typename MontageType, typename Resampler>
  void initializeResampler(typename Resampler::Pointer resampler);

  /**
   * @brief stitchMontageHelper
   * @param resampler
   */
  template <typename PixelType, typename Resampler>
  void executeStitching(typename Resampler::Pointer resampler, unsigned streamSubdivisions);

  /**
   * @brief convertMontageToD3D
   */
  template <typename PixelType, typename OriginalImageType>
  void convertMontageToD3D(OriginalImageType* image);

public:
  ITKStitchMontage(const ITKStitchMontage&) = delete;            // Copy Constructor Not Implemented
  ITKStitchMontage(ITKStitchMontage&&) = delete;                 // Move Constructor Not Implemented
  ITKStitchMontage& operator=(const ITKStitchMontage&) = delete; // Copy Assignment Not Implemented
  ITKStitchMontage& operator=(ITKStitchMontage&&) = delete;      // Move Assignment Not Implemented
};
