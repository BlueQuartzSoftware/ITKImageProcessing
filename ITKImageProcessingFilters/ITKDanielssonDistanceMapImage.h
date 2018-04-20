// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKDanielssonDistanceMapImage_h_
#define _ITKDanielssonDistanceMapImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkDanielssonDistanceMapImageFilter.h>


/**
 * @brief The ITKDanielssonDistanceMapImage class. See [Filter documentation](@ref ITKDanielssonDistanceMapImage) for details.
 */
class ITKDanielssonDistanceMapImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKDanielssonDistanceMapImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(bool InputIsBinary READ getInputIsBinary WRITE setInputIsBinary)
  PYB11_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)
  PYB11_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)

public:
  SIMPL_SHARED_POINTERS(ITKDanielssonDistanceMapImage)
  SIMPL_FILTER_NEW_MACRO(ITKDanielssonDistanceMapImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKDanielssonDistanceMapImage, AbstractFilter)

  ~ITKDanielssonDistanceMapImage() override;

  SIMPL_FILTER_PARAMETER(bool, InputIsBinary)
  Q_PROPERTY(bool InputIsBinary READ getInputIsBinary WRITE setInputIsBinary)

  SIMPL_FILTER_PARAMETER(bool, SquaredDistance)
  Q_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)

  SIMPL_FILTER_PARAMETER(bool, UseImageSpacing)
  Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)


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
  ITKDanielssonDistanceMapImage();

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
  ITKDanielssonDistanceMapImage(const ITKDanielssonDistanceMapImage&) = delete;    // Copy Constructor Not Implemented
  ITKDanielssonDistanceMapImage(ITKDanielssonDistanceMapImage&&) = delete;         // Move Constructor Not Implemented
  ITKDanielssonDistanceMapImage& operator=(const ITKDanielssonDistanceMapImage&) = delete; // Copy Assignment Not Implemented
  ITKDanielssonDistanceMapImage& operator=(ITKDanielssonDistanceMapImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKDanielssonDistanceMapImage_H_ */
