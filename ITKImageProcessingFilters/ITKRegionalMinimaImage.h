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
#include <itkRegionalMinimaImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKRegionalMinimaImage class. See [Filter documentation](@ref ITKRegionalMinimaImage) for details.
 */
class ITKImageProcessing_EXPORT ITKRegionalMinimaImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKRegionalMinimaImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKRegionalMinimaImage)
  PYB11_FILTER_NEW_MACRO(ITKRegionalMinimaImage)
  PYB11_FILTER_PARAMETER(double, BackgroundValue)
  PYB11_FILTER_PARAMETER(double, ForegroundValue)
  PYB11_FILTER_PARAMETER(bool, FullyConnected)
  PYB11_FILTER_PARAMETER(bool, FlatIsMinima)
  PYB11_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)
  PYB11_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(bool FlatIsMinima READ getFlatIsMinima WRITE setFlatIsMinima)
#endif

public:
    using Self = ITKRegionalMinimaImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKRegionalMinimaImage> New();

    /**
    * @brief Returns the name of the class for ITKRegionalMinimaImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKRegionalMinimaImage
    */
    static QString ClassName();


  ~ITKRegionalMinimaImage() override;

    /**
    * @brief Setter property for BackgroundValue
    */
    void setBackgroundValue(double value); 
    /**
    * @brief Getter property for BackgroundValue
    * @return Value of BackgroundValue
    */
    double getBackgroundValue() const;

  Q_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)

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
    * @brief Setter property for FlatIsMinima
    */
    void setFlatIsMinima(bool value); 
    /**
    * @brief Getter property for FlatIsMinima
    * @return Value of FlatIsMinima
    */
    bool getFlatIsMinima() const;

  Q_PROPERTY(bool FlatIsMinima READ getFlatIsMinima WRITE setFlatIsMinima)


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
  ITKRegionalMinimaImage();

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
  ITKRegionalMinimaImage(const ITKRegionalMinimaImage&) = delete;    // Copy Constructor Not Implemented
  ITKRegionalMinimaImage(ITKRegionalMinimaImage&&) = delete;         // Move Constructor Not Implemented
  ITKRegionalMinimaImage& operator=(const ITKRegionalMinimaImage&) = delete; // Copy Assignment Not Implemented
  ITKRegionalMinimaImage& operator=(ITKRegionalMinimaImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_BackgroundValue = {};
    double m_ForegroundValue = {};
    bool m_FullyConnected = {};
    bool m_FlatIsMinima = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

