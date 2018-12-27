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
#pragma once

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/IDataArray.h"

#include "ZeissImport/ZeissImportConstants.h"

#ifdef ZEISS_HDF_SUPPORT
#include "H5Support/H5Lite.h"
#include "H5Support/H5Utilities.h"
#endif

#define STATIC_NEW_METHODS(SuperClass, thisClass, type)                                                                                                                                                \
  static SuperClass::Pointer New(int32_t idTag, const QString& value)                                                                                                                                  \
  {                                                                                                                                                                                                    \
    auto ptr = std::make_shared<thisClass>(idTag);                                                                                                                                                     \
    bool ok = ptr->setValue(value);                                                                                                                                                                    \
    if(ok == false)                                                                                                                                                                                    \
    {                                                                                                                                                                                                  \
      ptr = thisClass::NullPointer();                                                                                                                                                                  \
    }                                                                                                                                                                                                  \
    return ptr;                                                                                                                                                                                        \
  }                                                                                                                                                                                                    \
                                                                                                                                                                                                       \
  static SuperClass::Pointer New(int32_t idTag, type value)                                                                                                                                            \
  {                                                                                                                                                                                                    \
    auto* p = new thisClass(idTag);                                                                                                                                                                    \
    SuperClass::Pointer ptr(static_cast<SuperClass*>(p));                                                                                                                                              \
    p->setValue(value);                                                                                                                                                                                \
    return ptr;                                                                                                                                                                                        \
  }

#define VALUE_DECLARATION(type)\
virtual bool setValue(const QString &value);\
bool setValue(type value);\
type getValue();\

