// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBinaryThresholdProjectionImage_h_
#define _ITKBinaryThresholdProjectionImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKBinaryThresholdProjectionImage class. See [Filter documentation](@ref ITKBinaryThresholdProjectionImage) for details.
 */
class ITKBinaryThresholdProjectionImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBinaryThresholdProjectionImage)
    SIMPL_STATIC_NEW_MACRO(ITKBinaryThresholdProjectionImage)
    SIMPL_TYPE_MACRO_SUPER(ITKBinaryThresholdProjectionImage, AbstractFilter)

    virtual ~ITKBinaryThresholdProjectionImage();

    SIMPL_FILTER_PARAMETER(double, ProjectionDimension)
    Q_PROPERTY(double ProjectionDimension READ getProjectionDimension WRITE setProjectionDimension)

    SIMPL_FILTER_PARAMETER(double, ThresholdValue)
    Q_PROPERTY(double ThresholdValue READ getThresholdValue WRITE setThresholdValue)

    SIMPL_FILTER_PARAMETER(int, ForegroundValue)
    Q_PROPERTY(int ForegroundValue READ getForegroundValue WRITE setForegroundValue)

    SIMPL_FILTER_PARAMETER(int, BackgroundValue)
    Q_PROPERTY(int BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)



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
    ITKBinaryThresholdProjectionImage();

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

    ITKBinaryThresholdProjectionImage(const ITKBinaryThresholdProjectionImage&); // Copy Constructor Not Implemented
    void operator=(const ITKBinaryThresholdProjectionImage&); // Operator '=' Not Implemented
};

#endif /* _ITKBinaryThresholdProjectionImage_H_ */
