// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryMinMaxCurvatureFlowImage_h_
#define _ITKBinaryMinMaxCurvatureFlowImage_h_

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
#include <itkBinaryMinMaxCurvatureFlowImageFilter.h>


/**
 * @brief The ITKBinaryMinMaxCurvatureFlowImage class. See [Filter documentation](@ref ITKBinaryMinMaxCurvatureFlowImage) for details.
 */
class ITKBinaryMinMaxCurvatureFlowImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKBinaryMinMaxCurvatureFlowImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)
  PYB11_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)
  PYB11_PROPERTY(int StencilRadius READ getStencilRadius WRITE setStencilRadius)
  PYB11_PROPERTY(double Threshold READ getThreshold WRITE setThreshold)

public:
  SIMPL_SHARED_POINTERS(ITKBinaryMinMaxCurvatureFlowImage)
  SIMPL_FILTER_NEW_MACRO(ITKBinaryMinMaxCurvatureFlowImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBinaryMinMaxCurvatureFlowImage, AbstractFilter)

  ~ITKBinaryMinMaxCurvatureFlowImage() override;

  SIMPL_FILTER_PARAMETER(double, TimeStep)
  Q_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)

  SIMPL_FILTER_PARAMETER(double, NumberOfIterations)
  Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)

  SIMPL_FILTER_PARAMETER(int, StencilRadius)
  Q_PROPERTY(int StencilRadius READ getStencilRadius WRITE setStencilRadius)

  SIMPL_FILTER_PARAMETER(double, Threshold)
  Q_PROPERTY(double Threshold READ getThreshold WRITE setThreshold)


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
  ITKBinaryMinMaxCurvatureFlowImage();

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
  ITKBinaryMinMaxCurvatureFlowImage(const ITKBinaryMinMaxCurvatureFlowImage&) = delete;    // Copy Constructor Not Implemented
  ITKBinaryMinMaxCurvatureFlowImage(ITKBinaryMinMaxCurvatureFlowImage&&) = delete;         // Move Constructor Not Implemented
  ITKBinaryMinMaxCurvatureFlowImage& operator=(const ITKBinaryMinMaxCurvatureFlowImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryMinMaxCurvatureFlowImage& operator=(ITKBinaryMinMaxCurvatureFlowImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKBinaryMinMaxCurvatureFlowImage_H_ */
