/*
 * Your License or Copyright can go here
 */

#ifndef _ITKPatchBasedDenoisingImage_h_
#define _ITKPatchBasedDenoisingImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkGaussianRandomSpatialNeighborSubsampler.h>
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkPatchBasedDenoisingImageFilter.h>


/**
 * @brief The ITKPatchBasedDenoisingImage class. See [Filter documentation](@ref ITKPatchBasedDenoisingImage) for details.
 */
class ITKPatchBasedDenoisingImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKPatchBasedDenoisingImage)
    SIMPL_STATIC_NEW_MACRO(ITKPatchBasedDenoisingImage)
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
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel() override;

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getSubGroupName() override;

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  protected:
    ITKPatchBasedDenoisingImage();

    /**
     * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
     */
    void virtual dataCheckInternal() override;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename InputImageType, typename OutputImageType, unsigned int Dimension>
    void dataCheck();

    /**
    * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
    */
    void virtual filterInternal() override;

    /**
    * @brief Applies the filter
    */
    template<typename InputImageType, typename OutputImageType, unsigned int Dimension>
    void filter();

  private:

    ITKPatchBasedDenoisingImage(const ITKPatchBasedDenoisingImage&); // Copy Constructor Not Implemented
    void operator=(const ITKPatchBasedDenoisingImage&); // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKPatchBasedDenoisingImage_H_ */
