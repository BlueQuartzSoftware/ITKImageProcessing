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
#include <itkShiftScaleImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKShiftScaleImage class. See [Filter documentation](@ref ITKShiftScaleImage) for details.
 */
class ITKImageProcessing_EXPORT ITKShiftScaleImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKShiftScaleImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKShiftScaleImage)
  PYB11_FILTER_NEW_MACRO(ITKShiftScaleImage)
  PYB11_FILTER_PARAMETER(double, Shift)
  PYB11_FILTER_PARAMETER(double, Scale)
  PYB11_PROPERTY(double Shift READ getShift WRITE setShift)
  PYB11_PROPERTY(double Scale READ getScale WRITE setScale)
#endif

public:
    using Self = ITKShiftScaleImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKShiftScaleImage> New();

    /**
    * @brief Returns the name of the class for ITKShiftScaleImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKShiftScaleImage
    */
    static QString ClassName();


  ~ITKShiftScaleImage() override;

    /**
    * @brief Setter property for Shift
    */
    void setShift(double value); 
    /**
    * @brief Getter property for Shift
    * @return Value of Shift
    */
    double getShift() const;

  Q_PROPERTY(double Shift READ getShift WRITE setShift)

    /**
    * @brief Setter property for Scale
    */
    void setScale(double value); 
    /**
    * @brief Getter property for Scale
    * @return Value of Scale
    */
    double getScale() const;

  Q_PROPERTY(double Scale READ getScale WRITE setScale)


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
  ITKShiftScaleImage();

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
  ITKShiftScaleImage(const ITKShiftScaleImage&) = delete;    // Copy Constructor Not Implemented
  ITKShiftScaleImage(ITKShiftScaleImage&&) = delete;         // Move Constructor Not Implemented
  ITKShiftScaleImage& operator=(const ITKShiftScaleImage&) = delete; // Copy Assignment Not Implemented
  ITKShiftScaleImage& operator=(ITKShiftScaleImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Shift = {};
    double m_Scale = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

