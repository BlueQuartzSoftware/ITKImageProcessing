// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryMinMaxCurvatureFlowImage_h_
#define _ITKBinaryMinMaxCurvatureFlowImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKBinaryMinMaxCurvatureFlowImage class. See [Filter documentation](@ref ITKBinaryMinMaxCurvatureFlowImage) for details.
 */
class ITKBinaryMinMaxCurvatureFlowImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBinaryMinMaxCurvatureFlowImage)
    SIMPL_STATIC_NEW_MACRO(ITKBinaryMinMaxCurvatureFlowImage)
    SIMPL_TYPE_MACRO_SUPER(ITKBinaryMinMaxCurvatureFlowImage, AbstractFilter)

    virtual ~ITKBinaryMinMaxCurvatureFlowImage();

    SIMPL_FILTER_PARAMETER(double, TimeStep)
    Q_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)

    SIMPL_FILTER_PARAMETER(double, NumberOfIterations)
    Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)

    SIMPL_FILTER_PARAMETER(int, StencilRadius)
    Q_PROPERTY(int StencilRadius READ getStencilRadius WRITE setStencilRadius)

    SIMPL_FILTER_PARAMETER(double, Threshold)
    Q_PROPERTY(double Threshold READ getThreshold WRITE setThreshold)



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
    ITKBinaryMinMaxCurvatureFlowImage();

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

    ITKBinaryMinMaxCurvatureFlowImage(const ITKBinaryMinMaxCurvatureFlowImage&); // Copy Constructor Not Implemented
    void operator=(const ITKBinaryMinMaxCurvatureFlowImage&); // Operator '=' Not Implemented
};

#endif /* _ITKBinaryMinMaxCurvatureFlowImage_H_ */
