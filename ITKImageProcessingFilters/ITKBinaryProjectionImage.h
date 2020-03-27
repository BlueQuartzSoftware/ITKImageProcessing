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
#include <itkBinaryProjectionImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBinaryProjectionImage class. See [Filter documentation](@ref ITKBinaryProjectionImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBinaryProjectionImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKBinaryProjectionImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKBinaryProjectionImage)
  PYB11_FILTER_NEW_MACRO(ITKBinaryProjectionImage)
  PYB11_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)
  PYB11_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKBinaryProjectionImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKBinaryProjectionImage> New();

    /**
    * @brief Returns the name of the class for ITKBinaryProjectionImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKBinaryProjectionImage
    */
    static QString ClassName();


  ~ITKBinaryProjectionImage() override;

    /**
    * @brief Setter property for ProjectionDimension
    */
    void setProjectionDimension(double value); 
    /**
    * @brief Getter property for ProjectionDimension
    * @return Value of ProjectionDimension
    */
    double getProjectionDimension() const;

  Q_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)

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
  ITKBinaryProjectionImage();

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
  ITKBinaryProjectionImage(const ITKBinaryProjectionImage&) = delete;    // Copy Constructor Not Implemented
  ITKBinaryProjectionImage(ITKBinaryProjectionImage&&) = delete;         // Move Constructor Not Implemented
  ITKBinaryProjectionImage& operator=(const ITKBinaryProjectionImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryProjectionImage& operator=(ITKBinaryProjectionImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_ProjectionDimension = {};
    double m_ForegroundValue = {};
    double m_BackgroundValue = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

