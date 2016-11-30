// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKCollidingFrontsImage_h_
#define _ITKCollidingFrontsImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKCollidingFrontsImage class. See [Filter documentation](@ref ITKCollidingFrontsImage) for details.
 */
class ITKCollidingFrontsImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKCollidingFrontsImage)
    SIMPL_STATIC_NEW_MACRO(ITKCollidingFrontsImage)
    SIMPL_TYPE_MACRO_SUPER(ITKCollidingFrontsImage, AbstractFilter)

    virtual ~ITKCollidingFrontsImage();

    SIMPL_FILTER_PARAMETER(double, SeedPoints1)
    Q_PROPERTY(double SeedPoints1 READ getSeedPoints1 WRITE setSeedPoints1)

    SIMPL_FILTER_PARAMETER(double, SeedPoints2)
    Q_PROPERTY(double SeedPoints2 READ getSeedPoints2 WRITE setSeedPoints2)

    SIMPL_FILTER_PARAMETER(bool, ApplyConnectivity)
    Q_PROPERTY(bool ApplyConnectivity READ getApplyConnectivity WRITE setApplyConnectivity)

    SIMPL_FILTER_PARAMETER(double, NegativeEpsilon)
    Q_PROPERTY(double NegativeEpsilon READ getNegativeEpsilon WRITE setNegativeEpsilon)

    SIMPL_FILTER_PARAMETER(bool, StopOnTargets)
    Q_PROPERTY(bool StopOnTargets READ getStopOnTargets WRITE setStopOnTargets)



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
    ITKCollidingFrontsImage();

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

    ITKCollidingFrontsImage(const ITKCollidingFrontsImage&); // Copy Constructor Not Implemented
    void operator=(const ITKCollidingFrontsImage&); // Operator '=' Not Implemented
};

#endif /* _ITKCollidingFrontsImage_H_ */
