/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGaussianBlur_h_
#define _ITKGaussianBlur_h_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include "ITKImageBase.h"

/**
 * @brief The ITKGaussianBlur class. See [Filter documentation](@ref ITKGaussianBlur) for details.
 */
class ITKGaussianBlur : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKGaussianBlur)
    SIMPL_STATIC_NEW_MACRO(ITKGaussianBlur)
    SIMPL_TYPE_MACRO_SUPER(ITKGaussianBlur, AbstractFilter)

    virtual ~ITKGaussianBlur();

    SIMPL_FILTER_PARAMETER(int, MaximumKernelWidth)
    Q_PROPERTY(int MaximumKernelWidth READ getMaximumKernelWidth WRITE setMaximumKernelWidth)

    SIMPL_FILTER_PARAMETER(double, Variance)
    Q_PROPERTY(double Variance READ getVariance WRITE setVariance)

    SIMPL_FILTER_PARAMETER(double, MaximumError)
    Q_PROPERTY(double MaximumError READ getMaximumError WRITE setMaximumError)

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
    ITKGaussianBlur();

    /**
     * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
     */
    void virtual dataCheckInternal() override;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename PixelType>
    void dataCheck();

    /* @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
    */
    void virtual filterInternal() override;

    /**
    * @brief Applies the filter
    */
    template<typename PixelType>
    void filter();

    /**
     * @brief Initializes all the private instance variables.
     */
    void initialize();


  private:

    DEFINE_IDATAARRAY_VARIABLE(NewCellArray)

    ITKGaussianBlur(const ITKGaussianBlur&); // Copy Constructor Not Implemented
    void operator=(const ITKGaussianBlur&); // Operator '=' Not Implemented
};

#endif /* _ITKGaussianBlur_H_ */
