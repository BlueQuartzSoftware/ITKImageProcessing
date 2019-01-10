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

#error THIS FILE IS NO LONGER USED OR COMPILED

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"

#include <string>

namespace Zeiss {
namespace DirectoryPatterns
{
const std::string Dot(".");
const std::string _Files("_Files");
const std::string _P("_p");
const std::string MetaXML("_meta.xml");
} // namespace DirectoryPatterns
}


/**
 *
 */
class ZeissDirectoryPatterns
{
  public:
    SIMPL_SHARED_POINTERS(ZeissDirectoryPatterns)
    SIMPL_TYPE_MACRO(ZeissDirectoryPatterns)
    SIMPL_STATIC_NEW_MACRO(ZeissDirectoryPatterns)


    virtual ~ZeissDirectoryPatterns();

    SIMPL_INSTANCE_STRING_PROPERTY(Prefix)
    SIMPL_INSTANCE_STRING_PROPERTY(Suffix)
    SIMPL_INSTANCE_STRING_PROPERTY(Extension)

    std::string generateZeissDirectory(int slice);
    std::string generateZeissDirectory(const std::string &slice);

    std::string generateZeissMetaXMLName(int slice);
    std::string generateZeissMetaXMLName(const std::string &slice);

    std::string generateZeissTileName(int slice, int numberWidth, int tileIndex);

    int parseDocumentName(const std::string& guessedPrefix, const std::string& zeissDocumentName);

    int extractSlice(const std::string &directoryName);

    void print(std::ostream &ostream);

  protected:
    ZeissDirectoryPatterns();

  public:
    ZeissDirectoryPatterns(const ZeissDirectoryPatterns&) = delete; // Copy Constructor Not Implemented
    ZeissDirectoryPatterns(ZeissDirectoryPatterns&&) = delete;      // Move Constructor Not Implemented
    ZeissDirectoryPatterns& operator=(const ZeissDirectoryPatterns&) = delete; // Copy Assignment Not Implemented
    ZeissDirectoryPatterns& operator=(ZeissDirectoryPatterns&&) = delete;      // Move Assignment Not Implemented
};

