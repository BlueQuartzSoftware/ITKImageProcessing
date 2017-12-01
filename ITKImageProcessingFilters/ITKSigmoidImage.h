// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSigmoidImage_h_
#define _ITKSigmoidImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkSigmoidImageFilter.h>

/**
 * @brief The ITKSigmoidImage class. See [Filter documentation](@ref ITKSigmoidImage) for details.
 */
class ITKSigmoidImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKSigmoidImage)
  SIMPL_STATIC_NEW_MACRO(ITKSigmoidImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKSigmoidImage, AbstractFilter)

  virtual ~ITKSigmoidImage();

  SIMPL_FILTER_PARAMETER(double, Alpha)
  Q_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)

  SIMPL_FILTER_PARAMETER(double, Beta)
  Q_PROPERTY(double Beta READ getBeta WRITE setBeta)

  SIMPL_FILTER_PARAMETER(double, OutputMaximum)
  Q_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)

  SIMPL_FILTER_PARAMETER(double, OutputMinimum)
  Q_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)

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
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKSigmoidImage();

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
  ITKSigmoidImage(const ITKSigmoidImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKSigmoidImage&);  // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKSigmoidImage_H_ */
