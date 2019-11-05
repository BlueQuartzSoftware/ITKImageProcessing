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

#include "ITKImageProcessingBase.h"

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

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKBinaryClosingByReconstructionImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKBinaryClosingByReconstructionImage)
  PYB11_FILTER_NEW_MACRO(ITKBinaryClosingByReconstructionImage)
  PYB11_FILTER_PARAMETER(double, ForegroundValue)
  PYB11_FILTER_PARAMETER(bool, FullyConnected)
  PYB11_FILTER_PARAMETER(FloatVec3Type, KernelRadius)
  PYB11_FILTER_PARAMETER(int, KernelType)
  PYB11_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(FloatVec3Type KernelRadius READ getKernelRadius WRITE setKernelRadius)
  PYB11_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)
#endif

public:
    using Self = ITKBinaryClosingByReconstructionImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKBinaryClosingByReconstructionImage> New();

    /**
    * @brief Returns the name of the class for ITKBinaryClosingByReconstructionImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKBinaryClosingByReconstructionImage
    */
    static QString ClassName();


  ~ITKBinaryClosingByReconstructionImage() override;

    /**
    * @brief Setter property for ForegroundValue
    */
    void setForegroundValue(double value); 
    /**
    * @brief Getter property for ForegroundValue
    * @return Value of ForegroundValue
    */
    double getForegroundValue() const;

  Q_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)

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
    * @brief Setter property for KernelRadius
    */
    void setKernelRadius(const FloatVec3Type& value); 
    /**
    * @brief Getter property for KernelRadius
    * @return Value of KernelRadius
    */
    FloatVec3Type getKernelRadius() const;

  Q_PROPERTY(FloatVec3Type KernelRadius READ getKernelRadius WRITE setKernelRadius)

    /**
    * @brief Setter property for KernelType
    */
    void setKernelType(int value); 
    /**
    * @brief Getter property for KernelType
    * @return Value of KernelType
    */
    int getKernelType() const;

  Q_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)


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

  private:
    double m_ForegroundValue = {};
    bool m_FullyConnected = {};
    FloatVec3Type m_KernelRadius = {};
    int m_KernelType = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

