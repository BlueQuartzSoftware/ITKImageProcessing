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
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkMinMaxCurvatureFlowImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKMinMaxCurvatureFlowImage class. See [Filter documentation](@ref ITKMinMaxCurvatureFlowImage) for details.
 */
class ITKImageProcessing_EXPORT ITKMinMaxCurvatureFlowImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKMinMaxCurvatureFlowImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKMinMaxCurvatureFlowImage)
  PYB11_FILTER_NEW_MACRO(ITKMinMaxCurvatureFlowImage)
  PYB11_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)
  PYB11_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)
  PYB11_PROPERTY(int StencilRadius READ getStencilRadius WRITE setStencilRadius)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKMinMaxCurvatureFlowImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKMinMaxCurvatureFlowImage> New();

    /**
    * @brief Returns the name of the class for ITKMinMaxCurvatureFlowImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKMinMaxCurvatureFlowImage
    */
    static QString ClassName();


  ~ITKMinMaxCurvatureFlowImage() override;

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
    * @brief Setter property for StencilRadius
    */
    void setStencilRadius(int value); 
    /**
    * @brief Getter property for StencilRadius
    * @return Value of StencilRadius
    */
    int getStencilRadius() const;
  Q_PROPERTY(int StencilRadius READ getStencilRadius WRITE setStencilRadius)


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
  ITKMinMaxCurvatureFlowImage();

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
  ITKMinMaxCurvatureFlowImage(const ITKMinMaxCurvatureFlowImage&) = delete;    // Copy Constructor Not Implemented
  ITKMinMaxCurvatureFlowImage(ITKMinMaxCurvatureFlowImage&&) = delete;         // Move Constructor Not Implemented
  ITKMinMaxCurvatureFlowImage& operator=(const ITKMinMaxCurvatureFlowImage&) = delete; // Copy Assignment Not Implemented
  ITKMinMaxCurvatureFlowImage& operator=(ITKMinMaxCurvatureFlowImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_TimeStep = {};
    double m_NumberOfIterations = {};
    int m_StencilRadius = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

