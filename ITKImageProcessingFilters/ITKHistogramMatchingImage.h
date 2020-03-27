/*
 * Your License or Copyright can go here
 */

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include <memory>

#include "ITKImageProcessingBase.h"

#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkHistogramMatchingImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKHistogramMatchingImage class. See [Filter documentation](@ref ITKHistogramMatchingImage) for details.
 */
class ITKImageProcessing_EXPORT ITKHistogramMatchingImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKHistogramMatchingImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKHistogramMatchingImage)
  PYB11_FILTER_NEW_MACRO(ITKHistogramMatchingImage)
  PYB11_PROPERTY(DataArrayPath ReferenceCellArrayPath READ getReferenceCellArrayPath WRITE setReferenceCellArrayPath)
  PYB11_PROPERTY(double NumberOfHistogramLevels READ getNumberOfHistogramLevels WRITE setNumberOfHistogramLevels)
  PYB11_PROPERTY(double NumberOfMatchPoints READ getNumberOfMatchPoints WRITE setNumberOfMatchPoints)
  PYB11_PROPERTY(bool ThresholdAtMeanIntensity READ getThresholdAtMeanIntensity WRITE setThresholdAtMeanIntensity)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKHistogramMatchingImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKHistogramMatchingImage> New();

    /**
    * @brief Returns the name of the class for ITKHistogramMatchingImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKHistogramMatchingImage
    */
    static QString ClassName();


  ~ITKHistogramMatchingImage() override;

    /**
    * @brief Setter property for ReferenceCellArrayPath
    */
    void setReferenceCellArrayPath(const DataArrayPath& value); 
    /**
    * @brief Getter property for ReferenceCellArrayPath
    * @return Value of ReferenceCellArrayPath
    */
    DataArrayPath getReferenceCellArrayPath() const;

  Q_PROPERTY(DataArrayPath ReferenceCellArrayPath READ getReferenceCellArrayPath WRITE setReferenceCellArrayPath)

    /**
    * @brief Setter property for NumberOfHistogramLevels
    */
    void setNumberOfHistogramLevels(double value); 
    /**
    * @brief Getter property for NumberOfHistogramLevels
    * @return Value of NumberOfHistogramLevels
    */
    double getNumberOfHistogramLevels() const;

  Q_PROPERTY(double NumberOfHistogramLevels READ getNumberOfHistogramLevels WRITE setNumberOfHistogramLevels)

    /**
    * @brief Setter property for NumberOfMatchPoints
    */
    void setNumberOfMatchPoints(double value); 
    /**
    * @brief Getter property for NumberOfMatchPoints
    * @return Value of NumberOfMatchPoints
    */
    double getNumberOfMatchPoints() const;

  Q_PROPERTY(double NumberOfMatchPoints READ getNumberOfMatchPoints WRITE setNumberOfMatchPoints)

    /**
    * @brief Setter property for ThresholdAtMeanIntensity
    */
    void setThresholdAtMeanIntensity(bool value); 
    /**
    * @brief Getter property for ThresholdAtMeanIntensity
    * @return Value of ThresholdAtMeanIntensity
    */
    bool getThresholdAtMeanIntensity() const;

  Q_PROPERTY(bool ThresholdAtMeanIntensity READ getThresholdAtMeanIntensity WRITE setThresholdAtMeanIntensity)

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  QString getHumanLabel() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  QUuid getUuid() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

protected:
  ITKHistogramMatchingImage();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheck() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension> void dataCheckImpl();

  /**
  * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
  */
  void filterInternal() override;

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

public:
  ITKHistogramMatchingImage(const ITKHistogramMatchingImage&) = delete; // Copy Constructor Not Implemented
  ITKHistogramMatchingImage(ITKHistogramMatchingImage&&) = delete;      // Move Constructor Not Implemented
  ITKHistogramMatchingImage& operator=(const ITKHistogramMatchingImage&) = delete; // Copy Assignment Not Implemented
  ITKHistogramMatchingImage& operator=(ITKHistogramMatchingImage&&) = delete;      // Move Assignment Not Implemented

  private:
    DataArrayPath m_ReferenceCellArrayPath = {};
    double m_NumberOfHistogramLevels = {};
    double m_NumberOfMatchPoints = {};
    bool m_ThresholdAtMeanIntensity = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

