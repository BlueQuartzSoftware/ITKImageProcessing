// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKBoundedReciprocalImage_h_
#define _ITKBoundedReciprocalImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKBoundedReciprocalImage class. See [Filter documentation](@ref ITKBoundedReciprocalImage) for details.
 */
class ITKBoundedReciprocalImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKBoundedReciprocalImage)
    SIMPL_STATIC_NEW_MACRO(ITKBoundedReciprocalImage)
    SIMPL_TYPE_MACRO_SUPER(ITKBoundedReciprocalImage, AbstractFilter)

    virtual ~ITKBoundedReciprocalImage();



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
    ITKBoundedReciprocalImage();

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

    ITKBoundedReciprocalImage(const ITKBoundedReciprocalImage&); // Copy Constructor Not Implemented
    void operator=(const ITKBoundedReciprocalImage&); // Operator '=' Not Implemented
};

#endif /* _ITKBoundedReciprocalImage_H_ */
