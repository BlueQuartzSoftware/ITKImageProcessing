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
#include <itkRescaleIntensityImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKRescaleIntensityImage class. See [Filter documentation](@ref ITKRescaleIntensityImage) for details.
 */
class ITKImageProcessing_EXPORT ITKRescaleIntensityImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKRescaleIntensityImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKRescaleIntensityImage)
  PYB11_FILTER_NEW_MACRO(ITKRescaleIntensityImage)
  PYB11_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)
  PYB11_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)
  PYB11_PROPERTY(int OutputType READ getOutputType WRITE setOutputType)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKRescaleIntensityImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKRescaleIntensityImage> New();

    /**
    * @brief Returns the name of the class for ITKRescaleIntensityImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKRescaleIntensityImage
    */
    static QString ClassName();


  ~ITKRescaleIntensityImage() override;

    /**
    * @brief Setter property for OutputMinimum
    */
    void setOutputMinimum(double value); 
    /**
    * @brief Getter property for OutputMinimum
    * @return Value of OutputMinimum
    */
    double getOutputMinimum() const;

  Q_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)

    /**
    * @brief Setter property for OutputMaximum
    */
    void setOutputMaximum(double value); 
    /**
    * @brief Getter property for OutputMaximum
    * @return Value of OutputMaximum
    */
    double getOutputMaximum() const;

  Q_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)

    /**
    * @brief Setter property for OutputType
    */
    void setOutputType(int value); 
    /**
    * @brief Getter property for OutputType
    * @return Value of OutputType
    */
    int getOutputType() const;

  Q_PROPERTY(int OutputType READ getOutputType WRITE setOutputType)

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
  ITKRescaleIntensityImage();

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

  /**
  * @brief Checks 'value' can be casted to OutputPixelType.
  */
  template <typename OutputPixelType> void CheckEntryBounds(double value, QString name);

public:
  ITKRescaleIntensityImage(const ITKRescaleIntensityImage&) = delete;    // Copy Constructor Not Implemented
  ITKRescaleIntensityImage(ITKRescaleIntensityImage&&) = delete;         // Move Constructor Not Implemented
  ITKRescaleIntensityImage& operator=(const ITKRescaleIntensityImage&) = delete; // Copy Assignment Not Implemented
  ITKRescaleIntensityImage& operator=(ITKRescaleIntensityImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_OutputMinimum = {};
    double m_OutputMaximum = {};
    int m_OutputType = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

