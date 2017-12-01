/*
 * Your License or Copyright can go here
 */

#ifndef _ITKRGBToLuminanceImage_h_
#define _ITKRGBToLuminanceImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkRGBToLuminanceImageFilter.h>

/**
 * @brief The ITKRGBToLuminanceImage class. See [Filter documentation](@ref ITKRGBToLuminanceImage) for details.
 */
class ITKRGBToLuminanceImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKRGBToLuminanceImage)
  SIMPL_STATIC_NEW_MACRO(ITKRGBToLuminanceImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKRGBToLuminanceImage, AbstractFilter)

  virtual ~ITKRGBToLuminanceImage();

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
  ITKRGBToLuminanceImage();

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
  ITKRGBToLuminanceImage(const ITKRGBToLuminanceImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKRGBToLuminanceImage&);         // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKRGBToLuminanceImage_H_ */
