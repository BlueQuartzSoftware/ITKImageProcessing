// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKAbsImage_h_
#define _ITKAbsImage_h_

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkAbsImageFilter.h>


/**
 * @brief The ITKAbsImage class. See [Filter documentation](@ref ITKAbsImage) for details.
 */
class ITKAbsImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKAbsImage)
    SIMPL_STATIC_NEW_MACRO(ITKAbsImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKAbsImage, AbstractFilter)

    virtual ~ITKAbsImage();



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
    ITKAbsImage();

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

    ITKAbsImage(const ITKAbsImage&); // Copy Constructor Not Implemented
    void operator=(const ITKAbsImage&); // Operator '=' Not Implemented
};

#pragma clang diagnostic pop

#endif /* _ITKAbsImage_H_ */
