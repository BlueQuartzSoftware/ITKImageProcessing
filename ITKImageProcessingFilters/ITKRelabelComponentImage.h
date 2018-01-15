// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKRelabelComponentImage_h_
#define _ITKRelabelComponentImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkRelabelComponentImageFilter.h>

/**
 * @brief The ITKRelabelComponentImage class. See [Filter documentation](@ref ITKRelabelComponentImage) for details.
 */
class ITKRelabelComponentImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKRelabelComponentImage)
  SIMPL_STATIC_NEW_MACRO(ITKRelabelComponentImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKRelabelComponentImage, AbstractFilter)

  virtual ~ITKRelabelComponentImage();

  SIMPL_FILTER_PARAMETER(double, MinimumObjectSize)
  Q_PROPERTY(double MinimumObjectSize READ getMinimumObjectSize WRITE setMinimumObjectSize)

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
  ITKRelabelComponentImage();

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
  ITKRelabelComponentImage(const ITKRelabelComponentImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKRelabelComponentImage&);           // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKRelabelComponentImage_H_ */
