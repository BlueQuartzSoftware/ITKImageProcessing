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
#ifndef _zeissmetafactory_h_
#define _zeissmetafactory_h_

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

    virtual ~ZeissMetaFactory() {}

    virtual AbstractZeissMetaData::Pointer createMetaEntry() = 0;
  protected:
    ZeissMetaFactory() {}

  private:
    ZeissMetaFactory(const ZeissMetaFactory&);    // Copy Constructor Not Implemented
      void operator=(const ZeissMetaFactory&);  // Operator '=' Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class Int32ZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(Int32ZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(Int32ZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry();

  protected:
    Int32ZeissMetaFactory() {}

  private:
    Int32ZeissMetaFactory(const Int32ZeissMetaFactory&);    // Copy Constructor Not Implemented
      void operator=(const Int32ZeissMetaFactory&);  // Operator '=' Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class Int64ZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(Int64ZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(Int64ZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry();

  protected:
    Int64ZeissMetaFactory() {}

  private:
    Int64ZeissMetaFactory(const Int64ZeissMetaFactory&);    // Copy Constructor Not Implemented
      void operator=(const Int64ZeissMetaFactory&);  // Operator '=' Not Implemented
};



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class FloatZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(FloatZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(FloatZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry();

  protected:
    FloatZeissMetaFactory() {}

  private:
    FloatZeissMetaFactory(const FloatZeissMetaFactory&);    // Copy Constructor Not Implemented
      void operator=(const FloatZeissMetaFactory&);  // Operator '=' Not Implemented
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
class StringZeissMetaFactory : public ZeissMetaFactory
{
  public:
    SIMPL_SHARED_POINTERS(StringZeissMetaFactory);
    SIMPL_NEW_SUPERCLASS(StringZeissMetaFactory, ZeissMetaFactory);

    AbstractZeissMetaData::Pointer createMetaEntry();

  protected:
    StringZeissMetaFactory() {}

  private:
    StringZeissMetaFactory(const StringZeissMetaFactory&);    // Copy Constructor Not Implemented
      void operator=(const StringZeissMetaFactory&);  // Operator '=' Not Implemented
};




#endif /* ZEISSMETAFACTORY_HPP_ */
