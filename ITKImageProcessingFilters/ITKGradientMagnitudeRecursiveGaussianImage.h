// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGradientMagnitudeRecursiveGaussianImage_h_
#define _ITKGradientMagnitudeRecursiveGaussianImage_h_

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
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>


/**
 * @brief The ITKGradientMagnitudeRecursiveGaussianImage class. See [Filter documentation](@ref ITKGradientMagnitudeRecursiveGaussianImage) for details.
 */
class ITKGradientMagnitudeRecursiveGaussianImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKGradientMagnitudeRecursiveGaussianImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double Sigma READ getSigma WRITE setSigma)
  PYB11_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)

public:
  SIMPL_SHARED_POINTERS(ITKGradientMagnitudeRecursiveGaussianImage)
  SIMPL_STATIC_NEW_MACRO(ITKGradientMagnitudeRecursiveGaussianImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKGradientMagnitudeRecursiveGaussianImage, AbstractFilter)

  virtual ~ITKGradientMagnitudeRecursiveGaussianImage();

  SIMPL_FILTER_PARAMETER(double, Sigma)
  Q_PROPERTY(double Sigma READ getSigma WRITE setSigma)

  SIMPL_FILTER_PARAMETER(bool, NormalizeAcrossScale)
  Q_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)


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
  ITKGradientMagnitudeRecursiveGaussianImage();

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
  ITKGradientMagnitudeRecursiveGaussianImage(const ITKGradientMagnitudeRecursiveGaussianImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKGradientMagnitudeRecursiveGaussianImage&) = delete;                                // Move assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKGradientMagnitudeRecursiveGaussianImage_H_ */
