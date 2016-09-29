// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKFastApproximateRankImage_h_
#define _ITKFastApproximateRankImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKFastApproximateRankImage class. See [Filter documentation](@ref ITKFastApproximateRankImage) for details.
 */
class ITKFastApproximateRankImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKFastApproximateRankImage)
    SIMPL_STATIC_NEW_MACRO(ITKFastApproximateRankImage)
    SIMPL_TYPE_MACRO_SUPER(ITKFastApproximateRankImage, AbstractFilter)

    virtual ~ITKFastApproximateRankImage();

    SIMPL_FILTER_PARAMETER(double, Rank)
    Q_PROPERTY(double Rank READ getRank WRITE setRank)

    SIMPL_FILTER_PARAMETER(double, Radius)
    Q_PROPERTY(double Radius READ getRadius WRITE setRadius)



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
    ITKFastApproximateRankImage();

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

    ITKFastApproximateRankImage(const ITKFastApproximateRankImage&); // Copy Constructor Not Implemented
    void operator=(const ITKFastApproximateRankImage&); // Operator '=' Not Implemented
};

#endif /* _ITKFastApproximateRankImage_H_ */
