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
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkZeroCrossingImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKZeroCrossingImage class. See [Filter documentation](@ref ITKZeroCrossingImage) for details.
 */
class ITKImageProcessing_EXPORT ITKZeroCrossingImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKZeroCrossingImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKZeroCrossingImage)
  PYB11_FILTER_NEW_MACRO(ITKZeroCrossingImage)
  PYB11_FILTER_PARAMETER(int, ForegroundValue)
  PYB11_FILTER_PARAMETER(int, BackgroundValue)
  PYB11_PROPERTY(int ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_PROPERTY(int BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)
#endif

public:
    using Self = ITKZeroCrossingImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKZeroCrossingImage> New();

    /**
    * @brief Returns the name of the class for ITKZeroCrossingImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKZeroCrossingImage
    */
    static QString ClassName();


  ~ITKZeroCrossingImage() override;

    /**
    * @brief Setter property for ForegroundValue
    */
    void setForegroundValue(int value); 
    /**
    * @brief Getter property for ForegroundValue
    * @return Value of ForegroundValue
    */
    int getForegroundValue() const;

  Q_PROPERTY(int ForegroundValue READ getForegroundValue WRITE setForegroundValue)

    /**
    * @brief Setter property for BackgroundValue
    */
    void setBackgroundValue(int value); 
    /**
    * @brief Getter property for BackgroundValue
    * @return Value of BackgroundValue
    */
    int getBackgroundValue() const;

  Q_PROPERTY(int BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)


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
  ITKZeroCrossingImage();

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
  ITKZeroCrossingImage(const ITKZeroCrossingImage&) = delete;    // Copy Constructor Not Implemented
  ITKZeroCrossingImage(ITKZeroCrossingImage&&) = delete;         // Move Constructor Not Implemented
  ITKZeroCrossingImage& operator=(const ITKZeroCrossingImage&) = delete; // Copy Assignment Not Implemented
  ITKZeroCrossingImage& operator=(ITKZeroCrossingImage&&) = delete;      // Move Assignment Not Implemented

  private:
    int m_ForegroundValue = {};
    int m_BackgroundValue = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

