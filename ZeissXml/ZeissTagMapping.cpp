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

#include "ZeissTagMapping.h"
#include "ITKImageProcessing/ZeissXml/ZeissMetaEntry.h"
#include "ITKImageProcessing/ZeissXml/ZeissMetaFactory.h"


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
  QMap<int, QString>::iterator i = _idNameMap.find(idTag);
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
  ZeissNameIdMap::iterator i = _nameIdMap.find(name);
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
  ZeissFactoryMap::iterator i = _idFactoryMap.find(idTag);
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissUnitMapping::ZeissUnitMapping() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissUnitMapping::~ZeissUnitMapping() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissUnitMapping::Pointer ZeissUnitMapping::Instance()
{
  static ZeissUnitMapping::Pointer singleton;

  if(singleton.get() == nullptr)
  {
    singleton.reset(new ZeissUnitMapping());
  }
  return singleton;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ZeissUnitMapping::nameForId(int idtag)
{
  switch(idtag)
  {
  case Zeiss::MetaXML::ziScalingTypeMask:
    return Zeiss::MetaXML::k_ScalingTypeMask;

  case Zeiss::MetaXML::ziScalingTypePixel:
    return Zeiss::MetaXML::k_ScalingTypePixel;

  case Zeiss::MetaXML::ziScalingTypeGigameter:
    return Zeiss::MetaXML::k_ScalingTypeGigameter;

  case Zeiss::MetaXML::ziScalingTypeMegameter:
    return Zeiss::MetaXML::k_ScalingTypeMegameter;

  case Zeiss::MetaXML::ziScalingTypeKilometer:
    return Zeiss::MetaXML::k_ScalingTypeKilometer;

  case Zeiss::MetaXML::ziScalingTypeHectometer:
    return Zeiss::MetaXML::k_ScalingTypeHectometer;

  case Zeiss::MetaXML::ziScalingTypeDecameter:
    return Zeiss::MetaXML::k_ScalingTypeDecameter;

  case Zeiss::MetaXML::ziScalingTypeMeter:
    return Zeiss::MetaXML::k_ScalingTypeMeter;

  case Zeiss::MetaXML::ziScalingTypeDecimeter:
    return Zeiss::MetaXML::k_ScalingTypeDecimeter;

  case Zeiss::MetaXML::ziScalingTypeCentimeter:
    return Zeiss::MetaXML::k_ScalingTypeCentimeter;

  case Zeiss::MetaXML::ziScalingTypeMillimeter:
    return Zeiss::MetaXML::k_ScalingTypeMillimeter;

  case Zeiss::MetaXML::ziScalingTypeMicrometer:
    return Zeiss::MetaXML::k_ScalingTypeMicrometer;

  case Zeiss::MetaXML::ziScalingTypeNanometer:
    return Zeiss::MetaXML::k_ScalingTypeNanometer;

  case Zeiss::MetaXML::ziScalingTypeSecond:
    return Zeiss::MetaXML::k_ScalingTypeSecond;

  case Zeiss::MetaXML::ziScalingTypeMillisecond:
    return Zeiss::MetaXML::k_ScalingTypeMillisecond;

  case Zeiss::MetaXML::ziScalingTypeMicrosecond:
    return Zeiss::MetaXML::k_ScalingTypeMicrosecond;

  case Zeiss::MetaXML::ziScalingTypeMinute:
    return Zeiss::MetaXML::k_ScalingTypeMinute;

  case Zeiss::MetaXML::ziScalingTypeHour:
    return Zeiss::MetaXML::k_ScalingTypeHour;

  case Zeiss::MetaXML::ziScalingTypeInch:
    return Zeiss::MetaXML::k_ScalingTypeInch;

  case Zeiss::MetaXML::ziScalingTypeMil:
    return Zeiss::MetaXML::k_ScalingTypeMil;
  }

  return "Unknown";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IGeometry::LengthUnit ZeissUnitMapping::convertToIGeometryLengthUnit(int idTag)
{
  switch(idTag)
  {
  case Zeiss::MetaXML::ziScalingTypeMask:
    return IGeometry::LengthUnit::Unknown;

  case Zeiss::MetaXML::ziScalingTypePixel:
    return IGeometry::LengthUnit::Unknown;

  case Zeiss::MetaXML::ziScalingTypeGigameter:
    return IGeometry::LengthUnit::Gigameter;

  case Zeiss::MetaXML::ziScalingTypeMegameter:
    return IGeometry::LengthUnit::Megameter;

  case Zeiss::MetaXML::ziScalingTypeKilometer:
    return IGeometry::LengthUnit::Kilometer;

  case Zeiss::MetaXML::ziScalingTypeHectometer:
    return IGeometry::LengthUnit::Hectometer;

  case Zeiss::MetaXML::ziScalingTypeDecameter:
    return IGeometry::LengthUnit::Decameter;

  case Zeiss::MetaXML::ziScalingTypeMeter:
    return IGeometry::LengthUnit::Meter;

  case Zeiss::MetaXML::ziScalingTypeDecimeter:
    return IGeometry::LengthUnit::Decimeter;

  case Zeiss::MetaXML::ziScalingTypeCentimeter:
    return IGeometry::LengthUnit::Centimeter;

  case Zeiss::MetaXML::ziScalingTypeMillimeter:
    return IGeometry::LengthUnit::Millimeter;

  case Zeiss::MetaXML::ziScalingTypeMicrometer:
    return IGeometry::LengthUnit::Micrometer;

  case Zeiss::MetaXML::ziScalingTypeNanometer:
    return IGeometry::LengthUnit::Nanometer;

  case Zeiss::MetaXML::ziScalingTypeSecond:
    return IGeometry::LengthUnit::Unknown;

  case Zeiss::MetaXML::ziScalingTypeMillisecond:
    return IGeometry::LengthUnit::Unknown;

  case Zeiss::MetaXML::ziScalingTypeMicrosecond:
    return IGeometry::LengthUnit::Unknown;

  case Zeiss::MetaXML::ziScalingTypeMinute:
    return IGeometry::LengthUnit::Unknown;

  case Zeiss::MetaXML::ziScalingTypeHour:
    return IGeometry::LengthUnit::Unknown;

  case Zeiss::MetaXML::ziScalingTypeInch:
    return IGeometry::LengthUnit::Unknown;

  case Zeiss::MetaXML::ziScalingTypeMil:
    return IGeometry::LengthUnit::Mil;
  }

  return IGeometry::LengthUnit::Unknown;
}
