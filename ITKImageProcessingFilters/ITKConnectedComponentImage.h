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
#include <itkConnectedComponentImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKConnectedComponentImage class. See [Filter documentation](@ref ITKConnectedComponentImage) for details.
 */
class ITKImageProcessing_EXPORT ITKConnectedComponentImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKConnectedComponentImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(double ObjectCount READ getObjectCount WRITE setObjectCount)

public:
  SIMPL_SHARED_POINTERS(ITKConnectedComponentImage)
  SIMPL_FILTER_NEW_MACRO(ITKConnectedComponentImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKConnectedComponentImage, AbstractFilter)

  ~ITKConnectedComponentImage() override;

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

  SIMPL_FILTER_PARAMETER(double, ObjectCount)
  Q_PROPERTY(double ObjectCount READ getObjectCount)


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
  ITKConnectedComponentImage();

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
  ITKConnectedComponentImage(const ITKConnectedComponentImage&) = delete;    // Copy Constructor Not Implemented
  ITKConnectedComponentImage(ITKConnectedComponentImage&&) = delete;         // Move Constructor Not Implemented
  ITKConnectedComponentImage& operator=(const ITKConnectedComponentImage&) = delete; // Copy Assignment Not Implemented
  ITKConnectedComponentImage& operator=(ITKConnectedComponentImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

