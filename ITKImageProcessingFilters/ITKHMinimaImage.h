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
#include <itkHMinimaImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKHMinimaImage class. See [Filter documentation](@ref ITKHMinimaImage) for details.
 */
class ITKImageProcessing_EXPORT ITKHMinimaImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKHMinimaImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKHMinimaImage)
  PYB11_FILTER_NEW_MACRO(ITKHMinimaImage)
  PYB11_FILTER_PARAMETER(double, Height)
  PYB11_FILTER_PARAMETER(bool, FullyConnected)
  PYB11_PROPERTY(double Height READ getHeight WRITE setHeight)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
#endif

public:
    using Self = ITKHMinimaImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKHMinimaImage> New();

    /**
    * @brief Returns the name of the class for ITKHMinimaImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKHMinimaImage
    */
    static QString ClassName();


  ~ITKHMinimaImage() override;

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
  ITKHMinimaImage();

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
  ITKHMinimaImage(const ITKHMinimaImage&) = delete;    // Copy Constructor Not Implemented
  ITKHMinimaImage(ITKHMinimaImage&&) = delete;         // Move Constructor Not Implemented
  ITKHMinimaImage& operator=(const ITKHMinimaImage&) = delete; // Copy Assignment Not Implemented
  ITKHMinimaImage& operator=(ITKHMinimaImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Height = {};
    bool m_FullyConnected = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

