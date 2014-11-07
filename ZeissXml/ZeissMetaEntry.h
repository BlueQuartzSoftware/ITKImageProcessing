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
#ifndef ZEISSMETAENTRY_HPP_
#define ZEISSMETAENTRY_HPP_

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/DataArrays/IDataArray.h"

#include "ZeissImport/ZeissImportConstants.h"

#ifdef ZEISS_HDF_SUPPORT
#include "H5Support/H5Lite.h"
#include "H5Support/H5Utilities.h"
#endif

#define STATIC_NEW_METHODS(SuperClass, thisClass, type)\
static SuperClass::Pointer New(int32_t idTag, const QString &value) {\
  SuperClass::Pointer ptr (new thisClass(idTag));\
  bool ok = ptr->setValue(value);\
  if (ok == false) { ptr = SuperClass::NullPointer(); }\
  return ptr;\
}\
\
static SuperClass::Pointer New(int32_t idTag, type value) {\
  thisClass* p = new thisClass(idTag);\
  SuperClass::Pointer ptr ( static_cast<SuperClass*>(p) );\
  p->setValue(value);\
  return ptr;\
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
    DREAM3D_SHARED_POINTERS(AbstractZeissMetaData)
    DREAM3D_TYPE_MACRO(AbstractZeissMetaData)

    virtual ~AbstractZeissMetaData() {}

    DREAM3D_INSTANCE_PROPERTY(int32_t, ZeissIdTag)

    virtual bool setValue(const QString &value) = 0;
#ifdef ZEISS_HDF_SUPPORT
    virtual int writeHDF5Attribute(hid_t fileId, const QString &datasetPath) = 0;
    virtual int readHDF5Attribute(hid_t fileId, const QString &datasetPath) = 0;
#endif
    virtual void printValue(std::ostream &out) = 0;

    virtual IDataArray::Pointer createDataArray(bool allocate = true) = 0;

  protected:
    AbstractZeissMetaData() {m_ZeissIdTag = 0;}


  private:
    AbstractZeissMetaData(const AbstractZeissMetaData&);    // Copy Constructor Not Implemented
    void operator=(const AbstractZeissMetaData&);  // Operator '=' Not Implemented
};


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class Int32ZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    DREAM3D_SHARED_POINTERS(Int32ZeissMetaEntry)
    DREAM3D_TYPE_MACRO(Int32ZeissMetaEntry)
    DREAM3D_STATIC_NEW_MACRO(Int32ZeissMetaEntry)

    STATIC_NEW_METHODS(AbstractZeissMetaData, Int32ZeissMetaEntry, int32_t)

    virtual ~Int32ZeissMetaEntry() { }


    VALUE_DECLARATION(int32_t)

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif

    void printValue(std::ostream &out) { out << _value; }

    virtual IDataArray::Pointer createDataArray(bool allocate = true);


  protected:
    Int32ZeissMetaEntry()
    {
      this->_value = 0;
    }

    explicit Int32ZeissMetaEntry(int32_t idTag)
    {
      setZeissIdTag(idTag);
    }


  private:

    int _value;

    Int32ZeissMetaEntry(const Int32ZeissMetaEntry&);    // Copy Constructor Not Implemented
    void operator=(const Int32ZeissMetaEntry&);  // Operator '=' Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class Int64ZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    DREAM3D_SHARED_POINTERS(Int64ZeissMetaEntry)
    DREAM3D_TYPE_MACRO(Int64ZeissMetaEntry)
    DREAM3D_STATIC_NEW_MACRO(Int64ZeissMetaEntry)

    STATIC_NEW_METHODS(AbstractZeissMetaData, Int64ZeissMetaEntry, int64_t)

    VALUE_DECLARATION(int32_t)

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif
    void printValue(std::ostream &out) { out << _value; }

    virtual IDataArray::Pointer createDataArray(bool allocate = true);

  protected:
    Int64ZeissMetaEntry()
    {
      this->_value = 0;
    }

    explicit Int64ZeissMetaEntry(int32_t idTag)
    {
        setZeissIdTag(idTag);
    }


  private:
    long long int _value;

    Int64ZeissMetaEntry(const Int64ZeissMetaEntry&);    // Copy Constructor Not Implemented
    void operator=(const Int64ZeissMetaEntry&);  // Operator '=' Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class FloatZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    DREAM3D_SHARED_POINTERS(FloatZeissMetaEntry)
    DREAM3D_TYPE_MACRO(FloatZeissMetaEntry)
    DREAM3D_STATIC_NEW_MACRO(FloatZeissMetaEntry)

    STATIC_NEW_METHODS(AbstractZeissMetaData, FloatZeissMetaEntry, float)

    virtual ~FloatZeissMetaEntry() {}

    virtual bool setValue(const QString &value)
     {
      if(value.isNull() ) { return false;}
      if(value.isEmpty() ) { return false;}
      bool ok = false;

      _value = value.toFloat(&ok);
      return ok;
     }

    bool setValue(float f)
    {
      _value = f;
      return true;
    }

    float getValue() { return _value; }

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif

    void printValue(std::ostream &out) { out << _value; }

    virtual IDataArray::Pointer createDataArray(bool allocate = true);


  protected:
    FloatZeissMetaEntry()
    {
      _value = 0.0f;
    }
    explicit FloatZeissMetaEntry(int32_t idTag)
    {
        setZeissIdTag(idTag);
    }

  private:
    float _value;

    FloatZeissMetaEntry(const FloatZeissMetaEntry&);    // Copy Constructor Not Implemented
    void operator=(const FloatZeissMetaEntry&);  // Operator '=' Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class StringZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    DREAM3D_SHARED_POINTERS(StringZeissMetaEntry)
    DREAM3D_TYPE_MACRO(StringZeissMetaEntry)
    DREAM3D_STATIC_NEW_MACRO(StringZeissMetaEntry)

    static AbstractZeissMetaData::Pointer New(int32_t idTag, const QString &value) {
      AbstractZeissMetaData::Pointer ptr (new StringZeissMetaEntry(idTag));
      bool ok = ptr->setValue(value);
      if (ok == false) { ptr = AbstractZeissMetaData::NullPointer(); }
      return ptr;
    }

    virtual ~StringZeissMetaEntry() {}

    virtual bool setValue(const QString &value)
     {
       _value = value;
       return true;
     }

    QString getValue() { return _value; }

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif

    void printValue(std::ostream &out) { out << _value.toStdString(); }

    virtual IDataArray::Pointer createDataArray(bool allocate = true);

  protected:
    StringZeissMetaEntry()
    {
      _value = "";
    }

    explicit StringZeissMetaEntry(int32_t idTag)
    {
       setZeissIdTag(idTag);
    }


  private:
    QString _value;

    StringZeissMetaEntry(const StringZeissMetaEntry&);    // Copy Constructor Not Implemented
      void operator=(const StringZeissMetaEntry&);  // Operator '=' Not Implemented
};

#endif /* ZEISSMETAENTRY_HPP_ */
