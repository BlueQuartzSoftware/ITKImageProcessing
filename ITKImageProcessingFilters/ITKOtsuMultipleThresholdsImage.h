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

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>

/**
 * @brief The ITKOtsuMultipleThresholdsImage class. See [Filter documentation](@ref ITKOtsuMultipleThresholdsImage) for details.
 */
class ITKOtsuMultipleThresholdsImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKOtsuMultipleThresholdsImage)
  SIMPL_STATIC_NEW_MACRO(ITKOtsuMultipleThresholdsImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKOtsuMultipleThresholdsImage, AbstractFilter)

  virtual ~ITKOtsuMultipleThresholdsImage();

  SIMPL_FILTER_PARAMETER(int, NumberOfThresholds)
  Q_PROPERTY(int NumberOfThresholds READ getNumberOfThresholds WRITE setNumberOfThresholds)

  SIMPL_FILTER_PARAMETER(int, LabelOffset)
  Q_PROPERTY(int LabelOffset READ getLabelOffset WRITE setLabelOffset)

  SIMPL_FILTER_PARAMETER(double, NumberOfHistogramBins)
  Q_PROPERTY(double NumberOfHistogramBins READ getNumberOfHistogramBins WRITE setNumberOfHistogramBins)

  SIMPL_FILTER_PARAMETER(bool, ValleyEmphasis)
  Q_PROPERTY(bool ValleyEmphasis READ getValleyEmphasis WRITE setValleyEmphasis)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getSubGroupName() override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  virtual const QUuid getUuid() override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

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
  ITKOtsuMultipleThresholdsImage(const ITKOtsuMultipleThresholdsImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKOtsuMultipleThresholdsImage&);                 // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKOtsuMultipleThresholdsImage_H_ */
