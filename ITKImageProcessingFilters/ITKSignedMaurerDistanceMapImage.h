// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSignedMaurerDistanceMapImage_h_
#define _ITKSignedMaurerDistanceMapImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKSignedMaurerDistanceMapImage class. See [Filter documentation](@ref ITKSignedMaurerDistanceMapImage) for details.
 */
class ITKSignedMaurerDistanceMapImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKSignedMaurerDistanceMapImage)
    SIMPL_STATIC_NEW_MACRO(ITKSignedMaurerDistanceMapImage)
    SIMPL_TYPE_MACRO_SUPER(ITKSignedMaurerDistanceMapImage, AbstractFilter)

    virtual ~ITKSignedMaurerDistanceMapImage();

    SIMPL_FILTER_PARAMETER(bool, InsideIsPositive)
    Q_PROPERTY(bool InsideIsPositive READ getInsideIsPositive WRITE setInsideIsPositive)

    SIMPL_FILTER_PARAMETER(bool, SquaredDistance)
    Q_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)

    SIMPL_FILTER_PARAMETER(bool, UseImageSpacing)
    Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)



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
    ITKSignedMaurerDistanceMapImage();

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

    ITKSignedMaurerDistanceMapImage(const ITKSignedMaurerDistanceMapImage&); // Copy Constructor Not Implemented
    void operator=(const ITKSignedMaurerDistanceMapImage&); // Operator '=' Not Implemented
};

#endif /* _ITKSignedMaurerDistanceMapImage_H_ */
