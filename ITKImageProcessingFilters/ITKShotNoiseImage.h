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
#include <itkShotNoiseImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKShotNoiseImage class. See [Filter documentation](@ref ITKShotNoiseImage) for details.
 */
class ITKImageProcessing_EXPORT ITKShotNoiseImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKShotNoiseImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKShotNoiseImage)
  PYB11_FILTER_NEW_MACRO(ITKShotNoiseImage)
  PYB11_PROPERTY(double Scale READ getScale WRITE setScale)
  PYB11_PROPERTY(double Seed READ getSeed WRITE setSeed)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKShotNoiseImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKShotNoiseImage> New();

    /**
    * @brief Returns the name of the class for ITKShotNoiseImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKShotNoiseImage
    */
    static QString ClassName();


  ~ITKShotNoiseImage() override;

    /**
    * @brief Setter property for Scale
    */
    void setScale(double value); 
    /**
    * @brief Getter property for Scale
    * @return Value of Scale
    */
    double getScale() const;
  Q_PROPERTY(double Scale READ getScale WRITE setScale)

    /**
    * @brief Setter property for Seed
    */
    void setSeed(double value); 
    /**
    * @brief Getter property for Seed
    * @return Value of Seed
    */
    double getSeed() const;
  Q_PROPERTY(double Seed READ getSeed WRITE setSeed)


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
  ITKShotNoiseImage();

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
  ITKShotNoiseImage(const ITKShotNoiseImage&) = delete;    // Copy Constructor Not Implemented
  ITKShotNoiseImage(ITKShotNoiseImage&&) = delete;         // Move Constructor Not Implemented
  ITKShotNoiseImage& operator=(const ITKShotNoiseImage&) = delete; // Copy Assignment Not Implemented
  ITKShotNoiseImage& operator=(ITKShotNoiseImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Scale = {};
    double m_Seed = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

