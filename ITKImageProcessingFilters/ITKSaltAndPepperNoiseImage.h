// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSaltAndPepperNoiseImage_h_
#define _ITKSaltAndPepperNoiseImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKSaltAndPepperNoiseImage class. See [Filter documentation](@ref ITKSaltAndPepperNoiseImage) for details.
 */
class ITKSaltAndPepperNoiseImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKSaltAndPepperNoiseImage)
    SIMPL_STATIC_NEW_MACRO(ITKSaltAndPepperNoiseImage)
    SIMPL_TYPE_MACRO_SUPER(ITKSaltAndPepperNoiseImage, AbstractFilter)

    virtual ~ITKSaltAndPepperNoiseImage();

    SIMPL_FILTER_PARAMETER(double, Probability)
    Q_PROPERTY(double Probability READ getProbability WRITE setProbability)

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
    ITKSaltAndPepperNoiseImage();

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

    ITKSaltAndPepperNoiseImage(const ITKSaltAndPepperNoiseImage&); // Copy Constructor Not Implemented
    void operator=(const ITKSaltAndPepperNoiseImage&); // Operator '=' Not Implemented
};

#endif /* _ITKSaltAndPepperNoiseImage_H_ */
