// File automatically generated

/*
 * Your License or Copyright can go here
 */

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkFFTNormalizedCorrelationImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKFFTNormalizedCorrelationImage class. See [Filter documentation](@ref ITKFFTNormalizedCorrelationImage) for details.
 */
class ITKImageProcessing_EXPORT ITKFFTNormalizedCorrelationImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKFFTNormalizedCorrelationImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(DataArrayPath MovingCellArrayPath READ getMovingCellArrayPath WRITE setMovingCellArrayPath)
  PYB11_PROPERTY(double RequiredNumberOfOverlappingPixels READ getRequiredNumberOfOverlappingPixels WRITE setRequiredNumberOfOverlappingPixels)
  PYB11_PROPERTY(double RequiredFractionOfOverlappingPixels READ getRequiredFractionOfOverlappingPixels WRITE setRequiredFractionOfOverlappingPixels)

public:
  SIMPL_SHARED_POINTERS(ITKFFTNormalizedCorrelationImage)
  SIMPL_FILTER_NEW_MACRO(ITKFFTNormalizedCorrelationImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKFFTNormalizedCorrelationImage, AbstractFilter)

  SIMPL_FILTER_PARAMETER(DataArrayPath, MovingCellArrayPath)
  Q_PROPERTY(DataArrayPath MovingCellArrayPath READ getMovingCellArrayPath WRITE setMovingCellArrayPath)

  ~ITKFFTNormalizedCorrelationImage() override;

  SIMPL_FILTER_PARAMETER(double, RequiredNumberOfOverlappingPixels)
  Q_PROPERTY(double RequiredNumberOfOverlappingPixels READ getRequiredNumberOfOverlappingPixels WRITE setRequiredNumberOfOverlappingPixels)

  SIMPL_FILTER_PARAMETER(double, RequiredFractionOfOverlappingPixels)
  Q_PROPERTY(double RequiredFractionOfOverlappingPixels READ getRequiredFractionOfOverlappingPixels WRITE setRequiredFractionOfOverlappingPixels)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  const QString getHumanLabel() const override;

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
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKFFTNormalizedCorrelationImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheck();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

public:
  ITKFFTNormalizedCorrelationImage(const ITKFFTNormalizedCorrelationImage&) = delete; // Copy Constructor Not Implemented
  ITKFFTNormalizedCorrelationImage(ITKFFTNormalizedCorrelationImage&&) = delete;      // Move Constructor Not Implemented
  ITKFFTNormalizedCorrelationImage& operator=(const ITKFFTNormalizedCorrelationImage&) = delete; // Copy Assignment Not Implemented
  ITKFFTNormalizedCorrelationImage& operator=(ITKFFTNormalizedCorrelationImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

