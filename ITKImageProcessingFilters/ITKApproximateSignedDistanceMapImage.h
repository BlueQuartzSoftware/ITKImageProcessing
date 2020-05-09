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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkApproximateSignedDistanceMapImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKApproximateSignedDistanceMapImage class. See [Filter documentation](@ref ITKApproximateSignedDistanceMapImage) for details.
 */
class ITKImageProcessing_EXPORT ITKApproximateSignedDistanceMapImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKApproximateSignedDistanceMapImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKApproximateSignedDistanceMapImage)
  PYB11_FILTER_NEW_MACRO(ITKApproximateSignedDistanceMapImage)
  PYB11_PROPERTY(double InsideValue READ getInsideValue WRITE setInsideValue)
  PYB11_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKApproximateSignedDistanceMapImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKApproximateSignedDistanceMapImage> New();

  /**
   * @brief Returns the name of the class for ITKApproximateSignedDistanceMapImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKApproximateSignedDistanceMapImage
   */
  static QString ClassName();

  ~ITKApproximateSignedDistanceMapImage() override;

  /**
   * @brief Setter property for InsideValue
   */
  void setInsideValue(double value);
  /**
   * @brief Getter property for InsideValue
   * @return Value of InsideValue
   */
  double getInsideValue() const;
  Q_PROPERTY(double InsideValue READ getInsideValue WRITE setInsideValue)

  /**
   * @brief Setter property for OutsideValue
   */
  void setOutsideValue(double value);
  /**
   * @brief Getter property for OutsideValue
   * @return Value of OutsideValue
   */
  double getOutsideValue() const;
  Q_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)

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
  ITKApproximateSignedDistanceMapImage();

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
  ITKApproximateSignedDistanceMapImage(const ITKApproximateSignedDistanceMapImage&) = delete;            // Copy Constructor Not Implemented
  ITKApproximateSignedDistanceMapImage(ITKApproximateSignedDistanceMapImage&&) = delete;                 // Move Constructor Not Implemented
  ITKApproximateSignedDistanceMapImage& operator=(const ITKApproximateSignedDistanceMapImage&) = delete; // Copy Assignment Not Implemented
  ITKApproximateSignedDistanceMapImage& operator=(ITKApproximateSignedDistanceMapImage&&) = delete;      // Move Assignment Not Implemented

private:
  double m_InsideValue = {};
  double m_OutsideValue = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
