// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKIsolatedConnectedImage_h_
#define _ITKIsolatedConnectedImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKIsolatedConnectedImage class. See [Filter documentation](@ref ITKIsolatedConnectedImage) for details.
 */
class ITKIsolatedConnectedImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKIsolatedConnectedImage)
    SIMPL_STATIC_NEW_MACRO(ITKIsolatedConnectedImage)
    SIMPL_TYPE_MACRO_SUPER(ITKIsolatedConnectedImage, AbstractFilter)

    virtual ~ITKIsolatedConnectedImage();

    SIMPL_FILTER_PARAMETER(double, Seed1)
    Q_PROPERTY(double Seed1 READ getSeed1 WRITE setSeed1)

    SIMPL_FILTER_PARAMETER(double, Seed2)
    Q_PROPERTY(double Seed2 READ getSeed2 WRITE setSeed2)

    SIMPL_FILTER_PARAMETER(double, Lower)
    Q_PROPERTY(double Lower READ getLower WRITE setLower)

    SIMPL_FILTER_PARAMETER(double, Upper)
    Q_PROPERTY(double Upper READ getUpper WRITE setUpper)

    SIMPL_FILTER_PARAMETER(int, ReplaceValue)
    Q_PROPERTY(int ReplaceValue READ getReplaceValue WRITE setReplaceValue)

    SIMPL_FILTER_PARAMETER(double, IsolatedValueTolerance)
    Q_PROPERTY(double IsolatedValueTolerance READ getIsolatedValueTolerance WRITE setIsolatedValueTolerance)

    SIMPL_FILTER_PARAMETER(bool, FindUpperThreshold)
    Q_PROPERTY(bool FindUpperThreshold READ getFindUpperThreshold WRITE setFindUpperThreshold)



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
    ITKIsolatedConnectedImage();

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

    ITKIsolatedConnectedImage(const ITKIsolatedConnectedImage&); // Copy Constructor Not Implemented
    void operator=(const ITKIsolatedConnectedImage&); // Operator '=' Not Implemented
};

#endif /* _ITKIsolatedConnectedImage_H_ */
