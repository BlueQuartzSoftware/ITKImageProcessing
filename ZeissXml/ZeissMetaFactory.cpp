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


#include "ZeissMetaFactory.h"


AbstractZeissMetaData::Pointer Int32ZeissMetaFactory::createMetaEntry()
{
  AbstractZeissMetaData::Pointer ptr = Int32ZeissMetaEntry::New();
  return ptr;
}


AbstractZeissMetaData::Pointer Int64ZeissMetaFactory::createMetaEntry()
{
  AbstractZeissMetaData::Pointer ptr = Int64ZeissMetaEntry::New();
  return ptr;
}


AbstractZeissMetaData::Pointer FloatZeissMetaFactory::createMetaEntry()
{
  AbstractZeissMetaData::Pointer ptr = FloatZeissMetaEntry::New();
  return ptr;
}


AbstractZeissMetaData::Pointer StringZeissMetaFactory::createMetaEntry()
{
  AbstractZeissMetaData::Pointer ptr = StringZeissMetaEntry::New();
  return ptr;
}

