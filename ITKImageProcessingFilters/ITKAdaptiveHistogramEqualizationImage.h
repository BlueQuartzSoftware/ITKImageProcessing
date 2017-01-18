// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKAdaptiveHistogramEqualizationImage_h_
#define _ITKAdaptiveHistogramEqualizationImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkAdaptiveHistogramEqualizationImageFilter.h>
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/FloatFilterParameter.h>


/**
 * @brief The ITKAdaptiveHistogramEqualizationImage class. See [Filter documentation](@ref ITKAdaptiveHistogramEqualizationImage) for details.
 */
class ITKAdaptiveHistogramEqualizationImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKAdaptiveHistogramEqualizationImage)
    SIMPL_STATIC_NEW_MACRO(ITKAdaptiveHistogramEqualizationImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKAdaptiveHistogramEqualizationImage, AbstractFilter)

    virtual ~ITKAdaptiveHistogramEqualizationImage();

    SIMPL_FILTER_PARAMETER(FloatVec3_t, Radius)
    Q_PROPERTY(FloatVec3_t Radius READ getRadius WRITE setRadius)

    SIMPL_FILTER_PARAMETER(float, Alpha)
    Q_PROPERTY(float Alpha READ getAlpha WRITE setAlpha)

    SIMPL_FILTER_PARAMETER(float, Beta)
    Q_PROPERTY(float Beta READ getBeta WRITE setBeta)

    SIMPL_FILTER_PARAMETER(bool, UseLookupTable)
    Q_PROPERTY(bool UseLookupTable READ getUseLookupTable WRITE setUseLookupTable)



    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel() override;

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getSubGroupName() override;
    
    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  protected:
    ITKAdaptiveHistogramEqualizationImage();

    /**
     * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
     */
    void virtual dataCheckInternal() override;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename InputImageType, typename OutputImageType, unsigned int Dimension>
    void dataCheck();

    /**
    * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
    */
    void virtual filterInternal() override;

    /**
    * @brief Applies the filter
    */
    template<typename InputImageType, typename OutputImageType, unsigned int Dimension>
    void filter();

  private:

    ITKAdaptiveHistogramEqualizationImage(const ITKAdaptiveHistogramEqualizationImage&); // Copy Constructor Not Implemented
    void operator=(const ITKAdaptiveHistogramEqualizationImage&); // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKAdaptiveHistogramEqualizationImage_H_ */
