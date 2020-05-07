/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "ZeissMetaEntry.h"

#ifdef ZEISS_HDF_SUPPORT
#include "H5Support/H5Lite.h"
#include "H5Support/H5Utilities.h"
#include "H5Support/QH5Lite.h"
#endif

#include <QtCore/QDebug>

#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataArrays/StringDataArray.h"

#include "ITKImageProcessing/ZeissXml/ZeissTagMapping.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
#define WriteHDF5Attribute_Implementation(thisClass)                                                                                                                                                   \
  int thisClass::writeHDF5Attribute(hid_t fileId, const QString& datasetPath)                                                                                                                          \
  {                                                                                                                                                                                                    \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();                                                                                                                                     \
    QString tagName = tagMap->nameForId(this->getZeissIdTag());                                                                                                                                        \
    if(tagName.isEmpty() == true)                                                                                                                                                                      \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::writeHDF5Attribute:  Error writing attribute. No zeiss id was found "                                                                                         \
               << " for id=" << getZeissIdTag();                                                                                                                                                       \
      return -19;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    int32_t err = QH5Lite::writeScalarAttribute(fileId, datasetPath, tagName, m_Value);                                                                                                                \
    if(err < 0)                                                                                                                                                                                        \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::writeHDF5Attribute: Error writing attribute " << tagName << " with value " << m_Value << " to data set " << datasetPath;                                      \
      return err;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    return err;                                                                                                                                                                                        \
  }

#define ReadHDF5Attribute_DEFINITION(type, thisClass)                                                                                                                                                  \
  int thisClass::readHDF5Attribute(hid_t fileId, const QString& datasetPath)                                                                                                                           \
  {                                                                                                                                                                                                    \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();                                                                                                                                     \
    QString tagName = tagMap->nameForId(getZeissIdTag());                                                                                                                                              \
    if(tagName.isEmpty() == true)                                                                                                                                                                      \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::readHDF5Attribute: Error reading attribute. No zeiss id was found "                                                                                           \
               << " for id=" << getZeissIdTag();                                                                                                                                                       \
      return -19;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    int32_t err = QH5Lite::readScalarAttribute<type>(fileId, datasetPath, tagName, m_Value);                                                                                                           \
    if(err < 0)                                                                                                                                                                                        \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::readHDF5Attribute: Error reading attribute " << tagName << " from data set " << datasetPath;                                                                  \
      return err;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    return err;                                                                                                                                                                                        \
  }

#define WriteHDF5Attribute_StrImplementation(thisClass)                                                                                                                                                \
  int thisClass::writeHDF5Attribute(hid_t fileId, const QString& datasetPath)                                                                                                                          \
  {                                                                                                                                                                                                    \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();                                                                                                                                     \
    QString tagName = tagMap->nameForId(getZeissIdTag());                                                                                                                                              \
    if(tagName.isEmpty() == true)                                                                                                                                                                      \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::writeHDF5Attribute: Error writing attribute. No zeiss id was found "                                                                                          \
               << " for id=" << getZeissIdTag();                                                                                                                                                       \
      return -19;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    int32_t err = QH5Lite::writeStringAttribute(fileId, datasetPath, tagName, m_Value);                                                                                                                \
    if(err < 0)                                                                                                                                                                                        \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::writeHDF5Attribute:Error adding attribute " << tagName << " with value " << m_Value << " to data set " << datasetPath;                                        \
      return err;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    return err;                                                                                                                                                                                        \
  }

#define ReadHDF5Attribute_STR_DEFINITION(thisClass)                                                                                                                                                    \
  int thisClass::readHDF5Attribute(hid_t fileId, const QString& datasetPath)                                                                                                                           \
  {                                                                                                                                                                                                    \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();                                                                                                                                     \
    QString tagName = tagMap->nameForId(getZeissIdTag());                                                                                                                                              \
    if(tagName.isEmpty() == true)                                                                                                                                                                      \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::readHDF5Attribute: Error reading attribute. No zeiss id was found "                                                                                           \
               << " for id=" << getZeissIdTag();                                                                                                                                                       \
      return -19;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    int32_t err = QH5Lite::readStringAttribute(fileId, datasetPath, tagName, m_Value);                                                                                                                 \
    if(err < 0)                                                                                                                                                                                        \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::readHDF5Attribute: Error reading attribute " << tagName << " from data set " << datasetPath;                                                                  \
      return err;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    return err;                                                                                                                                                                                        \
  }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
