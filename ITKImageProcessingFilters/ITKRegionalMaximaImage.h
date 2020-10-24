// File automatically generated

/*
 * Your License or Copyright can go here
 */

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#pragma clang diagnostic ignored "-Wself-assign-field"
#endif

#include <memory>

#include <itkRegionalMaximaImageFilter.h>

#include "SIMPLib/SIMPLib.h"

// Auto includes
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"
#include "ITKImageProcessingBase.h"

/**
 * @brief The ITKRegionalMaximaImage class. See [Filter documentation](@ref ITKRegionalMaximaImage) for details.
 */
class ITKImageProcessing_EXPORT ITKRegionalMaximaImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKRegionalMaximaImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKRegionalMaximaImage)
  PYB11_FILTER_NEW_MACRO(ITKRegionalMaximaImage)
  PYB11_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)
  PYB11_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(bool FlatIsMaxima READ getFlatIsMaxima WRITE setFlatIsMaxima)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKRegionalMaximaImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKRegionalMaximaImage> New();

  /**
   * @brief Returns the name of the class for ITKRegionalMaximaImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKRegionalMaximaImage
   */
  static QString ClassName();

  ~ITKRegionalMaximaImage() override;

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
   * @brief Setter property for FlatIsMaxima
   */
  void setFlatIsMaxima(bool value);
  /**
   * @brief Getter property for FlatIsMaxima
   * @return Value of FlatIsMaxima
   */
  bool getFlatIsMaxima() const;
  Q_PROPERTY(bool FlatIsMaxima READ getFlatIsMaxima WRITE setFlatIsMaxima)

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
  ITKRegionalMaximaImage();

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
  ITKRegionalMaximaImage(const ITKRegionalMaximaImage&) = delete;            // Copy Constructor Not Implemented
  ITKRegionalMaximaImage(ITKRegionalMaximaImage&&) = delete;                 // Move Constructor Not Implemented
  ITKRegionalMaximaImage& operator=(const ITKRegionalMaximaImage&) = delete; // Copy Assignment Not Implemented
  ITKRegionalMaximaImage& operator=(ITKRegionalMaximaImage&&) = delete;      // Move Assignment Not Implemented

private:
  double m_BackgroundValue = {};
  double m_ForegroundValue = {};
  bool m_FullyConnected = {};
  bool m_FlatIsMaxima = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
