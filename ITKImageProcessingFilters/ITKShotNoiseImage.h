// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKShotNoiseImage_h_
#define _ITKShotNoiseImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkShotNoiseImageFilter.h>

/**
 * @brief The ITKShotNoiseImage class. See [Filter documentation](@ref ITKShotNoiseImage) for details.
 */
class ITKShotNoiseImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKShotNoiseImage)
  SIMPL_STATIC_NEW_MACRO(ITKShotNoiseImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKShotNoiseImage, AbstractFilter)

  virtual ~ITKShotNoiseImage();

  SIMPL_FILTER_PARAMETER(double, Scale)
  Q_PROPERTY(double Scale READ getScale WRITE setScale)

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
  ITKShotNoiseImage();

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
  ITKShotNoiseImage(const ITKShotNoiseImage&); // Copy Constructor Not Implemented
  void operator=(const ITKShotNoiseImage&);    // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKShotNoiseImage_H_ */
