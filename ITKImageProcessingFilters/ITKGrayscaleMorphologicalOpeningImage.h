// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGrayscaleMorphologicalOpeningImage_h_
#define _ITKGrayscaleMorphologicalOpeningImage_h_

#include "ITKImageBase.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

/**
 * @brief The ITKGrayscaleMorphologicalOpeningImage class. See [Filter documentation](@ref ITKGrayscaleMorphologicalOpeningImage) for details.
 */
class ITKGrayscaleMorphologicalOpeningImage : public ITKImageBase
{
  Q_OBJECT

  public:
    SIMPL_SHARED_POINTERS(ITKGrayscaleMorphologicalOpeningImage)
    SIMPL_STATIC_NEW_MACRO(ITKGrayscaleMorphologicalOpeningImage)
    SIMPL_TYPE_MACRO_SUPER(ITKGrayscaleMorphologicalOpeningImage, AbstractFilter)

    virtual ~ITKGrayscaleMorphologicalOpeningImage();

    SIMPL_FILTER_PARAMETER(bool, SafeBorder)
    Q_PROPERTY(bool SafeBorder READ getSafeBorder WRITE setSafeBorder)



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
    ITKGrayscaleMorphologicalOpeningImage();

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

    ITKGrayscaleMorphologicalOpeningImage(const ITKGrayscaleMorphologicalOpeningImage&); // Copy Constructor Not Implemented
    void operator=(const ITKGrayscaleMorphologicalOpeningImage&); // Operator '=' Not Implemented
};

#endif /* _ITKGrayscaleMorphologicalOpeningImage_H_ */
