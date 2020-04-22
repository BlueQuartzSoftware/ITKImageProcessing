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
#include <itkSigmoidImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKSigmoidImage class. See [Filter documentation](@ref ITKSigmoidImage) for details.
 */
class ITKImageProcessing_EXPORT ITKSigmoidImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKSigmoidImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKSigmoidImage)
  PYB11_FILTER_NEW_MACRO(ITKSigmoidImage)
  PYB11_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)
  PYB11_PROPERTY(double Beta READ getBeta WRITE setBeta)
  PYB11_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)
  PYB11_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKSigmoidImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKSigmoidImage> New();

    /**
    * @brief Returns the name of the class for ITKSigmoidImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKSigmoidImage
    */
    static QString ClassName();


  ~ITKSigmoidImage() override;

    /**
    * @brief Setter property for Alpha
    */
    void setAlpha(double value); 
    /**
    * @brief Getter property for Alpha
    * @return Value of Alpha
    */
    double getAlpha() const;
  Q_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)

    /**
    * @brief Setter property for Beta
    */
    void setBeta(double value); 
    /**
    * @brief Getter property for Beta
    * @return Value of Beta
    */
    double getBeta() const;
  Q_PROPERTY(double Beta READ getBeta WRITE setBeta)

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
  ITKSigmoidImage();

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
  ITKSigmoidImage(const ITKSigmoidImage&) = delete;    // Copy Constructor Not Implemented
  ITKSigmoidImage(ITKSigmoidImage&&) = delete;         // Move Constructor Not Implemented
  ITKSigmoidImage& operator=(const ITKSigmoidImage&) = delete; // Copy Assignment Not Implemented
  ITKSigmoidImage& operator=(ITKSigmoidImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Alpha = {};
    double m_Beta = {};
    double m_OutputMaximum = {};
    double m_OutputMinimum = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

