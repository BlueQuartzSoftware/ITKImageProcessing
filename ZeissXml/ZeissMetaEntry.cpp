///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           F33615-03-C-5220
//
///////////////////////////////////////////////////////////////////////////////

#include "ZeissMetaEntry.h"

#ifdef ZEISS_HDF_SUPPORT
#include "H5Support/QH5Lite.h"
#include "H5Support/H5Utilities.h"
#endif

#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/DataArrays/StringDataArray.h"

#include "ZeissImport/ZeissXml/ZeissTagMapping.h"



#define ZME_CHECK_ATTRIBUTE_ADDITION(ds, attr, value) \
  if ( err < 0 ) {\
    qDebug() << "Error adding attribute " << attr << " with value " << value \
             <<   " to data set " << ds; retErr = err;}

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
      std::cout << "writeHDF5Attribute: Error writing attribute. No zeiss id was found "                                                                                                               \
                << " for id=" << getZeissIdTag();                                                                                                                                                      \
      return -19;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    int32_t err = QH5Lite::writeScalarAttribute(fileId, datasetPath, tagName, m_Value);                                                                                                                \
    if(err < 0)                                                                                                                                                                                        \
    {                                                                                                                                                                                                  \
      qDebug() << "Error writing attribute " << tagName << " with value " << m_Value << " to data set " << datasetPath;                                                                                \
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
      std::cout << "readHDF5Attribute: Error reading attribute. No zeiss id was found "                                                                                                                \
                << " for id=" << getZeissIdTag();                                                                                                                                                      \
      return -19;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    int32_t err = QH5Lite::readScalarAttribute<type>(fileId, datasetPath, tagName, m_Value);                                                                                                           \
    if(err < 0)                                                                                                                                                                                        \
    {                                                                                                                                                                                                  \
      qDebug() << "Error reading attribute " << tagName << " from data set " << datasetPath;                                                                                                           \
      return err;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    return err;                                                                                                                                                                                        \
  }

#if 1
#define WriteHDF5Attribute_StrImplementation(thisClass)                                                                                                                                                \
  int thisClass::writeHDF5Attribute(hid_t fileId, const QString& datasetPath)                                                                                                                          \
  {                                                                                                                                                                                                    \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();                                                                                                                                     \
    QString tagName = tagMap->nameForId(getZeissIdTag());                                                                                                                                              \
    if(tagName.isEmpty() == true)                                                                                                                                                                      \
    {                                                                                                                                                                                                  \
      std::cout << "writeHDF5Attribute: Error writing attribute. No zeiss id was found "                                                                                                               \
                << " for id=" << getZeissIdTag();                                                                                                                                                      \
      return -19;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    int32_t err = QH5Lite::writeStringAttribute(fileId, datasetPath, tagName, m_Value);                                                                                                                \
    if(err < 0)                                                                                                                                                                                        \
    {                                                                                                                                                                                                  \
      qDebug() << "Error adding attribute " << tagName << " with value " << m_Value << " to data set " << datasetPath;                                                                                 \
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
      std::cout << "readHDF5Attribute: Error reading attribute. No zeiss id was found "                                                                                                                \
                << " for id=" << getZeissIdTag();                                                                                                                                                      \
      return -19;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    int32_t err = QH5Lite::readStringAttribute(fileId, datasetPath, tagName, m_Value);                                                                                                                 \
    if(err < 0)                                                                                                                                                                                        \
    {                                                                                                                                                                                                  \
      qDebug() << "Error reading attribute " << tagName << " from data set " << datasetPath;                                                                                                           \
      return err;                                                                                                                                                                                      \
    }                                                                                                                                                                                                  \
    return err;                                                                                                                                                                                        \
  }
#else

#define ReadHDF5Attribute_STR_DEFINITION(thisClass)\
  int thisClass::readHDF5Attribute(hid_t fileId, const QString &datasetPath) { \
    std::cout << "NOT IMPLEMENTED"; return -1;}

#define WriteHDF5Attribute_StrImplementation(thisClass)\
  int thisClass::writeHDF5Attribute(hid_t fileId, const QString &datasetPath)\
  { std::cout << "NOT IMPLEMENTED"; return -1;}

#endif

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
  Type ClassName::getValue()                                                                                                                                                                           \
  {                                                                                                                                                                                                    \
    return m_Value;                                                                                                                                                                                    \
  }                                                                                                                                                                                                    \
  IDataArray::Pointer ClassName::createDataArray(bool allocate)                                                                                                                                        \
  {                                                                                                                                                                                                    \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();                                                                                                                                     \
    QString tagName = tagMap->nameForId(this->getZeissIdTag());                                                                                                                                        \
    if(tagName.isEmpty() == true)                                                                                                                                                                      \
    {                                                                                                                                                                                                  \
      std::cout << "createDataArray: Error retrieving name of AxioVisio tag.. No zeiss id was found "                                                                                                  \
                << " for id=" << getZeissIdTag();                                                                                                                                                      \
      return DataArray<Type>::NullPointer();                                                                                                                                                           \
    }                                                                                                                                                                                                  \
    DataArray<Type>::Pointer array = DataArray<Type>::CreateArray(1, tagName, allocate);                                                                                                               \
    if(allocate)                                                                                                                                                                                       \
    {                                                                                                                                                                                                  \
      array->setValue(0, m_Value);                                                                                                                                                                     \
    }                                                                                                                                                                                                  \
    return array;                                                                                                                                                                                      \
  }                                                                                                                                                                                                    \
  void ClassName::printValue(std::ostream& out)                                                                                                                                                        \
  {                                                                                                                                                                                                    \
    out << m_Value;                                                                                                                                                                                    \
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
        WriteHDF5Attribute_Implementation(Int64ZeissMetaEntry) ReadHDF5Attribute_DEFINITION(qint64, Int64ZeissMetaEntry)
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

IDataArray::Pointer StringZeissMetaEntry::createDataArray(bool allocate)
{
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();
  QString tagName = tagMap->nameForId(this->getZeissIdTag());
  if(tagName.isEmpty())
  {
    std::cout << "StringZeissMetaEntry::createDataArray: Error creating StringArray. No Zeiss id was found "
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

void StringZeissMetaEntry::printValue(std::ostream& out)
{
  out << m_Value.toStdString();
}

#ifdef ZEISS_HDF_SUPPORT

int StringZeissMetaEntry::writeHDF5Attribute(hid_t fileId, const QString& datasetPath)
{
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();
  QString tagName = tagMap->nameForId(getZeissIdTag());
  if(tagName.isEmpty())
  {
    std::cout << "writeHDF5Attribute: Error writing attribute. No zeiss id was found "
              << " for id=" << getZeissIdTag();
    return -19;
  }
  int32_t err = QH5Lite::writeStringAttribute(fileId, datasetPath, tagName, m_Value);
  if(err < 0)
  {
    qDebug() << "Error adding attribute " << tagName << " with value " << m_Value << " to data set " << datasetPath;
    return err;
  }
  return err;
}

ReadHDF5Attribute_STR_DEFINITION(StringZeissMetaEntry)
#endif

