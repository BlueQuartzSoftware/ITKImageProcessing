// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryImageToLabelMap_h_
#define _ITKBinaryImageToLabelMap_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKBinaryImageToLabelMap class. See [Filter documentation](@ref ITKBinaryImageToLabelMap) for details.
 */
class ITKBinaryImageToLabelMap : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBinaryImageToLabelMap)
    SIMPL_STATIC_NEW_MACRO(ITKBinaryImageToLabelMap)
    SIMPL_TYPE_MACRO_SUPER(ITKBinaryImageToLabelMap, AbstractFilter)

    virtual ~ITKBinaryImageToLabelMap();

    SIMPL_FILTER_PARAMETER(bool, FullyConnected)
    Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

    SIMPL_FILTER_PARAMETER(double, InputForegroundValue)
    Q_PROPERTY(double InputForegroundValue READ getInputForegroundValue WRITE setInputForegroundValue)

    SIMPL_FILTER_PARAMETER(double, OutputBackgroundValue)
    Q_PROPERTY(double OutputBackgroundValue READ getOutputBackgroundValue WRITE setOutputBackgroundValue)



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
    ITKBinaryImageToLabelMap();

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

    ITKBinaryImageToLabelMap(const ITKBinaryImageToLabelMap&); // Copy Constructor Not Implemented
    void operator=(const ITKBinaryImageToLabelMap&); // Operator '=' Not Implemented
};

#endif /* _ITKBinaryImageToLabelMap_H_ */
