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
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <itkBoxMeanImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKBoxMeanImage class. See [Filter documentation](@ref ITKBoxMeanImage) for details.
 */
class ITKImageProcessing_EXPORT ITKBoxMeanImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKBoxMeanImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKBoxMeanImage)
  PYB11_FILTER_NEW_MACRO(ITKBoxMeanImage)
  PYB11_PROPERTY(FloatVec3Type Radius READ getRadius WRITE setRadius)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKBoxMeanImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKBoxMeanImage> New();

    /**
    * @brief Returns the name of the class for ITKBoxMeanImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKBoxMeanImage
    */
    static QString ClassName();


  ~ITKBoxMeanImage() override;

    /**
    * @brief Setter property for Radius
    */
    void setRadius(const FloatVec3Type& value); 
    /**
    * @brief Getter property for Radius
    * @return Value of Radius
    */
    FloatVec3Type getRadius() const;
  Q_PROPERTY(FloatVec3Type Radius READ getRadius WRITE setRadius)

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
  ITKBoxMeanImage();

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
  ITKBoxMeanImage(const ITKBoxMeanImage&) = delete;    // Copy Constructor Not Implemented
  ITKBoxMeanImage(ITKBoxMeanImage&&) = delete;         // Move Constructor Not Implemented
  ITKBoxMeanImage& operator=(const ITKBoxMeanImage&) = delete; // Copy Assignment Not Implemented
  ITKBoxMeanImage& operator=(ITKBoxMeanImage&&) = delete;      // Move Assignment Not Implemented

  private:
    FloatVec3Type m_Radius = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

