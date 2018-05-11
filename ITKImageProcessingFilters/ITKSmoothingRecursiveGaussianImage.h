// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSmoothingRecursiveGaussianImage_h_
#define _ITKSmoothingRecursiveGaussianImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <itkSmoothingRecursiveGaussianImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKSmoothingRecursiveGaussianImage class. See [Filter documentation](@ref ITKSmoothingRecursiveGaussianImage) for details.
 */
class ITKImageProcessing_EXPORT ITKSmoothingRecursiveGaussianImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKSmoothingRecursiveGaussianImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(FloatVec3_t Sigma READ getSigma WRITE setSigma)
  PYB11_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)

public:
  SIMPL_SHARED_POINTERS(ITKSmoothingRecursiveGaussianImage)
  SIMPL_FILTER_NEW_MACRO(ITKSmoothingRecursiveGaussianImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKSmoothingRecursiveGaussianImage, AbstractFilter)

  ~ITKSmoothingRecursiveGaussianImage() override;

  SIMPL_FILTER_PARAMETER(FloatVec3_t, Sigma)
  Q_PROPERTY(FloatVec3_t Sigma READ getSigma WRITE setSigma)

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
  ITKSmoothingRecursiveGaussianImage();

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
  ITKSmoothingRecursiveGaussianImage(const ITKSmoothingRecursiveGaussianImage&) = delete;    // Copy Constructor Not Implemented
  ITKSmoothingRecursiveGaussianImage(ITKSmoothingRecursiveGaussianImage&&) = delete;         // Move Constructor Not Implemented
  ITKSmoothingRecursiveGaussianImage& operator=(const ITKSmoothingRecursiveGaussianImage&) = delete; // Copy Assignment Not Implemented
  ITKSmoothingRecursiveGaussianImage& operator=(ITKSmoothingRecursiveGaussianImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKSmoothingRecursiveGaussianImage_H_ */
