/* ============================================================================
 * Copyright (c) 2019 BlueQuartz Software, LLC
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#pragma once

#include <memory>

#include <QtXml/QDomElement>

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"

class AbstractFilter;
class ZeissTagsXmlSection;
using ZeissTagsXmlSectionPtr = std::shared_ptr<ZeissTagsXmlSection>;

class MetaXmlUtils
{
public:
  MetaXmlUtils();
  ~MetaXmlUtils();

  MetaXmlUtils(const MetaXmlUtils&) = delete;            // Copy Constructor Not Implemented
  MetaXmlUtils(MetaXmlUtils&&) = delete;                 // Move Constructor Not Implemented
  MetaXmlUtils& operator=(const MetaXmlUtils&) = delete; // Copy Assignment Not Implemented
  MetaXmlUtils& operator=(MetaXmlUtils&&) = delete;      // Move Assignment Not Implemented

  static ITKImageProcessing_EXPORT ZeissTagsXmlSectionPtr ParseTagsSection(AbstractFilter* filter, QDomElement& tags);
  static ITKImageProcessing_EXPORT ZeissTagsXmlSectionPtr ParseScalingSection(AbstractFilter* filter, QDomElement& tags);

  static ITKImageProcessing_EXPORT int32_t GetInt32Entry(AbstractFilter* filter, ZeissTagsXmlSection* tagsSection, int idValue);

  static ITKImageProcessing_EXPORT float GetFloatEntry(AbstractFilter* filter, ZeissTagsXmlSection* tagsSection, int idValue);

  static ITKImageProcessing_EXPORT int32_t CalculatePaddingDigits(int32_t count);

protected:
private:
};
