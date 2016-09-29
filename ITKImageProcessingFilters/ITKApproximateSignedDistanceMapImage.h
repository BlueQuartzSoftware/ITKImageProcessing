// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKApproximateSignedDistanceMapImage_h_
#define _ITKApproximateSignedDistanceMapImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKApproximateSignedDistanceMapImage class. See [Filter documentation](@ref ITKApproximateSignedDistanceMapImage) for details.
 */
class ITKApproximateSignedDistanceMapImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKApproximateSignedDistanceMapImage)
    SIMPL_STATIC_NEW_MACRO(ITKApproximateSignedDistanceMapImage)
    SIMPL_TYPE_MACRO_SUPER(ITKApproximateSignedDistanceMapImage, AbstractFilter)

    virtual ~ITKApproximateSignedDistanceMapImage();

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
    ITKApproximateSignedDistanceMapImage();

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

    ITKApproximateSignedDistanceMapImage(const ITKApproximateSignedDistanceMapImage&); // Copy Constructor Not Implemented
    void operator=(const ITKApproximateSignedDistanceMapImage&); // Operator '=' Not Implemented
};

#endif /* _ITKApproximateSignedDistanceMapImage_H_ */
