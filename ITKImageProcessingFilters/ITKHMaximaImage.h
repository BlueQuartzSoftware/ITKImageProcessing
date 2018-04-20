// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKHMaximaImage_h_
#define _ITKHMaximaImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkHMaximaImageFilter.h>


/**
 * @brief The ITKHMaximaImage class. See [Filter documentation](@ref ITKHMaximaImage) for details.
 */
class ITKHMaximaImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKHMaximaImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double Height READ getHeight WRITE setHeight)

public:
  SIMPL_SHARED_POINTERS(ITKHMaximaImage)
  SIMPL_FILTER_NEW_MACRO(ITKHMaximaImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKHMaximaImage, AbstractFilter)

  ~ITKHMaximaImage() override;

  SIMPL_FILTER_PARAMETER(double, Height)
  Q_PROPERTY(double Height READ getHeight WRITE setHeight)


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
  ITKHMaximaImage();

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
  ITKHMaximaImage(const ITKHMaximaImage&) = delete;    // Copy Constructor Not Implemented
  ITKHMaximaImage(ITKHMaximaImage&&) = delete;         // Move Constructor Not Implemented
  ITKHMaximaImage& operator=(const ITKHMaximaImage&) = delete; // Copy Assignment Not Implemented
  ITKHMaximaImage& operator=(ITKHMaximaImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKHMaximaImage_H_ */
