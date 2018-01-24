/*
 * Your License or Copyright can go here
 */

#ifndef _ITKHistogramMatchingImage_h_
#define _ITKHistogramMatchingImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkHistogramMatchingImageFilter.h>

/**
 * @brief The ITKHistogramMatchingImage class. See [Filter documentation](@ref ITKHistogramMatchingImage) for details.
 */
class ITKHistogramMatchingImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKHistogramMatchingImage)
  SIMPL_STATIC_NEW_MACRO(ITKHistogramMatchingImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKHistogramMatchingImage, AbstractFilter)

  virtual ~ITKHistogramMatchingImage();

  SIMPL_FILTER_PARAMETER(DataArrayPath, ReferenceCellArrayPath)
  Q_PROPERTY(DataArrayPath ReferenceCellArrayPath READ getReferenceCellArrayPath WRITE setReferenceCellArrayPath)

  SIMPL_FILTER_PARAMETER(double, NumberOfHistogramLevels)
  Q_PROPERTY(double NumberOfHistogramLevels READ getNumberOfHistogramLevels WRITE setNumberOfHistogramLevels)

  SIMPL_FILTER_PARAMETER(double, NumberOfMatchPoints)
  Q_PROPERTY(double NumberOfMatchPoints READ getNumberOfMatchPoints WRITE setNumberOfMatchPoints)

  SIMPL_FILTER_PARAMETER(bool, ThresholdAtMeanIntensity)
  Q_PROPERTY(bool ThresholdAtMeanIntensity READ getThresholdAtMeanIntensity WRITE setThresholdAtMeanIntensity)

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
  ITKHistogramMatchingImage();

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
   * @brief getImageDimension Returns the dimension of an image.
   */
  size_t getImageDimension(const DataArrayPath& path);

  /**
   * @brief CheckArrayExists Returns 1 if the DataArrayPath exists, 0 otherwise.
   */
  int CheckArrayExists(const DataArrayPath& path);

  /**
   * @brief CompareImageTypes Compares image types (dimension, component type, pixel type).
   *  Sets error condition if they are different.
   */
  void CompareImageTypes(const DataArrayPath& path1, const DataArrayPath& path2);

  /**
   * @brief CompareImagePixelTypes Compares image pixel types. Sets error condition if they are different.
   */
  void CompareImagePixelTypes(const DataArrayPath& path1, const DataArrayPath& path2);

private:
  ITKHistogramMatchingImage(const ITKHistogramMatchingImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKHistogramMatchingImage&);            // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKHistogramMatchingImage_H_ */
