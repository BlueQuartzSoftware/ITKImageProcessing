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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkGaussianRandomSpatialNeighborSubsampler.h>
#include <itkPatchBasedDenoisingImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKPatchBasedDenoisingImage class. See [Filter documentation](@ref ITKPatchBasedDenoisingImage) for details.
 */
class ITKImageProcessing_EXPORT ITKPatchBasedDenoisingImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKPatchBasedDenoisingImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKPatchBasedDenoisingImage)
  PYB11_FILTER_NEW_MACRO(ITKPatchBasedDenoisingImage)
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
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
  using Self = ITKPatchBasedDenoisingImage;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<ITKPatchBasedDenoisingImage> New();

  /**
   * @brief Returns the name of the class for ITKPatchBasedDenoisingImage
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ITKPatchBasedDenoisingImage
   */
  static QString ClassName();

  ~ITKPatchBasedDenoisingImage() override;

  /**
   * @brief Setter property for KernelBandwidthSigma
   */
  void setKernelBandwidthSigma(double value);
  /**
   * @brief Getter property for KernelBandwidthSigma
   * @return Value of KernelBandwidthSigma
   */
  double getKernelBandwidthSigma() const;
  Q_PROPERTY(double KernelBandwidthSigma READ getKernelBandwidthSigma WRITE setKernelBandwidthSigma)

  /**
   * @brief Setter property for PatchRadius
   */
  void setPatchRadius(double value);
  /**
   * @brief Getter property for PatchRadius
   * @return Value of PatchRadius
   */
  double getPatchRadius() const;
  Q_PROPERTY(double PatchRadius READ getPatchRadius WRITE setPatchRadius)

  /**
   * @brief Setter property for NumberOfIterations
   */
  void setNumberOfIterations(double value);
  /**
   * @brief Getter property for NumberOfIterations
   * @return Value of NumberOfIterations
   */
  double getNumberOfIterations() const;
  Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)

  /**
   * @brief Setter property for NumberOfSamplePatches
   */
  void setNumberOfSamplePatches(double value);
  /**
   * @brief Getter property for NumberOfSamplePatches
   * @return Value of NumberOfSamplePatches
   */
  double getNumberOfSamplePatches() const;
  Q_PROPERTY(double NumberOfSamplePatches READ getNumberOfSamplePatches WRITE setNumberOfSamplePatches)

  /**
   * @brief Setter property for SampleVariance
   */
  void setSampleVariance(double value);
  /**
   * @brief Getter property for SampleVariance
   * @return Value of SampleVariance
   */
  double getSampleVariance() const;
  Q_PROPERTY(double SampleVariance READ getSampleVariance WRITE setSampleVariance)

  /**
   * @brief Setter property for NoiseSigma
   */
  void setNoiseSigma(double value);
  /**
   * @brief Getter property for NoiseSigma
   * @return Value of NoiseSigma
   */
  double getNoiseSigma() const;
  Q_PROPERTY(double NoiseSigma READ getNoiseSigma WRITE setNoiseSigma)

  /**
   * @brief Setter property for NoiseModelFidelityWeight
   */
  void setNoiseModelFidelityWeight(double value);
  /**
   * @brief Getter property for NoiseModelFidelityWeight
   * @return Value of NoiseModelFidelityWeight
   */
  double getNoiseModelFidelityWeight() const;
  Q_PROPERTY(double NoiseModelFidelityWeight READ getNoiseModelFidelityWeight WRITE setNoiseModelFidelityWeight)

  /**
   * @brief Setter property for AlwaysTreatComponentsAsEuclidean
   */
  void setAlwaysTreatComponentsAsEuclidean(bool value);
  /**
   * @brief Getter property for AlwaysTreatComponentsAsEuclidean
   * @return Value of AlwaysTreatComponentsAsEuclidean
   */
  bool getAlwaysTreatComponentsAsEuclidean() const;
  Q_PROPERTY(bool AlwaysTreatComponentsAsEuclidean READ getAlwaysTreatComponentsAsEuclidean WRITE setAlwaysTreatComponentsAsEuclidean)

  /**
   * @brief Setter property for KernelBandwidthEstimation
   */
  void setKernelBandwidthEstimation(bool value);
  /**
   * @brief Getter property for KernelBandwidthEstimation
   * @return Value of KernelBandwidthEstimation
   */
  bool getKernelBandwidthEstimation() const;
  Q_PROPERTY(bool KernelBandwidthEstimation READ getKernelBandwidthEstimation WRITE setKernelBandwidthEstimation)

  /**
   * @brief Setter property for KernelBandwidthMultiplicationFactor
   */
  void setKernelBandwidthMultiplicationFactor(double value);
  /**
   * @brief Getter property for KernelBandwidthMultiplicationFactor
   * @return Value of KernelBandwidthMultiplicationFactor
   */
  double getKernelBandwidthMultiplicationFactor() const;
  Q_PROPERTY(double KernelBandwidthMultiplicationFactor READ getKernelBandwidthMultiplicationFactor WRITE setKernelBandwidthMultiplicationFactor)

  /**
   * @brief Setter property for KernelBandwidthUpdateFrequency
   */
  void setKernelBandwidthUpdateFrequency(double value);
  /**
   * @brief Getter property for KernelBandwidthUpdateFrequency
   * @return Value of KernelBandwidthUpdateFrequency
   */
  double getKernelBandwidthUpdateFrequency() const;
  Q_PROPERTY(double KernelBandwidthUpdateFrequency READ getKernelBandwidthUpdateFrequency WRITE setKernelBandwidthUpdateFrequency)

  /**
   * @brief Setter property for KernelBandwidthFractionPixelsForEstimation
   */
  void setKernelBandwidthFractionPixelsForEstimation(double value);
  /**
   * @brief Getter property for KernelBandwidthFractionPixelsForEstimation
   * @return Value of KernelBandwidthFractionPixelsForEstimation
   */
  double getKernelBandwidthFractionPixelsForEstimation() const;
  Q_PROPERTY(double KernelBandwidthFractionPixelsForEstimation READ getKernelBandwidthFractionPixelsForEstimation WRITE setKernelBandwidthFractionPixelsForEstimation)

  /**
   * @brief Setter property for NoiseModel
   */
  void setNoiseModel(int value);
  /**
   * @brief Getter property for NoiseModel
   * @return Value of NoiseModel
   */
  int getNoiseModel() const;
  Q_PROPERTY(int NoiseModel READ getNoiseModel WRITE setNoiseModel)

  /**
   * @brief Setter property for NumberOfThreads
   */
  void setNumberOfThreads(int value);
  /**
   * @brief Getter property for NumberOfThreads
   * @return Value of NumberOfThreads
   */
  int getNumberOfThreads() const;
  Q_PROPERTY(int NumberOfThreads READ getNumberOfThreads WRITE setNumberOfThreads)

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
  ITKPatchBasedDenoisingImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheck() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension>
  void dataCheckImpl();

  /**
   * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
   */
  void filterInternal() override;

  /**
   * @brief Applies the filter
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension>
  void filter();

public:
  ITKPatchBasedDenoisingImage(const ITKPatchBasedDenoisingImage&) = delete;            // Copy Constructor Not Implemented
  ITKPatchBasedDenoisingImage(ITKPatchBasedDenoisingImage&&) = delete;                 // Move Constructor Not Implemented
  ITKPatchBasedDenoisingImage& operator=(const ITKPatchBasedDenoisingImage&) = delete; // Copy Assignment Not Implemented
  ITKPatchBasedDenoisingImage& operator=(ITKPatchBasedDenoisingImage&&) = delete;      // Move Assignment Not Implemented

private:
  double m_KernelBandwidthSigma = {};
  double m_PatchRadius = {};
  double m_NumberOfIterations = {};
  double m_NumberOfSamplePatches = {};
  double m_SampleVariance = {};
  double m_NoiseSigma = {};
  double m_NoiseModelFidelityWeight = {};
  bool m_AlwaysTreatComponentsAsEuclidean = {};
  bool m_KernelBandwidthEstimation = {};
  double m_KernelBandwidthMultiplicationFactor = {};
  double m_KernelBandwidthUpdateFrequency = {};
  double m_KernelBandwidthFractionPixelsForEstimation = {};
  int m_NoiseModel = {};
  int m_NumberOfThreads = {};
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
