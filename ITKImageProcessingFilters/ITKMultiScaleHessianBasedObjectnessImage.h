// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMultiScaleHessianBasedObjectnessImage_h_
#define _ITKMultiScaleHessianBasedObjectnessImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>

/**
 * @brief The ITKMultiScaleHessianBasedObjectnessImage class. See [Filter documentation](@ref ITKMultiScaleHessianBasedObjectnessImage) for details.
 */
class ITKMultiScaleHessianBasedObjectnessImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKMultiScaleHessianBasedObjectnessImage)
  SIMPL_STATIC_NEW_MACRO(ITKMultiScaleHessianBasedObjectnessImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMultiScaleHessianBasedObjectnessImage, AbstractFilter)

  virtual ~ITKMultiScaleHessianBasedObjectnessImage();

  SIMPL_FILTER_PARAMETER(int, ObjectDimension)
  Q_PROPERTY(int ObjectDimension READ getObjectDimension WRITE setObjectDimension)

  SIMPL_FILTER_PARAMETER(double, Alpha)
  Q_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)

  SIMPL_FILTER_PARAMETER(double, Beta)
  Q_PROPERTY(double Beta READ getBeta WRITE setBeta)

  SIMPL_FILTER_PARAMETER(double, Gamma)
  Q_PROPERTY(double Gamma READ getGamma WRITE setGamma)

  SIMPL_FILTER_PARAMETER(bool, ScaleObjectnessMeasure)
  Q_PROPERTY(bool ScaleObjectnessMeasure READ getScaleObjectnessMeasure WRITE setScaleObjectnessMeasure)

  SIMPL_FILTER_PARAMETER(bool, BrightObject)
  Q_PROPERTY(bool BrightObject READ getBrightObject WRITE setBrightObject)

  SIMPL_FILTER_PARAMETER(double, SigmaMinimum)
  Q_PROPERTY(double SigmaMinimum READ getSigmaMinimum WRITE setSigmaMinimum)

  SIMPL_FILTER_PARAMETER(double, SigmaMaximum)
  Q_PROPERTY(double SigmaMaximum READ getSigmaMaximum WRITE setSigmaMaximum)

  SIMPL_FILTER_PARAMETER(double, NumberOfSigmaSteps)
  Q_PROPERTY(double NumberOfSigmaSteps READ getNumberOfSigmaSteps WRITE setNumberOfSigmaSteps)

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
  ITKMultiScaleHessianBasedObjectnessImage();

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
  ITKMultiScaleHessianBasedObjectnessImage(const ITKMultiScaleHessianBasedObjectnessImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKMultiScaleHessianBasedObjectnessImage&);                           // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKMultiScaleHessianBasedObjectnessImage_H_ */
