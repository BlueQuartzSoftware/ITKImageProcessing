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
#include <itkBoundedReciprocalImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBoundedReciprocalImage class. See [Filter documentation](@ref ITKBoundedReciprocalImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBoundedReciprocalImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
    using Self = ITKBoundedReciprocalImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKBoundedReciprocalImage> New();

    /**
    * @brief Returns the name of the class for ITKBoundedReciprocalImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKBoundedReciprocalImage
    */
    static QString ClassName();


  ~ITKBoundedReciprocalImage() override;

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
  ITKBoundedReciprocalImage();

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
  ITKBoundedReciprocalImage(const ITKBoundedReciprocalImage&) = delete;    // Copy Constructor Not Implemented
  ITKBoundedReciprocalImage(ITKBoundedReciprocalImage&&) = delete;         // Move Constructor Not Implemented
  ITKBoundedReciprocalImage& operator=(const ITKBoundedReciprocalImage&) = delete; // Copy Assignment Not Implemented
  ITKBoundedReciprocalImage& operator=(ITKBoundedReciprocalImage&&) = delete;      // Move Assignment Not Implemented

  private:

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

