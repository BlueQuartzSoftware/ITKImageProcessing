// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKLabelContourImage_h_
#define _ITKLabelContourImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKLabelContourImage class. See [Filter documentation](@ref ITKLabelContourImage) for details.
 */
class ITKLabelContourImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKLabelContourImage)
    SIMPL_STATIC_NEW_MACRO(ITKLabelContourImage)
    SIMPL_TYPE_MACRO_SUPER(ITKLabelContourImage, AbstractFilter)

    virtual ~ITKLabelContourImage();

    SIMPL_FILTER_PARAMETER(bool, FullyConnected)
    Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

    SIMPL_FILTER_PARAMETER(double, BackgroundValue)
    Q_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)



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
    ITKLabelContourImage();

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

    ITKLabelContourImage(const ITKLabelContourImage&); // Copy Constructor Not Implemented
    void operator=(const ITKLabelContourImage&); // Operator '=' Not Implemented
};

#endif /* _ITKLabelContourImage_H_ */