#define AXIOVISION_NUMERIC_DEFINITION(ClassName, Type, ConvertMethod)                                                                                                                                  \
  ClassName::ClassName() = default;                                                                                                                                                                    \
  ClassName::ClassName(int32_t idTag)                                                                                                                                                                  \
  {                                                                                                                                                                                                    \
    setZeissIdTag(idTag);                                                                                                                                                                              \
  }                                                                                                                                                                                                    \
  ClassName::~ClassName() = default;                                                                                                                                                                   \
  bool ClassName::setValue(const QString& value)                                                                                                                                                       \
  {                                                                                                                                                                                                    \
    bool ok = false;                                                                                                                                                                                   \
    m_Value = value.ConvertMethod(&ok);                                                                                                                                                                \
    return ok;                                                                                                                                                                                         \
  }                                                                                                                                                                                                    \
                                                                                                                                                                                                       \
  bool ClassName::setValue(Type value)                                                                                                                                                                 \
  {                                                                                                                                                                                                    \
    m_Value = value;                                                                                                                                                                                   \
    return true;                                                                                                                                                                                       \
  }                                                                                                                                                                                                    \
  Type ClassName::getValue() const                                                                                                                                                                     \
  {                                                                                                                                                                                                    \
    return m_Value;                                                                                                                                                                                    \
  }                                                                                                                                                                                                    \
  IDataArray::Pointer ClassName::createDataArray(bool allocate) const                                                                                                                                  \
  {                                                                                                                                                                                                    \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();                                                                                                                                     \
    QString tagName = tagMap->nameForId(this->getZeissIdTag());                                                                                                                                        \
    if(tagName.isEmpty() == true)                                                                                                                                                                      \
    {                                                                                                                                                                                                  \
      qDebug() << getNameOfClass() << "::createDataArray: Error retrieving name of AxioVisio tag.. No zeiss id was found "                                                                             \
               << " for id=" << getZeissIdTag();                                                                                                                                                       \
      return DataArray<Type>::NullPointer();                                                                                                                                                           \
    }                                                                                                                                                                                                  \
    DataArray<Type>::Pointer array = DataArray<Type>::CreateArray(1, tagName, allocate);                                                                                                               \
    if(allocate)                                                                                                                                                                                       \
    {                                                                                                                                                                                                  \
      array->setValue(0, m_Value);                                                                                                                                                                     \
    }                                                                                                                                                                                                  \
    return array;                                                                                                                                                                                      \
  }                                                                                                                                                                                                    \
  void ClassName::printValue(std::ostream& out) const                                                                                                                                                  \
  {                                                                                                                                                                                                    \
    out << m_Value;                                                                                                                                                                                    \
  }                                                                                                                                                                                                    \
  QString ClassName::toString() const                                                                                                                                                                  \
  {                                                                                                                                                                                                    \
    QString s = QString::number(m_Value);                                                                                                                                                              \
    return s;                                                                                                                                                                                          \
  }

// -----------------------------------------------------------------------------
// Int32ZeissMetaEntry Implementations
// -----------------------------------------------------------------------------
AXIOVISION_NUMERIC_DEFINITION(Int32ZeissMetaEntry, int32_t, toInt)
#ifdef ZEISS_HDF_SUPPORT
WriteHDF5Attribute_Implementation(Int32ZeissMetaEntry) ReadHDF5Attribute_DEFINITION(int32_t, Int32ZeissMetaEntry)
#endif

    // -----------------------------------------------------------------------------
    // Int64ZeissMetaEntry Implementations
    // -----------------------------------------------------------------------------
    AXIOVISION_NUMERIC_DEFINITION(Int64ZeissMetaEntry, int64_t, toLongLong)
#ifdef ZEISS_HDF_SUPPORT
        WriteHDF5Attribute_Implementation(Int64ZeissMetaEntry) ReadHDF5Attribute_DEFINITION(int64_t, Int64ZeissMetaEntry)
#endif

    // -----------------------------------------------------------------------------
    // FloatZeissMetaEntry Implementations
    // -----------------------------------------------------------------------------
    AXIOVISION_NUMERIC_DEFINITION(FloatZeissMetaEntry, float, toFloat)
#ifdef ZEISS_HDF_SUPPORT
        WriteHDF5Attribute_Implementation(FloatZeissMetaEntry) ReadHDF5Attribute_DEFINITION(float, FloatZeissMetaEntry)
#endif

    // -----------------------------------------------------------------------------
    // StringZeissMetaEntry Implementations
    // -----------------------------------------------------------------------------
    StringZeissMetaEntry::~StringZeissMetaEntry() = default;

