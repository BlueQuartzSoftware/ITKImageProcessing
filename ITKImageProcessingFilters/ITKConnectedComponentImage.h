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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkConnectedComponentImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKConnectedComponentImage class. See [Filter documentation](@ref ITKConnectedComponentImage) for details.
 */
class ITKImageProcessing_EXPORT ITKConnectedComponentImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKConnectedComponentImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKConnectedComponentImage)
  PYB11_FILTER_NEW_MACRO(ITKConnectedComponentImage)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(double ObjectCount READ getObjectCount WRITE setObjectCount)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKConnectedComponentImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKConnectedComponentImage> New();

  /**
   * @brief Returns the name of the class for ITKConnectedComponentImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKConnectedComponentImage
   */
  static QString ClassName();

  ~ITKConnectedComponentImage() override;

  /**
   * @brief Setter property for FullyConnected
   */
  void setFullyConnected(bool value);
  /**
   * @brief Getter property for FullyConnected
   * @return Value of FullyConnected
   */
  bool getFullyConnected() const;
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

  /**
   * @brief Setter property for ObjectCount
   */
  void setObjectCount(double value);
  /**
   * @brief Getter property for ObjectCount
   * @return Value of ObjectCount
   */
  double getObjectCount() const;
  Q_PROPERTY(double ObjectCount READ getObjectCount)

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
  ITKConnectedComponentImage();

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
  ITKConnectedComponentImage(const ITKConnectedComponentImage&) = delete;            // Copy Constructor Not Implemented
  ITKConnectedComponentImage(ITKConnectedComponentImage&&) = delete;                 // Move Constructor Not Implemented
  ITKConnectedComponentImage& operator=(const ITKConnectedComponentImage&) = delete; // Copy Assignment Not Implemented
  ITKConnectedComponentImage& operator=(ITKConnectedComponentImage&&) = delete;      // Move Assignment Not Implemented

private:
  bool m_FullyConnected = {};
  double m_ObjectCount = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
