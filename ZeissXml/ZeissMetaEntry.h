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
#pragma once

#include <memory>

#include "SIMPLib/SIMPLib.h"

class IDataArray;
using IDataArrayShPtrType = std::shared_ptr<IDataArray>;

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

#ifdef ZEISS_HDF_SUPPORT
#include "H5Support/H5Lite.h"
#include "H5Support/H5Utilities.h"
#endif

using IDataArrayShPtrType = std::shared_ptr<IDataArray>;

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

#define VALUE_DECLARATION(type)                                                                                                                                                                        \
  bool setValue(const QString& value) override;                                                                                                                                                        \
  bool setValue(type value);                                                                                                                                                                           \
  type getValue() const;

#define HDF5_READ_WRITE_DECLARATIONS()                                                                                                                                                                 \
  int writeHDF5Attribute(hid_t fileId, const QString& datasetPath) override;                                                                                                                           \
  int readHDF5Attribute(hid_t fileId, const QString& datasetPath) override;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImageProcessing_EXPORT AbstractZeissMetaData
{
  public:
    using Self = AbstractZeissMetaData;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    /**
     * @brief Returns the name of the class for AbstractZeissMetaData
     */
    virtual QString getNameOfClass() const;
    /**
     * @brief Returns the name of the class for AbstractZeissMetaData
     */
    static QString ClassName();

    virtual ~AbstractZeissMetaData() = default;

    /**
     * @brief Setter property for ZeissIdTag
     */
    void setZeissIdTag(int32_t value);
    /**
     * @brief Getter property for ZeissIdTag
     * @return Value of ZeissIdTag
     */
    int32_t getZeissIdTag() const;

    virtual bool setValue(const QString &value) = 0;
#ifdef ZEISS_HDF_SUPPORT
    virtual int writeHDF5Attribute(hid_t fileId, const QString &datasetPath) = 0;
    virtual int readHDF5Attribute(hid_t fileId, const QString &datasetPath) = 0;
#endif
    virtual void printValue(std::ostream& out) const = 0;

    virtual QString toString() const = 0;

    virtual IDataArrayShPtrType createDataArray(bool allocate = true) const = 0;

  protected:
    AbstractZeissMetaData();

  private:
    int32_t m_ZeissIdTag = 0;

  public:
    AbstractZeissMetaData(const AbstractZeissMetaData&) = delete; // Copy Constructor Not Implemented
    AbstractZeissMetaData(AbstractZeissMetaData&&) = delete;      // Move Constructor Not Implemented
    AbstractZeissMetaData& operator=(const AbstractZeissMetaData&) = delete; // Copy Assignment Not Implemented
    AbstractZeissMetaData& operator=(AbstractZeissMetaData&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ITKImageProcessing_EXPORT Int32ZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    using Self = Int32ZeissMetaEntry;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    /**
     * @brief Returns the name of the class for Int32ZeissMetaEntry
     */
    QString getNameOfClass() const override;
    /**
     * @brief Returns the name of the class for Int32ZeissMetaEntry
     */
    static QString ClassName();

    static Pointer New();

    Int32ZeissMetaEntry();

    explicit Int32ZeissMetaEntry(int32_t idTag);

    STATIC_NEW_METHODS(AbstractZeissMetaData, Int32ZeissMetaEntry, int32_t)

    ~Int32ZeissMetaEntry() override;

    VALUE_DECLARATION(int32_t)

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif

    void printValue(std::ostream& out) const override;

    QString toString() const override;

    IDataArrayShPtrType createDataArray(bool allocate = true) const override;

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
class ITKImageProcessing_EXPORT Int64ZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    using Self = Int64ZeissMetaEntry;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    /**
     * @brief Returns the name of the class for Int64ZeissMetaEntry
     */
    QString getNameOfClass() const override;
    /**
     * @brief Returns the name of the class for Int64ZeissMetaEntry
     */
    static QString ClassName();

    static Pointer New();

    STATIC_NEW_METHODS(AbstractZeissMetaData, Int64ZeissMetaEntry, int64_t)

    Int64ZeissMetaEntry();
    explicit Int64ZeissMetaEntry(int32_t idTag);

    ~Int64ZeissMetaEntry() override;

    VALUE_DECLARATION(int64_t)

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif
    void printValue(std::ostream& out) const override;

    QString toString() const override;

    IDataArrayShPtrType createDataArray(bool allocate = true) const override;

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
class ITKImageProcessing_EXPORT FloatZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    using Self = FloatZeissMetaEntry;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    /**
     * @brief Returns the name of the class for FloatZeissMetaEntry
     */
    QString getNameOfClass() const override;
    /**
     * @brief Returns the name of the class for FloatZeissMetaEntry
     */
    static QString ClassName();

    static Pointer New();

    STATIC_NEW_METHODS(AbstractZeissMetaData, FloatZeissMetaEntry, float)

    FloatZeissMetaEntry();
    explicit FloatZeissMetaEntry(int32_t idTag);

    ~FloatZeissMetaEntry() override;

    VALUE_DECLARATION(float)

#ifdef ZEISS_HDF_SUPPORT
    HDF5_READ_WRITE_DECLARATIONS()
#endif

    void printValue(std::ostream& out) const override;

    QString toString() const override;

    IDataArrayShPtrType createDataArray(bool allocate = true) const override;

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
class ITKImageProcessing_EXPORT StringZeissMetaEntry : public AbstractZeissMetaData
{
  public:
    using Self = StringZeissMetaEntry;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<Self>;
    static Pointer NullPointer();

    /**
     * @brief Returns the name of the class for StringZeissMetaEntry
     */
    QString getNameOfClass() const override;
    /**
     * @brief Returns the name of the class for StringZeissMetaEntry
     */
    static QString ClassName();

    static Pointer New();

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

    void printValue(std::ostream& out) const override;

    QString toString() const override;

    IDataArrayShPtrType createDataArray(bool allocate = true) const override;

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


