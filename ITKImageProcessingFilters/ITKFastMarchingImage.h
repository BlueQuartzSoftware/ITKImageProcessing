// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKFastMarchingImage_h_
#define _ITKFastMarchingImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKFastMarchingImage class. See [Filter documentation](@ref ITKFastMarchingImage) for details.
 */
class ITKFastMarchingImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKFastMarchingImage)
    SIMPL_STATIC_NEW_MACRO(ITKFastMarchingImage)
    SIMPL_TYPE_MACRO_SUPER(ITKFastMarchingImage, AbstractFilter)

    virtual ~ITKFastMarchingImage();

    SIMPL_FILTER_PARAMETER(double, TrialPoints)
    Q_PROPERTY(double TrialPoints READ getTrialPoints WRITE setTrialPoints)

    SIMPL_FILTER_PARAMETER(double, NormalizationFactor)
    Q_PROPERTY(double NormalizationFactor READ getNormalizationFactor WRITE setNormalizationFactor)

    SIMPL_FILTER_PARAMETER(double, StoppingValue)
    Q_PROPERTY(double StoppingValue READ getStoppingValue WRITE setStoppingValue)



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
    ITKFastMarchingImage();

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

    ITKFastMarchingImage(const ITKFastMarchingImage&); // Copy Constructor Not Implemented
    void operator=(const ITKFastMarchingImage&); // Operator '=' Not Implemented
};

#endif /* _ITKFastMarchingImage_H_ */
