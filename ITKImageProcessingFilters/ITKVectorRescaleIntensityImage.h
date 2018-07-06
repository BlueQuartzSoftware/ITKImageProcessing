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
#include <itkVectorRescaleIntensityImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKVectorRescaleIntensityImage class. See [Filter documentation](@ref ITKVectorRescaleIntensityImage) for details.
 */
class ITKImageProcessing_EXPORT ITKVectorRescaleIntensityImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKVectorRescaleIntensityImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double OutputMaximumMagnitude READ getOutputMaximumMagnitude WRITE setOutputMaximumMagnitude)
  PYB11_PROPERTY(int OutputType READ getOutputType WRITE setOutputType)

public:
  SIMPL_SHARED_POINTERS(ITKVectorRescaleIntensityImage)
  SIMPL_FILTER_NEW_MACRO(ITKVectorRescaleIntensityImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKVectorRescaleIntensityImage, AbstractFilter)

  ~ITKVectorRescaleIntensityImage() override;

  SIMPL_FILTER_PARAMETER(double, OutputMaximumMagnitude)
  Q_PROPERTY(double OutputMaximumMagnitude READ getOutputMaximumMagnitude WRITE setOutputMaximumMagnitude)

  SIMPL_FILTER_PARAMETER(int, OutputType)
  Q_PROPERTY(int OutputType READ getOutputType WRITE setOutputType)

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
  ITKVectorRescaleIntensityImage();

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

  /**
  * @brief Checks 'value' can be casted to OutputPixelType.
  */
  template <typename OutputPixelType> void CheckEntryBounds(double value, QString name);

private:
  ITKVectorRescaleIntensityImage(const ITKVectorRescaleIntensityImage&) = delete; // Copy Constructor Not Implemented
  ITKVectorRescaleIntensityImage(ITKVectorRescaleIntensityImage&&) = delete;      // Move Constructor Not Implemented
  ITKVectorRescaleIntensityImage& operator=(const ITKVectorRescaleIntensityImage&) = delete; // Copy Assignment Not Implemented
  ITKVectorRescaleIntensityImage& operator=(ITKVectorRescaleIntensityImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

