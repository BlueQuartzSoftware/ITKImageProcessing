// File automatically generated

/*
 * Your License or Copyright can go here
 */

#pragma once

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
#include <itkBinaryClosingByReconstructionImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBinaryClosingByReconstructionImage class. See [Filter documentation](@ref ITKBinaryClosingByReconstructionImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBinaryClosingByReconstructionImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKBinaryClosingByReconstructionImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(FloatVec3Type KernelRadius READ getKernelRadius WRITE setKernelRadius)
  PYB11_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)

public:
  SIMPL_SHARED_POINTERS(ITKBinaryClosingByReconstructionImage)
  SIMPL_FILTER_NEW_MACRO(ITKBinaryClosingByReconstructionImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBinaryClosingByReconstructionImage, AbstractFilter)

  ~ITKBinaryClosingByReconstructionImage() override;

  SIMPL_FILTER_PARAMETER(double, ForegroundValue)
  Q_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

  SIMPL_FILTER_PARAMETER(FloatVec3Type, KernelRadius)
  Q_PROPERTY(FloatVec3Type KernelRadius READ getKernelRadius WRITE setKernelRadius)

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
  ITKBinaryClosingByReconstructionImage();

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
  ITKBinaryClosingByReconstructionImage(const ITKBinaryClosingByReconstructionImage&) = delete;    // Copy Constructor Not Implemented
  ITKBinaryClosingByReconstructionImage(ITKBinaryClosingByReconstructionImage&&) = delete;         // Move Constructor Not Implemented
  ITKBinaryClosingByReconstructionImage& operator=(const ITKBinaryClosingByReconstructionImage&) = delete; // Copy Assignment Not Implemented
  ITKBinaryClosingByReconstructionImage& operator=(ITKBinaryClosingByReconstructionImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

