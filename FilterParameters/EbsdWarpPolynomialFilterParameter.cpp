/* ============================================================================
 * Copyright (c) 2009-2016 BlueQuartz Software, LLC
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
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
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
 * The code contained herein was partially funded by the following contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "EbsdWarpPolynomialFilterParameter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdWarpPolynomialFilterParameter::EbsdWarpPolynomialFilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdWarpPolynomialFilterParameter::~EbsdWarpPolynomialFilterParameter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdWarpPolynomialFilterParameter::Pointer EbsdWarpPolynomialFilterParameter::Create(const QString& humanLabel, const QString& propertyName, const FloatVec7Type& defaultValue, Category category,
                                                                                     const SetterCallbackType& setterCallback, const GetterCallbackType& getterCallback, int groupIndex)
{

  EbsdWarpPolynomialFilterParameter::Pointer ptr = EbsdWarpPolynomialFilterParameter::New();
  ptr->setHumanLabel(humanLabel);
  ptr->setPropertyName(propertyName);
  QVariant v;
  v.setValue(defaultValue);
  ptr->setDefaultValue(v);
  ptr->setCategory(category);
  ptr->setGroupIndex(groupIndex);
  ptr->setSetterCallback(setterCallback);
  ptr->setGetterCallback(getterCallback);

  return ptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString EbsdWarpPolynomialFilterParameter::getWidgetType() const
{
  return QString("EbsdWarpPolynomialWidget");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdWarpPolynomialFilterParameter::readJson(const QJsonObject& json)
{
  QJsonValue jsonValue = json[getPropertyName()];
  if(jsonValue.isUndefined())
  {
    jsonValue = json[getLegacyPropertyName()];
  }
  if(!jsonValue.isUndefined() && m_SetterCallback)
  {
    QJsonObject json = jsonValue.toObject();
    FloatVec7Type floatVec7;
    if(json["a0"].isDouble())
    {
      floatVec7[0] = static_cast<float>(json["a0"].toDouble());
    }

    if(json["a1"].isDouble())
    {
      floatVec7[1] = static_cast<float>(json["a1"].toDouble());
    }

    if(json["a2"].isDouble())
    {
      floatVec7[2] = static_cast<float>(json["a2"].toDouble());
    }

    if(json["a3"].isDouble())
    {
      floatVec7[3] = static_cast<float>(json["a3"].toDouble());
    }

    if(json["a4"].isDouble())
    {
      floatVec7[4] = static_cast<float>(json["a4"].toDouble());
    }

    if(json["a5"].isDouble())
    {
      floatVec7[5] = static_cast<float>(json["a5"].toDouble());
    }

    if(json["a6"].isDouble())
    {
      floatVec7[6] = static_cast<float>(json["a6"].toDouble());
    }
    m_SetterCallback(floatVec7);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdWarpPolynomialFilterParameter::writeJson(QJsonObject& obj) const
{
  if(m_GetterCallback)
  {
    FloatVec7Type floatVec7 = m_GetterCallback();
    QJsonObject json;
    json["a0"] = static_cast<double>(floatVec7[0]);
    json["a1"] = static_cast<double>(floatVec7[1]);
    json["a2"] = static_cast<double>(floatVec7[2]);
    json["a3"] = static_cast<double>(floatVec7[3]);
    json["a4"] = static_cast<double>(floatVec7[4]);
    json["a5"] = static_cast<double>(floatVec7[5]);
    json["a6"] = static_cast<double>(floatVec7[6]);

    obj[getPropertyName()] = json;
  }
}

// -----------------------------------------------------------------------------
EbsdWarpPolynomialFilterParameter::Pointer EbsdWarpPolynomialFilterParameter::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
EbsdWarpPolynomialFilterParameter::Pointer EbsdWarpPolynomialFilterParameter::New()
{
  Pointer sharedPtr(new(EbsdWarpPolynomialFilterParameter));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
QString EbsdWarpPolynomialFilterParameter::getNameOfClass() const
{
  return QString("EbsdWarpPolynomialFilterParameter");
}

// -----------------------------------------------------------------------------
QString EbsdWarpPolynomialFilterParameter::ClassName()
{
  return QString("EbsdWarpPolynomialFilterParameter");
}

// -----------------------------------------------------------------------------
void EbsdWarpPolynomialFilterParameter::setSetterCallback(const EbsdWarpPolynomialFilterParameter::SetterCallbackType& value)
{
  m_SetterCallback = value;
}

// -----------------------------------------------------------------------------
EbsdWarpPolynomialFilterParameter::SetterCallbackType EbsdWarpPolynomialFilterParameter::getSetterCallback() const
{
  return m_SetterCallback;
}

// -----------------------------------------------------------------------------
void EbsdWarpPolynomialFilterParameter::setGetterCallback(const EbsdWarpPolynomialFilterParameter::GetterCallbackType& value)
{
  m_GetterCallback = value;
}

// -----------------------------------------------------------------------------
EbsdWarpPolynomialFilterParameter::GetterCallbackType EbsdWarpPolynomialFilterParameter::getGetterCallback() const
{
  return m_GetterCallback;
}
