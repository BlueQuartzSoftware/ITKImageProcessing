// File automatically generated

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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkIsoContourDistanceImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKIsoContourDistanceImage class. See [Filter documentation](@ref ITKIsoContourDistanceImage) for details.
 */
class ITKImageProcessing_EXPORT ITKIsoContourDistanceImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKIsoContourDistanceImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKIsoContourDistanceImage)
  PYB11_FILTER_NEW_MACRO(ITKIsoContourDistanceImage)
  PYB11_PROPERTY(double LevelSetValue READ getLevelSetValue WRITE setLevelSetValue)
  PYB11_PROPERTY(double FarValue READ getFarValue WRITE setFarValue)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKIsoContourDistanceImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKIsoContourDistanceImage> New();

    /**
    * @brief Returns the name of the class for ITKIsoContourDistanceImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKIsoContourDistanceImage
    */
    static QString ClassName();


  ~ITKIsoContourDistanceImage() override;

    /**
    * @brief Setter property for LevelSetValue
    */
    void setLevelSetValue(double value); 
    /**
    * @brief Getter property for LevelSetValue
    * @return Value of LevelSetValue
    */
    double getLevelSetValue() const;
  Q_PROPERTY(double LevelSetValue READ getLevelSetValue WRITE setLevelSetValue)

    /**
    * @brief Setter property for FarValue
    */
    void setFarValue(double value); 
    /**
    * @brief Getter property for FarValue
    * @return Value of FarValue
    */
    double getFarValue() const;
  Q_PROPERTY(double FarValue READ getFarValue WRITE setFarValue)


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
  ITKIsoContourDistanceImage();

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

public:
  ITKIsoContourDistanceImage(const ITKIsoContourDistanceImage&) = delete;    // Copy Constructor Not Implemented
  ITKIsoContourDistanceImage(ITKIsoContourDistanceImage&&) = delete;         // Move Constructor Not Implemented
  ITKIsoContourDistanceImage& operator=(const ITKIsoContourDistanceImage&) = delete; // Copy Assignment Not Implemented
  ITKIsoContourDistanceImage& operator=(ITKIsoContourDistanceImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_LevelSetValue = {};
    double m_FarValue = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

