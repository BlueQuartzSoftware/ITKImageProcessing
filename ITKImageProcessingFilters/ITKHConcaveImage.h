// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKHConcaveImage_h_
#define _ITKHConcaveImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKHConcaveImage class. See [Filter documentation](@ref ITKHConcaveImage) for details.
 */
class ITKHConcaveImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKHConcaveImage)
    SIMPL_STATIC_NEW_MACRO(ITKHConcaveImage)
    SIMPL_TYPE_MACRO_SUPER(ITKHConcaveImage, AbstractFilter)

    virtual ~ITKHConcaveImage();

    SIMPL_FILTER_PARAMETER(double, Height)
    Q_PROPERTY(double Height READ getHeight WRITE setHeight)

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
    ITKHConcaveImage();

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

    ITKHConcaveImage(const ITKHConcaveImage&); // Copy Constructor Not Implemented
    void operator=(const ITKHConcaveImage&); // Operator '=' Not Implemented
};

#endif /* _ITKHConcaveImage_H_ */
