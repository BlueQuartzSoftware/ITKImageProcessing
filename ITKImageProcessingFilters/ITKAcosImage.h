// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKAcosImage_h_
#define _ITKAcosImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkAcosImageFilter.h>


/**
 * @brief The ITKAcosImage class. See [Filter documentation](@ref ITKAcosImage) for details.
 */
class ITKAcosImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKAcosImage)
    SIMPL_STATIC_NEW_MACRO(ITKAcosImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKAcosImage, AbstractFilter)

    virtual ~ITKAcosImage();



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
    ITKAcosImage();

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

    ITKAcosImage(const ITKAcosImage&); // Copy Constructor Not Implemented
    void operator=(const ITKAcosImage&); // Operator '=' Not Implemented
};

#endif /* _ITKAcosImage_H_ */