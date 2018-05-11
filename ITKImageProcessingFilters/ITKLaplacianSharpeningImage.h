// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKLaplacianSharpeningImage_h_
#define _ITKLaplacianSharpeningImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkLaplacianSharpeningImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKLaplacianSharpeningImage class. See [Filter documentation](@ref ITKLaplacianSharpeningImage) for details.
 */
class ITKImageProcessing_EXPORT ITKLaplacianSharpeningImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKLaplacianSharpeningImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)

public:
  SIMPL_SHARED_POINTERS(ITKLaplacianSharpeningImage)
  SIMPL_FILTER_NEW_MACRO(ITKLaplacianSharpeningImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKLaplacianSharpeningImage, AbstractFilter)

  ~ITKLaplacianSharpeningImage() override;

  SIMPL_FILTER_PARAMETER(bool, UseImageSpacing)
  Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)


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
  ITKLaplacianSharpeningImage();

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
  ITKLaplacianSharpeningImage(const ITKLaplacianSharpeningImage&) = delete;    // Copy Constructor Not Implemented
  ITKLaplacianSharpeningImage(ITKLaplacianSharpeningImage&&) = delete;         // Move Constructor Not Implemented
  ITKLaplacianSharpeningImage& operator=(const ITKLaplacianSharpeningImage&) = delete; // Copy Assignment Not Implemented
  ITKLaplacianSharpeningImage& operator=(ITKLaplacianSharpeningImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKLaplacianSharpeningImage_H_ */
