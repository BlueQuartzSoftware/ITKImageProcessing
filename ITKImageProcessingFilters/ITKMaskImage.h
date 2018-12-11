// File automatically generated

/*
 * Your License or Copyright can go here
 */

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkMaskImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKMaskImage class. See [Filter documentation](@ref ITKMaskImage) for details.
 */
class ITKImageProcessing_EXPORT ITKMaskImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKMaskImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)
  PYB11_PROPERTY(DataArrayPath MaskCellArrayPath READ getMaskCellArrayPath WRITE setMaskCellArrayPath)

public:
  SIMPL_SHARED_POINTERS(ITKMaskImage)
  SIMPL_FILTER_NEW_MACRO(ITKMaskImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMaskImage, AbstractFilter)

  ~ITKMaskImage() override;

  SIMPL_FILTER_PARAMETER(double, OutsideValue)
  Q_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)

  SIMPL_FILTER_PARAMETER(DataArrayPath, MaskCellArrayPath)
  Q_PROPERTY(DataArrayPath MaskCellArrayPath READ getMaskCellArrayPath WRITE setMaskCellArrayPath)

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
  SIMPL_INSTANCE_PROPERTY(DataContainerArray::Pointer, MaskContainerArray)

  ITKMaskImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheck();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void filterInternal() override;

  /**
  * @brief Applies the filter
  */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void filter();

  /**
  * @brief Converts data container MarkerCellArrayPath to uint16
  */

  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> typename std::enable_if<std::is_scalar<InputPixelType>::value>::type convertDataContainerType();

  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> typename std::enable_if<!std::is_scalar<InputPixelType>::value>::type convertDataContainerType();

public:
  ITKMaskImage(const ITKMaskImage&) = delete;            // Copy Constructor Not Implemented
  ITKMaskImage(ITKMaskImage&&) = delete;                 // Move Constructor Not Implemented
  ITKMaskImage& operator=(const ITKMaskImage&) = delete; // Copy Assignment Not Implemented
  ITKMaskImage& operator=(ITKMaskImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

