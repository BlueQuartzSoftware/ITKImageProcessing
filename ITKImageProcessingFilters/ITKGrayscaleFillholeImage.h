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
#include <itkGrayscaleFillholeImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKGrayscaleFillholeImage class. See [Filter documentation](@ref ITKGrayscaleFillholeImage) for details.
 */
class ITKImageProcessing_EXPORT ITKGrayscaleFillholeImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKGrayscaleFillholeImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKGrayscaleFillholeImage)
  PYB11_FILTER_NEW_MACRO(ITKGrayscaleFillholeImage)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKGrayscaleFillholeImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKGrayscaleFillholeImage> New();

  /**
   * @brief Returns the name of the class for ITKGrayscaleFillholeImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKGrayscaleFillholeImage
   */
  static QString ClassName();

  ~ITKGrayscaleFillholeImage() override;

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
  ITKGrayscaleFillholeImage();

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
  ITKGrayscaleFillholeImage(const ITKGrayscaleFillholeImage&) = delete;            // Copy Constructor Not Implemented
  ITKGrayscaleFillholeImage(ITKGrayscaleFillholeImage&&) = delete;                 // Move Constructor Not Implemented
  ITKGrayscaleFillholeImage& operator=(const ITKGrayscaleFillholeImage&) = delete; // Copy Assignment Not Implemented
  ITKGrayscaleFillholeImage& operator=(ITKGrayscaleFillholeImage&&) = delete;      // Move Assignment Not Implemented

private:
  bool m_FullyConnected = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
