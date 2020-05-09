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
#include <itkHConvexImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKHConvexImage class. See [Filter documentation](@ref ITKHConvexImage) for details.
 */
class ITKImageProcessing_EXPORT ITKHConvexImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKHConvexImage SUPERCLASS AbstractFilter)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKHConvexImage)
  PYB11_FILTER_NEW_MACRO(ITKHConvexImage)
  PYB11_PROPERTY(double Height READ getHeight WRITE setHeight)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKHConvexImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKHConvexImage> New();

  /**
   * @brief Returns the name of the class for ITKHConvexImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKHConvexImage
   */
  static QString ClassName();

  ~ITKHConvexImage() override;

  /**
   * @brief Setter property for Height
   */
  void setHeight(double value);
  /**
   * @brief Getter property for Height
   * @return Value of Height
   */
  double getHeight() const;
  Q_PROPERTY(double Height READ getHeight WRITE setHeight)

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
  ITKHConvexImage();

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
  ITKHConvexImage(const ITKHConvexImage&) = delete;            // Copy Constructor Not Implemented
  ITKHConvexImage(ITKHConvexImage&&) = delete;                 // Move Constructor Not Implemented
  ITKHConvexImage& operator=(const ITKHConvexImage&) = delete; // Copy Assignment Not Implemented
  ITKHConvexImage& operator=(ITKHConvexImage&&) = delete;      // Move Assignment Not Implemented

private:
  double m_Height = {};
  bool m_FullyConnected = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
