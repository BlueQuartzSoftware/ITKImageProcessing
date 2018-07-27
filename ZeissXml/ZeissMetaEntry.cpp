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
#define VALUE_DEFINITION(className, _value, ArrayType)\
  bool className::setValue(const QString &value)\
  {\
    bool ok = false;\
    _value = value.toInt(&ok, 10);\
    return ok;\
  }\
  \
  bool className::setValue(int32_t value)\
  {\
    _value = value;\
    return true;\
  }\
  int32_t className::getValue() { \
    return _value; }\
  IDataArray::Pointer className::createDataArray(bool allocate) {\
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();\
    QString tagName = tagMap->nameForId(this->getZeissIdTag());\
    if (tagName.isEmpty() == true) { std::cout << "writeHDF5Attribute: Error writing attribute. No zeiss id was found "\
                                                 << " for id=" << getZeissIdTag(); return ArrayType::NullPointer(); } \
    ArrayType::Pointer array = ArrayType::CreateArray(1, tagName, allocate);\
    if(allocate) {array->setValue(0, _value); }\
    return array;\
  }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
#define WriteHDF5Attribute_Implementation(thisClass)\
  int thisClass::writeHDF5Attribute(hid_t fileId, const QString &datasetPath)\
  {\
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();\
    QString tagName = tagMap->nameForId(this->getZeissIdTag());\
    if (tagName.isEmpty() == true) { std::cout << "writeHDF5Attribute: Error writing attribute. No zeiss id was found "\
                                                 << " for id=" << getZeissIdTag(); return -19; } \
    int32_t err = QH5Lite::writeScalarAttribute(fileId, datasetPath, tagName, _value );\
    if ( err < 0 ) {\
      qDebug() << "Error writing attribute " << tagName << " with value " << _value \
               <<   " to data set " << datasetPath; return err;}\
    return err;\
  }

#define ReadHDF5Attribute_DEFINITION(type, thisClass)\
  int thisClass::readHDF5Attribute(hid_t fileId, const QString &datasetPath) { \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();\
    QString tagName = tagMap->nameForId(getZeissIdTag());\
    if (tagName.isEmpty() == true) { std::cout << "readHDF5Attribute: Error reading attribute. No zeiss id was found "\
                                                 << " for id=" << getZeissIdTag(); return -19; } \
    int32_t err = QH5Lite::readScalarAttribute<type>(fileId, datasetPath, tagName, _value);\
    if ( err < 0 ) {\
      qDebug() << "Error reading attribute " << tagName << " from data set " << datasetPath;\
      return err;}\
    return err;\
  }


#if 1
#define WriteHDF5Attribute_StrImplementation(thisClass)\
  int thisClass::writeHDF5Attribute(hid_t fileId, const QString &datasetPath)\
  {\
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();\
    QString tagName = tagMap->nameForId(getZeissIdTag());\
    if (tagName.isEmpty() == true) { std::cout << "writeHDF5Attribute: Error writing attribute. No zeiss id was found "\
                                                 << " for id=" << getZeissIdTag(); return -19; } \
    int32_t err = QH5Lite::writeStringAttribute(fileId, datasetPath, tagName, _value );\
    if ( err < 0 ) {\
      qDebug() << "Error adding attribute " << tagName << " with value " << _value \
               <<   " to data set " << datasetPath; return err;}\
    return err;\
  }



#define ReadHDF5Attribute_STR_DEFINITION(thisClass)\
  int thisClass::readHDF5Attribute(hid_t fileId, const QString &datasetPath) { \
    ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();\
    QString tagName = tagMap->nameForId(getZeissIdTag());\
    if (tagName.isEmpty() == true) { std::cout << "readHDF5Attribute: Error reading attribute. No zeiss id was found "\
                                                 << " for id=" << getZeissIdTag(); return -19; } \
    int32_t err = QH5Lite::readStringAttribute(fileId, datasetPath, tagName, _value);\
    if ( err < 0 ) {\
      qDebug() << "Error reading attribute " << tagName << " from data set " << datasetPath;\
      return err;}\
    return err;\
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
// Int32ZeissMetaEntry Implementations
// -----------------------------------------------------------------------------
VALUE_DEFINITION(Int32ZeissMetaEntry, _value, Int32ArrayType)
#ifdef ZEISS_HDF_SUPPORT
WriteHDF5Attribute_Implementation(Int32ZeissMetaEntry)
ReadHDF5Attribute_DEFINITION(int32_t, Int32ZeissMetaEntry)
#endif

// -----------------------------------------------------------------------------
// Int64ZeissMetaEntry Implementations
// -----------------------------------------------------------------------------
VALUE_DEFINITION(Int64ZeissMetaEntry, _value, Int64ArrayType)
#ifdef ZEISS_HDF_SUPPORT
WriteHDF5Attribute_Implementation(Int64ZeissMetaEntry)
ReadHDF5Attribute_DEFINITION(qint64, Int64ZeissMetaEntry)
#endif

// -----------------------------------------------------------------------------
// FloatZeissMetaEntry Implementations
// -----------------------------------------------------------------------------
IDataArray::Pointer FloatZeissMetaEntry::createDataArray(bool allocate)
{
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();
  QString tagName = tagMap->nameForId(this->getZeissIdTag());
  if (tagName.isEmpty() == true)
  {
    std::cout << "writeHDF5Attribute: Error writing attribute. No zeiss id was found "
              << " for id=" << getZeissIdTag();
    return FloatArrayType::NullPointer();
  }
  FloatArrayType::Pointer array = FloatArrayType::CreateArray(1, tagName, allocate);
  if(allocate) {array->setValue(0, _value);}
  return array;
}
#ifdef ZEISS_HDF_SUPPORT
WriteHDF5Attribute_Implementation(FloatZeissMetaEntry)
ReadHDF5Attribute_DEFINITION(float, FloatZeissMetaEntry)
#endif

// -----------------------------------------------------------------------------
// StringZeissMetaEntry Implementations
// -----------------------------------------------------------------------------
IDataArray::Pointer StringZeissMetaEntry::createDataArray(bool allocate)
{
  ZeissTagMapping::Pointer tagMap = ZeissTagMapping::instance();
  QString tagName = tagMap->nameForId(this->getZeissIdTag());
  if (tagName.isEmpty() == true)
  {
    std::cout << "writeHDF5Attribute: Error writing attribute. No zeiss id was found "
              << " for id=" << getZeissIdTag();
    return StringDataArray::NullPointer();
  }
  StringDataArray::Pointer array = StringDataArray::CreateArray(1, tagName, allocate);
  if(allocate) {array->setValue(0, _value);}
  return array;
}
#ifdef ZEISS_HDF_SUPPORT
WriteHDF5Attribute_StrImplementation(StringZeissMetaEntry)
ReadHDF5Attribute_STR_DEFINITION(StringZeissMetaEntry)
#endif

