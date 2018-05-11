// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryDilateImage_h_
#define _ITKBinaryDilateImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkBinaryDilateImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBinaryDilateImage class. See [Filter documentation](@ref ITKBinaryDilateImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBinaryDilateImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKBinaryDilateImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)
  PYB11_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_PROPERTY(bool BoundaryToForeground READ getBoundaryToForeground WRITE setBoundaryToForeground)
  PYB11_PROPERTY(FloatVec3_t KernelRadius READ getKernelRadius WRITE setKernelRadius)
  PYB11_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)

public:
  SIMPL_SHARED_POINTERS(ITKBinaryDilateImage)
  SIMPL_FILTER_NEW_MACRO(ITKBinaryDilateImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBinaryDilateImage, AbstractFilter)

  ~ITKBinaryDilateImage() override;

  SIMPL_FILTER_PARAMETER(double, BackgroundValue)
  Q_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)

  SIMPL_FILTER_PARAMETER(double, ForegroundValue)
  Q_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)

  SIMPL_FILTER_PARAMETER(bool, BoundaryToForeground)
  Q_PROPERTY(bool BoundaryToForeground READ getBoundaryToForeground WRITE setBoundaryToForeground)

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
  ITKBinaryDilateImage();

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
  ITKBinaryDilateImage(const ITKBinaryDilateImage&) = delete;    // Copy Constructor Not Implemented
  ITKBinaryDilateImage(ITKBinaryDilateImage&&) = delete;         // Move Constructor Not Implemented
  ITKBinaryDilateImage& operator=(const ITKBinaryDilateImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryDilateImage& operator=(ITKBinaryDilateImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKBinaryDilateImage_H_ */
