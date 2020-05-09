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

#include <map>

#include "ITKImageProcessing/ZeissXml/ZeissMetaFactory.h"

#include "SIMPLib/Geometry/IGeometry.h"
#include "SIMPLib/SIMPLib.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

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
class ITKImageProcessing_EXPORT ZeissTagMapping
{
public:
  using Self = ZeissTagMapping;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  ~ZeissTagMapping();

  static ZeissTagMapping::Pointer instance();

  QString nameForId(int idtag);

  int idForName(const QString& name);

  ZeissMetaFactory::Pointer factoryForId(int idTag);

  AbstractZeissMetaData::Pointer metaDataForId(int idTag, const QString& value);

  AbstractZeissMetaData::Pointer metaDataForTagName(const QString& name);

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
  ZeissTagMapping(const ZeissTagMapping&) = delete;            // Copy Constructor Not Implemented
  ZeissTagMapping(ZeissTagMapping&&) = delete;                 // Move Constructor Not Implemented
  ZeissTagMapping& operator=(const ZeissTagMapping&) = delete; // Copy Assignment Not Implemented
  ZeissTagMapping& operator=(ZeissTagMapping&&) = delete;      // Move Assignment Not Implemented
};

/**
 * @brief The ZeissUnitMapping class
 */
class ZeissUnitMapping
{
public:
  using Self = ZeissUnitMapping;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

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
