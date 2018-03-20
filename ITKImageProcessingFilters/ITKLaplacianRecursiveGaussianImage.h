// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKLaplacianRecursiveGaussianImage_h_
#define _ITKLaplacianRecursiveGaussianImage_h_

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


/**
 * @brief The ITKLaplacianRecursiveGaussianImage class. See [Filter documentation](@ref ITKLaplacianRecursiveGaussianImage) for details.
 */
class ITKLaplacianRecursiveGaussianImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKLaplacianRecursiveGaussianImage)
  SIMPL_STATIC_NEW_MACRO(ITKLaplacianRecursiveGaussianImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKLaplacianRecursiveGaussianImage, AbstractFilter)

  virtual ~ITKLaplacianRecursiveGaussianImage();

  SIMPL_FILTER_PARAMETER(double, Sigma)
  Q_PROPERTY(double Sigma READ getSigma WRITE setSigma)

  SIMPL_FILTER_PARAMETER(bool, NormalizeAcrossScale)
  Q_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)


  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getSubGroupName() const override;

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

private:
  ITKLaplacianRecursiveGaussianImage(const ITKLaplacianRecursiveGaussianImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKLaplacianRecursiveGaussianImage&) = delete; // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKLaplacianRecursiveGaussianImage_H_ */
