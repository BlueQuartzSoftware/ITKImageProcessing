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
#include <itkCurvatureFlowImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKCurvatureFlowImage class. See [Filter documentation](@ref ITKCurvatureFlowImage) for details.
 */
class ITKImageProcessing_EXPORT ITKCurvatureFlowImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKCurvatureFlowImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKCurvatureFlowImage)
  PYB11_FILTER_NEW_MACRO(ITKCurvatureFlowImage)
  PYB11_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)
  PYB11_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKCurvatureFlowImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKCurvatureFlowImage> New();

    /**
    * @brief Returns the name of the class for ITKCurvatureFlowImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKCurvatureFlowImage
    */
    static QString ClassName();


  ~ITKCurvatureFlowImage() override;

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
  ITKCurvatureFlowImage();

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
  ITKCurvatureFlowImage(const ITKCurvatureFlowImage&) = delete;    // Copy Constructor Not Implemented
  ITKCurvatureFlowImage(ITKCurvatureFlowImage&&) = delete;         // Move Constructor Not Implemented
  ITKCurvatureFlowImage& operator=(const ITKCurvatureFlowImage&) = delete; // Copy Assignment Not Implemented
  ITKCurvatureFlowImage& operator=(ITKCurvatureFlowImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_TimeStep = {};
    double m_NumberOfIterations = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

