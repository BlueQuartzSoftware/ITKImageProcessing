// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKRegionalMinimaImage_h_
#define _ITKRegionalMinimaImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkRegionalMinimaImageFilter.h>


/**
 * @brief The ITKRegionalMinimaImage class. See [Filter documentation](@ref ITKRegionalMinimaImage) for details.
 */
class ITKRegionalMinimaImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKRegionalMinimaImage)
    SIMPL_STATIC_NEW_MACRO(ITKRegionalMinimaImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKRegionalMinimaImage, AbstractFilter)

    virtual ~ITKRegionalMinimaImage();

    SIMPL_FILTER_PARAMETER(double, BackgroundValue)
    Q_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)

    SIMPL_FILTER_PARAMETER(double, ForegroundValue)
    Q_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)

    SIMPL_FILTER_PARAMETER(bool, FullyConnected)
    Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

    SIMPL_FILTER_PARAMETER(bool, FlatIsMinima)
    Q_PROPERTY(bool FlatIsMinima READ getFlatIsMinima WRITE setFlatIsMinima)



    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel() override;

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getSubGroupName() override;
    
    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  protected:
    ITKRegionalMinimaImage();

    /**
     * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
     */
    void virtual dataCheckInternal() override;

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    template<typename InputImageType, typename OutputImageType, unsigned int Dimension>
    void dataCheck();

    /**
    * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
    */
    void virtual filterInternal() override;

    /**
    * @brief Applies the filter
    */
    template<typename InputImageType, typename OutputImageType, unsigned int Dimension>
    void filter();

  private:

    ITKRegionalMinimaImage(const ITKRegionalMinimaImage&); // Copy Constructor Not Implemented
    void operator=(const ITKRegionalMinimaImage&); // Operator '=' Not Implemented
};

#endif /* _ITKRegionalMinimaImage_H_ */
