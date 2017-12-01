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

#include "ZeissTagMapping.h"
#include "ZeissImport/ZeissXml/ZeissMetaEntry.h"
#include "ZeissImport/ZeissXml/ZeissMetaFactory.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissTagMapping::ZeissTagMapping()
{
  initIdNameMap();
  initNameIdMap();
  initFactoryMap();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissTagMapping::~ZeissTagMapping() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissTagMapping::Pointer ZeissTagMapping::instance()
{
  static ZeissTagMapping::Pointer singleton;

  if (singleton.get() == nullptr)
  {
    singleton.reset (new ZeissTagMapping() );
  }
  return singleton;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissTagMapping::nameForId(int idTag)
{
  QMap<int, QString>::const_iterator i = _idNameMap.find(idTag);
  if (i != _idNameMap.end())
  {
    return i.value();
  }
  return QString("");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ZeissTagMapping::idForName(const QString& name)
{
  ZeissNameIdMap::const_iterator i = _nameIdMap.find(name);
  if (i != _nameIdMap.end())
  {
    return i.value();
  }
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissMetaFactory::Pointer ZeissTagMapping::factoryForId(int idTag)
{
  ZeissFactoryMap::const_iterator i = _idFactoryMap.find(idTag);
  if (i != _idFactoryMap.end())
  {
    return i.value();
  }

  return ZeissMetaFactory::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractZeissMetaData::Pointer ZeissTagMapping::metaDataForTagName(const QString& name)
{
  int idTag = idForName(name);
  if (idTag < 0)
  {
    return AbstractZeissMetaData::NullPointer();
  }

  ZeissMetaFactory::Pointer f = _idFactoryMap[idTag];
  if (nullptr != f.get() )
  {
    AbstractZeissMetaData::Pointer ptr = f->createMetaEntry();
    ptr->setZeissIdTag(idTag);
    return ptr;
  }
  return AbstractZeissMetaData::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractZeissMetaData::Pointer ZeissTagMapping::metaDataForId(int idTag, const QString& value)
{
  ZeissMetaFactory::Pointer f = _idFactoryMap[idTag];
  if (nullptr != f.get() )
  {
    AbstractZeissMetaData::Pointer ptr = f->createMetaEntry();
    ptr->setZeissIdTag(idTag);
    ptr->setValue(value);
    return ptr;
  }
  return AbstractZeissMetaData::NullPointer();
}


#include "ZeissTagMapping_InitMaps.cpp"
