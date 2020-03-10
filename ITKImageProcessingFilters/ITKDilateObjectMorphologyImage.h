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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkDilateObjectMorphologyImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKDilateObjectMorphologyImage class. See [Filter documentation](@ref ITKDilateObjectMorphologyImage) for details.
 */
class ITKImageProcessing_EXPORT ITKDilateObjectMorphologyImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKDilateObjectMorphologyImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKDilateObjectMorphologyImage)
  PYB11_FILTER_NEW_MACRO(ITKDilateObjectMorphologyImage)
  PYB11_FILTER_PARAMETER(double, ObjectValue)
  PYB11_FILTER_PARAMETER(FloatVec3Type, KernelRadius)
  PYB11_FILTER_PARAMETER(int, KernelType)
  PYB11_PROPERTY(double ObjectValue READ getObjectValue WRITE setObjectValue)
  PYB11_PROPERTY(FloatVec3Type KernelRadius READ getKernelRadius WRITE setKernelRadius)
  PYB11_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)
#endif

public:
    using Self = ITKDilateObjectMorphologyImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKDilateObjectMorphologyImage> New();

    /**
    * @brief Returns the name of the class for ITKDilateObjectMorphologyImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKDilateObjectMorphologyImage
    */
    static QString ClassName();


  ~ITKDilateObjectMorphologyImage() override;

    /**
    * @brief Setter property for ObjectValue
    */
    void setObjectValue(double value); 
    /**
    * @brief Getter property for ObjectValue
    * @return Value of ObjectValue
    */
    double getObjectValue() const;

  Q_PROPERTY(double ObjectValue READ getObjectValue WRITE setObjectValue)

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
  ITKDilateObjectMorphologyImage();

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
  ITKDilateObjectMorphologyImage(const ITKDilateObjectMorphologyImage&) = delete;    // Copy Constructor Not Implemented
  ITKDilateObjectMorphologyImage(ITKDilateObjectMorphologyImage&&) = delete;         // Move Constructor Not Implemented
  ITKDilateObjectMorphologyImage& operator=(const ITKDilateObjectMorphologyImage&) = delete; // Copy Assignment Not Implemented
  ITKDilateObjectMorphologyImage& operator=(ITKDilateObjectMorphologyImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_ObjectValue = {};
    FloatVec3Type m_KernelRadius = {};
    int m_KernelType = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

