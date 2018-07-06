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
#include <itkInvertIntensityImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKInvertIntensityImage class. See [Filter documentation](@ref ITKInvertIntensityImage) for details.
 */
class ITKImageProcessing_EXPORT ITKInvertIntensityImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKInvertIntensityImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double Maximum READ getMaximum WRITE setMaximum)

public:
  SIMPL_SHARED_POINTERS(ITKInvertIntensityImage)
  SIMPL_FILTER_NEW_MACRO(ITKInvertIntensityImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKInvertIntensityImage, AbstractFilter)

  ~ITKInvertIntensityImage() override;

  SIMPL_FILTER_PARAMETER(double, Maximum)
  Q_PROPERTY(double Maximum READ getMaximum WRITE setMaximum)


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
  ITKInvertIntensityImage();

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
  ITKInvertIntensityImage(const ITKInvertIntensityImage&) = delete;    // Copy Constructor Not Implemented
  ITKInvertIntensityImage(ITKInvertIntensityImage&&) = delete;         // Move Constructor Not Implemented
  ITKInvertIntensityImage& operator=(const ITKInvertIntensityImage&) = delete; // Copy Assignment Not Implemented
  ITKInvertIntensityImage& operator=(ITKInvertIntensityImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

