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

#include "ZeissImport/ZeissXml/ZeissMetaEntry.h"

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(ZeissMetaFactory);

    virtual ~ZeissMetaFactory() = default;

    virtual AbstractZeissMetaData::Pointer createMetaEntry() = 0;
  protected:
    ZeissMetaFactory() = default;

  public:
    ZeissMetaFactory(const ZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    ZeissMetaFactory(ZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    ZeissMetaFactory& operator=(const ZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    ZeissMetaFactory& operator=(ZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class Int32ZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(Int32ZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(Int32ZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry() override;

  protected:
    Int32ZeissMetaFactory() = default;

  public:
    Int32ZeissMetaFactory(const Int32ZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    Int32ZeissMetaFactory(Int32ZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    Int32ZeissMetaFactory& operator=(const Int32ZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    Int32ZeissMetaFactory& operator=(Int32ZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class Int64ZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(Int64ZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(Int64ZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry() override;

  protected:
    Int64ZeissMetaFactory() = default;

  public:
    Int64ZeissMetaFactory(const Int64ZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    Int64ZeissMetaFactory(Int64ZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    Int64ZeissMetaFactory& operator=(const Int64ZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    Int64ZeissMetaFactory& operator=(Int64ZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class FloatZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(FloatZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(FloatZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry() override;

  protected:
    FloatZeissMetaFactory() = default;

  public:
    FloatZeissMetaFactory(const FloatZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    FloatZeissMetaFactory(FloatZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    FloatZeissMetaFactory& operator=(const FloatZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    FloatZeissMetaFactory& operator=(FloatZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class StringZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(StringZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(StringZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry() override;

  protected:
    StringZeissMetaFactory() = default;

  public:
    StringZeissMetaFactory(const StringZeissMetaFactory&) = delete; // Copy Constructor Not Implemented
    StringZeissMetaFactory(StringZeissMetaFactory&&) = delete;      // Move Constructor Not Implemented
    StringZeissMetaFactory& operator=(const StringZeissMetaFactory&) = delete; // Copy Assignment Not Implemented
    StringZeissMetaFactory& operator=(StringZeissMetaFactory&&) = delete;      // Move Assignment Not Implemented
};
