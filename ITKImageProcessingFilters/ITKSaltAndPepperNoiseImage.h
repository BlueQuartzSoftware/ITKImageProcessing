// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSaltAndPepperNoiseImage_h_
#define _ITKSaltAndPepperNoiseImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkSaltAndPepperNoiseImageFilter.h>

/**
 * @brief The ITKSaltAndPepperNoiseImage class. See [Filter documentation](@ref ITKSaltAndPepperNoiseImage) for details.
 */
class ITKSaltAndPepperNoiseImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKSaltAndPepperNoiseImage)
  SIMPL_STATIC_NEW_MACRO(ITKSaltAndPepperNoiseImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKSaltAndPepperNoiseImage, AbstractFilter)

  virtual ~ITKSaltAndPepperNoiseImage();

  SIMPL_FILTER_PARAMETER(double, Probability)
  Q_PROPERTY(double Probability READ getProbability WRITE setProbability)

  SIMPL_FILTER_PARAMETER(double, Seed)
  Q_PROPERTY(double Seed READ getSeed WRITE setSeed)

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
  ITKSaltAndPepperNoiseImage();

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
  ITKSaltAndPepperNoiseImage(const ITKSaltAndPepperNoiseImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKSaltAndPepperNoiseImage&);             // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKSaltAndPepperNoiseImage_H_ */
