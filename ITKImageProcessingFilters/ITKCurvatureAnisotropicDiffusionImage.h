// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKCurvatureAnisotropicDiffusionImage_h_
#define _ITKCurvatureAnisotropicDiffusionImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKCurvatureAnisotropicDiffusionImage class. See [Filter documentation](@ref ITKCurvatureAnisotropicDiffusionImage) for details.
 */
class ITKCurvatureAnisotropicDiffusionImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKCurvatureAnisotropicDiffusionImage)
    SIMPL_STATIC_NEW_MACRO(ITKCurvatureAnisotropicDiffusionImage)
    SIMPL_TYPE_MACRO_SUPER(ITKCurvatureAnisotropicDiffusionImage, AbstractFilter)

    virtual ~ITKCurvatureAnisotropicDiffusionImage();

    SIMPL_FILTER_PARAMETER(double, TimeStep)
    Q_PROPERTY(double TimeStep READ getTimeStep WRITE setTimeStep)

    SIMPL_FILTER_PARAMETER(double, ConductanceParameter)
    Q_PROPERTY(double ConductanceParameter READ getConductanceParameter WRITE setConductanceParameter)

    SIMPL_FILTER_PARAMETER(double, ConductanceScalingUpdateInterval)
    Q_PROPERTY(double ConductanceScalingUpdateInterval READ getConductanceScalingUpdateInterval WRITE setConductanceScalingUpdateInterval)

    SIMPL_FILTER_PARAMETER(double, NumberOfIterations)
    Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)



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
    ITKCurvatureAnisotropicDiffusionImage();

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

    ITKCurvatureAnisotropicDiffusionImage(const ITKCurvatureAnisotropicDiffusionImage&); // Copy Constructor Not Implemented
    void operator=(const ITKCurvatureAnisotropicDiffusionImage&); // Operator '=' Not Implemented
};

#endif /* _ITKCurvatureAnisotropicDiffusionImage_H_ */
