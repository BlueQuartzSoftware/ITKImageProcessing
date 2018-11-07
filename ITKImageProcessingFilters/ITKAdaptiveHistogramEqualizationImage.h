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
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <itkAdaptiveHistogramEqualizationImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKAdaptiveHistogramEqualizationImage class. See [Filter documentation](@ref ITKAdaptiveHistogramEqualizationImage) for details.
 */
class ITKImageProcessing_EXPORT ITKAdaptiveHistogramEqualizationImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKAdaptiveHistogramEqualizationImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(FloatVec3_t Radius READ getRadius WRITE setRadius)
  PYB11_PROPERTY(float Alpha READ getAlpha WRITE setAlpha)
  PYB11_PROPERTY(float Beta READ getBeta WRITE setBeta)

public:
  SIMPL_SHARED_POINTERS(ITKAdaptiveHistogramEqualizationImage)
  SIMPL_FILTER_NEW_MACRO(ITKAdaptiveHistogramEqualizationImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKAdaptiveHistogramEqualizationImage, AbstractFilter)

  ~ITKAdaptiveHistogramEqualizationImage() override;

  SIMPL_FILTER_PARAMETER(FloatVec3_t, Radius)
  Q_PROPERTY(FloatVec3_t Radius READ getRadius WRITE setRadius)

  SIMPL_FILTER_PARAMETER(float, Alpha)
  Q_PROPERTY(float Alpha READ getAlpha WRITE setAlpha)

  SIMPL_FILTER_PARAMETER(float, Beta)
  Q_PROPERTY(float Beta READ getBeta WRITE setBeta)

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
  ITKAdaptiveHistogramEqualizationImage();

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
  ITKAdaptiveHistogramEqualizationImage(const ITKAdaptiveHistogramEqualizationImage&) = delete;    // Copy Constructor Not Implemented
  ITKAdaptiveHistogramEqualizationImage(ITKAdaptiveHistogramEqualizationImage&&) = delete;         // Move Constructor Not Implemented
  ITKAdaptiveHistogramEqualizationImage& operator=(const ITKAdaptiveHistogramEqualizationImage&) = delete; // Copy Assignment Not Implemented
  ITKAdaptiveHistogramEqualizationImage& operator=(ITKAdaptiveHistogramEqualizationImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

