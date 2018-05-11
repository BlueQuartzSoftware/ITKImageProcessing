// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKMorphologicalWatershedFromMarkersImage_h_
#define _ITKMorphologicalWatershedFromMarkersImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkMorphologicalWatershedFromMarkersImageFilter.h>

/**
 * @brief The ITKMorphologicalWatershedFromMarkersImage class. See [Filter documentation](@ref ITKMorphologicalWatershedFromMarkersImage) for details.


#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"


 WARNING: This filter is not thread safe. It uses a member variable to cast the marker image into the expected pixel type (uint16).
 */
class ITKImageProcessing_EXPORT ITKMorphologicalWatershedFromMarkersImage : public ITKImageProcessingBase
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ITKMorphologicalWatershedFromMarkersImage SUPERCLASS ITKImageProcessingBase)
  PYB11_PROPERTY(bool MarkWatershedLine READ getMarkWatershedLine WRITE setMarkWatershedLine)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(DataArrayPath MarkerCellArrayPath READ getMarkerCellArrayPath WRITE setMarkerCellArrayPath)

public:
  SIMPL_SHARED_POINTERS(ITKMorphologicalWatershedFromMarkersImage)
  SIMPL_FILTER_NEW_MACRO(ITKMorphologicalWatershedFromMarkersImage)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMorphologicalWatershedFromMarkersImage, AbstractFilter)

  ~ITKMorphologicalWatershedFromMarkersImage() override;

  SIMPL_FILTER_PARAMETER(bool, MarkWatershedLine)
  Q_PROPERTY(bool MarkWatershedLine READ getMarkWatershedLine WRITE setMarkWatershedLine)

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

  SIMPL_FILTER_PARAMETER(DataArrayPath, MarkerCellArrayPath)
  Q_PROPERTY(DataArrayPath MarkerCellArrayPath READ getMarkerCellArrayPath WRITE setMarkerCellArrayPath)

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
  SIMPL_INSTANCE_PROPERTY(DataContainerArray::Pointer, MarkerContainerArray)

  ITKMorphologicalWatershedFromMarkersImage();

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

  /**
  * @brief Converts data container MarkerCellArrayPath to uint16
  */
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void convertDataContainerType();

private:
  ITKMorphologicalWatershedFromMarkersImage(const ITKMorphologicalWatershedFromMarkersImage&) = delete; // Copy Constructor Not Implemented
  ITKMorphologicalWatershedFromMarkersImage(ITKMorphologicalWatershedFromMarkersImage&&) = delete;      // Move Constructor Not Implemented
  ITKMorphologicalWatershedFromMarkersImage& operator=(const ITKMorphologicalWatershedFromMarkersImage&) = delete; // Copy Assignment Not Implemented
  ITKMorphologicalWatershedFromMarkersImage& operator=(ITKMorphologicalWatershedFromMarkersImage&&) = delete;      // Move Assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKMorphologicalWatershedFromMarkersImage_H_ */
