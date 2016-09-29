// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKDiscreteGaussianDerivativeImage_h_
#define _ITKDiscreteGaussianDerivativeImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKDiscreteGaussianDerivativeImage class. See [Filter documentation](@ref ITKDiscreteGaussianDerivativeImage) for details.
 */
class ITKDiscreteGaussianDerivativeImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKDiscreteGaussianDerivativeImage)
    SIMPL_STATIC_NEW_MACRO(ITKDiscreteGaussianDerivativeImage)
    SIMPL_TYPE_MACRO_SUPER(ITKDiscreteGaussianDerivativeImage, AbstractFilter)

    virtual ~ITKDiscreteGaussianDerivativeImage();

    SIMPL_FILTER_PARAMETER(double, Variance)
    Q_PROPERTY(double Variance READ getVariance WRITE setVariance)

    SIMPL_FILTER_PARAMETER(double, Order)
    Q_PROPERTY(double Order READ getOrder WRITE setOrder)

    SIMPL_FILTER_PARAMETER(double, MaximumKernelWidth)
    Q_PROPERTY(double MaximumKernelWidth READ getMaximumKernelWidth WRITE setMaximumKernelWidth)

    SIMPL_FILTER_PARAMETER(double, MaximumError)
    Q_PROPERTY(double MaximumError READ getMaximumError WRITE setMaximumError)

    SIMPL_FILTER_PARAMETER(bool, UseImageSpacing)
    Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)

    SIMPL_FILTER_PARAMETER(bool, NormalizeAcrossScale)
    Q_PROPERTY(bool NormalizeAcrossScale READ getNormalizeAcrossScale WRITE setNormalizeAcrossScale)



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
    ITKDiscreteGaussianDerivativeImage();

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

    ITKDiscreteGaussianDerivativeImage(const ITKDiscreteGaussianDerivativeImage&); // Copy Constructor Not Implemented
    void operator=(const ITKDiscreteGaussianDerivativeImage&); // Operator '=' Not Implemented
};

#endif /* _ITKDiscreteGaussianDerivativeImage_H_ */
