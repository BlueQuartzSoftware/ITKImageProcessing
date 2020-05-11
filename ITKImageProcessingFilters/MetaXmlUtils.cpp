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
#include "MetaXmlUtils.h"

#include "SIMPLib/Filtering/AbstractFilter.h"

#include "ITKImageProcessing/ZeissXml/ZeissTagsXmlSection.h"
#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"
#include "ITKImageProcessing/ZeissXml/ZeissTagMapping.h"
#include "ITKImageProcessing/ZeissXml/ZeissMetaEntry.h"

MetaXmlUtils::MetaXmlUtils() = default;

MetaXmlUtils::~MetaXmlUtils() = default;

// -----------------------------------------------------------------------------
ZeissTagsXmlSectionPtr MetaXmlUtils::ParseTagsSection(AbstractFilter* filter, QDomElement& tags)
{
  int count = -1;
  bool ok = false;

  QDomElement countEle = tags.namedItem(ITKImageProcessingConstants::Xml::Count).toElement();

  count = countEle.text().toInt(&ok, 10);
  if(!ok)
  {
    QString ss = QObject::tr("Error Parsing 'Count' Tag in Root 'Tags' DOM element");
    if(filter != nullptr)
    {
      filter->setErrorCondition(-70001, ss);
    }
    else
    {
      qDebug() << ss;
    }
    return ZeissTagsXmlSection::NullPointer();
  }

  ZeissTagsXmlSection::Pointer rootTagsSection = ZeissTagsXmlSection::New();
  ZeissTagMapping::Pointer tagMapping = ZeissTagMapping::instance();

  std::set<int> unknownTags;

  for(int c = 0; c < count; c++)
  {
    QString Vx = QString("V%1").arg(c);
    QString Ix = QString("I%1").arg(c);

    QDomElement vxEle = tags.namedItem(Vx).toElement();
    QDomElement ixEle = tags.namedItem(Ix).toElement();

    qint32 idValue = ixEle.text().toInt(&ok, 10);
    AbstractZeissMetaData::Pointer ptr = ZeissTagMapping::instance()->metaDataForId(idValue, vxEle.text());
    if(nullptr != ptr.get() && vxEle.text().size() > 0)
    {
      rootTagsSection->addMetaDataEntry(ptr);
    }
  }
  return rootTagsSection;
}

// -----------------------------------------------------------------------------
ZeissTagsXmlSectionPtr MetaXmlUtils::ParseScalingSection(AbstractFilter* filter, QDomElement& tags)
{
  int count = -1;

  QDomElement keyElement = tags.firstChildElement(ITKImageProcessingConstants::Xml::Key);

  ZeissTagsXmlSection::Pointer rootTagsSection = ZeissTagsXmlSection::New();
  ZeissTagMapping::Pointer tagMapping = ZeissTagMapping::instance();

  std::set<int> unknownTags;
  count = 3;
  int32_t fakeId = 20000;
  for(int c = 0; c < count; c++)
  {
    QString factor = ITKImageProcessingConstants::Xml::Factor + QString("_%1").arg(c);
    QString typeX = ITKImageProcessingConstants::Xml::Type + QString("_%1").arg(c);
    QString unit = ITKImageProcessingConstants::Xml::Unit + QString("_%1").arg(c);
    QString origin = ITKImageProcessingConstants::Xml::Origin + QString("_%1").arg(c);
    QString angle = ITKImageProcessingConstants::Xml::Angle + QString("_%1").arg(c);

    QDomElement element = tags.firstChildElement(factor);
    AbstractZeissMetaData::Pointer fPtr = FloatZeissMetaEntry::New(fakeId++, element.text());
    rootTagsSection->addMetaDataEntry(fPtr);

    element = tags.firstChildElement(typeX);
    AbstractZeissMetaData::Pointer iPtr = Int32ZeissMetaEntry::New(fakeId++, element.text());
    rootTagsSection->addMetaDataEntry(iPtr);

    element = tags.firstChildElement(unit);
    AbstractZeissMetaData::Pointer sPtr = StringZeissMetaEntry::New(fakeId++, element.text());
    rootTagsSection->addMetaDataEntry(sPtr);

    element = tags.firstChildElement(origin);
    fPtr = FloatZeissMetaEntry::New(fakeId++, element.text());
    rootTagsSection->addMetaDataEntry(fPtr);

    element = tags.firstChildElement(angle);
    fPtr = FloatZeissMetaEntry::New(fakeId++, element.text());
    rootTagsSection->addMetaDataEntry(fPtr);
  }
  return rootTagsSection;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MetaXmlUtils::GetInt32Entry(AbstractFilter* filter, ZeissTagsXmlSection* tagsSection, int idValue)
{
  AbstractZeissMetaData::Pointer ptr = tagsSection->getEntry(idValue);
  if(nullptr == ptr)
  {

    QString msg;
    QTextStream errStrm(&msg);
    errStrm << "AxioVision Import: XML Section for '" << ZeissTagMapping::instance()->nameForId(idValue) << "' was not found.";
    if(filter != nullptr)
    {
      filter->setErrorCondition(-70600, msg);
    }
    else
    {
      qDebug() << msg;
    }
    return 0;
  }

  Int32ZeissMetaEntry::Pointer valuePtr = ZeissMetaEntry::convert<Int32ZeissMetaEntry>(ptr);
  if(nullptr == valuePtr)
  {
    QString msg;
    QTextStream errStrm(&msg);
    errStrm << "AxioVision Import: Could not convert '" << ZeissTagMapping::instance()->nameForId(idValue) << "' tag to an integer.";
    if(filter != nullptr)
    {
      filter->setErrorCondition(-70601, msg);
    }
    else
    {
      qDebug() << msg;
    }
    return 0;
  }

  return valuePtr->getValue();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
float MetaXmlUtils::GetFloatEntry(AbstractFilter* filter, ZeissTagsXmlSection* tagsSection, int idValue)
{
  AbstractZeissMetaData::Pointer ptr = tagsSection->getEntry(idValue);
  if(nullptr == ptr)
  {
    QString msg;
    QTextStream errStrm(&msg);
    errStrm << "AxioVision Import: XML Section for '" << ZeissTagMapping::instance()->nameForId(idValue) << "' was not found.";
    if(filter != nullptr)
    {
      filter->setErrorCondition(-70602, msg);
    }
    else
    {
      qDebug() << msg;
    }
    return std::numeric_limits<float>::quiet_NaN();
  }

  FloatZeissMetaEntry::Pointer valuePtr = ZeissMetaEntry::convert<FloatZeissMetaEntry>(ptr);
  if(nullptr == valuePtr)
  {
    QString msg;
    QTextStream errStrm(&msg);
    errStrm << "AxioVision Import: Could not convert '" << ZeissTagMapping::instance()->nameForId(idValue) << "' tag to a float.";
    if(filter != nullptr)
    {
      filter->setErrorCondition(-70603, msg);
    }
    else
    {
      qDebug() << msg;
    }
    return std::numeric_limits<float>::quiet_NaN();
  }

  return valuePtr->getValue();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t MetaXmlUtils::CalculatePaddingDigits(int32_t count)
{
  int zeroPadding = 0;
  if(count > 0)
  {
    zeroPadding++;
  }
  if(count > 9)
  {
    zeroPadding++;
  }
  if(count > 99)
  {
    zeroPadding++;
  }
  if(count > 999)
  {
    zeroPadding++;
  }
  if(count > 9999)
  {
    zeroPadding++;
  }
  return zeroPadding;
}
