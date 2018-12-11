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

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKMultiScaleHessianBasedObjectnessImage class. See [Filter documentation](@ref ITKMultiScaleHessianBasedObjectnessImage) for details.
 */
class ITKImageProcessing_EXPORT ITKMultiScaleHessianBasedObjectnessImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKMultiScaleHessianBasedObjectnessImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(int ObjectDimension READ getObjectDimension WRITE setObjectDimension)
  PYB11_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)
  PYB11_PROPERTY(double Beta READ getBeta WRITE setBeta)
  PYB11_PROPERTY(double Gamma READ getGamma WRITE setGamma)
  PYB11_PROPERTY(bool ScaleObjectnessMeasure READ getScaleObjectnessMeasure WRITE setScaleObjectnessMeasure)
  PYB11_PROPERTY(bool BrightObject READ getBrightObject WRITE setBrightObject)
  PYB11_PROPERTY(double SigmaMinimum READ getSigmaMinimum WRITE setSigmaMinimum)
  PYB11_PROPERTY(double SigmaMaximum READ getSigmaMaximum WRITE setSigmaMaximum)
  PYB11_PROPERTY(double NumberOfSigmaSteps READ getNumberOfSigmaSteps WRITE setNumberOfSigmaSteps)

public:
  SIMPL_SHARED_POINTERS(ITKMultiScaleHessianBasedObjectnessImage)
  SIMPL_FILTER_NEW_MACRO(ITKMultiScaleHessianBasedObjectnessImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMultiScaleHessianBasedObjectnessImage, AbstractFilter)

  ~ITKMultiScaleHessianBasedObjectnessImage() override;

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
  ITKMultiScaleHessianBasedObjectnessImage();

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
  ITKMultiScaleHessianBasedObjectnessImage(const ITKMultiScaleHessianBasedObjectnessImage&) = delete; // Copy Constructor Not Implemented
  ITKMultiScaleHessianBasedObjectnessImage(ITKMultiScaleHessianBasedObjectnessImage&&) = delete;      // Move Constructor Not Implemented
  ITKMultiScaleHessianBasedObjectnessImage& operator=(const ITKMultiScaleHessianBasedObjectnessImage&) = delete; // Copy Assignment Not Implemented
  ITKMultiScaleHessianBasedObjectnessImage& operator=(ITKMultiScaleHessianBasedObjectnessImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

