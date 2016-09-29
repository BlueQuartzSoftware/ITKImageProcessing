// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKJoinSeriesImage_h_
#define _ITKJoinSeriesImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKJoinSeriesImage class. See [Filter documentation](@ref ITKJoinSeriesImage) for details.
 */
class ITKJoinSeriesImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKJoinSeriesImage)
    SIMPL_STATIC_NEW_MACRO(ITKJoinSeriesImage)
    SIMPL_TYPE_MACRO_SUPER(ITKJoinSeriesImage, AbstractFilter)

    virtual ~ITKJoinSeriesImage();

    SIMPL_FILTER_PARAMETER(double, Origin)
    Q_PROPERTY(double Origin READ getOrigin WRITE setOrigin)

    SIMPL_FILTER_PARAMETER(double, Spacing)
    Q_PROPERTY(double Spacing READ getSpacing WRITE setSpacing)



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
    ITKJoinSeriesImage();

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

    ITKJoinSeriesImage(const ITKJoinSeriesImage&); // Copy Constructor Not Implemented
    void operator=(const ITKJoinSeriesImage&); // Operator '=' Not Implemented
};

#endif /* _ITKJoinSeriesImage_H_ */
