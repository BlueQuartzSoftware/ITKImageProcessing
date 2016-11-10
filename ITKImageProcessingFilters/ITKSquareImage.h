// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSquareImage_h_
#define _ITKSquareImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkSquareImageFilter.h>


/**
 * @brief The ITKSquareImage class. See [Filter documentation](@ref ITKSquareImage) for details.
 */
class ITKSquareImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKSquareImage)
    SIMPL_STATIC_NEW_MACRO(ITKSquareImage)
    SIMPL_TYPE_MACRO_SUPER(ITKSquareImage, AbstractFilter)

    virtual ~ITKSquareImage();



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
    ITKSquareImage();

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

    ITKSquareImage(const ITKSquareImage&); // Copy Constructor Not Implemented
    void operator=(const ITKSquareImage&); // Operator '=' Not Implemented
};

#endif /* _ITKSquareImage_H_ */