IDataArray::Pointer StringZeissMetaEntry::createDataArray(bool allocate) const
{
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();
  QString tagName = tagMap->nameForId(this->getZeissIdTag());
  if(tagName.isEmpty())
  {
    qDebug() << getNameOfClass() << "::createDataArray: Error creating StringArray. No Zeiss id was found "
             << " for id=" << getZeissIdTag();
    return StringDataArray::NullPointer();
  }
  StringDataArray::Pointer array = StringDataArray::CreateArray(1, tagName, allocate);
  if(allocate)
  {
    array->setValue(0, m_Value);
  }
  return array;
}
bool StringZeissMetaEntry::setValue(const QString& value)
{
  m_Value = value;
  return true;
}

QString StringZeissMetaEntry::getValue()
{
  return m_Value;
}

void StringZeissMetaEntry::printValue(std::ostream& out) const
{
  out << m_Value.toStdString();
}

QString StringZeissMetaEntry::toString() const
{
  return m_Value;
}

#ifdef ZEISS_HDF_SUPPORT

int StringZeissMetaEntry::writeHDF5Attribute(hid_t fileId, const QString& datasetPath)
{
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();
  QString tagName = tagMap->nameForId(getZeissIdTag());
  if(tagName.isEmpty())
  {
    qDebug() << getNameOfClass() << "::writeHDF5Attribute: Error writing attribute. No zeiss id was found "
             << " for id=" << getZeissIdTag();
    return -19;
  }
  int32_t err = QH5Lite::writeStringAttribute(fileId, datasetPath, tagName, m_Value);
  if(err < 0)
  {
    qDebug() << getNameOfClass() << "::writeHDF5Attribute: Error adding attribute " << tagName << " with value " << m_Value << " to data set " << datasetPath;
    return err;
  }
  return err;
}

ReadHDF5Attribute_STR_DEFINITION(StringZeissMetaEntry)
#endif

    AbstractZeissMetaData::AbstractZeissMetaData() = default;

// -----------------------------------------------------------------------------
AbstractZeissMetaData::Pointer AbstractZeissMetaData::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
QString AbstractZeissMetaData::getNameOfClass() const
{
  return QString("AbstractZeissMetaData");
}

// -----------------------------------------------------------------------------
QString AbstractZeissMetaData::ClassName()
{
  return QString("AbstractZeissMetaData");
}

// -----------------------------------------------------------------------------
void AbstractZeissMetaData::setZeissIdTag(int32_t value)
{
  m_ZeissIdTag = value;
}

// -----------------------------------------------------------------------------
int32_t AbstractZeissMetaData::getZeissIdTag() const
{
  return m_ZeissIdTag;
}

// -----------------------------------------------------------------------------
Int32ZeissMetaEntry::Pointer Int32ZeissMetaEntry::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
QString Int32ZeissMetaEntry::getNameOfClass() const
{
  return QString("Int32ZeissMetaEntry");
}

// -----------------------------------------------------------------------------
QString Int32ZeissMetaEntry::ClassName()
{
  return QString("Int32ZeissMetaEntry");
}

// -----------------------------------------------------------------------------
Int32ZeissMetaEntry::Pointer Int32ZeissMetaEntry::New()
{
  Pointer sharedPtr(new(Int32ZeissMetaEntry));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
Int64ZeissMetaEntry::Pointer Int64ZeissMetaEntry::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
QString Int64ZeissMetaEntry::getNameOfClass() const
{
  return QString("Int64ZeissMetaEntry");
}

// -----------------------------------------------------------------------------
QString Int64ZeissMetaEntry::ClassName()
{
  return QString("Int64ZeissMetaEntry");
}

// -----------------------------------------------------------------------------
Int64ZeissMetaEntry::Pointer Int64ZeissMetaEntry::New()
{
  Pointer sharedPtr(new(Int64ZeissMetaEntry));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
FloatZeissMetaEntry::Pointer FloatZeissMetaEntry::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
QString FloatZeissMetaEntry::getNameOfClass() const
{
  return QString("FloatZeissMetaEntry");
}

// -----------------------------------------------------------------------------
QString FloatZeissMetaEntry::ClassName()
{
  return QString("FloatZeissMetaEntry");
}

// -----------------------------------------------------------------------------
FloatZeissMetaEntry::Pointer FloatZeissMetaEntry::New()
{
  Pointer sharedPtr(new(FloatZeissMetaEntry));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
StringZeissMetaEntry::Pointer StringZeissMetaEntry::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
QString StringZeissMetaEntry::getNameOfClass() const
{
  return QString("StringZeissMetaEntry");
}

// -----------------------------------------------------------------------------
QString StringZeissMetaEntry::ClassName()
{
  return QString("StringZeissMetaEntry");
}

// -----------------------------------------------------------------------------
StringZeissMetaEntry::Pointer StringZeissMetaEntry::New()
{
  Pointer sharedPtr(new(StringZeissMetaEntry));
  return sharedPtr;
}
