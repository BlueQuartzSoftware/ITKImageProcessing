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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkBilateralImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBilateralImage class. See [Filter documentation](@ref ITKBilateralImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBilateralImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKBilateralImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double DomainSigma READ getDomainSigma WRITE setDomainSigma)
  PYB11_PROPERTY(double RangeSigma READ getRangeSigma WRITE setRangeSigma)
  PYB11_PROPERTY(double NumberOfRangeGaussianSamples READ getNumberOfRangeGaussianSamples WRITE setNumberOfRangeGaussianSamples)

public:
  SIMPL_SHARED_POINTERS(ITKBilateralImage)
  SIMPL_FILTER_NEW_MACRO(ITKBilateralImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBilateralImage, AbstractFilter)

  ~ITKBilateralImage() override;

  SIMPL_FILTER_PARAMETER(double, DomainSigma)
  Q_PROPERTY(double DomainSigma READ getDomainSigma WRITE setDomainSigma)

  SIMPL_FILTER_PARAMETER(double, RangeSigma)
  Q_PROPERTY(double RangeSigma READ getRangeSigma WRITE setRangeSigma)

  SIMPL_FILTER_PARAMETER(double, NumberOfRangeGaussianSamples)
  Q_PROPERTY(double NumberOfRangeGaussianSamples READ getNumberOfRangeGaussianSamples WRITE setNumberOfRangeGaussianSamples)


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
  ITKBilateralImage();

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
  ITKBilateralImage(const ITKBilateralImage&) = delete;    // Copy Constructor Not Implemented
  ITKBilateralImage(ITKBilateralImage&&) = delete;         // Move Constructor Not Implemented
  ITKBilateralImage& operator=(const ITKBilateralImage&) = delete; // Copy Assignment Not Implemented
  ITKBilateralImage& operator=(ITKBilateralImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

