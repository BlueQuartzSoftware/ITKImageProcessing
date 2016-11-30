// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKZeroCrossingBasedEdgeDetectionImage_h_
#define _ITKZeroCrossingBasedEdgeDetectionImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKZeroCrossingBasedEdgeDetectionImage class. See [Filter documentation](@ref ITKZeroCrossingBasedEdgeDetectionImage) for details.
 */
class ITKZeroCrossingBasedEdgeDetectionImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKZeroCrossingBasedEdgeDetectionImage)
    SIMPL_STATIC_NEW_MACRO(ITKZeroCrossingBasedEdgeDetectionImage)
    SIMPL_TYPE_MACRO_SUPER(ITKZeroCrossingBasedEdgeDetectionImage, AbstractFilter)

    virtual ~ITKZeroCrossingBasedEdgeDetectionImage();

    SIMPL_FILTER_PARAMETER(double, Variance)
    Q_PROPERTY(double Variance READ getVariance WRITE setVariance)

    SIMPL_FILTER_PARAMETER(int, ForegroundValue)
    Q_PROPERTY(int ForegroundValue READ getForegroundValue WRITE setForegroundValue)

    SIMPL_FILTER_PARAMETER(int, BackgroundValue)
    Q_PROPERTY(int BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)

    SIMPL_FILTER_PARAMETER(double, MaximumError)
    Q_PROPERTY(double MaximumError READ getMaximumError WRITE setMaximumError)



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
    ITKZeroCrossingBasedEdgeDetectionImage();

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

    ITKZeroCrossingBasedEdgeDetectionImage(const ITKZeroCrossingBasedEdgeDetectionImage&); // Copy Constructor Not Implemented
    void operator=(const ITKZeroCrossingBasedEdgeDetectionImage&); // Operator '=' Not Implemented
};

#endif /* _ITKZeroCrossingBasedEdgeDetectionImage_H_ */
