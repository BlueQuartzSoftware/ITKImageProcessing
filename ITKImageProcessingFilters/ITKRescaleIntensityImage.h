// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKRescaleIntensityImage_h_
#define _ITKRescaleIntensityImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkRescaleIntensityImageFilter.h>


/**
 * @brief The ITKRescaleIntensityImage class. See [Filter documentation](@ref ITKRescaleIntensityImage) for details.
 */
class ITKRescaleIntensityImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKRescaleIntensityImage)
    SIMPL_STATIC_NEW_MACRO(ITKRescaleIntensityImage)
    SIMPL_TYPE_MACRO_SUPER(ITKRescaleIntensityImage, AbstractFilter)

    virtual ~ITKRescaleIntensityImage();

    SIMPL_FILTER_PARAMETER(double, OutputMinimum)
    Q_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)

    SIMPL_FILTER_PARAMETER(double, OutputMaximum)
    Q_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)



    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel();

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters();

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

  protected:
    ITKRescaleIntensityImage();

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

    ITKRescaleIntensityImage(const ITKRescaleIntensityImage&); // Copy Constructor Not Implemented
    void operator=(const ITKRescaleIntensityImage&); // Operator '=' Not Implemented
};

#endif /* _ITKRescaleIntensityImage_H_ */
