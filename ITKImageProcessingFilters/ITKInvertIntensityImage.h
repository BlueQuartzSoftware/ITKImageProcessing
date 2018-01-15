// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKInvertIntensityImage_h_
#define _ITKInvertIntensityImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkInvertIntensityImageFilter.h>

/**
 * @brief The ITKInvertIntensityImage class. See [Filter documentation](@ref ITKInvertIntensityImage) for details.
 */
class ITKInvertIntensityImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKInvertIntensityImage)
  SIMPL_STATIC_NEW_MACRO(ITKInvertIntensityImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKInvertIntensityImage, AbstractFilter)

  virtual ~ITKInvertIntensityImage();

  SIMPL_FILTER_PARAMETER(double, Maximum)
  Q_PROPERTY(double Maximum READ getMaximum WRITE setMaximum)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getSubGroupName() override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  virtual const QUuid getUuid() override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

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
  ITKInvertIntensityImage(const ITKInvertIntensityImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKInvertIntensityImage&);          // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKInvertIntensityImage_H_ */
