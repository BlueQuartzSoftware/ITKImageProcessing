// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKStandardDeviationProjectionImage_h_
#define _ITKStandardDeviationProjectionImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKStandardDeviationProjectionImage class. See [Filter documentation](@ref ITKStandardDeviationProjectionImage) for details.
 */
class ITKStandardDeviationProjectionImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKStandardDeviationProjectionImage)
    SIMPL_STATIC_NEW_MACRO(ITKStandardDeviationProjectionImage)
    SIMPL_TYPE_MACRO_SUPER(ITKStandardDeviationProjectionImage, AbstractFilter)

    virtual ~ITKStandardDeviationProjectionImage();

    SIMPL_FILTER_PARAMETER(double, ProjectionDimension)
    Q_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)



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
    ITKStandardDeviationProjectionImage();

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

    ITKStandardDeviationProjectionImage(const ITKStandardDeviationProjectionImage&); // Copy Constructor Not Implemented
    void operator=(const ITKStandardDeviationProjectionImage&); // Operator '=' Not Implemented
};

#endif /* _ITKStandardDeviationProjectionImage_H_ */
