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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkDoubleThresholdImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKDoubleThresholdImage class. See [Filter documentation](@ref ITKDoubleThresholdImage) for details.
 */
class ITKImageProcessing_EXPORT ITKDoubleThresholdImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKDoubleThresholdImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double Threshold1 READ getThreshold1 WRITE setThreshold1)
  PYB11_PROPERTY(double Threshold2 READ getThreshold2 WRITE setThreshold2)
  PYB11_PROPERTY(double Threshold3 READ getThreshold3 WRITE setThreshold3)
  PYB11_PROPERTY(double Threshold4 READ getThreshold4 WRITE setThreshold4)
  PYB11_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)
  PYB11_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

public:
  SIMPL_SHARED_POINTERS(ITKDoubleThresholdImage)
  SIMPL_FILTER_NEW_MACRO(ITKDoubleThresholdImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKDoubleThresholdImage, AbstractFilter)

  ~ITKDoubleThresholdImage() override;

  SIMPL_FILTER_PARAMETER(double, Threshold1)
  Q_PROPERTY(double Threshold1 READ getThreshold1 WRITE setThreshold1)

  SIMPL_FILTER_PARAMETER(double, Threshold2)
  Q_PROPERTY(double Threshold2 READ getThreshold2 WRITE setThreshold2)

  SIMPL_FILTER_PARAMETER(double, Threshold3)
  Q_PROPERTY(double Threshold3 READ getThreshold3 WRITE setThreshold3)

  SIMPL_FILTER_PARAMETER(double, Threshold4)
  Q_PROPERTY(double Threshold4 READ getThreshold4 WRITE setThreshold4)

  SIMPL_FILTER_PARAMETER(int, InsideValue)
  Q_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)

  SIMPL_FILTER_PARAMETER(int, OutsideValue)
  Q_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)

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
  ITKDoubleThresholdImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheck();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

public:
  ITKDoubleThresholdImage(const ITKDoubleThresholdImage&) = delete;    // Copy Constructor Not Implemented
  ITKDoubleThresholdImage(ITKDoubleThresholdImage&&) = delete;         // Move Constructor Not Implemented
  ITKDoubleThresholdImage& operator=(const ITKDoubleThresholdImage&) = delete; // Copy Assignment Not Implemented
  ITKDoubleThresholdImage& operator=(ITKDoubleThresholdImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

