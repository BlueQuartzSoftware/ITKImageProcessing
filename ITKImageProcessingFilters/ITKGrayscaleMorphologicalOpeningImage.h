// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGrayscaleMorphologicalOpeningImage_h_
#define _ITKGrayscaleMorphologicalOpeningImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkGrayscaleMorphologicalOpeningImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKGrayscaleMorphologicalOpeningImage class. See [Filter documentation](@ref ITKGrayscaleMorphologicalOpeningImage) for details.
 */
class ITKImageProcessing_EXPORT ITKGrayscaleMorphologicalOpeningImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKGrayscaleMorphologicalOpeningImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(bool SafeBorder READ getSafeBorder WRITE setSafeBorder)
  PYB11_PROPERTY(FloatVec3_t KernelRadius READ getKernelRadius WRITE setKernelRadius)
  PYB11_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)

public:
  SIMPL_SHARED_POINTERS(ITKGrayscaleMorphologicalOpeningImage)
  SIMPL_FILTER_NEW_MACRO(ITKGrayscaleMorphologicalOpeningImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKGrayscaleMorphologicalOpeningImage, AbstractFilter)

  ~ITKGrayscaleMorphologicalOpeningImage() override;

  SIMPL_FILTER_PARAMETER(bool, SafeBorder)
  Q_PROPERTY(bool SafeBorder READ getSafeBorder WRITE setSafeBorder)

  SIMPL_FILTER_PARAMETER(FloatVec3_t, KernelRadius)
  Q_PROPERTY(FloatVec3_t KernelRadius READ getKernelRadius WRITE setKernelRadius)

  SIMPL_FILTER_PARAMETER(int, KernelType)
  Q_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)


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
  ITKGrayscaleMorphologicalOpeningImage();

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
  ITKGrayscaleMorphologicalOpeningImage(const ITKGrayscaleMorphologicalOpeningImage&) = delete;    // Copy Constructor Not Implemented
  ITKGrayscaleMorphologicalOpeningImage(ITKGrayscaleMorphologicalOpeningImage&&) = delete;         // Move Constructor Not Implemented
  ITKGrayscaleMorphologicalOpeningImage& operator=(const ITKGrayscaleMorphologicalOpeningImage&) = delete; // Copy Assignment Not Implemented
  ITKGrayscaleMorphologicalOpeningImage& operator=(ITKGrayscaleMorphologicalOpeningImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKGrayscaleMorphologicalOpeningImage_H_ */
