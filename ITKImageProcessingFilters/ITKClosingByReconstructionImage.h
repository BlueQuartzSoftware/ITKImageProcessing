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
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkClosingByReconstructionImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKClosingByReconstructionImage class. See [Filter documentation](@ref ITKClosingByReconstructionImage) for details.
 */
class ITKImageProcessing_EXPORT ITKClosingByReconstructionImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKClosingByReconstructionImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKClosingByReconstructionImage)
  PYB11_FILTER_NEW_MACRO(ITKClosingByReconstructionImage)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(bool PreserveIntensities READ getPreserveIntensities WRITE setPreserveIntensities)
  PYB11_PROPERTY(FloatVec3Type KernelRadius READ getKernelRadius WRITE setKernelRadius)
  PYB11_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKClosingByReconstructionImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKClosingByReconstructionImage> New();

    /**
    * @brief Returns the name of the class for ITKClosingByReconstructionImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKClosingByReconstructionImage
    */
    static QString ClassName();


  ~ITKClosingByReconstructionImage() override;

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
    * @brief Setter property for PreserveIntensities
    */
    void setPreserveIntensities(bool value); 
    /**
    * @brief Getter property for PreserveIntensities
    * @return Value of PreserveIntensities
    */
    bool getPreserveIntensities() const;

  Q_PROPERTY(bool PreserveIntensities READ getPreserveIntensities WRITE setPreserveIntensities)

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
  ITKClosingByReconstructionImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheck() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheckImpl();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

public:
  ITKClosingByReconstructionImage(const ITKClosingByReconstructionImage&) = delete;    // Copy Constructor Not Implemented
  ITKClosingByReconstructionImage(ITKClosingByReconstructionImage&&) = delete;         // Move Constructor Not Implemented
  ITKClosingByReconstructionImage& operator=(const ITKClosingByReconstructionImage&) = delete; // Copy Assignment Not Implemented
  ITKClosingByReconstructionImage& operator=(ITKClosingByReconstructionImage&&) = delete;      // Move Assignment Not Implemented

  private:
    bool m_FullyConnected = {};
    bool m_PreserveIntensities = {};
    FloatVec3Type m_KernelRadius = {};
    int m_KernelType = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

