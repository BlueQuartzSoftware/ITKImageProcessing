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

#include "ZeissTagsXmlSection.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissTagsXmlSection::ZeissTagsXmlSection() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissTagsXmlSection::~ZeissTagsXmlSection() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissTagsXmlSection::addMetaDataEntry(AbstractZeissMetaData::Pointer& entry)
{
  int idTag = entry->getZeissIdTag();
  QMap<int, AbstractZeissMetaData::Pointer>::iterator e = m_MetaDataMap.find(idTag);
  if(e != m_MetaDataMap.end())
  {
    m_MetaDataMap.erase(e);
  }
  m_MetaDataMap[entry->getZeissIdTag()] = entry;

  //  std::pair<int, AbstractZeissMetaData::Pointer> p(entry->getZeissIdTag(), entry);
  //  m_MetaDataMap.insert(p);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissTagsXmlSection::removeMetaDataEntry(int idTag)
{
  MetaDataType::iterator entry = m_MetaDataMap.find(idTag);
  if(entry != m_MetaDataMap.end())
  {
    m_MetaDataMap.erase(entry);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ZeissTagsXmlSection::count()
{
  return m_MetaDataMap.count();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissTagsXmlSection::MetaDataType& ZeissTagsXmlSection::getMetaDataMap()
{
  return m_MetaDataMap;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractZeissMetaData::Pointer ZeissTagsXmlSection::getEntry(int idTag)
{
  AbstractZeissMetaData::Pointer ptr = AbstractZeissMetaData::NullPointer();
  MetaDataType::iterator entry = m_MetaDataMap.find(idTag);
  if(entry != m_MetaDataMap.end())
  {
    return entry.value();
  }
  return ptr;
}

// -----------------------------------------------------------------------------
ZeissTagsXmlSection::Pointer ZeissTagsXmlSection::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
QString ZeissTagsXmlSection::getNameOfClass() const
{
  return QString("ZeissTagsXmlSection");
}

// -----------------------------------------------------------------------------
QString ZeissTagsXmlSection::ClassName()
{
  return QString("ZeissTagsXmlSection");
}

// -----------------------------------------------------------------------------
ZeissTagsXmlSection::Pointer ZeissTagsXmlSection::New()
{
  Pointer sharedPtr(new(ZeissTagsXmlSection));
  return sharedPtr;
}
