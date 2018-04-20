// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryThinningImage_h_
#define _ITKBinaryThinningImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <itkBinaryThinningImageFilter.h>


/**
 * @brief The ITKBinaryThinningImage class. See [Filter documentation](@ref ITKBinaryThinningImage) for details.
 */
class ITKBinaryThinningImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKBinaryThinningImage)
  SIMPL_FILTER_NEW_MACRO(ITKBinaryThinningImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBinaryThinningImage, AbstractFilter)

  ~ITKBinaryThinningImage() override;

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
  ITKBinaryThinningImage();

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
  ITKBinaryThinningImage(const ITKBinaryThinningImage&) = delete;    // Copy Constructor Not Implemented
  ITKBinaryThinningImage(ITKBinaryThinningImage&&) = delete;         // Move Constructor Not Implemented
  ITKBinaryThinningImage& operator=(const ITKBinaryThinningImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryThinningImage& operator=(ITKBinaryThinningImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKBinaryThinningImage_H_ */
