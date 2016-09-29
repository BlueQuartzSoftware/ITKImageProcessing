// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKAdditiveGaussianNoiseImage_h_
#define _ITKAdditiveGaussianNoiseImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKAdditiveGaussianNoiseImage class. See [Filter documentation](@ref ITKAdditiveGaussianNoiseImage) for details.
 */
class ITKAdditiveGaussianNoiseImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKAdditiveGaussianNoiseImage)
    SIMPL_STATIC_NEW_MACRO(ITKAdditiveGaussianNoiseImage)
    SIMPL_TYPE_MACRO_SUPER(ITKAdditiveGaussianNoiseImage, AbstractFilter)

    virtual ~ITKAdditiveGaussianNoiseImage();

    SIMPL_FILTER_PARAMETER(double, StandardDeviation)
    Q_PROPERTY(double StandardDeviation READ getStandardDeviation WRITE setStandardDeviation)

    SIMPL_FILTER_PARAMETER(double, Mean)
    Q_PROPERTY(double Mean READ getMean WRITE setMean)

    SIMPL_FILTER_PARAMETER(double, Seed)
    Q_PROPERTY(double Seed READ getSeed WRITE setSeed)



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
    ITKAdditiveGaussianNoiseImage();

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

    ITKAdditiveGaussianNoiseImage(const ITKAdditiveGaussianNoiseImage&); // Copy Constructor Not Implemented
    void operator=(const ITKAdditiveGaussianNoiseImage&); // Operator '=' Not Implemented
};

#endif /* _ITKAdditiveGaussianNoiseImage_H_ */
