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
#include <itkBinaryThresholdImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBinaryThresholdImage class. See [Filter documentation](@ref ITKBinaryThresholdImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBinaryThresholdImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKBinaryThresholdImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKBinaryThresholdImage)
  PYB11_FILTER_NEW_MACRO(ITKBinaryThresholdImage)
  PYB11_PROPERTY(double LowerThreshold READ getLowerThreshold WRITE setLowerThreshold)
  PYB11_PROPERTY(double UpperThreshold READ getUpperThreshold WRITE setUpperThreshold)
  PYB11_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)
  PYB11_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKBinaryThresholdImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKBinaryThresholdImage> New();

    /**
    * @brief Returns the name of the class for ITKBinaryThresholdImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKBinaryThresholdImage
    */
    static QString ClassName();


  ~ITKBinaryThresholdImage() override;

    /**
    * @brief Setter property for LowerThreshold
    */
    void setLowerThreshold(double value); 
    /**
    * @brief Getter property for LowerThreshold
    * @return Value of LowerThreshold
    */
    double getLowerThreshold() const;
  Q_PROPERTY(double LowerThreshold READ getLowerThreshold WRITE setLowerThreshold)

    /**
    * @brief Setter property for UpperThreshold
    */
    void setUpperThreshold(double value); 
    /**
    * @brief Getter property for UpperThreshold
    * @return Value of UpperThreshold
    */
    double getUpperThreshold() const;
  Q_PROPERTY(double UpperThreshold READ getUpperThreshold WRITE setUpperThreshold)

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
  ITKBinaryThresholdImage();

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
  ITKBinaryThresholdImage(const ITKBinaryThresholdImage&) = delete;    // Copy Constructor Not Implemented
  ITKBinaryThresholdImage(ITKBinaryThresholdImage&&) = delete;         // Move Constructor Not Implemented
  ITKBinaryThresholdImage& operator=(const ITKBinaryThresholdImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryThresholdImage& operator=(ITKBinaryThresholdImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_LowerThreshold = {};
    double m_UpperThreshold = {};
    int m_InsideValue = {};
    int m_OutsideValue = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

