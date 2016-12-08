// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKStandardDeviationProjectionImage_h_
#define _ITKStandardDeviationProjectionImage_h_

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkStandardDeviationProjectionImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


/**
 * @brief The ITKStandardDeviationProjectionImage class. See [Filter documentation](@ref ITKStandardDeviationProjectionImage) for details.
 */
class ITKStandardDeviationProjectionImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKStandardDeviationProjectionImage)
    SIMPL_STATIC_NEW_MACRO(ITKStandardDeviationProjectionImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKStandardDeviationProjectionImage, AbstractFilter)

    virtual ~ITKStandardDeviationProjectionImage();

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
    ITKStandardDeviationProjectionImage();

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

    ITKStandardDeviationProjectionImage(const ITKStandardDeviationProjectionImage&); // Copy Constructor Not Implemented
    void operator=(const ITKStandardDeviationProjectionImage&); // Operator '=' Not Implemented
};

#pragma clang diagnostic pop

#endif /* _ITKStandardDeviationProjectionImage_H_ */
