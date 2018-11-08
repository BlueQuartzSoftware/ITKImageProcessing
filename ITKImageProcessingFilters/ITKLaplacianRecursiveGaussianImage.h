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
#include <itkLaplacianRecursiveGaussianImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKLaplacianRecursiveGaussianImage class. See [Filter documentation](@ref ITKLaplacianRecursiveGaussianImage) for details.
 */
class ITKImageProcessing_EXPORT ITKLaplacianRecursiveGaussianImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKLaplacianRecursiveGaussianImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double Sigma READ getSigma WRITE setSigma)
  PYB11_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)

public:
  SIMPL_SHARED_POINTERS(ITKLaplacianRecursiveGaussianImage)
  SIMPL_FILTER_NEW_MACRO(ITKLaplacianRecursiveGaussianImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKLaplacianRecursiveGaussianImage, AbstractFilter)

  ~ITKLaplacianRecursiveGaussianImage() override;

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
  ITKLaplacianRecursiveGaussianImage();

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

public:
  ITKLaplacianRecursiveGaussianImage(const ITKLaplacianRecursiveGaussianImage&) = delete;    // Copy Constructor Not Implemented
  ITKLaplacianRecursiveGaussianImage(ITKLaplacianRecursiveGaussianImage&&) = delete;         // Move Constructor Not Implemented
  ITKLaplacianRecursiveGaussianImage& operator=(const ITKLaplacianRecursiveGaussianImage&) = delete; // Copy Assignment Not Implemented
  ITKLaplacianRecursiveGaussianImage& operator=(ITKLaplacianRecursiveGaussianImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

