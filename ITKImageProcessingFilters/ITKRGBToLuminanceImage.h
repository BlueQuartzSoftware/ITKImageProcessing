/*
 * Your License or Copyright can go here
 */

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkRGBToLuminanceImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKRGBToLuminanceImage class. See [Filter documentation](@ref ITKRGBToLuminanceImage) for details.
 */
class ITKImageProcessing_EXPORT ITKRGBToLuminanceImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKRGBToLuminanceImage)
  SIMPL_FILTER_NEW_MACRO(ITKRGBToLuminanceImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKRGBToLuminanceImage, AbstractFilter)

  ~ITKRGBToLuminanceImage() override;

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

public:
  ITKRGBToLuminanceImage(const ITKRGBToLuminanceImage&) = delete; // Copy Constructor Not Implemented
  ITKRGBToLuminanceImage(ITKRGBToLuminanceImage&&) = delete;      // Move Constructor Not Implemented
  ITKRGBToLuminanceImage& operator=(const ITKRGBToLuminanceImage&) = delete; // Copy Assignment Not Implemented
  ITKRGBToLuminanceImage& operator=(ITKRGBToLuminanceImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

