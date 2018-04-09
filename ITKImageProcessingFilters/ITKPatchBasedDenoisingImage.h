/*
 * Your License or Copyright can go here
 */

#ifndef _ITKPatchBasedDenoisingImage_h_
#define _ITKPatchBasedDenoisingImage_h_

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
#include <itkGaussianRandomSpatialNeighborSubsampler.h>
#include <itkPatchBasedDenoisingImageFilter.h>

/**
 * @brief The ITKPatchBasedDenoisingImage class. See [Filter documentation](@ref ITKPatchBasedDenoisingImage) for details.
 */
class ITKPatchBasedDenoisingImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKPatchBasedDenoisingImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double KernelBandwidthSigma READ getKernelBandwidthSigma WRITE setKernelBandwidthSigma)
  PYB11_PROPERTY(double PatchRadius READ getPatchRadius WRITE setPatchRadius)
  PYB11_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)
  PYB11_PROPERTY(double NumberOfSamplePatches READ getNumberOfSamplePatches WRITE setNumberOfSamplePatches)
  PYB11_PROPERTY(double SampleVariance READ getSampleVariance WRITE setSampleVariance)
  PYB11_PROPERTY(double NoiseSigma READ getNoiseSigma WRITE setNoiseSigma)
  PYB11_PROPERTY(double NoiseModelFidelityWeight READ getNoiseModelFidelityWeight WRITE setNoiseModelFidelityWeight)
  PYB11_PROPERTY(bool AlwaysTreatComponentsAsEuclidean READ getAlwaysTreatComponentsAsEuclidean WRITE setAlwaysTreatComponentsAsEuclidean)
  PYB11_PROPERTY(bool KernelBandwidthEstimation READ getKernelBandwidthEstimation WRITE setKernelBandwidthEstimation)
  PYB11_PROPERTY(double KernelBandwidthMultiplicationFactor READ getKernelBandwidthMultiplicationFactor WRITE setKernelBandwidthMultiplicationFactor)
  PYB11_PROPERTY(double KernelBandwidthUpdateFrequency READ getKernelBandwidthUpdateFrequency WRITE setKernelBandwidthUpdateFrequency)
  PYB11_PROPERTY(double KernelBandwidthFractionPixelsForEstimation READ getKernelBandwidthFractionPixelsForEstimation WRITE setKernelBandwidthFractionPixelsForEstimation)
  PYB11_PROPERTY(int NoiseModel READ getNoiseModel WRITE setNoiseModel)
  PYB11_PROPERTY(int NumberOfThreads READ getNumberOfThreads WRITE setNumberOfThreads)

public:
  SIMPL_SHARED_POINTERS(ITKPatchBasedDenoisingImage)
  SIMPL_FILTER_NEW_MACRO(ITKPatchBasedDenoisingImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKPatchBasedDenoisingImage, AbstractFilter)

  virtual ~ITKPatchBasedDenoisingImage();

  SIMPL_FILTER_PARAMETER(double, KernelBandwidthSigma)
  Q_PROPERTY(double KernelBandwidthSigma READ getKernelBandwidthSigma WRITE setKernelBandwidthSigma)

  SIMPL_FILTER_PARAMETER(double, PatchRadius)
  Q_PROPERTY(double PatchRadius READ getPatchRadius WRITE setPatchRadius)

  SIMPL_FILTER_PARAMETER(double, NumberOfIterations)
  Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)

  SIMPL_FILTER_PARAMETER(double, NumberOfSamplePatches)
  Q_PROPERTY(double NumberOfSamplePatches READ getNumberOfSamplePatches WRITE setNumberOfSamplePatches)

  SIMPL_FILTER_PARAMETER(double, SampleVariance)
  Q_PROPERTY(double SampleVariance READ getSampleVariance WRITE setSampleVariance)

  SIMPL_FILTER_PARAMETER(double, NoiseSigma)
  Q_PROPERTY(double NoiseSigma READ getNoiseSigma WRITE setNoiseSigma)

  SIMPL_FILTER_PARAMETER(double, NoiseModelFidelityWeight)
  Q_PROPERTY(double NoiseModelFidelityWeight READ getNoiseModelFidelityWeight WRITE setNoiseModelFidelityWeight)

  SIMPL_FILTER_PARAMETER(bool, AlwaysTreatComponentsAsEuclidean)
  Q_PROPERTY(bool AlwaysTreatComponentsAsEuclidean READ getAlwaysTreatComponentsAsEuclidean WRITE setAlwaysTreatComponentsAsEuclidean)

  SIMPL_FILTER_PARAMETER(bool, KernelBandwidthEstimation)
  Q_PROPERTY(bool KernelBandwidthEstimation READ getKernelBandwidthEstimation WRITE setKernelBandwidthEstimation)

  SIMPL_FILTER_PARAMETER(double, KernelBandwidthMultiplicationFactor)
  Q_PROPERTY(double KernelBandwidthMultiplicationFactor READ getKernelBandwidthMultiplicationFactor WRITE setKernelBandwidthMultiplicationFactor)

  SIMPL_FILTER_PARAMETER(double, KernelBandwidthUpdateFrequency)
  Q_PROPERTY(double KernelBandwidthUpdateFrequency READ getKernelBandwidthUpdateFrequency WRITE setKernelBandwidthUpdateFrequency)

  SIMPL_FILTER_PARAMETER(double, KernelBandwidthFractionPixelsForEstimation)
  Q_PROPERTY(double KernelBandwidthFractionPixelsForEstimation READ getKernelBandwidthFractionPixelsForEstimation WRITE setKernelBandwidthFractionPixelsForEstimation)

  SIMPL_FILTER_PARAMETER(int, NoiseModel)
  Q_PROPERTY(int NoiseModel READ getNoiseModel WRITE setNoiseModel)

  SIMPL_FILTER_PARAMETER(int, NumberOfThreads)
  Q_PROPERTY(int NumberOfThreads READ getNumberOfThreads WRITE setNumberOfThreads)

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
  ITKPatchBasedDenoisingImage();

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
  ITKPatchBasedDenoisingImage(const ITKPatchBasedDenoisingImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKPatchBasedDenoisingImage&);                       // Move assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKPatchBasedDenoisingImage_H_ */
