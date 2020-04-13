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
#include <itkSpeckleNoiseImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKSpeckleNoiseImage class. See [Filter documentation](@ref ITKSpeckleNoiseImage) for details.
 */
class ITKImageProcessing_EXPORT ITKSpeckleNoiseImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKSpeckleNoiseImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKSpeckleNoiseImage)
  PYB11_FILTER_NEW_MACRO(ITKSpeckleNoiseImage)
  PYB11_PROPERTY(double StandardDeviation READ getStandardDeviation WRITE setStandardDeviation)
  PYB11_PROPERTY(double Seed READ getSeed WRITE setSeed)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKSpeckleNoiseImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKSpeckleNoiseImage> New();

    /**
    * @brief Returns the name of the class for ITKSpeckleNoiseImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKSpeckleNoiseImage
    */
    static QString ClassName();


  ~ITKSpeckleNoiseImage() override;

    /**
    * @brief Setter property for StandardDeviation
    */
    void setStandardDeviation(double value); 
    /**
    * @brief Getter property for StandardDeviation
    * @return Value of StandardDeviation
    */
    double getStandardDeviation() const;
  Q_PROPERTY(double StandardDeviation READ getStandardDeviation WRITE setStandardDeviation)

    /**
    * @brief Setter property for Seed
    */
    void setSeed(double value); 
    /**
    * @brief Getter property for Seed
    * @return Value of Seed
    */
    double getSeed() const;
  Q_PROPERTY(double Seed READ getSeed WRITE setSeed)


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
  ITKSpeckleNoiseImage();

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
  ITKSpeckleNoiseImage(const ITKSpeckleNoiseImage&) = delete;    // Copy Constructor Not Implemented
  ITKSpeckleNoiseImage(ITKSpeckleNoiseImage&&) = delete;         // Move Constructor Not Implemented
  ITKSpeckleNoiseImage& operator=(const ITKSpeckleNoiseImage&) = delete; // Copy Assignment Not Implemented
  ITKSpeckleNoiseImage& operator=(ITKSpeckleNoiseImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_StandardDeviation = {};
    double m_Seed = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

