// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKThresholdMaximumConnectedComponentsImage_h_
#define _ITKThresholdMaximumConnectedComponentsImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkThresholdMaximumConnectedComponentsImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


/**
 * @brief The ITKThresholdMaximumConnectedComponentsImage class. See [Filter documentation](@ref ITKThresholdMaximumConnectedComponentsImage) for details.
 */
class ITKThresholdMaximumConnectedComponentsImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKThresholdMaximumConnectedComponentsImage)
    SIMPL_STATIC_NEW_MACRO(ITKThresholdMaximumConnectedComponentsImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKThresholdMaximumConnectedComponentsImage, AbstractFilter)

    virtual ~ITKThresholdMaximumConnectedComponentsImage();

    SIMPL_FILTER_PARAMETER(double, MinimumObjectSizeInPixels)
    Q_PROPERTY(double MinimumObjectSizeInPixels READ getMinimumObjectSizeInPixels WRITE setMinimumObjectSizeInPixels)

    SIMPL_FILTER_PARAMETER(double, UpperBoundary)
    Q_PROPERTY(double UpperBoundary READ getUpperBoundary WRITE setUpperBoundary)

    SIMPL_FILTER_PARAMETER(int, InsideValue)
    Q_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)

    SIMPL_FILTER_PARAMETER(int, OutsideValue)
    Q_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)



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
    ITKThresholdMaximumConnectedComponentsImage();

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

    ITKThresholdMaximumConnectedComponentsImage(const ITKThresholdMaximumConnectedComponentsImage&); // Copy Constructor Not Implemented
    void operator=(const ITKThresholdMaximumConnectedComponentsImage&); // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKThresholdMaximumConnectedComponentsImage_H_ */
