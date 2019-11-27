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
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <itkDiscreteGaussianImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKDiscreteGaussianImage class. See [Filter documentation](@ref ITKDiscreteGaussianImage) for details.
 */
class ITKImageProcessing_EXPORT ITKDiscreteGaussianImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKDiscreteGaussianImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKDiscreteGaussianImage)
  PYB11_FILTER_NEW_MACRO(ITKDiscreteGaussianImage)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Variance)
  PYB11_FILTER_PARAMETER(int32_t, MaximumKernelWidth)
  PYB11_FILTER_PARAMETER(FloatVec3Type, MaximumError)
  PYB11_FILTER_PARAMETER(bool, UseImageSpacing)
  PYB11_PROPERTY(FloatVec3Type Variance READ getVariance WRITE setVariance)
  PYB11_PROPERTY(int32_t MaximumKernelWidth READ getMaximumKernelWidth WRITE setMaximumKernelWidth)
  PYB11_PROPERTY(FloatVec3Type MaximumError READ getMaximumError WRITE setMaximumError)
  PYB11_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)
#endif

public:
    using Self = ITKDiscreteGaussianImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKDiscreteGaussianImage> New();

    /**
    * @brief Returns the name of the class for ITKDiscreteGaussianImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKDiscreteGaussianImage
    */
    static QString ClassName();


  ~ITKDiscreteGaussianImage() override;

    /**
    * @brief Setter property for Variance
    */
    void setVariance(const FloatVec3Type& value); 
    /**
    * @brief Getter property for Variance
    * @return Value of Variance
    */
    FloatVec3Type getVariance() const;

  Q_PROPERTY(FloatVec3Type Variance READ getVariance WRITE setVariance)

    /**
    * @brief Setter property for MaximumKernelWidth
    */
    void setMaximumKernelWidth(int32_t value); 
    /**
    * @brief Getter property for MaximumKernelWidth
    * @return Value of MaximumKernelWidth
    */
    int32_t getMaximumKernelWidth() const;

  Q_PROPERTY(int32_t MaximumKernelWidth READ getMaximumKernelWidth WRITE setMaximumKernelWidth)

    /**
    * @brief Setter property for MaximumError
    */
    void setMaximumError(const FloatVec3Type& value); 
    /**
    * @brief Getter property for MaximumError
    * @return Value of MaximumError
    */
    FloatVec3Type getMaximumError() const;

  Q_PROPERTY(FloatVec3Type MaximumError READ getMaximumError WRITE setMaximumError)

    /**
    * @brief Setter property for UseImageSpacing
    */
    void setUseImageSpacing(bool value); 
    /**
    * @brief Getter property for UseImageSpacing
    * @return Value of UseImageSpacing
    */
    bool getUseImageSpacing() const;

  Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)


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
  ITKDiscreteGaussianImage();

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
  ITKDiscreteGaussianImage(const ITKDiscreteGaussianImage&) = delete;    // Copy Constructor Not Implemented
  ITKDiscreteGaussianImage(ITKDiscreteGaussianImage&&) = delete;         // Move Constructor Not Implemented
  ITKDiscreteGaussianImage& operator=(const ITKDiscreteGaussianImage&) = delete; // Copy Assignment Not Implemented
  ITKDiscreteGaussianImage& operator=(ITKDiscreteGaussianImage&&) = delete;      // Move Assignment Not Implemented

  private:
    FloatVec3Type m_Variance = {};
    int32_t m_MaximumKernelWidth = {};
    FloatVec3Type m_MaximumError = {};
    bool m_UseImageSpacing = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

