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
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <itkProxTVImageFilter.h>


/**
 * @brief The ITKProxTVImage class. See [Filter documentation](@ref ITKProxTVImage) for details.
 */
class ITKProxTVImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
    using Self = ITKProxTVImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKProxTVImage> New();

    /**
    * @brief Returns the name of the class for ITKProxTVImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKProxTVImage
    */
    static QString ClassName();


  ~ITKProxTVImage() override;

    /**
    * @brief Setter property for MaximumNumberOfIterations
    */
    void setMaximumNumberOfIterations(double value); 
    /**
    * @brief Getter property for MaximumNumberOfIterations
    * @return Value of MaximumNumberOfIterations
    */
    double getMaximumNumberOfIterations() const;

  Q_PROPERTY(double MaximumNumberOfIterations READ getMaximumNumberOfIterations WRITE setMaximumNumberOfIterations)

    /**
    * @brief Setter property for Weights
    */
    void setWeights(const FloatVec3Type& value); 
    /**
    * @brief Getter property for Weights
    * @return Value of Weights
    */
    FloatVec3Type getWeights() const;

  Q_PROPERTY(FloatVec3Type Weights READ getWeights WRITE setWeights)

    /**
    * @brief Setter property for Norms
    */
    void setNorms(const FloatVec3Type& value); 
    /**
    * @brief Getter property for Norms
    * @return Value of Norms
    */
    FloatVec3Type getNorms() const;

  Q_PROPERTY(FloatVec3Type Norms READ getNorms WRITE setNorms)


  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer
  newFilterInstance(bool copyFilterParameters) const override;

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
  void readFilterParameters(AbstractFilterParametersReader * reader, int index) override;

protected:
  ITKProxTVImage();

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

private:
    double m_MaximumNumberOfIterations = {};
    FloatVec3Type m_Weights = {};
    FloatVec3Type m_Norms = {};

  ITKProxTVImage(const ITKProxTVImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKProxTVImage&) = delete;   // Move assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
