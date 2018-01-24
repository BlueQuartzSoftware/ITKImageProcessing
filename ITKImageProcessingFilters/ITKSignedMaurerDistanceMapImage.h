// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKSignedMaurerDistanceMapImage_h_
#define _ITKSignedMaurerDistanceMapImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <itkSignedMaurerDistanceMapImageFilter.h>

/**
 * @brief The ITKSignedMaurerDistanceMapImage class. See [Filter documentation](@ref ITKSignedMaurerDistanceMapImage) for details.
 */
class ITKSignedMaurerDistanceMapImage : public ITKImageBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKSignedMaurerDistanceMapImage)
  SIMPL_STATIC_NEW_MACRO(ITKSignedMaurerDistanceMapImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKSignedMaurerDistanceMapImage, AbstractFilter)

  virtual ~ITKSignedMaurerDistanceMapImage();

  SIMPL_FILTER_PARAMETER(bool, InsideIsPositive)
  Q_PROPERTY(bool InsideIsPositive READ getInsideIsPositive WRITE setInsideIsPositive)

  SIMPL_FILTER_PARAMETER(bool, SquaredDistance)
  Q_PROPERTY(bool SquaredDistance READ getSquaredDistance WRITE setSquaredDistance)

  SIMPL_FILTER_PARAMETER(bool, UseImageSpacing)
  Q_PROPERTY(bool UseImageSpacing READ getUseImageSpacing WRITE setUseImageSpacing)

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
  ITKSignedMaurerDistanceMapImage();

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

private:
  ITKSignedMaurerDistanceMapImage(const ITKSignedMaurerDistanceMapImage&) = delete; // Copy Constructor Not Implemented
  void operator=(const ITKSignedMaurerDistanceMapImage&);                  // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKSignedMaurerDistanceMapImage_H_ */
