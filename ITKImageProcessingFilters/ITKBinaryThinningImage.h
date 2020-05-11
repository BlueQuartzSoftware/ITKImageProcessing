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
#include <itkBinaryThinningImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBinaryThinningImage class. See [Filter documentation](@ref ITKBinaryThinningImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBinaryThinningImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  using Self = ITKBinaryThinningImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKBinaryThinningImage> New();

  /**
   * @brief Returns the name of the class for ITKBinaryThinningImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKBinaryThinningImage
   */
  static QString ClassName();

  ~ITKBinaryThinningImage() override;

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
  ITKBinaryThinningImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheck() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension>
  void dataCheckImpl();

  /**
   * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
   */
  void filterInternal() override;

  /**
   * @brief Applies the filter
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension>
  void filter();

public:
  ITKBinaryThinningImage(const ITKBinaryThinningImage&) = delete;            // Copy Constructor Not Implemented
  ITKBinaryThinningImage(ITKBinaryThinningImage&&) = delete;                 // Move Constructor Not Implemented
  ITKBinaryThinningImage& operator=(const ITKBinaryThinningImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryThinningImage& operator=(ITKBinaryThinningImage&&) = delete;      // Move Assignment Not Implemented

private:
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
