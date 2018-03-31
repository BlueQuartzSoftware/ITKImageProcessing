// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKRescaleIntensityImage_h_
#define _ITKRescaleIntensityImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkRescaleIntensityImageFilter.h>


/**
 * @brief The ITKRescaleIntensityImage class. See [Filter documentation](@ref ITKRescaleIntensityImage) for details.
 */
class ITKRescaleIntensityImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKRescaleIntensityImage)
  SIMPL_FILTER_NEW_MACRO(ITKRescaleIntensityImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKRescaleIntensityImage, AbstractFilter)

  virtual ~ITKRescaleIntensityImage();

  SIMPL_FILTER_PARAMETER(double, OutputMinimum)
  Q_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)

  SIMPL_FILTER_PARAMETER(double, OutputMaximum)
  Q_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)

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
  ITKRescaleIntensityImage();

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
  ITKRescaleIntensityImage(const ITKRescaleIntensityImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKRescaleIntensityImage&) = delete; // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKRescaleIntensityImage_H_ */
