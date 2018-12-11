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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkShotNoiseImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKShotNoiseImage class. See [Filter documentation](@ref ITKShotNoiseImage) for details.
 */
class ITKImageProcessing_EXPORT ITKShotNoiseImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKShotNoiseImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double Scale READ getScale WRITE setScale)
  PYB11_PROPERTY(double Seed READ getSeed WRITE setSeed)

public:
  SIMPL_SHARED_POINTERS(ITKShotNoiseImage)
  SIMPL_FILTER_NEW_MACRO(ITKShotNoiseImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKShotNoiseImage, AbstractFilter)

  ~ITKShotNoiseImage() override;

  SIMPL_FILTER_PARAMETER(double, Scale)
  Q_PROPERTY(double Scale READ getScale WRITE setScale)

  SIMPL_FILTER_PARAMETER(double, Seed)
  Q_PROPERTY(double Seed READ getSeed WRITE setSeed)


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
  ITKShotNoiseImage();

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
  ITKShotNoiseImage(const ITKShotNoiseImage&) = delete;    // Copy Constructor Not Implemented
  ITKShotNoiseImage(ITKShotNoiseImage&&) = delete;         // Move Constructor Not Implemented
  ITKShotNoiseImage& operator=(const ITKShotNoiseImage&) = delete; // Copy Assignment Not Implemented
  ITKShotNoiseImage& operator=(ITKShotNoiseImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

