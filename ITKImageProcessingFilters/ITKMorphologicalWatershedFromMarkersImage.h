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

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkMorphologicalWatershedFromMarkersImageFilter.h>

/**
 * @brief The ITKMorphologicalWatershedFromMarkersImage class. See [Filter documentation](@ref ITKMorphologicalWatershedFromMarkersImage) for details.

 WARNING: This filter is not thread safe. It uses a member variable to cast the marker image into the expected pixel type (uint16).
 */
class ITKMorphologicalWatershedFromMarkersImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKMorphologicalWatershedFromMarkersImage)
  SIMPL_STATIC_NEW_MACRO(ITKMorphologicalWatershedFromMarkersImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKMorphologicalWatershedFromMarkersImage, AbstractFilter)

  virtual ~ITKMorphologicalWatershedFromMarkersImage();

  SIMPL_FILTER_PARAMETER(bool, MarkWatershedLine)
  Q_PROPERTY(bool MarkWatershedLine READ getMarkWatershedLine WRITE setMarkWatershedLine)

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)

  SIMPL_FILTER_PARAMETER(DataArrayPath, MarkerCellArrayPath)
  Q_PROPERTY(DataArrayPath MarkerCellArrayPath READ getMarkerCellArrayPath WRITE setMarkerCellArrayPath)

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
  void operator=(const ITKMorphologicalWatershedFromMarkersImage&);                            // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKMorphologicalWatershedFromMarkersImage_H_ */
