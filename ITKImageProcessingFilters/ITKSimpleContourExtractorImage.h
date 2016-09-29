// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSimpleContourExtractorImage_h_
#define _ITKSimpleContourExtractorImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKSimpleContourExtractorImage class. See [Filter documentation](@ref ITKSimpleContourExtractorImage) for details.
 */
class ITKSimpleContourExtractorImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKSimpleContourExtractorImage)
    SIMPL_STATIC_NEW_MACRO(ITKSimpleContourExtractorImage)
    SIMPL_TYPE_MACRO_SUPER(ITKSimpleContourExtractorImage, AbstractFilter)

    virtual ~ITKSimpleContourExtractorImage();

    SIMPL_FILTER_PARAMETER(double, InputForegroundValue)
    Q_PROPERTY(double InputForegroundValue READ getInputForegroundValue WRITE setInputForegroundValue)

    SIMPL_FILTER_PARAMETER(double, InputBackgroundValue)
    Q_PROPERTY(double InputBackgroundValue READ getInputBackgroundValue WRITE setInputBackgroundValue)

    SIMPL_FILTER_PARAMETER(double, Radius)
    Q_PROPERTY(double Radius READ getRadius WRITE setRadius)

    SIMPL_FILTER_PARAMETER(double, OutputForegroundValue)
    Q_PROPERTY(double OutputForegroundValue READ getOutputForegroundValue WRITE setOutputForegroundValue)

    SIMPL_FILTER_PARAMETER(double, OutputBackgroundValue)
    Q_PROPERTY(double OutputBackgroundValue READ getOutputBackgroundValue WRITE setOutputBackgroundValue)



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
    ITKSimpleContourExtractorImage();

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

    ITKSimpleContourExtractorImage(const ITKSimpleContourExtractorImage&); // Copy Constructor Not Implemented
    void operator=(const ITKSimpleContourExtractorImage&); // Operator '=' Not Implemented
};

#endif /* _ITKSimpleContourExtractorImage_H_ */
