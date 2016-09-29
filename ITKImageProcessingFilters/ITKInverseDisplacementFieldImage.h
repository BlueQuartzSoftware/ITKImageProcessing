// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKInverseDisplacementFieldImage_h_
#define _ITKInverseDisplacementFieldImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKInverseDisplacementFieldImage class. See [Filter documentation](@ref ITKInverseDisplacementFieldImage) for details.
 */
class ITKInverseDisplacementFieldImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKInverseDisplacementFieldImage)
    SIMPL_STATIC_NEW_MACRO(ITKInverseDisplacementFieldImage)
    SIMPL_TYPE_MACRO_SUPER(ITKInverseDisplacementFieldImage, AbstractFilter)

    virtual ~ITKInverseDisplacementFieldImage();

    SIMPL_FILTER_PARAMETER(double, Size)
    Q_PROPERTY(double Size READ getSize WRITE setSize)

    SIMPL_FILTER_PARAMETER(double, OutputOrigin)
    Q_PROPERTY(double OutputOrigin READ getOutputOrigin WRITE setOutputOrigin)

    SIMPL_FILTER_PARAMETER(double, OutputSpacing)
    Q_PROPERTY(double OutputSpacing READ getOutputSpacing WRITE setOutputSpacing)

    SIMPL_FILTER_PARAMETER(double, SubsamplingFactor)
    Q_PROPERTY(double SubsamplingFactor READ getSubsamplingFactor WRITE setSubsamplingFactor)



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
    ITKInverseDisplacementFieldImage();

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

    ITKInverseDisplacementFieldImage(const ITKInverseDisplacementFieldImage&); // Copy Constructor Not Implemented
    void operator=(const ITKInverseDisplacementFieldImage&); // Operator '=' Not Implemented
};

#endif /* _ITKInverseDisplacementFieldImage_H_ */
