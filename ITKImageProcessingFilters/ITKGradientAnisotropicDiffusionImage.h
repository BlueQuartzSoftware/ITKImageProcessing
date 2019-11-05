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
#include <algorithm>
#include <itkGradientAnisotropicDiffusionImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKGradientAnisotropicDiffusionImage class. See [Filter documentation](@ref ITKGradientAnisotropicDiffusionImage) for details.
 */
class ITKImageProcessing_EXPORT ITKGradientAnisotropicDiffusionImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKGradientAnisotropicDiffusionImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKGradientAnisotropicDiffusionImage)
  PYB11_FILTER_NEW_MACRO(ITKGradientAnisotropicDiffusionImage)
  PYB11_FILTER_PARAMETER(double, TimeStep)
  PYB11_FILTER_PARAMETER(double, ConductanceParameter)
  PYB11_FILTER_PARAMETER(double, ConductanceScalingUpdateInterval)
  PYB11_FILTER_PARAMETER(double, NumberOfIterations)
  PYB11_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)
  PYB11_PROPERTY(double ConductanceParameter READ getConductanceParameter WRITE setConductanceParameter)
  PYB11_PROPERTY(double ConductanceScalingUpdateInterval READ getConductanceScalingUpdateInterval WRITE setConductanceScalingUpdateInterval)
  PYB11_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)
#endif

public:
    using Self = ITKGradientAnisotropicDiffusionImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKGradientAnisotropicDiffusionImage> New();

    /**
    * @brief Returns the name of the class for ITKGradientAnisotropicDiffusionImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKGradientAnisotropicDiffusionImage
    */
    static QString ClassName();


  ~ITKGradientAnisotropicDiffusionImage() override;

    /**
    * @brief Setter property for TimeStep
    */
    void setTimeStep(double value); 
    /**
    * @brief Getter property for TimeStep
    * @return Value of TimeStep
    */
    double getTimeStep() const;

  Q_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)

    /**
    * @brief Setter property for ConductanceParameter
    */
    void setConductanceParameter(double value); 
    /**
    * @brief Getter property for ConductanceParameter
    * @return Value of ConductanceParameter
    */
    double getConductanceParameter() const;

  Q_PROPERTY(double ConductanceParameter READ getConductanceParameter WRITE setConductanceParameter)

    /**
    * @brief Setter property for ConductanceScalingUpdateInterval
    */
    void setConductanceScalingUpdateInterval(double value); 
    /**
    * @brief Getter property for ConductanceScalingUpdateInterval
    * @return Value of ConductanceScalingUpdateInterval
    */
    double getConductanceScalingUpdateInterval() const;

  Q_PROPERTY(double ConductanceScalingUpdateInterval READ getConductanceScalingUpdateInterval WRITE setConductanceScalingUpdateInterval)

    /**
    * @brief Setter property for NumberOfIterations
    */
    void setNumberOfIterations(double value); 
    /**
    * @brief Getter property for NumberOfIterations
    * @return Value of NumberOfIterations
    */
    double getNumberOfIterations() const;

  Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)


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
  ITKGradientAnisotropicDiffusionImage();

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
  ITKGradientAnisotropicDiffusionImage(const ITKGradientAnisotropicDiffusionImage&) = delete;    // Copy Constructor Not Implemented
  ITKGradientAnisotropicDiffusionImage(ITKGradientAnisotropicDiffusionImage&&) = delete;         // Move Constructor Not Implemented
  ITKGradientAnisotropicDiffusionImage& operator=(const ITKGradientAnisotropicDiffusionImage&) = delete; // Copy Assignment Not Implemented
  ITKGradientAnisotropicDiffusionImage& operator=(ITKGradientAnisotropicDiffusionImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_TimeStep = {};
    double m_ConductanceParameter = {};
    double m_ConductanceScalingUpdateInterval = {};
    double m_NumberOfIterations = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

