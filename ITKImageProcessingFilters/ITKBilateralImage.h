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
#include <itkBilateralImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBilateralImage class. See [Filter documentation](@ref ITKBilateralImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBilateralImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKBilateralImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKBilateralImage)
  PYB11_FILTER_NEW_MACRO(ITKBilateralImage)
  PYB11_FILTER_PARAMETER(double, DomainSigma)
  PYB11_FILTER_PARAMETER(double, RangeSigma)
  PYB11_FILTER_PARAMETER(double, NumberOfRangeGaussianSamples)
  PYB11_PROPERTY(double DomainSigma READ getDomainSigma WRITE setDomainSigma)
  PYB11_PROPERTY(double RangeSigma READ getRangeSigma WRITE setRangeSigma)
  PYB11_PROPERTY(double NumberOfRangeGaussianSamples READ getNumberOfRangeGaussianSamples WRITE setNumberOfRangeGaussianSamples)
#endif

public:
    using Self = ITKBilateralImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKBilateralImage> New();

    /**
    * @brief Returns the name of the class for ITKBilateralImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKBilateralImage
    */
    static QString ClassName();


  ~ITKBilateralImage() override;

    /**
    * @brief Setter property for DomainSigma
    */
    void setDomainSigma(double value); 
    /**
    * @brief Getter property for DomainSigma
    * @return Value of DomainSigma
    */
    double getDomainSigma() const;

  Q_PROPERTY(double DomainSigma READ getDomainSigma WRITE setDomainSigma)

    /**
    * @brief Setter property for RangeSigma
    */
    void setRangeSigma(double value); 
    /**
    * @brief Getter property for RangeSigma
    * @return Value of RangeSigma
    */
    double getRangeSigma() const;

  Q_PROPERTY(double RangeSigma READ getRangeSigma WRITE setRangeSigma)

    /**
    * @brief Setter property for NumberOfRangeGaussianSamples
    */
    void setNumberOfRangeGaussianSamples(double value); 
    /**
    * @brief Getter property for NumberOfRangeGaussianSamples
    * @return Value of NumberOfRangeGaussianSamples
    */
    double getNumberOfRangeGaussianSamples() const;

  Q_PROPERTY(double NumberOfRangeGaussianSamples READ getNumberOfRangeGaussianSamples WRITE setNumberOfRangeGaussianSamples)


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
  ITKBilateralImage();

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
  ITKBilateralImage(const ITKBilateralImage&) = delete;    // Copy Constructor Not Implemented
  ITKBilateralImage(ITKBilateralImage&&) = delete;         // Move Constructor Not Implemented
  ITKBilateralImage& operator=(const ITKBilateralImage&) = delete; // Copy Assignment Not Implemented
  ITKBilateralImage& operator=(ITKBilateralImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_DomainSigma = {};
    double m_RangeSigma = {};
    double m_NumberOfRangeGaussianSamples = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

