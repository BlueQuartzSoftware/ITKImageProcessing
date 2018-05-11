// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKNormalizeImage_h_
#define _ITKNormalizeImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <itkNormalizeImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKNormalizeImage class. See [Filter documentation](@ref ITKNormalizeImage) for details.
 */
class ITKImageProcessing_EXPORT ITKNormalizeImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKNormalizeImage)
  SIMPL_FILTER_NEW_MACRO(ITKNormalizeImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKNormalizeImage, AbstractFilter)

  ~ITKNormalizeImage() override;

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
  ITKNormalizeImage();

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
  ITKNormalizeImage(const ITKNormalizeImage&) = delete;    // Copy Constructor Not Implemented
  ITKNormalizeImage(ITKNormalizeImage&&) = delete;         // Move Constructor Not Implemented
  ITKNormalizeImage& operator=(const ITKNormalizeImage&) = delete; // Copy Assignment Not Implemented
  ITKNormalizeImage& operator=(ITKNormalizeImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKNormalizeImage_H_ */
