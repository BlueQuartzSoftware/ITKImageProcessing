// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBoundedReciprocalImage_h_
#define _ITKBoundedReciprocalImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <itkBoundedReciprocalImageFilter.h>

/**
 * @brief The ITKBoundedReciprocalImage class. See [Filter documentation](@ref ITKBoundedReciprocalImage) for details.
 */
class ITKBoundedReciprocalImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKBoundedReciprocalImage)
  SIMPL_STATIC_NEW_MACRO(ITKBoundedReciprocalImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBoundedReciprocalImage, AbstractFilter)

  virtual ~ITKBoundedReciprocalImage();

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
  ITKBoundedReciprocalImage();

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
  ITKBoundedReciprocalImage(const ITKBoundedReciprocalImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKBoundedReciprocalImage&);            // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKBoundedReciprocalImage_H_ */
