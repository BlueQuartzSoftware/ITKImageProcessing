// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGrayscaleErodeImage_h_
#define _ITKGrayscaleErodeImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkGrayscaleErodeImageFilter.h>


/**
 * @brief The ITKGrayscaleErodeImage class. See [Filter documentation](@ref ITKGrayscaleErodeImage) for details.
 */
class ITKGrayscaleErodeImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKGrayscaleErodeImage)
  SIMPL_STATIC_NEW_MACRO(ITKGrayscaleErodeImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKGrayscaleErodeImage, AbstractFilter)

  virtual ~ITKGrayscaleErodeImage();

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
  ITKGrayscaleErodeImage();

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
  ITKGrayscaleErodeImage(const ITKGrayscaleErodeImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKGrayscaleErodeImage&) = delete;            // Move assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKGrayscaleErodeImage_H_ */
