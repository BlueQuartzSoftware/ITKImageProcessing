// File automatically generated

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

// Auto includes
#include <itkBoundedReciprocalImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBoundedReciprocalImage class. See [Filter documentation](@ref ITKBoundedReciprocalImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBoundedReciprocalImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKBoundedReciprocalImage)
  SIMPL_FILTER_NEW_MACRO(ITKBoundedReciprocalImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBoundedReciprocalImage, AbstractFilter)

  ~ITKBoundedReciprocalImage() override;

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

public:
  ITKBoundedReciprocalImage(const ITKBoundedReciprocalImage&) = delete;    // Copy Constructor Not Implemented
  ITKBoundedReciprocalImage(ITKBoundedReciprocalImage&&) = delete;         // Move Constructor Not Implemented
  ITKBoundedReciprocalImage& operator=(const ITKBoundedReciprocalImage&) = delete; // Copy Assignment Not Implemented
  ITKBoundedReciprocalImage& operator=(ITKBoundedReciprocalImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

