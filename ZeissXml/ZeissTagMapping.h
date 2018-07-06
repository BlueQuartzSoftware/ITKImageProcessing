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

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include <map>

typedef QMap<int, QString>  ZeissIdNameMap;
typedef QMap<QString, int>  ZeissNameIdMap;
typedef QMap<int, ZeissMetaFactory::Pointer> ZeissFactoryMap;

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

    virtual ~ZeissTagMapping();

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


    ZeissTagMapping(const ZeissTagMapping&);    // Copy Constructor Not Implemented
    void operator=(const ZeissTagMapping&);     // Move assignment Not Implemented
};



#include "ZeissTagMappingConstants.h"



