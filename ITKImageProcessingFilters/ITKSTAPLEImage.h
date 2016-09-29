// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSTAPLEImage_h_
#define _ITKSTAPLEImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKSTAPLEImage class. See [Filter documentation](@ref ITKSTAPLEImage) for details.
 */
class ITKSTAPLEImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKSTAPLEImage)
    SIMPL_STATIC_NEW_MACRO(ITKSTAPLEImage)
    SIMPL_TYPE_MACRO_SUPER(ITKSTAPLEImage, AbstractFilter)

    virtual ~ITKSTAPLEImage();

    SIMPL_FILTER_PARAMETER(double, ConfidenceWeight)
    Q_PROPERTY(double ConfidenceWeight READ getConfidenceWeight WRITE setConfidenceWeight)

    SIMPL_FILTER_PARAMETER(double, ForegroundValue)
    Q_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)

    SIMPL_FILTER_PARAMETER(double, MaximumIterations)
    Q_PROPERTY(double MaximumIterations READ getMaximumIterations WRITE setMaximumIterations)



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
    ITKSTAPLEImage();

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

    ITKSTAPLEImage(const ITKSTAPLEImage&); // Copy Constructor Not Implemented
    void operator=(const ITKSTAPLEImage&); // Operator '=' Not Implemented
};

#endif /* _ITKSTAPLEImage_H_ */
