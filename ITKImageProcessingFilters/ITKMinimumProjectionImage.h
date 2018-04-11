// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMinimumProjectionImage_h_
#define _ITKMinimumProjectionImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkMinimumProjectionImageFilter.h>


/**
 * @brief The ITKMinimumProjectionImage class. See [Filter documentation](@ref ITKMinimumProjectionImage) for details.
 */
class ITKMinimumProjectionImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKMinimumProjectionImage)
  SIMPL_FILTER_NEW_MACRO(ITKMinimumProjectionImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMinimumProjectionImage, AbstractFilter)

  ~ITKMinimumProjectionImage() override;

  SIMPL_FILTER_PARAMETER(double, ProjectionDimension)
  Q_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)


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
  ITKMinimumProjectionImage();

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
  ITKMinimumProjectionImage(const ITKMinimumProjectionImage&) = delete;    // Copy Constructor Not Implemented
  ITKMinimumProjectionImage(ITKMinimumProjectionImage&&) = delete;         // Move Constructor Not Implemented
  ITKMinimumProjectionImage& operator=(const ITKMinimumProjectionImage&) = delete; // Copy Assignment Not Implemented
  ITKMinimumProjectionImage& operator=(ITKMinimumProjectionImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKMinimumProjectionImage_H_ */
