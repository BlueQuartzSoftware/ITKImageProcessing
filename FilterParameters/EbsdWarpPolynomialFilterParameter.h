/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include <memory>

#include <QtCore/QJsonObject>

#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/FilterParameters/FilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

/**
 * @brief SIMPL_NEW_EBSDWARPPOLYNOMIALFP This macro is a short-form way of instantiating an instance of
 * FileListInfoFilterParameter. There are 4 required parameters that are always passed to this macro
 * in the following order: HumanLabel, PropertyName, Category, FilterName (class name).
 *
 * Therefore, the macro should be written like this (this is a concrete example):
 * SIMPL_NEW_EBSDWARPPOLYNOMIALFP("HumanLabel", PropertyName, Category, FilterName)
 *
 * Example 1 (instantiated within a filter called [GenericExample](@ref genericexample)):
 * SIMPL_NEW_EBSDWARPPOLYNOMIALFP("Input File List", InputFileListInfo, FilterParameter::Category::Parameter, GenericExample);
 */
#define SIMPL_NEW_EBSDWARPPOLYNOMIAL_FP(...)                                                                                                                                                           \
  SIMPL_EXPAND(_FP_GET_OVERRIDE(__VA_ARGS__, SIMPL_NEW_FP_9, SIMPL_NEW_FP_8, SIMPL_NEW_FP_7, SIMPL_NEW_FP_6, SIMPL_NEW_FP_5, SIMPL_NEW_FP_4)(EbsdWarpPolynomialFilterParameter, __VA_ARGS__))

class ITKImageProcessing_EXPORT EbsdWarpPolynomialFilterParameter : public FilterParameter
{
public:
  using Self = EbsdWarpPolynomialFilterParameter;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<Self>;
  static Pointer NullPointer();

  static Pointer New();

  /**
   * @brief Returns the name of the class for EbsdWarpPolynomialFilterParameter
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for EbsdWarpPolynomialFilterParameter
   */
  static QString ClassName();

  using SetterCallbackType = std::function<void(FloatVec7Type)>;
  using GetterCallbackType = std::function<FloatVec7Type(void)>;

  /**
   * @brief New This function instantiates an instance of the FileListInfoFilterParameter. Although this
   * function is available to be used, the preferable way to instantiate an instance of this class is to use the
   * SIMPL_NEW_FILELISTINFO_FP(...) macro at the top of this file.

   * @param humanLabel The name that the users of DREAM.3D see for this filter parameter
   * @param propertyName The internal property name for this filter parameter.
   * @param defaultValue The value that this filter parameter will be initialized to by default.
   * @param category The category for the filter parameter in the DREAM.3D user interface.  There
   * are three categories: Parameter, Required Arrays, and Created Arrays.
   * @param setterCallback The method in the AbstractFilter subclass that <i>sets</i> the value of the property
  * that this FilterParameter subclass represents.
   * @param getterCallback The method in the AbstractFilter subclass that <i>gets</i> the value of the property
  * that this FilterParameter subclass represents.
   * @return
   */
  static Pointer Create(const QString& humanLabel, const QString& propertyName, const FloatVec7Type& defaultValue, Category category, const SetterCallbackType& setterCallback,
                        const GetterCallbackType& getterCallback, int groupIndex = -1);

  ~EbsdWarpPolynomialFilterParameter() override;

  QString getWidgetType() const override;

  /**
   * @brief readJson
   * @return
   */
  void readJson(const QJsonObject& json) override;

  /**
   * @brief writeJson
   * @return
   */
  void writeJson(QJsonObject& json) const override;

  /**
   * @param SetterCallback The method in the AbstractFilter subclass that <i>sets</i> the value of the property
   * that this FilterParameter subclass represents.
   * from the filter parameter.
   */
  /**
   * @brief Setter property for SetterCallback
   */
  void setSetterCallback(const EbsdWarpPolynomialFilterParameter::SetterCallbackType& value);
  /**
   * @brief Getter property for SetterCallback
   * @return Value of SetterCallback
   */
  EbsdWarpPolynomialFilterParameter::SetterCallbackType getSetterCallback() const;

  /**
   * @param GetterCallback The method in the AbstractFilter subclass that <i>gets</i> the value of the property
   * that this FilterParameter subclass represents.
   * @return The GetterCallback
   */
  /**
   * @brief Setter property for GetterCallback
   */
  void setGetterCallback(const EbsdWarpPolynomialFilterParameter::GetterCallbackType& value);
  /**
   * @brief Getter property for GetterCallback
   * @return Value of GetterCallback
   */
  EbsdWarpPolynomialFilterParameter::GetterCallbackType getGetterCallback() const;

protected:
  EbsdWarpPolynomialFilterParameter();

private:
  EbsdWarpPolynomialFilterParameter::SetterCallbackType m_SetterCallback = {};
  EbsdWarpPolynomialFilterParameter::GetterCallbackType m_GetterCallback = {};

public:
  EbsdWarpPolynomialFilterParameter(const EbsdWarpPolynomialFilterParameter&) = delete;            // Copy Constructor Not Implemented
  EbsdWarpPolynomialFilterParameter(EbsdWarpPolynomialFilterParameter&&) = delete;                 // Move Constructor Not Implemented
  EbsdWarpPolynomialFilterParameter& operator=(const EbsdWarpPolynomialFilterParameter&) = delete; // Copy Assignment Not Implemented
  EbsdWarpPolynomialFilterParameter& operator=(EbsdWarpPolynomialFilterParameter&&) = delete;      // Move Assignment Not Implemented
};

Q_DECLARE_METATYPE(FloatVec7Type)
