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
#include <itkDanielssonDistanceMapImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKDanielssonDistanceMapImage class. See [Filter documentation](@ref ITKDanielssonDistanceMapImage) for details.
 */
class ITKImageProcessing_EXPORT ITKDanielssonDistanceMapImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKDanielssonDistanceMapImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKDanielssonDistanceMapImage)
  PYB11_FILTER_NEW_MACRO(ITKDanielssonDistanceMapImage)
  PYB11_PROPERTY(bool InputIsBinary READ getInputIsBinary WRITE setInputIsBinary)
  PYB11_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)
  PYB11_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKDanielssonDistanceMapImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKDanielssonDistanceMapImage> New();

  /**
   * @brief Returns the name of the class for ITKDanielssonDistanceMapImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKDanielssonDistanceMapImage
   */
  static QString ClassName();

  ~ITKDanielssonDistanceMapImage() override;

  /**
   * @brief Setter property for InputIsBinary
   */
  void setInputIsBinary(bool value);
  /**
   * @brief Getter property for InputIsBinary
   * @return Value of InputIsBinary
   */
  bool getInputIsBinary() const;
  Q_PROPERTY(bool InputIsBinary READ getInputIsBinary WRITE setInputIsBinary)

  /**
   * @brief Setter property for SquaredDistance
   */
  void setSquaredDistance(bool value);
  /**
   * @brief Getter property for SquaredDistance
   * @return Value of SquaredDistance
   */
  bool getSquaredDistance() const;
  Q_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)

  /**
   * @brief Setter property for UseImageSpacing
   */
  void setUseImageSpacing(bool value);
  /**
   * @brief Getter property for UseImageSpacing
   * @return Value of UseImageSpacing
   */
  bool getUseImageSpacing() const;
  Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)

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
  ITKDanielssonDistanceMapImage();

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
  ITKDanielssonDistanceMapImage(const ITKDanielssonDistanceMapImage&) = delete;            // Copy Constructor Not Implemented
  ITKDanielssonDistanceMapImage(ITKDanielssonDistanceMapImage&&) = delete;                 // Move Constructor Not Implemented
  ITKDanielssonDistanceMapImage& operator=(const ITKDanielssonDistanceMapImage&) = delete; // Copy Assignment Not Implemented
  ITKDanielssonDistanceMapImage& operator=(ITKDanielssonDistanceMapImage&&) = delete;      // Move Assignment Not Implemented

private:
  bool m_InputIsBinary = {};
  bool m_SquaredDistance = {};
  bool m_UseImageSpacing = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
