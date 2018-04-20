// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGrayscaleGrindPeakImage_h_
#define _ITKGrayscaleGrindPeakImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkGrayscaleGrindPeakImageFilter.h>


/**
 * @brief The ITKGrayscaleGrindPeakImage class. See [Filter documentation](@ref ITKGrayscaleGrindPeakImage) for details.
 */
class ITKGrayscaleGrindPeakImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKGrayscaleGrindPeakImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

public:
  SIMPL_SHARED_POINTERS(ITKGrayscaleGrindPeakImage)
  SIMPL_FILTER_NEW_MACRO(ITKGrayscaleGrindPeakImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKGrayscaleGrindPeakImage, AbstractFilter)

  ~ITKGrayscaleGrindPeakImage() override;

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)


  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  const QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  const QUuid getUuid() override;

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
  void virtual dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheck();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void virtual filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

private:
  ITKGrayscaleGrindPeakImage(const ITKGrayscaleGrindPeakImage&) = delete;    // Copy Constructor Not Implemented
  ITKGrayscaleGrindPeakImage(ITKGrayscaleGrindPeakImage&&) = delete;         // Move Constructor Not Implemented
  ITKGrayscaleGrindPeakImage& operator=(const ITKGrayscaleGrindPeakImage&) = delete; // Copy Assignment Not Implemented
  ITKGrayscaleGrindPeakImage& operator=(ITKGrayscaleGrindPeakImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKGrayscaleGrindPeakImage_H_ */
