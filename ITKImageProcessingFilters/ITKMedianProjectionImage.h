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
#include <itkMedianProjectionImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKMedianProjectionImage class. See [Filter documentation](@ref ITKMedianProjectionImage) for details.
 */
class ITKImageProcessing_EXPORT ITKMedianProjectionImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKMedianProjectionImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKMedianProjectionImage)
  PYB11_FILTER_NEW_MACRO(ITKMedianProjectionImage)
  PYB11_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKMedianProjectionImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKMedianProjectionImage> New();

  /**
   * @brief Returns the name of the class for ITKMedianProjectionImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKMedianProjectionImage
   */
  static QString ClassName();

  ~ITKMedianProjectionImage() override;

  /**
   * @brief Setter property for ProjectionDimension
   */
  void setProjectionDimension(double value);
  /**
   * @brief Getter property for ProjectionDimension
   * @return Value of ProjectionDimension
   */
  double getProjectionDimension() const;
  Q_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)

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
  ITKMedianProjectionImage();

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
  ITKMedianProjectionImage(const ITKMedianProjectionImage&) = delete;            // Copy Constructor Not Implemented
  ITKMedianProjectionImage(ITKMedianProjectionImage&&) = delete;                 // Move Constructor Not Implemented
  ITKMedianProjectionImage& operator=(const ITKMedianProjectionImage&) = delete; // Copy Assignment Not Implemented
  ITKMedianProjectionImage& operator=(ITKMedianProjectionImage&&) = delete;      // Move Assignment Not Implemented

private:
  double m_ProjectionDimension = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
