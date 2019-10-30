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
#include <itkGrayscaleGrindPeakImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKGrayscaleGrindPeakImage class. See [Filter documentation](@ref ITKGrayscaleGrindPeakImage) for details.
 */
class ITKImageProcessing_EXPORT ITKGrayscaleGrindPeakImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKGrayscaleGrindPeakImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKGrayscaleGrindPeakImage)
  PYB11_FILTER_NEW_MACRO(ITKGrayscaleGrindPeakImage)
  PYB11_FILTER_PARAMETER(bool, FullyConnected)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
#endif

public:
    using Self = ITKGrayscaleGrindPeakImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKGrayscaleGrindPeakImage> New();

    /**
    * @brief Returns the name of the class for ITKGrayscaleGrindPeakImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKGrayscaleGrindPeakImage
    */
    static QString ClassName();


  ~ITKGrayscaleGrindPeakImage() override;

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
  ITKGrayscaleGrindPeakImage();

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
  ITKGrayscaleGrindPeakImage(const ITKGrayscaleGrindPeakImage&) = delete;    // Copy Constructor Not Implemented
  ITKGrayscaleGrindPeakImage(ITKGrayscaleGrindPeakImage&&) = delete;         // Move Constructor Not Implemented
  ITKGrayscaleGrindPeakImage& operator=(const ITKGrayscaleGrindPeakImage&) = delete; // Copy Assignment Not Implemented
  ITKGrayscaleGrindPeakImage& operator=(ITKGrayscaleGrindPeakImage&&) = delete;      // Move Assignment Not Implemented

  private:
    bool m_FullyConnected = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

