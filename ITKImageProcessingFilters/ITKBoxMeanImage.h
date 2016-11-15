// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBoxMeanImage_h_
#define _ITKBoxMeanImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkBoxMeanImageFilter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>


/**
 * @brief The ITKBoxMeanImage class. See [Filter documentation](@ref ITKBoxMeanImage) for details.
 */
class ITKBoxMeanImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBoxMeanImage)
    SIMPL_STATIC_NEW_MACRO(ITKBoxMeanImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKBoxMeanImage, AbstractFilter)

    virtual ~ITKBoxMeanImage();

    SIMPL_FILTER_PARAMETER(FloatVec3_t, Radius)
    Q_PROPERTY(FloatVec3_t Radius READ getRadius WRITE setRadius)



    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel() override;

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  protected:
    ITKBoxMeanImage();

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

    ITKBoxMeanImage(const ITKBoxMeanImage&); // Copy Constructor Not Implemented
    void operator=(const ITKBoxMeanImage&); // Operator '=' Not Implemented
};

#endif /* _ITKBoxMeanImage_H_ */
