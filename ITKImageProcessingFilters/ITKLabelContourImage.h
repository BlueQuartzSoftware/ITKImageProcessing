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
#include <itkLabelContourImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKLabelContourImage class. See [Filter documentation](@ref ITKLabelContourImage) for details.
 */
class ITKImageProcessing_EXPORT ITKLabelContourImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKLabelContourImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKLabelContourImage)
  PYB11_FILTER_NEW_MACRO(ITKLabelContourImage)
  PYB11_FILTER_PARAMETER(bool, FullyConnected)
  PYB11_FILTER_PARAMETER(double, BackgroundValue)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)
#endif

public:
    using Self = ITKLabelContourImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKLabelContourImage> New();

    /**
    * @brief Returns the name of the class for ITKLabelContourImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKLabelContourImage
    */
    static QString ClassName();


  ~ITKLabelContourImage() override;

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
  ITKLabelContourImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheck();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

public:
  ITKLabelContourImage(const ITKLabelContourImage&) = delete;    // Copy Constructor Not Implemented
  ITKLabelContourImage(ITKLabelContourImage&&) = delete;         // Move Constructor Not Implemented
  ITKLabelContourImage& operator=(const ITKLabelContourImage&) = delete; // Copy Assignment Not Implemented
  ITKLabelContourImage& operator=(ITKLabelContourImage&&) = delete;      // Move Assignment Not Implemented

  private:
    bool m_FullyConnected = {};
    double m_BackgroundValue = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

