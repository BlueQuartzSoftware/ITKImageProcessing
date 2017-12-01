// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKApproximateSignedDistanceMapImage_h_
#define _ITKApproximateSignedDistanceMapImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkApproximateSignedDistanceMapImageFilter.h>

/**
 * @brief The ITKApproximateSignedDistanceMapImage class. See [Filter documentation](@ref ITKApproximateSignedDistanceMapImage) for details.
 */
class ITKApproximateSignedDistanceMapImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKApproximateSignedDistanceMapImage)
  SIMPL_STATIC_NEW_MACRO(ITKApproximateSignedDistanceMapImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKApproximateSignedDistanceMapImage, AbstractFilter)

  virtual ~ITKApproximateSignedDistanceMapImage();

  SIMPL_FILTER_PARAMETER(double, InsideValue)
  Q_PROPERTY(double InsideValue READ getInsideValue WRITE setInsideValue)

  SIMPL_FILTER_PARAMETER(double, OutsideValue)
  Q_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)

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
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKApproximateSignedDistanceMapImage();

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
  ITKApproximateSignedDistanceMapImage(const ITKApproximateSignedDistanceMapImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKApproximateSignedDistanceMapImage&);                       // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKApproximateSignedDistanceMapImage_H_ */
