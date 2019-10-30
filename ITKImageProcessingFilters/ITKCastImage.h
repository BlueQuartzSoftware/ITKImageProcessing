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

#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkCastImageFilter.h>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief The ITKCastImage class. See [Filter documentation](@ref ITKCastImage) for details.
 */
class ITKImageProcessing_EXPORT ITKCastImage : public ITKImageProcessingBase
{
  Q_OBJECT

#ifdef SIMPL_ENABLE_PYTHON
  PYB11_CREATE_BINDINGS(ITKCastImage SUPERCLASS ITKImageProcessingBase)
  PYB11_SHARED_POINTERS(ITKCastImage)
  PYB11_FILTER_NEW_MACRO(ITKCastImage)
  PYB11_FILTER_PARAMETER(int, CastingType)
  PYB11_PROPERTY(int CastingType READ getCastingType WRITE setCastingType)
#endif

public:
    using Self = ITKCastImage;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    static std::shared_ptr<ITKCastImage> New();

    /**
    * @brief Returns the name of the class for ITKCastImage
    */
    QString getNameOfClass() const override;
    /**
    * @brief Returns the name of the class for ITKCastImage
    */
    static QString ClassName();


    /**
    * @brief Setter property for CastingType
    */
    void setCastingType(int value); 
    /**
    * @brief Getter property for CastingType
    * @return Value of CastingType
    */
    int getCastingType() const;

  Q_PROPERTY(int CastingType READ getCastingType WRITE setCastingType)

  ~ITKCastImage() override;

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
  ITKCastImage();

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
  ITKCastImage(const ITKCastImage&) = delete;            // Copy Constructor Not Implemented
  ITKCastImage(ITKCastImage&&) = delete;                 // Move Constructor Not Implemented
  ITKCastImage& operator=(const ITKCastImage&) = delete; // Copy Assignment Not Implemented
  ITKCastImage& operator=(ITKCastImage&&) = delete;      // Move Assignment Not Implemented

  private:
    int m_CastingType = {};

};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

