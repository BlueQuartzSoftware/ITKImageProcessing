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
#include <itkFFTNormalizedCorrelationImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKFFTNormalizedCorrelationImage class. See [Filter documentation](@ref ITKFFTNormalizedCorrelationImage) for details.
 */
class ITKImageProcessing_EXPORT ITKFFTNormalizedCorrelationImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKFFTNormalizedCorrelationImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKFFTNormalizedCorrelationImage)
  PYB11_FILTER_NEW_MACRO(ITKFFTNormalizedCorrelationImage)
  PYB11_PROPERTY(DataArrayPath MovingCellArrayPath READ getMovingCellArrayPath WRITE setMovingCellArrayPath)
  PYB11_PROPERTY(double RequiredNumberOfOverlappingPixels READ getRequiredNumberOfOverlappingPixels WRITE setRequiredNumberOfOverlappingPixels)
  PYB11_PROPERTY(double RequiredFractionOfOverlappingPixels READ getRequiredFractionOfOverlappingPixels WRITE setRequiredFractionOfOverlappingPixels)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKFFTNormalizedCorrelationImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKFFTNormalizedCorrelationImage> New();

    /**
    * @brief Returns the name of the class for ITKFFTNormalizedCorrelationImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKFFTNormalizedCorrelationImage
    */
    static QString ClassName();


    /**
    * @brief Setter property for MovingCellArrayPath
    */
    void setMovingCellArrayPath(const DataArrayPath& value); 
    /**
    * @brief Getter property for MovingCellArrayPath
    * @return Value of MovingCellArrayPath
    */
    DataArrayPath getMovingCellArrayPath() const;

  Q_PROPERTY(DataArrayPath MovingCellArrayPath READ getMovingCellArrayPath WRITE setMovingCellArrayPath)

  ~ITKFFTNormalizedCorrelationImage() override;

    /**
    * @brief Setter property for RequiredNumberOfOverlappingPixels
    */
    void setRequiredNumberOfOverlappingPixels(double value); 
    /**
    * @brief Getter property for RequiredNumberOfOverlappingPixels
    * @return Value of RequiredNumberOfOverlappingPixels
    */
    double getRequiredNumberOfOverlappingPixels() const;

  Q_PROPERTY(double RequiredNumberOfOverlappingPixels READ getRequiredNumberOfOverlappingPixels WRITE setRequiredNumberOfOverlappingPixels)

    /**
    * @brief Setter property for RequiredFractionOfOverlappingPixels
    */
    void setRequiredFractionOfOverlappingPixels(double value); 
    /**
    * @brief Getter property for RequiredFractionOfOverlappingPixels
    * @return Value of RequiredFractionOfOverlappingPixels
    */
    double getRequiredFractionOfOverlappingPixels() const;

  Q_PROPERTY(double RequiredFractionOfOverlappingPixels READ getRequiredFractionOfOverlappingPixels WRITE setRequiredFractionOfOverlappingPixels)

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
  ITKFFTNormalizedCorrelationImage();

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
  ITKFFTNormalizedCorrelationImage(const ITKFFTNormalizedCorrelationImage&) = delete; // Copy Constructor Not Implemented
  ITKFFTNormalizedCorrelationImage(ITKFFTNormalizedCorrelationImage&&) = delete;      // Move Constructor Not Implemented
  ITKFFTNormalizedCorrelationImage& operator=(const ITKFFTNormalizedCorrelationImage&) = delete; // Copy Assignment Not Implemented
  ITKFFTNormalizedCorrelationImage& operator=(ITKFFTNormalizedCorrelationImage&&) = delete;      // Move Assignment Not Implemented

  private:
    DataArrayPath m_MovingCellArrayPath = {};
    double m_RequiredNumberOfOverlappingPixels = {};
    double m_RequiredFractionOfOverlappingPixels = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

