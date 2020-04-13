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
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkDoubleThresholdImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKDoubleThresholdImage class. See [Filter documentation](@ref ITKDoubleThresholdImage) for details.
 */
class ITKImageProcessing_EXPORT ITKDoubleThresholdImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKDoubleThresholdImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKDoubleThresholdImage)
  PYB11_FILTER_NEW_MACRO(ITKDoubleThresholdImage)
  PYB11_PROPERTY(double Threshold1 READ getThreshold1 WRITE setThreshold1)
  PYB11_PROPERTY(double Threshold2 READ getThreshold2 WRITE setThreshold2)
  PYB11_PROPERTY(double Threshold3 READ getThreshold3 WRITE setThreshold3)
  PYB11_PROPERTY(double Threshold4 READ getThreshold4 WRITE setThreshold4)
  PYB11_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)
  PYB11_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKDoubleThresholdImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKDoubleThresholdImage> New();

    /**
    * @brief Returns the name of the class for ITKDoubleThresholdImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKDoubleThresholdImage
    */
    static QString ClassName();


  ~ITKDoubleThresholdImage() override;

    /**
    * @brief Setter property for Threshold1
    */
    void setThreshold1(double value); 
    /**
    * @brief Getter property for Threshold1
    * @return Value of Threshold1
    */
    double getThreshold1() const;
  Q_PROPERTY(double Threshold1 READ getThreshold1 WRITE setThreshold1)

    /**
    * @brief Setter property for Threshold2
    */
    void setThreshold2(double value); 
    /**
    * @brief Getter property for Threshold2
    * @return Value of Threshold2
    */
    double getThreshold2() const;
  Q_PROPERTY(double Threshold2 READ getThreshold2 WRITE setThreshold2)

    /**
    * @brief Setter property for Threshold3
    */
    void setThreshold3(double value); 
    /**
    * @brief Getter property for Threshold3
    * @return Value of Threshold3
    */
    double getThreshold3() const;
  Q_PROPERTY(double Threshold3 READ getThreshold3 WRITE setThreshold3)

    /**
    * @brief Setter property for Threshold4
    */
    void setThreshold4(double value); 
    /**
    * @brief Getter property for Threshold4
    * @return Value of Threshold4
    */
    double getThreshold4() const;
  Q_PROPERTY(double Threshold4 READ getThreshold4 WRITE setThreshold4)

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
  ITKDoubleThresholdImage();

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
  ITKDoubleThresholdImage(const ITKDoubleThresholdImage&) = delete;    // Copy Constructor Not Implemented
  ITKDoubleThresholdImage(ITKDoubleThresholdImage&&) = delete;         // Move Constructor Not Implemented
  ITKDoubleThresholdImage& operator=(const ITKDoubleThresholdImage&) = delete; // Copy Assignment Not Implemented
  ITKDoubleThresholdImage& operator=(ITKDoubleThresholdImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Threshold1 = {};
    double m_Threshold2 = {};
    double m_Threshold3 = {};
    double m_Threshold4 = {};
    int m_InsideValue = {};
    int m_OutsideValue = {};
    bool m_FullyConnected = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

