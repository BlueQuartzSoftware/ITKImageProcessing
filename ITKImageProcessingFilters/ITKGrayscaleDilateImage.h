// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGrayscaleDilateImage_h_
#define _ITKGrayscaleDilateImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkGrayscaleDilateImageFilter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>


/**
 * @brief The ITKGrayscaleDilateImage class. See [Filter documentation](@ref ITKGrayscaleDilateImage) for details.
 */
class ITKGrayscaleDilateImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKGrayscaleDilateImage)
    SIMPL_STATIC_NEW_MACRO(ITKGrayscaleDilateImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKGrayscaleDilateImage, AbstractFilter)
    
    virtual ~ITKGrayscaleDilateImage();

    SIMPL_FILTER_PARAMETER(FloatVec3_t, KernelRadius)
    Q_PROPERTY(FloatVec3_t KernelRadius READ getKernelRadius WRITE setKernelRadius)

    SIMPL_FILTER_PARAMETER(int, KernelType)
    Q_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)



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
    ITKGrayscaleDilateImage();

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

    ITKGrayscaleDilateImage(const ITKGrayscaleDilateImage&); // Copy Constructor Not Implemented
    void operator=(const ITKGrayscaleDilateImage&); // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKGrayscaleDilateImage_H_ */
