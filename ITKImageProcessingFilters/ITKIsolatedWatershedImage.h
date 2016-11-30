// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKIsolatedWatershedImage_h_
#define _ITKIsolatedWatershedImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKIsolatedWatershedImage class. See [Filter documentation](@ref ITKIsolatedWatershedImage) for details.
 */
class ITKIsolatedWatershedImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKIsolatedWatershedImage)
    SIMPL_STATIC_NEW_MACRO(ITKIsolatedWatershedImage)
    SIMPL_TYPE_MACRO_SUPER(ITKIsolatedWatershedImage, AbstractFilter)

    virtual ~ITKIsolatedWatershedImage();

    SIMPL_FILTER_PARAMETER(double, Seed1)
    Q_PROPERTY(double Seed1 READ getSeed1 WRITE setSeed1)

    SIMPL_FILTER_PARAMETER(double, Seed2)
    Q_PROPERTY(double Seed2 READ getSeed2 WRITE setSeed2)

    SIMPL_FILTER_PARAMETER(double, Threshold)
    Q_PROPERTY(double Threshold READ getThreshold WRITE setThreshold)

    SIMPL_FILTER_PARAMETER(double, UpperValueLimit)
    Q_PROPERTY(double UpperValueLimit READ getUpperValueLimit WRITE setUpperValueLimit)

    SIMPL_FILTER_PARAMETER(double, IsolatedValueTolerance)
    Q_PROPERTY(double IsolatedValueTolerance READ getIsolatedValueTolerance WRITE setIsolatedValueTolerance)

    SIMPL_FILTER_PARAMETER(int, ReplaceValue1)
    Q_PROPERTY(int ReplaceValue1 READ getReplaceValue1 WRITE setReplaceValue1)

    SIMPL_FILTER_PARAMETER(int, ReplaceValue2)
    Q_PROPERTY(int ReplaceValue2 READ getReplaceValue2 WRITE setReplaceValue2)



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
    ITKIsolatedWatershedImage();

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

    ITKIsolatedWatershedImage(const ITKIsolatedWatershedImage&); // Copy Constructor Not Implemented
    void operator=(const ITKIsolatedWatershedImage&); // Operator '=' Not Implemented
};

#endif /* _ITKIsolatedWatershedImage_H_ */
