/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
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
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
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

 //
 //  This code was written under United States Air Force Contract number
 //                           FA8650-07-D-5800
 //
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef ZEISSTAGSXMLSECTION_H_
#define ZEISSTAGSXMLSECTION_H_

#include <QtCore/QMap>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include "ZeissImport/ZeissImportConstants.h"
#include "ZeissImport/ZeissXml/ZeissMetaEntry.h"

/*
 *
 */
class ZeissTagsXmlSection
{
  public:
    SIMPL_SHARED_POINTERS(ZeissTagsXmlSection)
    SIMPL_TYPE_MACRO(ZeissTagsXmlSection)
    SIMPL_STATIC_NEW_MACRO(ZeissTagsXmlSection)
    typedef QMap<int, AbstractZeissMetaData::Pointer>  MetaDataType;
    //typedef boost::shared_ptr<MetaDataType>            MetaDataPtrType;

    virtual ~ZeissTagsXmlSection();

    //SIMPL_INSTANCE_PROPERTY(MetaDataType, MetaDataMap)
    MetaDataType& getMetaDataMap();

    void addMetaDataEntry(AbstractZeissMetaData::Pointer entry);
    void removeMetaDataEntry(int idTag);
    AbstractZeissMetaData::Pointer getEntry(int idTag);
    int count();


  protected:
    ZeissTagsXmlSection();

  private:

    MetaDataType m_MetaDataMap;

    ZeissTagsXmlSection(const ZeissTagsXmlSection&); // Copy Constructor Not Implemented
    void operator=(const ZeissTagsXmlSection&); // Operator '=' Not Implemented
};

#endif /* ZEISSTAGSXMLSECTION_H_ */
