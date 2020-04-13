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
#include <itkSumProjectionImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKSumProjectionImage class. See [Filter documentation](@ref ITKSumProjectionImage) for details.
 */
class ITKImageProcessing_EXPORT ITKSumProjectionImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKSumProjectionImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKSumProjectionImage)
  PYB11_FILTER_NEW_MACRO(ITKSumProjectionImage)
  PYB11_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKSumProjectionImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKSumProjectionImage> New();

    /**
    * @brief Returns the name of the class for ITKSumProjectionImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKSumProjectionImage
    */
    static QString ClassName();


  ~ITKSumProjectionImage() override;

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
  ITKSumProjectionImage();

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
  ITKSumProjectionImage(const ITKSumProjectionImage&) = delete;    // Copy Constructor Not Implemented
  ITKSumProjectionImage(ITKSumProjectionImage&&) = delete;         // Move Constructor Not Implemented
  ITKSumProjectionImage& operator=(const ITKSumProjectionImage&) = delete; // Copy Assignment Not Implemented
  ITKSumProjectionImage& operator=(ITKSumProjectionImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_ProjectionDimension = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

