// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinShrinkImage_h_
#define _ITKBinShrinkImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKBinShrinkImage class. See [Filter documentation](@ref ITKBinShrinkImage) for details.
 */
class ITKBinShrinkImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBinShrinkImage)
    SIMPL_STATIC_NEW_MACRO(ITKBinShrinkImage)
    SIMPL_TYPE_MACRO_SUPER(ITKBinShrinkImage, AbstractFilter)

    virtual ~ITKBinShrinkImage();

    SIMPL_FILTER_PARAMETER(double, ShrinkFactors)
    Q_PROPERTY(double ShrinkFactors READ getShrinkFactors WRITE setShrinkFactors)



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
    ITKBinShrinkImage();

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

    ITKBinShrinkImage(const ITKBinShrinkImage&); // Copy Constructor Not Implemented
    void operator=(const ITKBinShrinkImage&); // Operator '=' Not Implemented
};

#endif /* _ITKBinShrinkImage_H_ */
