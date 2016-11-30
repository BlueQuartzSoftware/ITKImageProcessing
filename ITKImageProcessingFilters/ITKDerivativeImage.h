// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKDerivativeImage_h_
#define _ITKDerivativeImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKDerivativeImage class. See [Filter documentation](@ref ITKDerivativeImage) for details.
 */
class ITKDerivativeImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKDerivativeImage)
    SIMPL_STATIC_NEW_MACRO(ITKDerivativeImage)
    SIMPL_TYPE_MACRO_SUPER(ITKDerivativeImage, AbstractFilter)

    virtual ~ITKDerivativeImage();

    SIMPL_FILTER_PARAMETER(double, Direction)
    Q_PROPERTY(double Direction READ getDirection WRITE setDirection)

    SIMPL_FILTER_PARAMETER(double, Order)
    Q_PROPERTY(double Order READ getOrder WRITE setOrder)

    SIMPL_FILTER_PARAMETER(bool, UseImageSpacing)
    Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)



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
    ITKDerivativeImage();

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

    ITKDerivativeImage(const ITKDerivativeImage&); // Copy Constructor Not Implemented
    void operator=(const ITKDerivativeImage&); // Operator '=' Not Implemented
};

#endif /* _ITKDerivativeImage_H_ */
