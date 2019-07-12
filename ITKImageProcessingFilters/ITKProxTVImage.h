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
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <itkProxTVImageFilter.h>


/**
 * @brief The ITKProxTVImage class. See [Filter documentation](@ref ITKProxTVImage) for details.
 */
class ITKProxTVImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKProxTVImage)
  SIMPL_FILTER_NEW_MACRO(ITKProxTVImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKProxTVImage, AbstractFilter)

  ~ITKProxTVImage() override;

  SIMPL_FILTER_PARAMETER(double, MaximumNumberOfIterations)
  Q_PROPERTY(double MaximumNumberOfIterations READ getMaximumNumberOfIterations WRITE setMaximumNumberOfIterations)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, Weights)
  Q_PROPERTY(FloatVec3Type Weights READ getWeights WRITE setWeights)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, Norms)
  Q_PROPERTY(FloatVec3Type Norms READ getNorms WRITE setNorms)


  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer
  newFilterInstance(bool copyFilterParameters) const override;

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
  void readFilterParameters(AbstractFilterParametersReader * reader, int index) override;

protected:
  ITKProxTVImage();

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

private:
  ITKProxTVImage(const ITKProxTVImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKProxTVImage&) = delete;   // Move assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
