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
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <SIMPLib/FilterParameters/FloatVec3FilterParameter.h>
#include <itkRelabelComponentImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKRelabelComponentImage class. See [Filter documentation](@ref ITKRelabelComponentImage) for details.
 */
class ITKImageProcessing_EXPORT ITKRelabelComponentImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKRelabelComponentImage SUPERCLASS ITKImageProcessingBase)
  PYB11_FILTER()
  PYB11_SHARED_POINTERS(ITKRelabelComponentImage)
  PYB11_FILTER_NEW_MACRO(ITKRelabelComponentImage)
  PYB11_PROPERTY(double MinimumObjectSize READ getMinimumObjectSize WRITE setMinimumObjectSize)
  PYB11_PROPERTY(bool SortByObjectSize READ getSortByObjectSize WRITE setSortByObjectSize)
  PYB11_PROPERTY(double NumberOfObjects READ getNumberOfObjects WRITE setNumberOfObjects)
  PYB11_PROPERTY(double OriginalNumberOfObjects READ getOriginalNumberOfObjects WRITE setOriginalNumberOfObjects)
  PYB11_PROPERTY(FloatVec3Type SizeOfObjectsInPhysicalUnits READ getSizeOfObjectsInPhysicalUnits WRITE setSizeOfObjectsInPhysicalUnits)
  PYB11_PROPERTY(FloatVec3Type SizeOfObjectsInPixels READ getSizeOfObjectsInPixels WRITE setSizeOfObjectsInPixels)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKRelabelComponentImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKRelabelComponentImage> New();

    /**
    * @brief Returns the name of the class for ITKRelabelComponentImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKRelabelComponentImage
    */
    static QString ClassName();


  ~ITKRelabelComponentImage() override;

    /**
    * @brief Setter property for MinimumObjectSize
    */
    void setMinimumObjectSize(double value); 
    /**
    * @brief Getter property for MinimumObjectSize
    * @return Value of MinimumObjectSize
    */
    double getMinimumObjectSize() const;
  Q_PROPERTY(double MinimumObjectSize READ getMinimumObjectSize WRITE setMinimumObjectSize)

    /**
    * @brief Setter property for SortByObjectSize
    */
    void setSortByObjectSize(bool value); 
    /**
    * @brief Getter property for SortByObjectSize
    * @return Value of SortByObjectSize
    */
    bool getSortByObjectSize() const;
  Q_PROPERTY(bool SortByObjectSize READ getSortByObjectSize WRITE setSortByObjectSize)

    /**
    * @brief Setter property for NumberOfObjects
    */
    void setNumberOfObjects(double value); 
    /**
    * @brief Getter property for NumberOfObjects
    * @return Value of NumberOfObjects
    */
    double getNumberOfObjects() const;
  Q_PROPERTY(double NumberOfObjects READ getNumberOfObjects)

    /**
    * @brief Setter property for OriginalNumberOfObjects
    */
    void setOriginalNumberOfObjects(double value); 
    /**
    * @brief Getter property for OriginalNumberOfObjects
    * @return Value of OriginalNumberOfObjects
    */
    double getOriginalNumberOfObjects() const;
  Q_PROPERTY(double OriginalNumberOfObjects READ getOriginalNumberOfObjects)

    /**
    * @brief Setter property for SizeOfObjectsInPhysicalUnits
    */
    void setSizeOfObjectsInPhysicalUnits(const FloatVec3Type& value); 
    /**
    * @brief Getter property for SizeOfObjectsInPhysicalUnits
    * @return Value of SizeOfObjectsInPhysicalUnits
    */
    FloatVec3Type getSizeOfObjectsInPhysicalUnits() const;
  Q_PROPERTY(FloatVec3Type SizeOfObjectsInPhysicalUnits READ getSizeOfObjectsInPhysicalUnits)

    /**
    * @brief Setter property for SizeOfObjectsInPixels
    */
    void setSizeOfObjectsInPixels(const FloatVec3Type& value); 
    /**
    * @brief Getter property for SizeOfObjectsInPixels
    * @return Value of SizeOfObjectsInPixels
    */
    FloatVec3Type getSizeOfObjectsInPixels() const;
  Q_PROPERTY(FloatVec3Type SizeOfObjectsInPixels READ getSizeOfObjectsInPixels)

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
  ITKRelabelComponentImage();

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
  ITKRelabelComponentImage(const ITKRelabelComponentImage&) = delete;    // Copy Constructor Not Implemented
  ITKRelabelComponentImage(ITKRelabelComponentImage&&) = delete;         // Move Constructor Not Implemented
  ITKRelabelComponentImage& operator=(const ITKRelabelComponentImage&) = delete; // Copy Assignment Not Implemented
  ITKRelabelComponentImage& operator=(ITKRelabelComponentImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_MinimumObjectSize = {};
    bool m_SortByObjectSize = {};
    double m_NumberOfObjects = {};
    double m_OriginalNumberOfObjects = {};
    FloatVec3Type m_SizeOfObjectsInPhysicalUnits = {};
    FloatVec3Type m_SizeOfObjectsInPixels = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

