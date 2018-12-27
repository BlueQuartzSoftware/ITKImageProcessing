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


#include "ZeissImport/ZeissXml/ZeissMetaFactory.h"

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Geometry/IGeometry.h"
#include "SIMPLib/SIMPLib.h"

#include <map>

using ZeissIdNameMap = QMap<int, QString>;
using ZeissNameIdMap = QMap<QString, int>;
using ZeissFactoryMap = QMap<int, ZeissMetaFactory::Pointer>;

/**
* @class ZeissTagMapping ZeissTagMapping.h R3D/Common/ZeissTagMapping.h
* @brief This class is used to map values from the Zeiss XML file to and from
* human readable values. It also creates factories for each of the tags found in the
* Zeiss XML file. For each value there is a const QString and an integer value
* that uniquely identifies the tag.
* @author Michael A. Jackson for BlueQuartz Software
* @date Jul 29, 2009
* @version 1.0
*/
class ZeissTagMapping
{
  public:
    SIMPL_SHARED_POINTERS(ZeissTagMapping)

    ~ZeissTagMapping();

    static ZeissTagMapping::Pointer instance();


    QString nameForId(int idtag);

    int idForName(const QString &name);

    ZeissMetaFactory::Pointer factoryForId(int idTag);

    AbstractZeissMetaData::Pointer metaDataForId(int idTag, const QString &value);

    AbstractZeissMetaData::Pointer metaDataForTagName(const QString &name);

  protected:
    ZeissTagMapping();

    void initIdNameMap();

    void initNameIdMap();

    void initFactoryMap();

  private:
    ZeissIdNameMap _idNameMap;
    ZeissNameIdMap _nameIdMap;
    ZeissFactoryMap _idFactoryMap;

  public:
    ZeissTagMapping(const ZeissTagMapping&) = delete; // Copy Constructor Not Implemented
    ZeissTagMapping(ZeissTagMapping&&) = delete;      // Move Constructor Not Implemented
    ZeissTagMapping& operator=(const ZeissTagMapping&) = delete; // Copy Assignment Not Implemented
    ZeissTagMapping& operator=(ZeissTagMapping&&) = delete;      // Move Assignment Not Implemented
};

/**
 * @brief The ZeissUnitMapping class
 */
class ZeissUnitMapping
{
public:
  SIMPL_SHARED_POINTERS(ZeissUnitMapping)

  ~ZeissUnitMapping();

  static ZeissUnitMapping::Pointer Instance();

  QString nameForId(int idtag);

  IGeometry::LengthUnit convertToIGeometryLengthUnit(int idTag);

  //  int idForName(const QString &name);

protected:
  ZeissUnitMapping();

public:
  ZeissUnitMapping(const ZeissUnitMapping&) = delete;            // Copy Constructor Not Implemented
  ZeissUnitMapping(ZeissUnitMapping&&) = delete;                 // Move Constructor Not Implemented
  ZeissUnitMapping& operator=(const ZeissUnitMapping&) = delete; // Copy Assignment Not Implemented
  ZeissUnitMapping& operator=(ZeissUnitMapping&&) = delete;      // Move Assignment Not Implemented
};

#include "ZeissTagMappingConstants.h"



