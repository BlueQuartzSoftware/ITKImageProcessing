// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMaximumProjectionImage_h_
#define _ITKMaximumProjectionImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkMaximumProjectionImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


/**
 * @brief The ITKMaximumProjectionImage class. See [Filter documentation](@ref ITKMaximumProjectionImage) for details.
 */
class ITKMaximumProjectionImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKMaximumProjectionImage)
    SIMPL_STATIC_NEW_MACRO(ITKMaximumProjectionImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMaximumProjectionImage, AbstractFilter)

    virtual ~ITKMaximumProjectionImage();

    SIMPL_FILTER_PARAMETER(double, ProjectionDimension)
    Q_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)



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
    ITKMaximumProjectionImage();

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

    ITKMaximumProjectionImage(const ITKMaximumProjectionImage&); // Copy Constructor Not Implemented
    void operator=(const ITKMaximumProjectionImage&); // Operator '=' Not Implemented
};

#endif /* _ITKMaximumProjectionImage_H_ */