// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKZeroCrossingImage_h_
#define _ITKZeroCrossingImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkZeroCrossingImageFilter.h>

/**
 * @brief The ITKZeroCrossingImage class. See [Filter documentation](@ref ITKZeroCrossingImage) for details.
 */
class ITKZeroCrossingImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKZeroCrossingImage)
  SIMPL_STATIC_NEW_MACRO(ITKZeroCrossingImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKZeroCrossingImage, AbstractFilter)

  virtual ~ITKZeroCrossingImage();

  SIMPL_FILTER_PARAMETER(int, ForegroundValue)
  Q_PROPERTY(int ForegroundValue READ getForegroundValue WRITE setForegroundValue)

  SIMPL_FILTER_PARAMETER(int, BackgroundValue)
  Q_PROPERTY(int BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getSubGroupName() const override;

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
  ITKZeroCrossingImage();

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
  ITKZeroCrossingImage(const ITKZeroCrossingImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKZeroCrossingImage&);       // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKZeroCrossingImage_H_ */
