// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKExpImage_h_
#define _ITKExpImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkExpImageFilter.h>


/**
 * @brief The ITKExpImage class. See [Filter documentation](@ref ITKExpImage) for details.
 */
class ITKExpImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKExpImage)
    SIMPL_STATIC_NEW_MACRO(ITKExpImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKExpImage, AbstractFilter)

    virtual ~ITKExpImage();



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
    ITKExpImage();

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

    ITKExpImage(const ITKExpImage&); // Copy Constructor Not Implemented
    void operator=(const ITKExpImage&); // Operator '=' Not Implemented
};

#endif /* _ITKExpImage_H_ */
