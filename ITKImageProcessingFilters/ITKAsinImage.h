// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKAsinImage_h_
#define _ITKAsinImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkAsinImageFilter.h>


/**
 * @brief The ITKAsinImage class. See [Filter documentation](@ref ITKAsinImage) for details.
 */
class ITKAsinImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKAsinImage)
    SIMPL_STATIC_NEW_MACRO(ITKAsinImage)
    SIMPL_TYPE_MACRO_SUPER(ITKAsinImage, AbstractFilter)

    virtual ~ITKAsinImage();



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
    ITKAsinImage();

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

    ITKAsinImage(const ITKAsinImage&); // Copy Constructor Not Implemented
    void operator=(const ITKAsinImage&); // Operator '=' Not Implemented
};

#endif /* _ITKAsinImage_H_ */
