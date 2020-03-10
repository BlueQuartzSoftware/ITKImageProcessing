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
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKOtsuMultipleThresholdsImage class. See [Filter documentation](@ref ITKOtsuMultipleThresholdsImage) for details.
 */
class ITKImageProcessing_EXPORT ITKOtsuMultipleThresholdsImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKOtsuMultipleThresholdsImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKOtsuMultipleThresholdsImage)
  PYB11_FILTER_NEW_MACRO(ITKOtsuMultipleThresholdsImage)
  PYB11_FILTER_PARAMETER(int, NumberOfThresholds)
  PYB11_FILTER_PARAMETER(int, LabelOffset)
  PYB11_FILTER_PARAMETER(double, NumberOfHistogramBins)
  PYB11_FILTER_PARAMETER(bool, ValleyEmphasis)
  PYB11_FILTER_PARAMETER(FloatVec3Type, Thresholds)
  PYB11_PROPERTY(int NumberOfThresholds READ getNumberOfThresholds WRITE setNumberOfThresholds)
  PYB11_PROPERTY(int LabelOffset READ getLabelOffset WRITE setLabelOffset)
  PYB11_PROPERTY(double NumberOfHistogramBins READ getNumberOfHistogramBins WRITE setNumberOfHistogramBins)
  PYB11_PROPERTY(bool ValleyEmphasis READ getValleyEmphasis WRITE setValleyEmphasis)
  PYB11_PROPERTY(FloatVec3Type Thresholds READ getThresholds WRITE setThresholds)
#endif

public:
    using Self = ITKOtsuMultipleThresholdsImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKOtsuMultipleThresholdsImage> New();

    /**
    * @brief Returns the name of the class for ITKOtsuMultipleThresholdsImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKOtsuMultipleThresholdsImage
    */
    static QString ClassName();


  ~ITKOtsuMultipleThresholdsImage() override;

    /**
    * @brief Setter property for NumberOfThresholds
    */
    void setNumberOfThresholds(int value); 
    /**
    * @brief Getter property for NumberOfThresholds
    * @return Value of NumberOfThresholds
    */
    int getNumberOfThresholds() const;

  Q_PROPERTY(int NumberOfThresholds READ getNumberOfThresholds WRITE setNumberOfThresholds)

    /**
    * @brief Setter property for LabelOffset
    */
    void setLabelOffset(int value); 
    /**
    * @brief Getter property for LabelOffset
    * @return Value of LabelOffset
    */
    int getLabelOffset() const;

  Q_PROPERTY(int LabelOffset READ getLabelOffset WRITE setLabelOffset)

    /**
    * @brief Setter property for NumberOfHistogramBins
    */
    void setNumberOfHistogramBins(double value); 
    /**
    * @brief Getter property for NumberOfHistogramBins
    * @return Value of NumberOfHistogramBins
    */
    double getNumberOfHistogramBins() const;

  Q_PROPERTY(double NumberOfHistogramBins READ getNumberOfHistogramBins WRITE setNumberOfHistogramBins)

    /**
    * @brief Setter property for ValleyEmphasis
    */
    void setValleyEmphasis(bool value); 
    /**
    * @brief Getter property for ValleyEmphasis
    * @return Value of ValleyEmphasis
    */
    bool getValleyEmphasis() const;

  Q_PROPERTY(bool ValleyEmphasis READ getValleyEmphasis WRITE setValleyEmphasis)

    /**
    * @brief Setter property for Thresholds
    */
    void setThresholds(const FloatVec3Type& value); 
    /**
    * @brief Getter property for Thresholds
    * @return Value of Thresholds
    */
    FloatVec3Type getThresholds() const;

  Q_PROPERTY(FloatVec3Type Thresholds READ getThresholds)

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
  ITKOtsuMultipleThresholdsImage();

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
  ITKOtsuMultipleThresholdsImage(const ITKOtsuMultipleThresholdsImage&) = delete;    // Copy Constructor Not Implemented
  ITKOtsuMultipleThresholdsImage(ITKOtsuMultipleThresholdsImage&&) = delete;         // Move Constructor Not Implemented
  ITKOtsuMultipleThresholdsImage& operator=(const ITKOtsuMultipleThresholdsImage&) = delete; // Copy Assignment Not Implemented
  ITKOtsuMultipleThresholdsImage& operator=(ITKOtsuMultipleThresholdsImage&&) = delete;      // Move Assignment Not Implemented

  private:
    int m_NumberOfThresholds = {};
    int m_LabelOffset = {};
    double m_NumberOfHistogramBins = {};
    bool m_ValleyEmphasis = {};
    FloatVec3Type m_Thresholds = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

