// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKGrayscaleMorphologicalClosingImage_h_
#define _ITKGrayscaleMorphologicalClosingImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <SIMPLib/FilterParameters/IntFilterParameter.h>
#include <itkGrayscaleMorphologicalClosingImageFilter.h>

/**
 * @brief The ITKGrayscaleMorphologicalClosingImage class. See [Filter documentation](@ref ITKGrayscaleMorphologicalClosingImage) for details.
 */
class ITKGrayscaleMorphologicalClosingImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKGrayscaleMorphologicalClosingImage)
  SIMPL_STATIC_NEW_MACRO(ITKGrayscaleMorphologicalClosingImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKGrayscaleMorphologicalClosingImage, AbstractFilter)

  virtual ~ITKGrayscaleMorphologicalClosingImage();

  SIMPL_FILTER_PARAMETER(bool, SafeBorder)
  Q_PROPERTY(bool SafeBorder READ getSafeBorder WRITE setSafeBorder)

  SIMPL_FILTER_PARAMETER(FloatVec3_t, KernelRadius)
  Q_PROPERTY(FloatVec3_t KernelRadius READ getKernelRadius WRITE setKernelRadius)

  SIMPL_FILTER_PARAMETER(int, KernelType)
  Q_PROPERTY(int KernelType READ getKernelType WRITE setKernelType)

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
  ITKGrayscaleMorphologicalClosingImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void virtual dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheck();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void virtual filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

private:
  ITKGrayscaleMorphologicalClosingImage(const ITKGrayscaleMorphologicalClosingImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKGrayscaleMorphologicalClosingImage&);                        // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKGrayscaleMorphologicalClosingImage_H_ */
