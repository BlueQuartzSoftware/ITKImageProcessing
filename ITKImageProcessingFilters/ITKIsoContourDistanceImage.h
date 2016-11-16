// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKIsoContourDistanceImage_h_
#define _ITKIsoContourDistanceImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

//Auto includes
#include <itkIsoContourDistanceImageFilter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>


/**
 * @brief The ITKIsoContourDistanceImage class. See [Filter documentation](@ref ITKIsoContourDistanceImage) for details.
 */
class ITKIsoContourDistanceImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKIsoContourDistanceImage)
    SIMPL_STATIC_NEW_MACRO(ITKIsoContourDistanceImage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKIsoContourDistanceImage, AbstractFilter)

    virtual ~ITKIsoContourDistanceImage();

    SIMPL_FILTER_PARAMETER(double, LevelSetValue)
    Q_PROPERTY(double LevelSetValue READ getLevelSetValue WRITE setLevelSetValue)

    SIMPL_FILTER_PARAMETER(double, FarValue)
    Q_PROPERTY(double FarValue READ getFarValue WRITE setFarValue)



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
    ITKIsoContourDistanceImage();

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

    ITKIsoContourDistanceImage(const ITKIsoContourDistanceImage&); // Copy Constructor Not Implemented
    void operator=(const ITKIsoContourDistanceImage&); // Operator '=' Not Implemented
};

#endif /* _ITKIsoContourDistanceImage_H_ */
