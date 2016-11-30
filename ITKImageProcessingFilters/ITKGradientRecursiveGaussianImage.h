// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGradientRecursiveGaussianImage_h_
#define _ITKGradientRecursiveGaussianImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKGradientRecursiveGaussianImage class. See [Filter documentation](@ref ITKGradientRecursiveGaussianImage) for details.
 */
class ITKGradientRecursiveGaussianImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKGradientRecursiveGaussianImage)
    SIMPL_STATIC_NEW_MACRO(ITKGradientRecursiveGaussianImage)
    SIMPL_TYPE_MACRO_SUPER(ITKGradientRecursiveGaussianImage, AbstractFilter)

    virtual ~ITKGradientRecursiveGaussianImage();

    SIMPL_FILTER_PARAMETER(double, Sigma)
    Q_PROPERTY(double Sigma READ getSigma WRITE setSigma)

    SIMPL_FILTER_PARAMETER(bool, NormalizeAcrossScale)
    Q_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)

    SIMPL_FILTER_PARAMETER(bool, UseImageDirection)
    Q_PROPERTY(bool UseImageDirection READ getUseImageDirection WRITE setUseImageDirection)



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
    ITKGradientRecursiveGaussianImage();

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

    ITKGradientRecursiveGaussianImage(const ITKGradientRecursiveGaussianImage&); // Copy Constructor Not Implemented
    void operator=(const ITKGradientRecursiveGaussianImage&); // Operator '=' Not Implemented
};

#endif /* _ITKGradientRecursiveGaussianImage_H_ */
