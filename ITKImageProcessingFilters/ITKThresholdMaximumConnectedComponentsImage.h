// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKThresholdMaximumConnectedComponentsImage_h_
#define _ITKThresholdMaximumConnectedComponentsImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKThresholdMaximumConnectedComponentsImage class. See [Filter documentation](@ref ITKThresholdMaximumConnectedComponentsImage) for details.
 */
class ITKThresholdMaximumConnectedComponentsImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKThresholdMaximumConnectedComponentsImage)
    SIMPL_STATIC_NEW_MACRO(ITKThresholdMaximumConnectedComponentsImage)
    SIMPL_TYPE_MACRO_SUPER(ITKThresholdMaximumConnectedComponentsImage, AbstractFilter)

    virtual ~ITKThresholdMaximumConnectedComponentsImage();

    SIMPL_FILTER_PARAMETER(double, MinimumObjectSizeInPixels)
    Q_PROPERTY(double MinimumObjectSizeInPixels READ getMinimumObjectSizeInPixels WRITE setMinimumObjectSizeInPixels)

    SIMPL_FILTER_PARAMETER(double, UpperBoundary)
    Q_PROPERTY(double UpperBoundary READ getUpperBoundary WRITE setUpperBoundary)

    SIMPL_FILTER_PARAMETER(int, InsideValue)
    Q_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)

    SIMPL_FILTER_PARAMETER(int, OutsideValue)
    Q_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)



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
    ITKThresholdMaximumConnectedComponentsImage();

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

    ITKThresholdMaximumConnectedComponentsImage(const ITKThresholdMaximumConnectedComponentsImage&); // Copy Constructor Not Implemented
    void operator=(const ITKThresholdMaximumConnectedComponentsImage&); // Operator '=' Not Implemented
};

#endif /* _ITKThresholdMaximumConnectedComponentsImage_H_ */
