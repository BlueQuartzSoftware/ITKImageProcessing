/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryThreshold_h_
#define _ITKBinaryThreshold_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKBinaryThreshold class. See [Filter documentation](@ref ITKBinaryThreshold) for details.
 */
class ITKBinaryThreshold : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBinaryThreshold)
    SIMPL_STATIC_NEW_MACRO(ITKBinaryThreshold)
    SIMPL_TYPE_MACRO_SUPER(ITKBinaryThreshold, AbstractFilter)

    virtual ~ITKBinaryThreshold();

    SIMPL_FILTER_PARAMETER(double, LowerThresholdValue)
    Q_PROPERTY(double LowerThresholdValue READ getLowerThresholdValue WRITE setLowerThresholdValue)

    SIMPL_FILTER_PARAMETER(double, UpperThresholdValue)
    Q_PROPERTY(double UpperThresholdValue READ getUpperThresholdValue WRITE setUpperThresholdValue)

    SIMPL_FILTER_PARAMETER(double, InsideValue)
    Q_PROPERTY(double InsideValue READ getInsideValue WRITE setInsideValue)

    SIMPL_FILTER_PARAMETER(double, OutsideValue)
    Q_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)

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
    ITKBinaryThreshold();

    /**
     * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
     */
    void virtual dataCheckInternal() override;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename PixelType>
    void dataCheck();

    /**
    * @brief CheckLimits Checks that value is inside the bounds of the chosen PixelType
    */
    template<typename PixelType>
    void CheckLimits(double value, QString name);

    /**
    * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
    */
    void virtual filterInternal() override;

    /**
    * @brief Applies the filter
    */
    template<typename PixelType>
    void filter();

  private:

    ITKBinaryThreshold(const ITKBinaryThreshold&); // Copy Constructor Not Implemented
    void operator=(const ITKBinaryThreshold&); // Operator '=' Not Implemented
};

#endif /* _ITKBinaryThreshold_H_ */
