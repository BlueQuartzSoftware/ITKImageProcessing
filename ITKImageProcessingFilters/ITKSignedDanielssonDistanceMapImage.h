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
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkSignedDanielssonDistanceMapImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKSignedDanielssonDistanceMapImage class. See [Filter documentation](@ref ITKSignedDanielssonDistanceMapImage) for details.
 */
class ITKImageProcessing_EXPORT ITKSignedDanielssonDistanceMapImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKSignedDanielssonDistanceMapImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKSignedDanielssonDistanceMapImage)
  PYB11_FILTER_NEW_MACRO(ITKSignedDanielssonDistanceMapImage)
  PYB11_FILTER_PARAMETER(bool, InsideIsPositive)
  PYB11_FILTER_PARAMETER(bool, SquaredDistance)
  PYB11_FILTER_PARAMETER(bool, UseImageSpacing)
  PYB11_PROPERTY(bool InsideIsPositive READ getInsideIsPositive WRITE setInsideIsPositive)
  PYB11_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)
  PYB11_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)
#endif

public:
    using Self = ITKSignedDanielssonDistanceMapImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKSignedDanielssonDistanceMapImage> New();

    /**
    * @brief Returns the name of the class for ITKSignedDanielssonDistanceMapImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKSignedDanielssonDistanceMapImage
    */
    static QString ClassName();


  ~ITKSignedDanielssonDistanceMapImage() override;

    /**
    * @brief Setter property for InsideIsPositive
    */
    void setInsideIsPositive(bool value); 
    /**
    * @brief Getter property for InsideIsPositive
    * @return Value of InsideIsPositive
    */
    bool getInsideIsPositive() const;

  Q_PROPERTY(bool InsideIsPositive READ getInsideIsPositive WRITE setInsideIsPositive)

    /**
    * @brief Setter property for SquaredDistance
    */
    void setSquaredDistance(bool value); 
    /**
    * @brief Getter property for SquaredDistance
    * @return Value of SquaredDistance
    */
    bool getSquaredDistance() const;

  Q_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)

    /**
    * @brief Setter property for UseImageSpacing
    */
    void setUseImageSpacing(bool value); 
    /**
    * @brief Getter property for UseImageSpacing
    * @return Value of UseImageSpacing
    */
    bool getUseImageSpacing() const;

  Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)


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
  ITKSignedDanielssonDistanceMapImage();

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

public:
  ITKSignedDanielssonDistanceMapImage(const ITKSignedDanielssonDistanceMapImage&) = delete;    // Copy Constructor Not Implemented
  ITKSignedDanielssonDistanceMapImage(ITKSignedDanielssonDistanceMapImage&&) = delete;         // Move Constructor Not Implemented
  ITKSignedDanielssonDistanceMapImage& operator=(const ITKSignedDanielssonDistanceMapImage&) = delete; // Copy Assignment Not Implemented
  ITKSignedDanielssonDistanceMapImage& operator=(ITKSignedDanielssonDistanceMapImage&&) = delete;      // Move Assignment Not Implemented

  private:
    bool m_InsideIsPositive = {};
    bool m_SquaredDistance = {};
    bool m_UseImageSpacing = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

