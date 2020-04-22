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
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkThresholdMaximumConnectedComponentsImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKThresholdMaximumConnectedComponentsImage class. See [Filter documentation](@ref ITKThresholdMaximumConnectedComponentsImage) for details.
 */
class ITKImageProcessing_EXPORT ITKThresholdMaximumConnectedComponentsImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKThresholdMaximumConnectedComponentsImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKThresholdMaximumConnectedComponentsImage)
  PYB11_FILTER_NEW_MACRO(ITKThresholdMaximumConnectedComponentsImage)
  PYB11_PROPERTY(double MinimumObjectSizeInPixels READ getMinimumObjectSizeInPixels WRITE setMinimumObjectSizeInPixels)
  PYB11_PROPERTY(double UpperBoundary READ getUpperBoundary WRITE setUpperBoundary)
  PYB11_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)
  PYB11_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKThresholdMaximumConnectedComponentsImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKThresholdMaximumConnectedComponentsImage> New();

    /**
    * @brief Returns the name of the class for ITKThresholdMaximumConnectedComponentsImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKThresholdMaximumConnectedComponentsImage
    */
    static QString ClassName();


  ~ITKThresholdMaximumConnectedComponentsImage() override;

    /**
    * @brief Setter property for MinimumObjectSizeInPixels
    */
    void setMinimumObjectSizeInPixels(double value); 
    /**
    * @brief Getter property for MinimumObjectSizeInPixels
    * @return Value of MinimumObjectSizeInPixels
    */
    double getMinimumObjectSizeInPixels() const;
  Q_PROPERTY(double MinimumObjectSizeInPixels READ getMinimumObjectSizeInPixels WRITE setMinimumObjectSizeInPixels)

    /**
    * @brief Setter property for UpperBoundary
    */
    void setUpperBoundary(double value); 
    /**
    * @brief Getter property for UpperBoundary
    * @return Value of UpperBoundary
    */
    double getUpperBoundary() const;
  Q_PROPERTY(double UpperBoundary READ getUpperBoundary WRITE setUpperBoundary)

    /**
    * @brief Setter property for InsideValue
    */
    void setInsideValue(int value); 
    /**
    * @brief Getter property for InsideValue
    * @return Value of InsideValue
    */
    int getInsideValue() const;
  Q_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)

    /**
    * @brief Setter property for OutsideValue
    */
    void setOutsideValue(int value); 
    /**
    * @brief Getter property for OutsideValue
    * @return Value of OutsideValue
    */
    int getOutsideValue() const;
  Q_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)


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
  ITKThresholdMaximumConnectedComponentsImage();

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
  ITKThresholdMaximumConnectedComponentsImage(const ITKThresholdMaximumConnectedComponentsImage&) = delete;    // Copy Constructor Not Implemented
  ITKThresholdMaximumConnectedComponentsImage(ITKThresholdMaximumConnectedComponentsImage&&) = delete;         // Move Constructor Not Implemented
  ITKThresholdMaximumConnectedComponentsImage& operator=(const ITKThresholdMaximumConnectedComponentsImage&) = delete; // Copy Assignment Not Implemented
  ITKThresholdMaximumConnectedComponentsImage& operator=(ITKThresholdMaximumConnectedComponentsImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_MinimumObjectSizeInPixels = {};
    double m_UpperBoundary = {};
    int m_InsideValue = {};
    int m_OutsideValue = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

