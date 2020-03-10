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
#include <SIMPLib/FilterParameters/FloatFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <itkAdaptiveHistogramEqualizationImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKAdaptiveHistogramEqualizationImage class. See [Filter documentation](@ref ITKAdaptiveHistogramEqualizationImage) for details.
 */
class ITKImageProcessing_EXPORT ITKAdaptiveHistogramEqualizationImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKAdaptiveHistogramEqualizationImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKAdaptiveHistogramEqualizationImage)
  PYB11_FILTER_NEW_MACRO(ITKAdaptiveHistogramEqualizationImage)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Radius)
  PYB11_FILTER_PARAMETER(float, Alpha)
  PYB11_FILTER_PARAMETER(float, Beta)
  PYB11_PROPERTY(FloatVec3Type Radius READ getRadius WRITE setRadius)
  PYB11_PROPERTY(float Alpha READ getAlpha WRITE setAlpha)
  PYB11_PROPERTY(float Beta READ getBeta WRITE setBeta)
#endif

public:
    using Self = ITKAdaptiveHistogramEqualizationImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKAdaptiveHistogramEqualizationImage> New();

    /**
    * @brief Returns the name of the class for ITKAdaptiveHistogramEqualizationImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKAdaptiveHistogramEqualizationImage
    */
    static QString ClassName();


  ~ITKAdaptiveHistogramEqualizationImage() override;

    /**
    * @brief Setter property for Radius
    */
    void setRadius(const FloatVec3Type& value); 
    /**
    * @brief Getter property for Radius
    * @return Value of Radius
    */
    FloatVec3Type getRadius() const;

  Q_PROPERTY(FloatVec3Type Radius READ getRadius WRITE setRadius)

    /**
    * @brief Setter property for Alpha
    */
    void setAlpha(float value); 
    /**
    * @brief Getter property for Alpha
    * @return Value of Alpha
    */
    float getAlpha() const;

  Q_PROPERTY(float Alpha READ getAlpha WRITE setAlpha)

    /**
    * @brief Setter property for Beta
    */
    void setBeta(float value); 
    /**
    * @brief Getter property for Beta
    * @return Value of Beta
    */
    float getBeta() const;

  Q_PROPERTY(float Beta READ getBeta WRITE setBeta)

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
  ITKAdaptiveHistogramEqualizationImage();

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
  ITKAdaptiveHistogramEqualizationImage(const ITKAdaptiveHistogramEqualizationImage&) = delete;    // Copy Constructor Not Implemented
  ITKAdaptiveHistogramEqualizationImage(ITKAdaptiveHistogramEqualizationImage&&) = delete;         // Move Constructor Not Implemented
  ITKAdaptiveHistogramEqualizationImage& operator=(const ITKAdaptiveHistogramEqualizationImage&) = delete; // Copy Assignment Not Implemented
  ITKAdaptiveHistogramEqualizationImage& operator=(ITKAdaptiveHistogramEqualizationImage&&) = delete;      // Move Assignment Not Implemented

  private:
    FloatVec3Type m_Radius = {};
    float m_Alpha = {};
    float m_Beta = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

