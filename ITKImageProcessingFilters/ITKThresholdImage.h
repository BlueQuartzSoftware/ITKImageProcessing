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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkThresholdImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKThresholdImage class. See [Filter documentation](@ref ITKThresholdImage) for details.
 */
class ITKImageProcessing_EXPORT ITKThresholdImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKThresholdImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double Lower READ getLower WRITE setLower)
  PYB11_PROPERTY(double Upper READ getUpper WRITE setUpper)
  PYB11_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)

public:
  SIMPL_SHARED_POINTERS(ITKThresholdImage)
  SIMPL_FILTER_NEW_MACRO(ITKThresholdImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKThresholdImage, AbstractFilter)

  ~ITKThresholdImage() override;

  SIMPL_FILTER_PARAMETER(double, Lower)
  Q_PROPERTY(double Lower READ getLower WRITE setLower)

  SIMPL_FILTER_PARAMETER(double, Upper)
  Q_PROPERTY(double Upper READ getUpper WRITE setUpper)

  SIMPL_FILTER_PARAMETER(double, OutsideValue)
  Q_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)


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
  ITKThresholdImage();

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
  ITKThresholdImage(const ITKThresholdImage&) = delete;    // Copy Constructor Not Implemented
  ITKThresholdImage(ITKThresholdImage&&) = delete;         // Move Constructor Not Implemented
  ITKThresholdImage& operator=(const ITKThresholdImage&) = delete; // Copy Assignment Not Implemented
  ITKThresholdImage& operator=(ITKThresholdImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

