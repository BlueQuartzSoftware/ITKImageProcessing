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

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKMultiScaleHessianBasedObjectnessImage class. See [Filter documentation](@ref ITKMultiScaleHessianBasedObjectnessImage) for details.
 */
class ITKImageProcessing_EXPORT ITKMultiScaleHessianBasedObjectnessImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKMultiScaleHessianBasedObjectnessImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKMultiScaleHessianBasedObjectnessImage)
  PYB11_FILTER_NEW_MACRO(ITKMultiScaleHessianBasedObjectnessImage)
  PYB11_FILTER_PARAMETER(int, ObjectDimension)
  PYB11_FILTER_PARAMETER(double, Alpha)
  PYB11_FILTER_PARAMETER(double, Beta)
  PYB11_FILTER_PARAMETER(double, Gamma)
  PYB11_FILTER_PARAMETER(bool, ScaleObjectnessMeasure)
  PYB11_FILTER_PARAMETER(bool, BrightObject)
  PYB11_FILTER_PARAMETER(double, SigmaMinimum)
  PYB11_FILTER_PARAMETER(double, SigmaMaximum)
  PYB11_FILTER_PARAMETER(double, NumberOfSigmaSteps)
  PYB11_PROPERTY(int ObjectDimension READ getObjectDimension WRITE setObjectDimension)
  PYB11_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)
  PYB11_PROPERTY(double Beta READ getBeta WRITE setBeta)
  PYB11_PROPERTY(double Gamma READ getGamma WRITE setGamma)
  PYB11_PROPERTY(bool ScaleObjectnessMeasure READ getScaleObjectnessMeasure WRITE setScaleObjectnessMeasure)
  PYB11_PROPERTY(bool BrightObject READ getBrightObject WRITE setBrightObject)
  PYB11_PROPERTY(double SigmaMinimum READ getSigmaMinimum WRITE setSigmaMinimum)
  PYB11_PROPERTY(double SigmaMaximum READ getSigmaMaximum WRITE setSigmaMaximum)
  PYB11_PROPERTY(double NumberOfSigmaSteps READ getNumberOfSigmaSteps WRITE setNumberOfSigmaSteps)
#endif

public:
    using Self = ITKMultiScaleHessianBasedObjectnessImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKMultiScaleHessianBasedObjectnessImage> New();

    /**
    * @brief Returns the name of the class for ITKMultiScaleHessianBasedObjectnessImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKMultiScaleHessianBasedObjectnessImage
    */
    static QString ClassName();


  ~ITKMultiScaleHessianBasedObjectnessImage() override;

    /**
    * @brief Setter property for ObjectDimension
    */
    void setObjectDimension(int value); 
    /**
    * @brief Getter property for ObjectDimension
    * @return Value of ObjectDimension
    */
    int getObjectDimension() const;

  Q_PROPERTY(int ObjectDimension READ getObjectDimension WRITE setObjectDimension)

    /**
    * @brief Setter property for Alpha
    */
    void setAlpha(double value); 
    /**
    * @brief Getter property for Alpha
    * @return Value of Alpha
    */
    double getAlpha() const;

  Q_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)

    /**
    * @brief Setter property for Beta
    */
    void setBeta(double value); 
    /**
    * @brief Getter property for Beta
    * @return Value of Beta
    */
    double getBeta() const;

  Q_PROPERTY(double Beta READ getBeta WRITE setBeta)

    /**
    * @brief Setter property for Gamma
    */
    void setGamma(double value); 
    /**
    * @brief Getter property for Gamma
    * @return Value of Gamma
    */
    double getGamma() const;

  Q_PROPERTY(double Gamma READ getGamma WRITE setGamma)

    /**
    * @brief Setter property for ScaleObjectnessMeasure
    */
    void setScaleObjectnessMeasure(bool value); 
    /**
    * @brief Getter property for ScaleObjectnessMeasure
    * @return Value of ScaleObjectnessMeasure
    */
    bool getScaleObjectnessMeasure() const;

  Q_PROPERTY(bool ScaleObjectnessMeasure READ getScaleObjectnessMeasure WRITE setScaleObjectnessMeasure)

    /**
    * @brief Setter property for BrightObject
    */
    void setBrightObject(bool value); 
    /**
    * @brief Getter property for BrightObject
    * @return Value of BrightObject
    */
    bool getBrightObject() const;

  Q_PROPERTY(bool BrightObject READ getBrightObject WRITE setBrightObject)

    /**
    * @brief Setter property for SigmaMinimum
    */
    void setSigmaMinimum(double value); 
    /**
    * @brief Getter property for SigmaMinimum
    * @return Value of SigmaMinimum
    */
    double getSigmaMinimum() const;

  Q_PROPERTY(double SigmaMinimum READ getSigmaMinimum WRITE setSigmaMinimum)

    /**
    * @brief Setter property for SigmaMaximum
    */
    void setSigmaMaximum(double value); 
    /**
    * @brief Getter property for SigmaMaximum
    * @return Value of SigmaMaximum
    */
    double getSigmaMaximum() const;

  Q_PROPERTY(double SigmaMaximum READ getSigmaMaximum WRITE setSigmaMaximum)

    /**
    * @brief Setter property for NumberOfSigmaSteps
    */
    void setNumberOfSigmaSteps(double value); 
    /**
    * @brief Getter property for NumberOfSigmaSteps
    * @return Value of NumberOfSigmaSteps
    */
    double getNumberOfSigmaSteps() const;

  Q_PROPERTY(double NumberOfSigmaSteps READ getNumberOfSigmaSteps WRITE setNumberOfSigmaSteps)

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
  ITKMultiScaleHessianBasedObjectnessImage();

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
  ITKMultiScaleHessianBasedObjectnessImage(const ITKMultiScaleHessianBasedObjectnessImage&) = delete; // Copy Constructor Not Implemented
  ITKMultiScaleHessianBasedObjectnessImage(ITKMultiScaleHessianBasedObjectnessImage&&) = delete;      // Move Constructor Not Implemented
  ITKMultiScaleHessianBasedObjectnessImage& operator=(const ITKMultiScaleHessianBasedObjectnessImage&) = delete; // Copy Assignment Not Implemented
  ITKMultiScaleHessianBasedObjectnessImage& operator=(ITKMultiScaleHessianBasedObjectnessImage&&) = delete;      // Move Assignment Not Implemented

  private:
    int m_ObjectDimension = {};
    double m_Alpha = {};
    double m_Beta = {};
    double m_Gamma = {};
    bool m_ScaleObjectnessMeasure = {};
    bool m_BrightObject = {};
    double m_SigmaMinimum = {};
    double m_SigmaMaximum = {};
    double m_NumberOfSigmaSteps = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

