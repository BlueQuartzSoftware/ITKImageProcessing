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
#include <itkBinomialBlurImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBinomialBlurImage class. See [Filter documentation](@ref ITKBinomialBlurImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBinomialBlurImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKBinomialBlurImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKBinomialBlurImage)
  PYB11_FILTER_NEW_MACRO(ITKBinomialBlurImage)
  PYB11_PROPERTY(double Repetitions READ getRepetitions WRITE setRepetitions)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKBinomialBlurImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKBinomialBlurImage> New();

    /**
    * @brief Returns the name of the class for ITKBinomialBlurImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKBinomialBlurImage
    */
    static QString ClassName();


  ~ITKBinomialBlurImage() override;

    /**
    * @brief Setter property for Repetitions
    */
    void setRepetitions(double value); 
    /**
    * @brief Getter property for Repetitions
    * @return Value of Repetitions
    */
    double getRepetitions() const;

  Q_PROPERTY(double Repetitions READ getRepetitions WRITE setRepetitions)


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
  ITKBinomialBlurImage();

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
  ITKBinomialBlurImage(const ITKBinomialBlurImage&) = delete;    // Copy Constructor Not Implemented
  ITKBinomialBlurImage(ITKBinomialBlurImage&&) = delete;         // Move Constructor Not Implemented
  ITKBinomialBlurImage& operator=(const ITKBinomialBlurImage&) = delete; // Copy Assignment Not Implemented
  ITKBinomialBlurImage& operator=(ITKBinomialBlurImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Repetitions = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

