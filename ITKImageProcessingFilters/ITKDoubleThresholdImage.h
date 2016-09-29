// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKDoubleThresholdImage_h_
#define _ITKDoubleThresholdImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKDoubleThresholdImage class. See [Filter documentation](@ref ITKDoubleThresholdImage) for details.
 */
class ITKDoubleThresholdImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKDoubleThresholdImage)
    SIMPL_STATIC_NEW_MACRO(ITKDoubleThresholdImage)
    SIMPL_TYPE_MACRO_SUPER(ITKDoubleThresholdImage, AbstractFilter)

    virtual ~ITKDoubleThresholdImage();

    SIMPL_FILTER_PARAMETER(double, Threshold1)
    Q_PROPERTY(double Threshold1 READ getThreshold1 WRITE setThreshold1)

    SIMPL_FILTER_PARAMETER(double, Threshold2)
    Q_PROPERTY(double Threshold2 READ getThreshold2 WRITE setThreshold2)

    SIMPL_FILTER_PARAMETER(double, Threshold3)
    Q_PROPERTY(double Threshold3 READ getThreshold3 WRITE setThreshold3)

    SIMPL_FILTER_PARAMETER(double, Threshold4)
    Q_PROPERTY(double Threshold4 READ getThreshold4 WRITE setThreshold4)

    SIMPL_FILTER_PARAMETER(int, InsideValue)
    Q_PROPERTY(int InsideValue READ getInsideValue WRITE setInsideValue)

    SIMPL_FILTER_PARAMETER(int, OutsideValue)
    Q_PROPERTY(int OutsideValue READ getOutsideValue WRITE setOutsideValue)

    SIMPL_FILTER_PARAMETER(bool, FullyConnected)
    Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)



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
    ITKDoubleThresholdImage();

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

    ITKDoubleThresholdImage(const ITKDoubleThresholdImage&); // Copy Constructor Not Implemented
    void operator=(const ITKDoubleThresholdImage&); // Operator '=' Not Implemented
};

#endif /* _ITKDoubleThresholdImage_H_ */
