// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKRegionalMinimaImage_h_
#define _ITKRegionalMinimaImage_h_

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
#include <itkRegionalMinimaImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKRegionalMinimaImage class. See [Filter documentation](@ref ITKRegionalMinimaImage) for details.
 */
class ITKImageProcessing_EXPORT ITKRegionalMinimaImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKRegionalMinimaImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double BackgroundValue READ getBackgroundValue WRITE setBackgroundValue)
  PYB11_PROPERTY(double ForegroundValue READ getForegroundValue WRITE setForegroundValue)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(bool FlatIsMinima READ getFlatIsMinima WRITE setFlatIsMinima)

public:
  SIMPL_SHARED_POINTERS(ITKRegionalMinimaImage)
  SIMPL_FILTER_NEW_MACRO(ITKRegionalMinimaImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKRegionalMinimaImage, AbstractFilter)

  ~ITKRegionalMinimaImage() override;

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
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  const QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  const QUuid getUuid() override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKRegionalMinimaImage();

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
  ITKRegionalMinimaImage(const ITKRegionalMinimaImage&) = delete;    // Copy Constructor Not Implemented
  ITKRegionalMinimaImage(ITKRegionalMinimaImage&&) = delete;         // Move Constructor Not Implemented
  ITKRegionalMinimaImage& operator=(const ITKRegionalMinimaImage&) = delete; // Copy Assignment Not Implemented
  ITKRegionalMinimaImage& operator=(ITKRegionalMinimaImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKRegionalMinimaImage_H_ */
