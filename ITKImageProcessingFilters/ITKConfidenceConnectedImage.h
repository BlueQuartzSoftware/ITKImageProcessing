// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKConfidenceConnectedImage_h_
#define _ITKConfidenceConnectedImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKConfidenceConnectedImage class. See [Filter documentation](@ref ITKConfidenceConnectedImage) for details.
 */
class ITKConfidenceConnectedImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKConfidenceConnectedImage)
    SIMPL_STATIC_NEW_MACRO(ITKConfidenceConnectedImage)
    SIMPL_TYPE_MACRO_SUPER(ITKConfidenceConnectedImage, AbstractFilter)

    virtual ~ITKConfidenceConnectedImage();

    SIMPL_FILTER_PARAMETER(double, NumberOfIterations)
    Q_PROPERTY(double NumberOfIterations READ getNumberOfIterations WRITE setNumberOfIterations)

    SIMPL_FILTER_PARAMETER(double, Multiplier)
    Q_PROPERTY(double Multiplier READ getMultiplier WRITE setMultiplier)

    SIMPL_FILTER_PARAMETER(double, InitialNeighborhoodRadius)
    Q_PROPERTY(double InitialNeighborhoodRadius READ getInitialNeighborhoodRadius WRITE setInitialNeighborhoodRadius)

    SIMPL_FILTER_PARAMETER(int, ReplaceValue)
    Q_PROPERTY(int ReplaceValue READ getReplaceValue WRITE setReplaceValue)



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
    ITKConfidenceConnectedImage();

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

    ITKConfidenceConnectedImage(const ITKConfidenceConnectedImage&); // Copy Constructor Not Implemented
    void operator=(const ITKConfidenceConnectedImage&); // Operator '=' Not Implemented
};

#endif /* _ITKConfidenceConnectedImage_H_ */
