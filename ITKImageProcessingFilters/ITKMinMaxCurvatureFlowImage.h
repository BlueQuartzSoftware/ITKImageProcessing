// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMinMaxCurvatureFlowImage_h_
#define _ITKMinMaxCurvatureFlowImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkMinMaxCurvatureFlowImageFilter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


/**
 * @brief The ITKMinMaxCurvatureFlowImage class. See [Filter documentation](@ref ITKMinMaxCurvatureFlowImage) for details.
 */
class ITKMinMaxCurvatureFlowImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKMinMaxCurvatureFlowImage)
    SIMPL_STATIC_NEW_MACRO(ITKMinMaxCurvatureFlowImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMinMaxCurvatureFlowImage, AbstractFilter)

    virtual ~ITKMinMaxCurvatureFlowImage();

    SIMPL_FILTER_PARAMETER(double, TimeStep)
    Q_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)

    SIMPL_FILTER_PARAMETER(double, NumberOfIterations)
    Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)

    SIMPL_FILTER_PARAMETER(int, StencilRadius)
    Q_PROPERTY(int StencilRadius READ getStencilRadius WRITE setStencilRadius)



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
    ITKMinMaxCurvatureFlowImage();

    /**
     * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
     */
    void virtual dataCheckInternal() override;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename PixelType, unsigned int Dimension>
    void dataCheck();

    /**
    * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
    */
    void virtual filterInternal() override;

    /**
    * @brief Applies the filter
    */
    template<typename PixelType, unsigned int Dimension>
    void filter();

  private:

    ITKMinMaxCurvatureFlowImage(const ITKMinMaxCurvatureFlowImage&); // Copy Constructor Not Implemented
    void operator=(const ITKMinMaxCurvatureFlowImage&); // Operator '=' Not Implemented
};

#endif /* _ITKMinMaxCurvatureFlowImage_H_ */
