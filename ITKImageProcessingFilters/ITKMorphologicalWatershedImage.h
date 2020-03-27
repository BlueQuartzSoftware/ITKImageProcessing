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
#include <itkMorphologicalWatershedImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKMorphologicalWatershedImage class. See [Filter documentation](@ref ITKMorphologicalWatershedImage) for details.
 */
class ITKImageProcessing_EXPORT ITKMorphologicalWatershedImage : public ITKImageProcessingBase
{
  Q_OBJECT

  // Start Python bindings declarations
  PYB11_BEGIN_BINDINGS(ITKMorphologicalWatershedImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKMorphologicalWatershedImage)
  PYB11_FILTER_NEW_MACRO(ITKMorphologicalWatershedImage)
  PYB11_PROPERTY(double Level READ getLevel WRITE setLevel)
  PYB11_PROPERTY(bool MarkWatershedLine READ getMarkWatershedLine WRITE setMarkWatershedLine)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_END_BINDINGS()
  // End Python bindings declarations

public:
    using Self = ITKMorphologicalWatershedImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKMorphologicalWatershedImage> New();

    /**
    * @brief Returns the name of the class for ITKMorphologicalWatershedImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKMorphologicalWatershedImage
    */
    static QString ClassName();


  ~ITKMorphologicalWatershedImage() override;

    /**
    * @brief Setter property for Level
    */
    void setLevel(double value); 
    /**
    * @brief Getter property for Level
    * @return Value of Level
    */
    double getLevel() const;

  Q_PROPERTY(double Level READ getLevel WRITE setLevel)

    /**
    * @brief Setter property for MarkWatershedLine
    */
    void setMarkWatershedLine(bool value); 
    /**
    * @brief Getter property for MarkWatershedLine
    * @return Value of MarkWatershedLine
    */
    bool getMarkWatershedLine() const;

  Q_PROPERTY(bool MarkWatershedLine READ getMarkWatershedLine WRITE setMarkWatershedLine)

    /**
    * @brief Setter property for FullyConnected
    */
    void setFullyConnected(bool value); 
    /**
    * @brief Getter property for FullyConnected
    * @return Value of FullyConnected
    */
    bool getFullyConnected() const;

  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)


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
  ITKMorphologicalWatershedImage();

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
  ITKMorphologicalWatershedImage(const ITKMorphologicalWatershedImage&) = delete;    // Copy Constructor Not Implemented
  ITKMorphologicalWatershedImage(ITKMorphologicalWatershedImage&&) = delete;         // Move Constructor Not Implemented
  ITKMorphologicalWatershedImage& operator=(const ITKMorphologicalWatershedImage&) = delete; // Copy Assignment Not Implemented
  ITKMorphologicalWatershedImage& operator=(ITKMorphologicalWatershedImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_Level = {};
    bool m_MarkWatershedLine = {};
    bool m_FullyConnected = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

