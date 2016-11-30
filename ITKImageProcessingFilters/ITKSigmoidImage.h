// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSigmoidImage_h_
#define _ITKSigmoidImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKSigmoidImage class. See [Filter documentation](@ref ITKSigmoidImage) for details.
 */
class ITKSigmoidImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKSigmoidImage)
    SIMPL_STATIC_NEW_MACRO(ITKSigmoidImage)
    SIMPL_TYPE_MACRO_SUPER(ITKSigmoidImage, AbstractFilter)

    virtual ~ITKSigmoidImage();

    SIMPL_FILTER_PARAMETER(double, Alpha)
    Q_PROPERTY(double Alpha READ getAlpha WRITE setAlpha)

    SIMPL_FILTER_PARAMETER(double, Beta)
    Q_PROPERTY(double Beta READ getBeta WRITE setBeta)

    SIMPL_FILTER_PARAMETER(double, OutputMaximum)
    Q_PROPERTY(double OutputMaximum READ getOutputMaximum WRITE setOutputMaximum)

    SIMPL_FILTER_PARAMETER(double, OutputMinimum)
    Q_PROPERTY(double OutputMinimum READ getOutputMinimum WRITE setOutputMinimum)



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
    ITKSigmoidImage();

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
    * @brief CheckIntegerEntry: Input types can only be of certain types:
      -float
      -double
      -bool
      -int
      For the other type, we have to use one of this primitive type, and verify that the
      value corresponds to what is expected.
    */
    template<typename VarType, typename SubsType>
    void CheckIntegerEntry(SubsType value, QString name);

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

    ITKSigmoidImage(const ITKSigmoidImage&); // Copy Constructor Not Implemented
    void operator=(const ITKSigmoidImage&); // Operator '=' Not Implemented
};

#endif /* _ITKSigmoidImage_H_ */
