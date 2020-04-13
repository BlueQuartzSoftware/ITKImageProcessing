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
#include <itkSaltAndPepperNoiseImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKSaltAndPepperNoiseImage class. See [Filter documentation](@ref ITKSaltAndPepperNoiseImage) for details.
 */
class ITKImageProcessing_EXPORT ITKSaltAndPepperNoiseImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKSaltAndPepperNoiseImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKSaltAndPepperNoiseImage)
  PYB11_FILTER_NEW_MACRO(ITKSaltAndPepperNoiseImage)
  PYB11_PROPERTY(double Probability READ getProbability WRITE setProbability)
  PYB11_PROPERTY(double Seed READ getSeed WRITE setSeed)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKSaltAndPepperNoiseImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKSaltAndPepperNoiseImage> New();

    /**
    * @brief Returns the name of the class for ITKSaltAndPepperNoiseImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKSaltAndPepperNoiseImage
    */
    static QString ClassName();


  ~ITKSaltAndPepperNoiseImage() override;

    /**
    * @brief Setter property for Probability
    */
    void setProbability(double value); 
    /**
    * @brief Getter property for Probability
    * @return Value of Probability
    */
    double getProbability() const;
  Q_PROPERTY(double Probability READ getProbability WRITE setProbability)

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
  ITKSaltAndPepperNoiseImage();

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
  ITKSaltAndPepperNoiseImage(const ITKSaltAndPepperNoiseImage&) = delete;    // Copy Constructor Not Implemented
  ITKSaltAndPepperNoiseImage(ITKSaltAndPepperNoiseImage&&) = delete;         // Move Constructor Not Implemented
  ITKSaltAndPepperNoiseImage& operator=(const ITKSaltAndPepperNoiseImage&) = delete; // Copy Assignment Not Implemented
  ITKSaltAndPepperNoiseImage& operator=(ITKSaltAndPepperNoiseImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Probability = {};
    double m_Seed = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

