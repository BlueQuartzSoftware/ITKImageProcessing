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
#include <itkHMaximaImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKHMaximaImage class. See [Filter documentation](@ref ITKHMaximaImage) for details.
 */
class ITKImageProcessing_EXPORT ITKHMaximaImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKHMaximaImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKHMaximaImage)
  PYB11_FILTER_NEW_MACRO(ITKHMaximaImage)
  PYB11_FILTER_PARAMETER(double, Height)
  PYB11_PROPERTY(double Height READ getHeight WRITE setHeight)
#endif

public:
    using Self = ITKHMaximaImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKHMaximaImage> New();

    /**
    * @brief Returns the name of the class for ITKHMaximaImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKHMaximaImage
    */
    static QString ClassName();


  ~ITKHMaximaImage() override;

    /**
    * @brief Setter property for Height
    */
    void setHeight(double value); 
    /**
    * @brief Getter property for Height
    * @return Value of Height
    */
    double getHeight() const;

  Q_PROPERTY(double Height READ getHeight WRITE setHeight)


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
  ITKHMaximaImage();

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
  ITKHMaximaImage(const ITKHMaximaImage&) = delete;    // Copy Constructor Not Implemented
  ITKHMaximaImage(ITKHMaximaImage&&) = delete;         // Move Constructor Not Implemented
  ITKHMaximaImage& operator=(const ITKHMaximaImage&) = delete; // Copy Assignment Not Implemented
  ITKHMaximaImage& operator=(ITKHMaximaImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Height = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

