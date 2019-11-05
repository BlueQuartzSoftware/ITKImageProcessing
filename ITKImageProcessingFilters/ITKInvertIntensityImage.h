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
#include <itkInvertIntensityImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKInvertIntensityImage class. See [Filter documentation](@ref ITKInvertIntensityImage) for details.
 */
class ITKImageProcessing_EXPORT ITKInvertIntensityImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKInvertIntensityImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKInvertIntensityImage)
  PYB11_FILTER_NEW_MACRO(ITKInvertIntensityImage)
  PYB11_FILTER_PARAMETER(double, Maximum)
  PYB11_PROPERTY(double Maximum READ getMaximum WRITE setMaximum)
#endif

public:
    using Self = ITKInvertIntensityImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKInvertIntensityImage> New();

    /**
    * @brief Returns the name of the class for ITKInvertIntensityImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKInvertIntensityImage
    */
    static QString ClassName();


  ~ITKInvertIntensityImage() override;

    /**
    * @brief Setter property for Maximum
    */
    void setMaximum(double value); 
    /**
    * @brief Getter property for Maximum
    * @return Value of Maximum
    */
    double getMaximum() const;

  Q_PROPERTY(double Maximum READ getMaximum WRITE setMaximum)


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
  ITKInvertIntensityImage();

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
  ITKInvertIntensityImage(const ITKInvertIntensityImage&) = delete;    // Copy Constructor Not Implemented
  ITKInvertIntensityImage(ITKInvertIntensityImage&&) = delete;         // Move Constructor Not Implemented
  ITKInvertIntensityImage& operator=(const ITKInvertIntensityImage&) = delete; // Copy Assignment Not Implemented
  ITKInvertIntensityImage& operator=(ITKInvertIntensityImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Maximum = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

