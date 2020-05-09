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
#include <itkBinaryContourImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBinaryContourImage class. See [Filter documentation](@ref ITKBinaryContourImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBinaryContourImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKBinaryContourImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKBinaryContourImage)
  PYB11_FILTER_NEW_MACRO(ITKBinaryContourImage)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)
  PYB11_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKBinaryContourImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKBinaryContourImage> New();

  /**
   * @brief Returns the name of the class for ITKBinaryContourImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKBinaryContourImage
   */
  static QString ClassName();

  ~ITKBinaryContourImage() override;

  /**
   * @brief Setter property for FullyConnected
   */
  void setFullyConnected(bool value);
  /**
   * @brief Getter property for FullyConnected
   * @return Value of FullyConnected
   */
  bool getFullyConnected() const;
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

  /**
   * @brief Setter property for BackgroundValue
   */
  void setBackgroundValue(double value);
  /**
   * @brief Getter property for BackgroundValue
   * @return Value of BackgroundValue
   */
  double getBackgroundValue() const;
  Q_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)

  /**
   * @brief Setter property for ForegroundValue
   */
  void setForegroundValue(double value);
  /**
   * @brief Getter property for ForegroundValue
   * @return Value of ForegroundValue
   */
  double getForegroundValue() const;
  Q_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)

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
  ITKBinaryContourImage();

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
  ITKBinaryContourImage(const ITKBinaryContourImage&) = delete;            // Copy Constructor Not Implemented
  ITKBinaryContourImage(ITKBinaryContourImage&&) = delete;                 // Move Constructor Not Implemented
  ITKBinaryContourImage& operator=(const ITKBinaryContourImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryContourImage& operator=(ITKBinaryContourImage&&) = delete;      // Move Assignment Not Implemented

private:
  bool m_FullyConnected = {};
  double m_BackgroundValue = {};
  double m_ForegroundValue = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
