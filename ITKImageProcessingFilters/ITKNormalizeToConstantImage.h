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
#include <itkNormalizeToConstantImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKNormalizeToConstantImage class. See [Filter documentation](@ref ITKNormalizeToConstantImage) for details.
 */
class ITKImageProcessing_EXPORT ITKNormalizeToConstantImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKNormalizeToConstantImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKNormalizeToConstantImage)
  PYB11_FILTER_NEW_MACRO(ITKNormalizeToConstantImage)
  PYB11_FILTER_PARAMETER(double, Constant)
  PYB11_PROPERTY(double Constant READ getConstant WRITE setConstant)
#endif

public:
    using Self = ITKNormalizeToConstantImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKNormalizeToConstantImage> New();

    /**
    * @brief Returns the name of the class for ITKNormalizeToConstantImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKNormalizeToConstantImage
    */
    static QString ClassName();


  ~ITKNormalizeToConstantImage() override;

    /**
    * @brief Setter property for Constant
    */
    void setConstant(double value); 
    /**
    * @brief Getter property for Constant
    * @return Value of Constant
    */
    double getConstant() const;

  Q_PROPERTY(double Constant READ getConstant WRITE setConstant)


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
  ITKNormalizeToConstantImage();

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
  ITKNormalizeToConstantImage(const ITKNormalizeToConstantImage&) = delete;    // Copy Constructor Not Implemented
  ITKNormalizeToConstantImage(ITKNormalizeToConstantImage&&) = delete;         // Move Constructor Not Implemented
  ITKNormalizeToConstantImage& operator=(const ITKNormalizeToConstantImage&) = delete; // Copy Assignment Not Implemented
  ITKNormalizeToConstantImage& operator=(ITKNormalizeToConstantImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Constant = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

