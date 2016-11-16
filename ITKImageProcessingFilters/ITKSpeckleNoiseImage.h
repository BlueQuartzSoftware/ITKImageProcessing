// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSpeckleNoiseImage_h_
#define _ITKSpeckleNoiseImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkSpeckleNoiseImageFilter.h>


/**
 * @brief The ITKSpeckleNoiseImage class. See [Filter documentation](@ref ITKSpeckleNoiseImage) for details.
 */
class ITKSpeckleNoiseImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKSpeckleNoiseImage)
    SIMPL_STATIC_NEW_MACRO(ITKSpeckleNoiseImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKSpeckleNoiseImage, AbstractFilter)

    virtual ~ITKSpeckleNoiseImage();

    SIMPL_FILTER_PARAMETER(double, StandardDeviation)
    Q_PROPERTY(double StandardDeviation READ getStandardDeviation WRITE setStandardDeviation)

    SIMPL_FILTER_PARAMETER(double, Seed)
    Q_PROPERTY(double Seed READ getSeed WRITE setSeed)



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
    ITKSpeckleNoiseImage();

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

    ITKSpeckleNoiseImage(const ITKSpeckleNoiseImage&); // Copy Constructor Not Implemented
    void operator=(const ITKSpeckleNoiseImage&); // Operator '=' Not Implemented
};

#endif /* _ITKSpeckleNoiseImage_H_ */
