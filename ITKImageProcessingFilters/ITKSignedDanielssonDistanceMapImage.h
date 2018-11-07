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
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkSignedDanielssonDistanceMapImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKSignedDanielssonDistanceMapImage class. See [Filter documentation](@ref ITKSignedDanielssonDistanceMapImage) for details.
 */
class ITKImageProcessing_EXPORT ITKSignedDanielssonDistanceMapImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKSignedDanielssonDistanceMapImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(bool InsideIsPositive READ getInsideIsPositive WRITE setInsideIsPositive)
  PYB11_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)
  PYB11_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)

public:
  SIMPL_SHARED_POINTERS(ITKSignedDanielssonDistanceMapImage)
  SIMPL_FILTER_NEW_MACRO(ITKSignedDanielssonDistanceMapImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKSignedDanielssonDistanceMapImage, AbstractFilter)

  ~ITKSignedDanielssonDistanceMapImage() override;

  SIMPL_FILTER_PARAMETER(bool, InsideIsPositive)
  Q_PROPERTY(bool InsideIsPositive READ getInsideIsPositive WRITE setInsideIsPositive)

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
  ITKSignedDanielssonDistanceMapImage();

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

public:
  ITKSignedDanielssonDistanceMapImage(const ITKSignedDanielssonDistanceMapImage&) = delete;    // Copy Constructor Not Implemented
  ITKSignedDanielssonDistanceMapImage(ITKSignedDanielssonDistanceMapImage&&) = delete;         // Move Constructor Not Implemented
  ITKSignedDanielssonDistanceMapImage& operator=(const ITKSignedDanielssonDistanceMapImage&) = delete; // Copy Assignment Not Implemented
  ITKSignedDanielssonDistanceMapImage& operator=(ITKSignedDanielssonDistanceMapImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

