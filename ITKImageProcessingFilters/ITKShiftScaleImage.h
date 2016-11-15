// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKShiftScaleImage_h_
#define _ITKShiftScaleImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkShiftScaleImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


/**
 * @brief The ITKShiftScaleImage class. See [Filter documentation](@ref ITKShiftScaleImage) for details.
 */
class ITKShiftScaleImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKShiftScaleImage)
    SIMPL_STATIC_NEW_MACRO(ITKShiftScaleImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKShiftScaleImage, AbstractFilter)

    virtual ~ITKShiftScaleImage();

    SIMPL_FILTER_PARAMETER(double, Shift)
    Q_PROPERTY(double Shift READ getShift WRITE setShift)

    SIMPL_FILTER_PARAMETER(double, Scale)
    Q_PROPERTY(double Scale READ getScale WRITE setScale)



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
    ITKShiftScaleImage();

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

    ITKShiftScaleImage(const ITKShiftScaleImage&); // Copy Constructor Not Implemented
    void operator=(const ITKShiftScaleImage&); // Operator '=' Not Implemented
};

#endif /* _ITKShiftScaleImage_H_ */
