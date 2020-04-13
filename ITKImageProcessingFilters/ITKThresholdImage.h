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
#include <itkThresholdImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKThresholdImage class. See [Filter documentation](@ref ITKThresholdImage) for details.
 */
class ITKImageProcessing_EXPORT ITKThresholdImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKThresholdImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKThresholdImage)
  PYB11_FILTER_NEW_MACRO(ITKThresholdImage)
  PYB11_PROPERTY(double Lower READ getLower WRITE setLower)
  PYB11_PROPERTY(double Upper READ getUpper WRITE setUpper)
  PYB11_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKThresholdImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKThresholdImage> New();

    /**
    * @brief Returns the name of the class for ITKThresholdImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKThresholdImage
    */
    static QString ClassName();


  ~ITKThresholdImage() override;

    /**
    * @brief Setter property for Lower
    */
    void setLower(double value); 
    /**
    * @brief Getter property for Lower
    * @return Value of Lower
    */
    double getLower() const;
  Q_PROPERTY(double Lower READ getLower WRITE setLower)

    /**
    * @brief Setter property for Upper
    */
    void setUpper(double value); 
    /**
    * @brief Getter property for Upper
    * @return Value of Upper
    */
    double getUpper() const;
  Q_PROPERTY(double Upper READ getUpper WRITE setUpper)

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
  ITKThresholdImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheck() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheckImpl();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

public:
  ITKThresholdImage(const ITKThresholdImage&) = delete;    // Copy Constructor Not Implemented
  ITKThresholdImage(ITKThresholdImage&&) = delete;         // Move Constructor Not Implemented
  ITKThresholdImage& operator=(const ITKThresholdImage&) = delete; // Copy Assignment Not Implemented
  ITKThresholdImage& operator=(ITKThresholdImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Lower = {};
    double m_Upper = {};
    double m_OutsideValue = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

