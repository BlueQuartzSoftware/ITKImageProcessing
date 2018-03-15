// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMorphologicalWatershedImage_h_
#define _ITKMorphologicalWatershedImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkMorphologicalWatershedImageFilter.h>


/**
 * @brief The ITKMorphologicalWatershedImage class. See [Filter documentation](@ref ITKMorphologicalWatershedImage) for details.
 */
class ITKMorphologicalWatershedImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKMorphologicalWatershedImage)
  SIMPL_STATIC_NEW_MACRO(ITKMorphologicalWatershedImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMorphologicalWatershedImage, AbstractFilter)

  virtual ~ITKMorphologicalWatershedImage();

  SIMPL_FILTER_PARAMETER(double, Level)
  Q_PROPERTY(double Level READ getLevel WRITE setLevel)

  SIMPL_FILTER_PARAMETER(bool, MarkWatershedLine)
  Q_PROPERTY(bool MarkWatershedLine READ getMarkWatershedLine WRITE setMarkWatershedLine)

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)


  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  virtual const QUuid getUuid() override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKMorphologicalWatershedImage();

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
  ITKMorphologicalWatershedImage(const ITKMorphologicalWatershedImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKMorphologicalWatershedImage&) = delete; // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKMorphologicalWatershedImage_H_ */
