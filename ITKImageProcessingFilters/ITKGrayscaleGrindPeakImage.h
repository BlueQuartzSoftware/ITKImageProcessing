// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGrayscaleGrindPeakImage_h_
#define _ITKGrayscaleGrindPeakImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkGrayscaleGrindPeakImageFilter.h>


/**
 * @brief The ITKGrayscaleGrindPeakImage class. See [Filter documentation](@ref ITKGrayscaleGrindPeakImage) for details.
 */
class ITKGrayscaleGrindPeakImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKGrayscaleGrindPeakImage)
    SIMPL_STATIC_NEW_MACRO(ITKGrayscaleGrindPeakImage)
    SIMPL_TYPE_MACRO_SUPER(ITKGrayscaleGrindPeakImage, AbstractFilter)

    virtual ~ITKGrayscaleGrindPeakImage();

    SIMPL_FILTER_PARAMETER(bool, FullyConnected)
    Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)



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
    ITKGrayscaleGrindPeakImage();

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

    ITKGrayscaleGrindPeakImage(const ITKGrayscaleGrindPeakImage&); // Copy Constructor Not Implemented
    void operator=(const ITKGrayscaleGrindPeakImage&); // Operator '=' Not Implemented
};

#endif /* _ITKGrayscaleGrindPeakImage_H_ */
