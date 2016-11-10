// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMinimumProjectionImage_h_
#define _ITKMinimumProjectionImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkMinimumProjectionImageFilter.h>


/**
 * @brief The ITKMinimumProjectionImage class. See [Filter documentation](@ref ITKMinimumProjectionImage) for details.
 */
class ITKMinimumProjectionImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKMinimumProjectionImage)
    SIMPL_STATIC_NEW_MACRO(ITKMinimumProjectionImage)
    SIMPL_TYPE_MACRO_SUPER(ITKMinimumProjectionImage, AbstractFilter)

    virtual ~ITKMinimumProjectionImage();

    SIMPL_FILTER_PARAMETER(double, ProjectionDimension)
    Q_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)



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
    ITKMinimumProjectionImage();

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

    ITKMinimumProjectionImage(const ITKMinimumProjectionImage&); // Copy Constructor Not Implemented
    void operator=(const ITKMinimumProjectionImage&); // Operator '=' Not Implemented
};

#endif /* _ITKMinimumProjectionImage_H_ */
