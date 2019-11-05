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
#include <itkIntensityWindowingImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKIntensityWindowingImage class. See [Filter documentation](@ref ITKIntensityWindowingImage) for details.
 */
class ITKImageProcessing_EXPORT ITKIntensityWindowingImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKIntensityWindowingImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKIntensityWindowingImage)
  PYB11_FILTER_NEW_MACRO(ITKIntensityWindowingImage)
  PYB11_FILTER_PARAMETER(double, WindowMinimum)
  PYB11_FILTER_PARAMETER(double, WindowMaximum)
  PYB11_FILTER_PARAMETER(double, OutputMinimum)
  PYB11_FILTER_PARAMETER(double, OutputMaximum)
  PYB11_PROPERTY(double WindowMinimum READ getWindowMinimum WRITE setWindowMinimum)
  PYB11_PROPERTY(double WindowMaximum READ getWindowMaximum WRITE setWindowMaximum)
  PYB11_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)
  PYB11_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)
#endif

public:
    using Self = ITKIntensityWindowingImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKIntensityWindowingImage> New();

    /**
    * @brief Returns the name of the class for ITKIntensityWindowingImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKIntensityWindowingImage
    */
    static QString ClassName();


  ~ITKIntensityWindowingImage() override;

    /**
    * @brief Setter property for WindowMinimum
    */
    void setWindowMinimum(double value); 
    /**
    * @brief Getter property for WindowMinimum
    * @return Value of WindowMinimum
    */
    double getWindowMinimum() const;

  Q_PROPERTY(double WindowMinimum READ getWindowMinimum WRITE setWindowMinimum)

    /**
    * @brief Setter property for WindowMaximum
    */
    void setWindowMaximum(double value); 
    /**
    * @brief Getter property for WindowMaximum
    * @return Value of WindowMaximum
    */
    double getWindowMaximum() const;

  Q_PROPERTY(double WindowMaximum READ getWindowMaximum WRITE setWindowMaximum)

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
  ITKIntensityWindowingImage();

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
  ITKIntensityWindowingImage(const ITKIntensityWindowingImage&) = delete;    // Copy Constructor Not Implemented
  ITKIntensityWindowingImage(ITKIntensityWindowingImage&&) = delete;         // Move Constructor Not Implemented
  ITKIntensityWindowingImage& operator=(const ITKIntensityWindowingImage&) = delete; // Copy Assignment Not Implemented
  ITKIntensityWindowingImage& operator=(ITKIntensityWindowingImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_WindowMinimum = {};
    double m_WindowMaximum = {};
    double m_OutputMinimum = {};
    double m_OutputMaximum = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

