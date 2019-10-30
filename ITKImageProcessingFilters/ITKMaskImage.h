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
#include <itkMaskImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKMaskImage class. See [Filter documentation](@ref ITKMaskImage) for details.
 */
class ITKImageProcessing_EXPORT ITKMaskImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKMaskImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKMaskImage)
  PYB11_FILTER_NEW_MACRO(ITKMaskImage)
  PYB11_FILTER_PARAMETER(double, OutsideValue)
  PYB11_FILTER_PARAMETER(DataArrayPath, MaskCellArrayPath)
  PYB11_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)
  PYB11_PROPERTY(DataArrayPath MaskCellArrayPath READ getMaskCellArrayPath WRITE setMaskCellArrayPath)
#endif

public:
    using Self = ITKMaskImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKMaskImage> New();

    /**
    * @brief Returns the name of the class for ITKMaskImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKMaskImage
    */
    static QString ClassName();


  ~ITKMaskImage() override;

    /**
    * @brief Setter property for OutsideValue
    */
    void setOutsideValue(double value); 
    /**
    * @brief Getter property for OutsideValue
    * @return Value of OutsideValue
    */
    double getOutsideValue() const;

  Q_PROPERTY(double OutsideValue READ getOutsideValue WRITE setOutsideValue)

    /**
    * @brief Setter property for MaskCellArrayPath
    */
    void setMaskCellArrayPath(const DataArrayPath& value); 
    /**
    * @brief Getter property for MaskCellArrayPath
    * @return Value of MaskCellArrayPath
    */
    DataArrayPath getMaskCellArrayPath() const;

  Q_PROPERTY(DataArrayPath MaskCellArrayPath READ getMaskCellArrayPath WRITE setMaskCellArrayPath)

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
    * @brief Setter property for MaskContainerArray
    */
    void setMaskContainerArray(const DataContainerArray::Pointer& value); 
    /**
    * @brief Getter property for MaskContainerArray
    * @return Value of MaskContainerArray
    */
    DataContainerArray::Pointer getMaskContainerArray() const;


  ITKMaskImage();

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

  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> typename std::enable_if<std::is_scalar<InputPixelType>::value>::type convertDataContainerType();

  template <typename InputPixelType, typename OutputPixelType, unsigned int Dimension> typename std::enable_if<!std::is_scalar<InputPixelType>::value>::type convertDataContainerType();

public:
  ITKMaskImage(const ITKMaskImage&) = delete;            // Copy Constructor Not Implemented
  ITKMaskImage(ITKMaskImage&&) = delete;                 // Move Constructor Not Implemented
  ITKMaskImage& operator=(const ITKMaskImage&) = delete; // Copy Assignment Not Implemented
  ITKMaskImage& operator=(ITKMaskImage&&) = delete;      // Move Assignment Not Implemented

  private:
    double m_OutsideValue = {};
    DataArrayPath m_MaskCellArrayPath = {};
    DataContainerArray::Pointer m_MaskContainerArray = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

