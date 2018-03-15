// File automatically generated

/*
 * Your License or Copyright can go here
 */

#ifndef _ITKVectorConnectedComponentImage_h_
#define _ITKVectorConnectedComponentImage_h_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif

#include "ITKImageProcessingBase.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/SIMPLib.h"

// Auto includes
#include <SIMPLib/FilterParameters/BooleanFilterParameter.h>
#include <SIMPLib/FilterParameters/DoubleFilterParameter.h>
#include <itkVectorConnectedComponentImageFilter.h>


/**
 * @brief The ITKVectorConnectedComponentImage class. See [Filter documentation](@ref ITKVectorConnectedComponentImage) for details.
 */
class ITKVectorConnectedComponentImage : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ITKVectorConnectedComponentImage)
  SIMPL_STATIC_NEW_MACRO(ITKVectorConnectedComponentImage)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ITKVectorConnectedComponentImage, AbstractFilter)

  virtual ~ITKVectorConnectedComponentImage();

  SIMPL_FILTER_PARAMETER(double, DistanceThreshold)
  Q_PROPERTY(double DistanceThreshold READ getDistanceThreshold WRITE setDistanceThreshold)

  SIMPL_FILTER_PARAMETER(bool, FullyConnected)
  Q_PROPERTY(bool FullyConnected READ getFullyConnected WRITE setFullyConnected)


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
  ITKVectorConnectedComponentImage();

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
  ITKVectorConnectedComponentImage(const ITKVectorConnectedComponentImage&) = delete;    // Copy Constructor Not Implemented
  void operator=(const ITKVectorConnectedComponentImage&) = delete; // Operator '=' Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _ITKVectorConnectedComponentImage_H_ */
