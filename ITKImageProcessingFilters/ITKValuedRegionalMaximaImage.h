// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKValuedRegionalMaximaImage_h_
#define _ITKValuedRegionalMaximaImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkValuedRegionalMaximaImageFilter.h>


/**
 * @brief The ITKValuedRegionalMaximaImage class. See [Filter documentation](@ref ITKValuedRegionalMaximaImage) for details.
 */
class ITKValuedRegionalMaximaImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKValuedRegionalMaximaImage)
  SIMPL_FILTER_NEW_MACRO(ITKValuedRegionalMaximaImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKValuedRegionalMaximaImage, AbstractFilter)

  virtual ~ITKValuedRegionalMaximaImage();

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

  SIMPL_FILTER_PARAMETER(bool, Flat)
  Q_PROPERTY(bool Flat READ getFlat)


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
  ITKValuedRegionalMaximaImage();

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
  ITKValuedRegionalMaximaImage(const ITKValuedRegionalMaximaImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKValuedRegionalMaximaImage&) = delete;                  // Move assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKValuedRegionalMaximaImage_H_ */
