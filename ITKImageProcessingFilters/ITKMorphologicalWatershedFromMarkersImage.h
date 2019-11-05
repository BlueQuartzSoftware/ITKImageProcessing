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
#include <itkMorphologicalWatershedFromMarkersImageFilter.h>

/**
 * @brief The ITKMorphologicalWatershedFromMarkersImage class. See [Filter documentation](@ref ITKMorphologicalWatershedFromMarkersImage) for details.


#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"


 WARNING: This filter is not thread safe. It uses a member variable to cast the marker image into the expected pixel type (uint16).
 */
class ITKImageProcessing_EXPORT ITKMorphologicalWatershedFromMarkersImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKMorphologicalWatershedFromMarkersImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKMorphologicalWatershedFromMarkersImage)
  PYB11_FILTER_NEW_MACRO(ITKMorphologicalWatershedFromMarkersImage)
  PYB11_FILTER_PARAMETER(bool, MarkWatershedLine)
  PYB11_FILTER_PARAMETER(bool, FullyConnected)
  PYB11_FILTER_PARAMETER(DataArrayPath, MarkerCellArrayPath)
  PYB11_PROPERTY(bool MarkWatershedLine READ getMarkWatershedLine WRITE setMarkWatershedLine)
  PYB11_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)
  PYB11_PROPERTY(DataArrayPath MarkerCellArrayPath READ getMarkerCellArrayPath WRITE setMarkerCellArrayPath)
#endif

public:
    using Self = ITKMorphologicalWatershedFromMarkersImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKMorphologicalWatershedFromMarkersImage> New();

    /**
    * @brief Returns the name of the class for ITKMorphologicalWatershedFromMarkersImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKMorphologicalWatershedFromMarkersImage
    */
    static QString ClassName();


  ~ITKMorphologicalWatershedFromMarkersImage() override;

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
    * @brief Setter property for MarkerCellArrayPath
    */
    void setMarkerCellArrayPath(const DataArrayPath& value); 
    /**
    * @brief Getter property for MarkerCellArrayPath
    * @return Value of MarkerCellArrayPath
    */
    DataArrayPath getMarkerCellArrayPath() const;

  Q_PROPERTY(DataArrayPath MarkerCellArrayPath READ getMarkerCellArrayPath WRITE setMarkerCellArrayPath)

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
    /**
    * @brief Setter property for MarkerContainerArray
    */
    void setMarkerContainerArray(const DataContainerArray::Pointer& value); 
    /**
    * @brief Getter property for MarkerContainerArray
    * @return Value of MarkerContainerArray
    */
    DataContainerArray::Pointer getMarkerContainerArray() const;


  ITKMorphologicalWatershedFromMarkersImage();

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
  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> void convertDataContainerType();

public:
  ITKMorphologicalWatershedFromMarkersImage(const ITKMorphologicalWatershedFromMarkersImage&) = delete; // Copy Constructor Not Implemented
  ITKMorphologicalWatershedFromMarkersImage(ITKMorphologicalWatershedFromMarkersImage&&) = delete;      // Move Constructor Not Implemented
  ITKMorphologicalWatershedFromMarkersImage& operator=(const ITKMorphologicalWatershedFromMarkersImage&) = delete; // Copy Assignment Not Implemented
  ITKMorphologicalWatershedFromMarkersImage& operator=(ITKMorphologicalWatershedFromMarkersImage&&) = delete;      // Move Assignment Not Implemented

  private:
    bool m_MarkWatershedLine = {};
    bool m_FullyConnected = {};
    DataArrayPath m_MarkerCellArrayPath = {};
    DataContainerArray::Pointer m_MarkerContainerArray = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

