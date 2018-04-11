// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKLog10Image_h_
#define _ITKLog10Image_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <itkLog10ImageFilter.h>


/**
 * @brief The ITKLog10Image class. See [Filter documentation](@ref ITKLog10Image) for details.
 */
class ITKLog10Image : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKLog10Image)
  SIMPL_FILTER_NEW_MACRO(ITKLog10Image)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKLog10Image, AbstractFilter)

  ~ITKLog10Image() override;

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
  ITKLog10Image();

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
  ITKLog10Image(const ITKLog10Image&) = delete;    // Copy Constructor Not Implemented
  ITKLog10Image(ITKLog10Image&&) = delete;         // Move Constructor Not Implemented
  ITKLog10Image& operator=(const ITKLog10Image&) = delete; // Copy Assignment Not Implemented
  ITKLog10Image& operator=(ITKLog10Image&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKLog10Image_H_ */
