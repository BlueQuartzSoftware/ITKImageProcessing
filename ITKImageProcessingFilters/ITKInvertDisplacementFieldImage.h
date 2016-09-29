// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKInvertDisplacementFieldImage_h_
#define _ITKInvertDisplacementFieldImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKInvertDisplacementFieldImage class. See [Filter documentation](@ref ITKInvertDisplacementFieldImage) for details.
 */
class ITKInvertDisplacementFieldImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKInvertDisplacementFieldImage)
    SIMPL_STATIC_NEW_MACRO(ITKInvertDisplacementFieldImage)
    SIMPL_TYPE_MACRO_SUPER(ITKInvertDisplacementFieldImage, AbstractFilter)

    virtual ~ITKInvertDisplacementFieldImage();

    SIMPL_FILTER_PARAMETER(double, MaximumNumberOfIterations)
    Q_PROPERTY(double MaximumNumberOfIterations READ getMaximumNumberOfIterations WRITE setMaximumNumberOfIterations)

    SIMPL_FILTER_PARAMETER(double, MaxErrorToleranceThreshold)
    Q_PROPERTY(double MaxErrorToleranceThreshold READ getMaxErrorToleranceThreshold WRITE setMaxErrorToleranceThreshold)

    SIMPL_FILTER_PARAMETER(double, MeanErrorToleranceThreshold)
    Q_PROPERTY(double MeanErrorToleranceThreshold READ getMeanErrorToleranceThreshold WRITE setMeanErrorToleranceThreshold)

    SIMPL_FILTER_PARAMETER(bool, EnforceBoundaryCondition)
    Q_PROPERTY(bool EnforceBoundaryCondition READ getEnforceBoundaryCondition WRITE setEnforceBoundaryCondition)



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
    ITKInvertDisplacementFieldImage();

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

    ITKInvertDisplacementFieldImage(const ITKInvertDisplacementFieldImage&); // Copy Constructor Not Implemented
    void operator=(const ITKInvertDisplacementFieldImage&); // Operator '=' Not Implemented
};

#endif /* _ITKInvertDisplacementFieldImage_H_ */
