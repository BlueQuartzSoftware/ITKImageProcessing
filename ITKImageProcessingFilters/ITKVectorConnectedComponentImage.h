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

#include <itkVectorConnectedComponentImageFilter.h>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DoubleFilterParameter.h"
#include "SIMPLib/SIMPLib.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"
#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImageProcessingBase.h"

/**
 * @brief The ITKVectorConnectedComponentImage class. See [Filter documentation](@ref ITKVectorConnectedComponentImage) for details.
 */
class ITKImageProcessing_EXPORT ITKVectorConnectedComponentImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKVectorConnectedComponentImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKVectorConnectedComponentImage)
  PYB11_FILTER_NEW_MACRO(ITKVectorConnectedComponentImage)
  PYB11_FILTER_PARAMETER(double, DistanceThreshold)
  PYB11_FILTER_PARAMETER(bool, FullyConnected)
  PYB11_PROPERTY(double DistanceThreshold READ getDistanceThreshold WRITE setDistanceThreshold)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
#endif

public:
    using Self = ITKVectorConnectedComponentImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKVectorConnectedComponentImage> New();

    /**
    * @brief Returns the name of the class for ITKVectorConnectedComponentImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKVectorConnectedComponentImage
    */
    static QString ClassName();


  ~ITKVectorConnectedComponentImage() override;

    /**
    * @brief Setter property for DistanceThreshold
    */
    void setDistanceThreshold(double value); 
    /**
    * @brief Getter property for DistanceThreshold
    * @return Value of DistanceThreshold
    */
    double getDistanceThreshold() const;

  Q_PROPERTY(double DistanceThreshold READ getDistanceThreshold WRITE setDistanceThreshold)

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
  ITKVectorConnectedComponentImage();

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
  ITKVectorConnectedComponentImage(const ITKVectorConnectedComponentImage&) = delete;    // Copy Constructor Not Implemented
  ITKVectorConnectedComponentImage(ITKVectorConnectedComponentImage&&) = delete;         // Move Constructor Not Implemented
  ITKVectorConnectedComponentImage& operator=(const ITKVectorConnectedComponentImage&) = delete; // Copy Assignment Not Implemented
  ITKVectorConnectedComponentImage& operator=(ITKVectorConnectedComponentImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_DistanceThreshold = {};
    bool m_FullyConnected = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

