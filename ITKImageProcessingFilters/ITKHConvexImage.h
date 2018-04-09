// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKHConvexImage_h_
#define _ITKHConvexImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkHConvexImageFilter.h>

/**
 * @brief The ITKHConvexImage class. See [Filter documentation](@ref ITKHConvexImage) for details.
 */
class ITKHConvexImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKHConvexImage SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(double Height READ getHeight WRITE setHeight)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

public:
  SIMPL_SHARED_POINTERS(ITKHConvexImage)
  SIMPL_STATIC_NEW_MACRO(ITKHConvexImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKHConvexImage, AbstractFilter)

  virtual ~ITKHConvexImage();

  SIMPL_FILTER_PARAMETER(double, Height)
  Q_PROPERTY(double Height READ getHeight WRITE setHeight)

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

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
  ITKHConvexImage();

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
  ITKHConvexImage(const ITKHConvexImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKHConvexImage&) = delete;  // Move assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKHConvexImage_H_ */
