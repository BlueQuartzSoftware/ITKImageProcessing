// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGradientAnisotropicDiffusionImage_h_
#define _ITKGradientAnisotropicDiffusionImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <algorithm>
#include <itkGradientAnisotropicDiffusionImageFilter.h>

/**
 * @brief The ITKGradientAnisotropicDiffusionImage class. See [Filter documentation](@ref ITKGradientAnisotropicDiffusionImage) for details.
 */
class ITKGradientAnisotropicDiffusionImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKGradientAnisotropicDiffusionImage)
  SIMPL_STATIC_NEW_MACRO(ITKGradientAnisotropicDiffusionImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKGradientAnisotropicDiffusionImage, AbstractFilter)

  virtual ~ITKGradientAnisotropicDiffusionImage();

  SIMPL_FILTER_PARAMETER(double, TimeStep)
  Q_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)

  SIMPL_FILTER_PARAMETER(double, ConductanceParameter)
  Q_PROPERTY(double ConductanceParameter READ getConductanceParameter WRITE setConductanceParameter)

  SIMPL_FILTER_PARAMETER(double, ConductanceScalingUpdateInterval)
  Q_PROPERTY(double ConductanceScalingUpdateInterval READ getConductanceScalingUpdateInterval WRITE setConductanceScalingUpdateInterval)

  SIMPL_FILTER_PARAMETER(double, NumberOfIterations)
  Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  virtual const QUuid getUuid() override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKGradientAnisotropicDiffusionImage();

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
  ITKGradientAnisotropicDiffusionImage(const ITKGradientAnisotropicDiffusionImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKGradientAnisotropicDiffusionImage&);                       // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKGradientAnisotropicDiffusionImage_H_ */
