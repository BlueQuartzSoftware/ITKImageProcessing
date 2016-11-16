// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBilateralImage_h_
#define _ITKBilateralImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkBilateralImageFilter.h>


/**
 * @brief The ITKBilateralImage class. See [Filter documentation](@ref ITKBilateralImage) for details.
 */
class ITKBilateralImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBilateralImage)
    SIMPL_STATIC_NEW_MACRO(ITKBilateralImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBilateralImage, AbstractFilter)

    virtual ~ITKBilateralImage();

    SIMPL_FILTER_PARAMETER(double, DomainSigma)
    Q_PROPERTY(double DomainSigma READ getDomainSigma WRITE setDomainSigma)

    SIMPL_FILTER_PARAMETER(double, RangeSigma)
    Q_PROPERTY(double RangeSigma READ getRangeSigma WRITE setRangeSigma)

    SIMPL_FILTER_PARAMETER(double, NumberOfRangeGaussianSamples)
    Q_PROPERTY(double NumberOfRangeGaussianSamples READ getNumberOfRangeGaussianSamples WRITE setNumberOfRangeGaussianSamples)



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
    ITKBilateralImage();

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

    ITKBilateralImage(const ITKBilateralImage&); // Copy Constructor Not Implemented
    void operator=(const ITKBilateralImage&); // Operator '=' Not Implemented
};

#endif /* _ITKBilateralImage_H_ */