#define HDF5_READ_WRITE_DECLARATIONS()\
int writeHDF5Attribute(hid_t fileId, const QString &datasetPath);\
int readHDF5Attribute(hid_t fileId, const QString &datasetPath);\



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class AbstractZeissMetaData
{
  public:
    SIMPL_SHARED_POINTERS(AbstractZeissMetaData)
    SIMPL_TYPE_MACRO(AbstractZeissMetaData)

    virtual ~AbstractZeissMetaData() = default;

    SIMPL_INSTANCE_PROPERTY(int32_t, ZeissIdTag)

    virtual bool setValue(const QString &value) = 0;
#ifdef ZEISS_HDF_SUPPORT
    virtual int writeHDF5Attribute(hid_t fileId, const QString &datasetPath) = 0;
    virtual int readHDF5Attribute(hid_t fileId, const QString &datasetPath) = 0;
#endif
    virtual void printValue(std::ostream &out) = 0;

    virtual IDataArray::Pointer createDataArray(bool allocate = true) = 0;

  protected:
    AbstractZeissMetaData() {m_ZeissIdTag = 0;}

  public:
    AbstractZeissMetaData(const AbstractZeissMetaData&) = delete; // Copy Constructor Not Implemented
    AbstractZeissMetaData(AbstractZeissMetaData&&) = delete;      // Move Constructor Not Implemented
    AbstractZeissMetaData& operator=(const AbstractZeissMetaData&) = delete; // Copy Assignment Not Implemented
    AbstractZeissMetaData& operator=(AbstractZeissMetaData&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class Int32ZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    SIMPL_SHARED_POINTERS(Int32ZeissMetaEntry)
    SIMPL_TYPE_MACRO(Int32ZeissMetaEntry)
    SIMPL_STATIC_NEW_MACRO(Int32ZeissMetaEntry)

    Int32ZeissMetaEntry();

    explicit Int32ZeissMetaEntry(int32_t idTag);

    STATIC_NEW_METHODS(AbstractZeissMetaData, Int32ZeissMetaEntry, int32_t)

    ~Int32ZeissMetaEntry() override;

    VALUE_DECLARATION(int32_t)

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif

    void printValue(std::ostream& out) override;

    IDataArray::Pointer createDataArray(bool allocate = true) override;

  protected:



  private:
    int m_Value = 0;

  public:
    Int32ZeissMetaEntry(const Int32ZeissMetaEntry&) = delete; // Copy Constructor Not Implemented
    Int32ZeissMetaEntry(Int32ZeissMetaEntry&&) = delete;      // Move Constructor Not Implemented
    Int32ZeissMetaEntry& operator=(const Int32ZeissMetaEntry&) = delete; // Copy Assignment Not Implemented
    Int32ZeissMetaEntry& operator=(Int32ZeissMetaEntry&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class Int64ZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    SIMPL_SHARED_POINTERS(Int64ZeissMetaEntry)
    SIMPL_TYPE_MACRO(Int64ZeissMetaEntry)
    SIMPL_STATIC_NEW_MACRO(Int64ZeissMetaEntry)

    STATIC_NEW_METHODS(AbstractZeissMetaData, Int64ZeissMetaEntry, int64_t)

    Int64ZeissMetaEntry();
    explicit Int64ZeissMetaEntry(int32_t idTag);

    ~Int64ZeissMetaEntry() override;

    VALUE_DECLARATION(int64_t)

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif
    void printValue(std::ostream& out) override;

    IDataArray::Pointer createDataArray(bool allocate = true) override;

  protected:


  private:
    int64_t m_Value = 0;

  public:
    Int64ZeissMetaEntry(const Int64ZeissMetaEntry&) = delete; // Copy Constructor Not Implemented
    Int64ZeissMetaEntry(Int64ZeissMetaEntry&&) = delete;      // Move Constructor Not Implemented
    Int64ZeissMetaEntry& operator=(const Int64ZeissMetaEntry&) = delete; // Copy Assignment Not Implemented
    Int64ZeissMetaEntry& operator=(Int64ZeissMetaEntry&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class FloatZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    SIMPL_SHARED_POINTERS(FloatZeissMetaEntry)
    SIMPL_TYPE_MACRO(FloatZeissMetaEntry)
    SIMPL_STATIC_NEW_MACRO(FloatZeissMetaEntry)

    STATIC_NEW_METHODS(AbstractZeissMetaData, FloatZeissMetaEntry, float)

    FloatZeissMetaEntry();
    explicit FloatZeissMetaEntry(int32_t idTag);

    ~FloatZeissMetaEntry() override;

    VALUE_DECLARATION(float)

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif

    void printValue(std::ostream& out) override;

    IDataArray::Pointer createDataArray(bool allocate = true) override;

  protected:
  private:
    float m_Value = 0.0f;

  public:
    FloatZeissMetaEntry(const FloatZeissMetaEntry&) = delete; // Copy Constructor Not Implemented
    FloatZeissMetaEntry(FloatZeissMetaEntry&&) = delete;      // Move Constructor Not Implemented
    FloatZeissMetaEntry& operator=(const FloatZeissMetaEntry&) = delete; // Copy Assignment Not Implemented
    FloatZeissMetaEntry& operator=(FloatZeissMetaEntry&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class StringZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    SIMPL_SHARED_POINTERS(StringZeissMetaEntry)
    SIMPL_TYPE_MACRO(StringZeissMetaEntry)
    SIMPL_STATIC_NEW_MACRO(StringZeissMetaEntry)

    static AbstractZeissMetaData::Pointer New(int32_t idTag, const QString &value) {
      AbstractZeissMetaData::Pointer ptr (new StringZeissMetaEntry(idTag));
      bool ok = ptr->setValue(value);
      if(!ok)
      {
        ptr = AbstractZeissMetaData::NullPointer();
      }
      return ptr;
    }

    ~StringZeissMetaEntry() override;

    bool setValue(const QString& value) override;
    QString getValue();

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif

    void printValue(std::ostream& out) override;

    IDataArray::Pointer createDataArray(bool allocate = true) override;

  protected:
    StringZeissMetaEntry()
    {
      m_Value = "";
    }

    explicit StringZeissMetaEntry(int32_t idTag)
    {
       setZeissIdTag(idTag);
    }


  private:
    QString m_Value;

  public:
    StringZeissMetaEntry(const StringZeissMetaEntry&) = delete; // Copy Constructor Not Implemented
    StringZeissMetaEntry(StringZeissMetaEntry&&) = delete;      // Move Constructor Not Implemented
    StringZeissMetaEntry& operator=(const StringZeissMetaEntry&) = delete; // Copy Assignment Not Implemented
    StringZeissMetaEntry& operator=(StringZeissMetaEntry&&) = delete;      // Move Assignment Not Implemented
};


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
namespace ZeissMetaEntry {


  template<typename DestMetaData>
  typename DestMetaData::Pointer convert(AbstractZeissMetaData::Pointer src)
  {

    StringZeissMetaEntry::Pointer srcPtr = std::dynamic_pointer_cast<StringZeissMetaEntry>(src);
    Q_ASSERT_X(srcPtr.get() != nullptr, "Could not Cast to StringZeissMetaEntry", "");


    typename DestMetaData::Pointer destPtr = DestMetaData::New();
    destPtr->setValue(srcPtr->getValue());
    return destPtr;

  }

}


