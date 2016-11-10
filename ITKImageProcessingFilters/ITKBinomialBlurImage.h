// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinomialBlurImage_h_
#define _ITKBinomialBlurImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkBinomialBlurImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


/**
 * @brief The ITKBinomialBlurImage class. See [Filter documentation](@ref ITKBinomialBlurImage) for details.
 */
class ITKBinomialBlurImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBinomialBlurImage)
    SIMPL_STATIC_NEW_MACRO(ITKBinomialBlurImage)
    SIMPL_TYPE_MACRO_SUPER(ITKBinomialBlurImage, AbstractFilter)

    virtual ~ITKBinomialBlurImage();

    SIMPL_FILTER_PARAMETER(double, Repetitions)
    Q_PROPERTY(double Repetitions READ getRepetitions WRITE setRepetitions)



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
    ITKBinomialBlurImage();

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

    ITKBinomialBlurImage(const ITKBinomialBlurImage&); // Copy Constructor Not Implemented
    void operator=(const ITKBinomialBlurImage&); // Operator '=' Not Implemented
};

#endif /* _ITKBinomialBlurImage_H_ */
