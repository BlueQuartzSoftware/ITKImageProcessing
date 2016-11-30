// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKFastMarchingUpwindGradientImage_h_
#define _ITKFastMarchingUpwindGradientImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKFastMarchingUpwindGradientImage class. See [Filter documentation](@ref ITKFastMarchingUpwindGradientImage) for details.
 */
class ITKFastMarchingUpwindGradientImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKFastMarchingUpwindGradientImage)
    SIMPL_STATIC_NEW_MACRO(ITKFastMarchingUpwindGradientImage)
    SIMPL_TYPE_MACRO_SUPER(ITKFastMarchingUpwindGradientImage, AbstractFilter)

    virtual ~ITKFastMarchingUpwindGradientImage();

    SIMPL_FILTER_PARAMETER(double, TrialPoints)
    Q_PROPERTY(double TrialPoints READ getTrialPoints WRITE setTrialPoints)

    SIMPL_FILTER_PARAMETER(double, NumberOfTargets)
    Q_PROPERTY(double NumberOfTargets READ getNumberOfTargets WRITE setNumberOfTargets)

    SIMPL_FILTER_PARAMETER(double, TargetPoints)
    Q_PROPERTY(double TargetPoints READ getTargetPoints WRITE setTargetPoints)

    SIMPL_FILTER_PARAMETER(double, TargetOffset)
    Q_PROPERTY(double TargetOffset READ getTargetOffset WRITE setTargetOffset)

    SIMPL_FILTER_PARAMETER(double, NormalizationFactor)
    Q_PROPERTY(double NormalizationFactor READ getNormalizationFactor WRITE setNormalizationFactor)



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
    ITKFastMarchingUpwindGradientImage();

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

    ITKFastMarchingUpwindGradientImage(const ITKFastMarchingUpwindGradientImage&); // Copy Constructor Not Implemented
    void operator=(const ITKFastMarchingUpwindGradientImage&); // Operator '=' Not Implemented
};

#endif /* _ITKFastMarchingUpwindGradientImage_H_ */
