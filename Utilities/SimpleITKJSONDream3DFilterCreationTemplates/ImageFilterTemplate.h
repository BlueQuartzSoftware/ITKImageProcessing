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
$
{
  IncludeName
}

/**
 * @brief The ${FilterName} class. See [Filter documentation](@ref ${FilterName}) for details.
 */
class $
{
  FilterName
} : public ITKImageProcessingBase
{
  Q_OBJECT

public:
  using Self = ${FilterName};
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  static std::shared_ptr<${FilterName}> New();

  /**
   * @brief Returns the name of the class for ${FilterName}
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ${FilterName}
   */
  static QString ClassName();

  ~${FilterName}() override;

  ${Parameters} /**
                 * @brief newFilterInstance Reimplemented from @see AbstractFilter class
                 */
  AbstractFilter::Pointer
  newFilterInstance(bool copyFilterParameters) const override;

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
  void readFilterParameters(AbstractFilterParametersReader * reader, int index) override;

protected:
  ${FilterName}();

  /**
   * @brief dataCheckInternal overloads dataCheckInternal in ITKImageBase and calls templated dataCheck
   */
  void dataCheckInternal() override;

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension>
  void dataCheck();

  /**
   * @brief filterInternal overloads filterInternal in ITKImageBase and calls templated filter
   */
  void filterInternal() override;

  /**
   * @brief Applies the filter
   */
  template <typename InputImageType, typename OutputImageType, unsigned int Dimension>
  void filter();

private:
  ${FilterName}(const ${FilterName}&) = delete;  // Copy Constructor Not Implemented
  void operator=(const ${FilterName}&) = delete; // Move assignment Not Implemented
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
