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
#include <itkValuedRegionalMaximaImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKValuedRegionalMaximaImage class. See [Filter documentation](@ref ITKValuedRegionalMaximaImage) for details.
 */
class ITKImageProcessing_EXPORT ITKValuedRegionalMaximaImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKValuedRegionalMaximaImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKValuedRegionalMaximaImage)
  PYB11_FILTER_NEW_MACRO(ITKValuedRegionalMaximaImage)
  PYB11_FILTER_PARAMETER(bool, FullyConnected)
  PYB11_FILTER_PARAMETER(bool, Flat)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(bool Flat READ getFlat WRITE setFlat)
#endif

public:
    using Self = ITKValuedRegionalMaximaImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKValuedRegionalMaximaImage> New();

    /**
    * @brief Returns the name of the class for ITKValuedRegionalMaximaImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKValuedRegionalMaximaImage
    */
    static QString ClassName();


  ~ITKValuedRegionalMaximaImage() override;

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
    * @brief Setter property for Flat
    */
    void setFlat(bool value); 
    /**
    * @brief Getter property for Flat
    * @return Value of Flat
    */
    bool getFlat() const;

  Q_PROPERTY(bool Flat READ getFlat)


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
  ITKValuedRegionalMaximaImage();

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
  ITKValuedRegionalMaximaImage(const ITKValuedRegionalMaximaImage&) = delete;    // Copy Constructor Not Implemented
  ITKValuedRegionalMaximaImage(ITKValuedRegionalMaximaImage&&) = delete;         // Move Constructor Not Implemented
  ITKValuedRegionalMaximaImage& operator=(const ITKValuedRegionalMaximaImage&) = delete; // Copy Assignment Not Implemented
  ITKValuedRegionalMaximaImage& operator=(ITKValuedRegionalMaximaImage&&) = delete;      // Move Assignment Not Implemented

  private:
    bool m_FullyConnected = {};
    bool m_Flat = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

