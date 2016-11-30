// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKVotingBinaryIterativeHoleFillingImage_h_
#define _ITKVotingBinaryIterativeHoleFillingImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKVotingBinaryIterativeHoleFillingImage class. See [Filter documentation](@ref ITKVotingBinaryIterativeHoleFillingImage) for details.
 */
class ITKVotingBinaryIterativeHoleFillingImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKVotingBinaryIterativeHoleFillingImage)
    SIMPL_STATIC_NEW_MACRO(ITKVotingBinaryIterativeHoleFillingImage)
    SIMPL_TYPE_MACRO_SUPER(ITKVotingBinaryIterativeHoleFillingImage, AbstractFilter)

    virtual ~ITKVotingBinaryIterativeHoleFillingImage();

    SIMPL_FILTER_PARAMETER(double, Radius)
    Q_PROPERTY(double Radius READ getRadius WRITE setRadius)

    SIMPL_FILTER_PARAMETER(double, MaximumNumberOfIterations)
    Q_PROPERTY(double MaximumNumberOfIterations READ getMaximumNumberOfIterations WRITE setMaximumNumberOfIterations)

    SIMPL_FILTER_PARAMETER(double, MajorityThreshold)
    Q_PROPERTY(double MajorityThreshold READ getMajorityThreshold WRITE setMajorityThreshold)

    SIMPL_FILTER_PARAMETER(double, ForegroundValue)
    Q_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)

    SIMPL_FILTER_PARAMETER(double, BackgroundValue)
    Q_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)



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
    ITKVotingBinaryIterativeHoleFillingImage();

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

    ITKVotingBinaryIterativeHoleFillingImage(const ITKVotingBinaryIterativeHoleFillingImage&); // Copy Constructor Not Implemented
    void operator=(const ITKVotingBinaryIterativeHoleFillingImage&); // Operator '=' Not Implemented
};

#endif /* _ITKVotingBinaryIterativeHoleFillingImage_H_ */
