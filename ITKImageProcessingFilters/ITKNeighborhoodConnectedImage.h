// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKNeighborhoodConnectedImage_h_
#define _ITKNeighborhoodConnectedImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKNeighborhoodConnectedImage class. See [Filter documentation](@ref ITKNeighborhoodConnectedImage) for details.
 */
class ITKNeighborhoodConnectedImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKNeighborhoodConnectedImage)
    SIMPL_STATIC_NEW_MACRO(ITKNeighborhoodConnectedImage)
    SIMPL_TYPE_MACRO_SUPER(ITKNeighborhoodConnectedImage, AbstractFilter)

    virtual ~ITKNeighborhoodConnectedImage();

    SIMPL_FILTER_PARAMETER(double, Lower)
    Q_PROPERTY(double Lower READ getLower WRITE setLower)

    SIMPL_FILTER_PARAMETER(double, Upper)
    Q_PROPERTY(double Upper READ getUpper WRITE setUpper)

    SIMPL_FILTER_PARAMETER(double, Radius)
    Q_PROPERTY(double Radius READ getRadius WRITE setRadius)

    SIMPL_FILTER_PARAMETER(double, ReplaceValue)
    Q_PROPERTY(double ReplaceValue READ getReplaceValue WRITE setReplaceValue)



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
    ITKNeighborhoodConnectedImage();

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

    ITKNeighborhoodConnectedImage(const ITKNeighborhoodConnectedImage&); // Copy Constructor Not Implemented
    void operator=(const ITKNeighborhoodConnectedImage&); // Operator '=' Not Implemented
};

#endif /* _ITKNeighborhoodConnectedImage_H_ */
