// File automatically generated

/*
 * Your License or Copyright can go here
 */

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include <memory>

#include "ITKImageProcessingBase.h"

#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKGradientMagnitudeRecursiveGaussianImage class. See [Filter documentation](@ref ITKGradientMagnitudeRecursiveGaussianImage) for details.
 */
class ITKImageProcessing_EXPORT ITKGradientMagnitudeRecursiveGaussianImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKGradientMagnitudeRecursiveGaussianImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKGradientMagnitudeRecursiveGaussianImage)
  PYB11_FILTER_NEW_MACRO(ITKGradientMagnitudeRecursiveGaussianImage)
  PYB11_PROPERTY(double Sigma READ getSigma WRITE setSigma)
  PYB11_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKGradientMagnitudeRecursiveGaussianImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKGradientMagnitudeRecursiveGaussianImage> New();

  /**
   * @brief Returns the name of the class for ITKGradientMagnitudeRecursiveGaussianImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKGradientMagnitudeRecursiveGaussianImage
   */
  static QString ClassName();

  ~ITKGradientMagnitudeRecursiveGaussianImage() override;

  /**
   * @brief Setter property for Sigma
   */
  void setSigma(double value);
  /**
   * @brief Getter property for Sigma
   * @return Value of Sigma
   */
  double getSigma() const;
  Q_PROPERTY(double Sigma READ getSigma WRITE setSigma)

  /**
   * @brief Setter property for NormalizeAcrossScale
   */
  void setNormalizeAcrossScale(bool value);
  /**
   * @brief Getter property for NormalizeAcrossScale
   * @return Value of NormalizeAcrossScale
   */
  bool getNormalizeAcrossScale() const;
  Q_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  QUuid getUuid() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKGradientMagnitudeRecursiveGaussianImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheck() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension>
  void dataCheckImpl();

  /**
   * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
   */
  void filterInternal() override;

  /**
   * @brief Applies the filter
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension>
  void filter();

public:
  ITKGradientMagnitudeRecursiveGaussianImage(const ITKGradientMagnitudeRecursiveGaussianImage&) = delete;            // Copy Constructor Not Implemented
  ITKGradientMagnitudeRecursiveGaussianImage(ITKGradientMagnitudeRecursiveGaussianImage&&) = delete;                 // Move Constructor Not Implemented
  ITKGradientMagnitudeRecursiveGaussianImage& operator=(const ITKGradientMagnitudeRecursiveGaussianImage&) = delete; // Copy Assignment Not Implemented
  ITKGradientMagnitudeRecursiveGaussianImage& operator=(ITKGradientMagnitudeRecursiveGaussianImage&&) = delete;      // Move Assignment Not Implemented

private:
  double m_Sigma = {};
  bool m_NormalizeAcrossScale = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
