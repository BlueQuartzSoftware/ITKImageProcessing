// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKOtsuMultipleThresholdsImage_h_
#define _ITKOtsuMultipleThresholdsImage_h_

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
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKOtsuMultipleThresholdsImage class. See [Filter documentation](@ref ITKOtsuMultipleThresholdsImage) for details.
 */
class ITKImageProcessing_EXPORT ITKOtsuMultipleThresholdsImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKOtsuMultipleThresholdsImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(int NumberOfThresholds READ getNumberOfThresholds WRITE setNumberOfThresholds)
  PYB11_PROPERTY(int LabelOffset READ getLabelOffset WRITE setLabelOffset)
  PYB11_PROPERTY(double NumberOfHistogramBins READ getNumberOfHistogramBins WRITE setNumberOfHistogramBins)
  PYB11_PROPERTY(bool ValleyEmphasis READ getValleyEmphasis WRITE setValleyEmphasis)
  PYB11_PROPERTY(FloatVec3_t Thresholds READ getThresholds WRITE setThresholds)

public:
  SIMPL_SHARED_POINTERS(ITKOtsuMultipleThresholdsImage)
  SIMPL_FILTER_NEW_MACRO(ITKOtsuMultipleThresholdsImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKOtsuMultipleThresholdsImage, AbstractFilter)

  ~ITKOtsuMultipleThresholdsImage() override;

  SIMPL_FILTER_PARAMETER(int, NumberOfThresholds)
  Q_PROPERTY(int NumberOfThresholds READ getNumberOfThresholds WRITE setNumberOfThresholds)

  SIMPL_FILTER_PARAMETER(int, LabelOffset)
  Q_PROPERTY(int LabelOffset READ getLabelOffset WRITE setLabelOffset)

  SIMPL_FILTER_PARAMETER(double, NumberOfHistogramBins)
  Q_PROPERTY(double NumberOfHistogramBins READ getNumberOfHistogramBins WRITE setNumberOfHistogramBins)

  SIMPL_FILTER_PARAMETER(bool, ValleyEmphasis)
  Q_PROPERTY(bool ValleyEmphasis READ getValleyEmphasis WRITE setValleyEmphasis)

  SIMPL_FILTER_PARAMETER(FloatVec3_t, Thresholds)
  Q_PROPERTY(FloatVec3_t Thresholds READ getThresholds)


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
  ITKOtsuMultipleThresholdsImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void virtual dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheck();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void virtual filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

private:
  ITKOtsuMultipleThresholdsImage(const ITKOtsuMultipleThresholdsImage&) = delete;    // Copy Constructor Not Implemented
  ITKOtsuMultipleThresholdsImage(ITKOtsuMultipleThresholdsImage&&) = delete;         // Move Constructor Not Implemented
  ITKOtsuMultipleThresholdsImage& operator=(const ITKOtsuMultipleThresholdsImage&) = delete; // Copy Assignment Not Implemented
  ITKOtsuMultipleThresholdsImage& operator=(ITKOtsuMultipleThresholdsImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKOtsuMultipleThresholdsImage_H_ */
