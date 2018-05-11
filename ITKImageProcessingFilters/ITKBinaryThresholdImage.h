// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryThresholdImage_h_
#define _ITKBinaryThresholdImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkBinaryThresholdImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBinaryThresholdImage class. See [Filter documentation](@ref ITKBinaryThresholdImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBinaryThresholdImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKBinaryThresholdImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double LowerThreshold READ getLowerThreshold WRITE setLowerThreshold)
  PYB11_PROPERTY(double UpperThreshold READ getUpperThreshold WRITE setUpperThreshold)
  PYB11_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)
  PYB11_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)

public:
  SIMPL_SHARED_POINTERS(ITKBinaryThresholdImage)
  SIMPL_FILTER_NEW_MACRO(ITKBinaryThresholdImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBinaryThresholdImage, AbstractFilter)

  ~ITKBinaryThresholdImage() override;

  SIMPL_FILTER_PARAMETER(double, LowerThreshold)
  Q_PROPERTY(double LowerThreshold READ getLowerThreshold WRITE setLowerThreshold)

  SIMPL_FILTER_PARAMETER(double, UpperThreshold)
  Q_PROPERTY(double UpperThreshold READ getUpperThreshold WRITE setUpperThreshold)

  SIMPL_FILTER_PARAMETER(int, InsideValue)
  Q_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)

  SIMPL_FILTER_PARAMETER(int, OutsideValue)
  Q_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)


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
  ITKBinaryThresholdImage();

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
  ITKBinaryThresholdImage(const ITKBinaryThresholdImage&) = delete;    // Copy Constructor Not Implemented
  ITKBinaryThresholdImage(ITKBinaryThresholdImage&&) = delete;         // Move Constructor Not Implemented
  ITKBinaryThresholdImage& operator=(const ITKBinaryThresholdImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryThresholdImage& operator=(ITKBinaryThresholdImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKBinaryThresholdImage_H_ */
