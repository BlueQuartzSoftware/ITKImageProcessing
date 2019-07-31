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

#include "AxioVisionV4Converter.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>

#include "SIMPLib/Common/Observable.h"

#include "ITKImageProcessing/ZeissXml/ZeissTagMapping.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AxioVisionV4Converter::AxioVisionV4Converter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AxioVisionV4Converter::~AxioVisionV4Converter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString AxioVisionV4Converter::ImportAsText(const QString& axioVisionFile, Observable* obs)
{
  QString convertedText;
  QString delimiter = "|";

  // Parse the XML file to get all the meta-data information and create all the
  // data structure that is needed.
  QFile xmlFile(axioVisionFile);
  QDomDocument domDocument;

  QTextStream stream(&convertedText);

  QString errorStr;
  int errorLine = -1;
  int errorColumn = -1;

  if(!domDocument.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn))
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
      obs->setErrorCondition(-3000, ss);
    }
    return {};
  }
  QDomElement root = domDocument.documentElement();

  // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
  QDomElement tags = root.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
  if(tags.isNull())
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("Could not find the <ROOT><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file");
      obs->setErrorCondition(-3001, ss);
    }
    return {};
  }

  ZeissTagsXmlSection::Pointer rootTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
  if(nullptr == rootTagsSection.get())
  {
    return {};
  }

  ZeissTagMapping::Pointer tagMapping = ZeissTagMapping::instance();

  ZeissTagsXmlSection::MetaDataType tagMetaData = rootTagsSection->getMetaDataMap();
  stream << "####### Start Root #######\n";
  for(const auto& entry : tagMetaData)
  {
    int32_t tagId = entry->getZeissIdTag();
    QString name = tagMapping->nameForId(tagId);
    stream << name << delimiter << entry->toString() << "\n";
  }

  stream << "####### End Root #######\n";
  stream << "\n";

  QDomElement scaling = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling);
  ZeissTagsXmlSection::Pointer scalingTagsSection = MetaXmlUtils::ParseScalingSection(nullptr, scaling);
  if(nullptr == scalingTagsSection.get())
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("The Scaling Tags Section inside the input AxioVision XML File was null.");
      obs->setErrorCondition(-3003, ss);
    }
    return {};
  }

  tagMetaData = scalingTagsSection->getMetaDataMap();
  stream << "####### Start Scaling #######\n";
  for(const auto& entry : tagMetaData)
  {
    int32_t tagId = entry->getZeissIdTag();
    QString name = tagMapping->nameForId(tagId);
    stream << name << delimiter << entry->toString() << "\n";
  }

  stream << "####### End Scaling #######\n";
  stream << "\n";

  int32_t imageCount = MetaXmlUtils::GetInt32Entry(nullptr, rootTagsSection.get(), Zeiss::MetaXML::ImageCountRawId);

  int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(imageCount);

  // Loop over every image in the _meta.xml file
  for(int p = 0; p < imageCount; p++)
  {
    // Generate the xml tag that is for this image
    QString pTag = GeneratePTag(p, imageCountPadding);

    // Drill down into the XML document....
    QDomElement photoEle = root.firstChildElement(pTag);
    if(photoEle.isNull())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Could not find the <ROOT><%1> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3004, ss);
      }
      return {};
    }
    // Get the TAGS section
    QDomElement tags = photoEle.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
    if(tags.isNull())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Could not find the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3005, ss);
      }
      return {};
    }
    // Now Parse the TAGS section
    ZeissTagsXmlSection::Pointer photoTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
    if(nullptr == photoTagsSection.get())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Error Parsing the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss AxioVision _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3006, ss);
      }
      return {};
    }
    ZeissTagsXmlSection::MetaDataType tagMetaData = photoTagsSection->getMetaDataMap();

    stream << "####### Start " << pTag << " #######\n";
    for(const auto& entry : tagMetaData)
    {
      int32_t tagId = entry->getZeissIdTag();
      QString name = tagMapping->nameForId(tagId);
      stream << name << delimiter << entry->toString() << "\n";
    }

    stream << "####### End " << pTag << " #######\n";
    stream << "\n";
  }

  return convertedText;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QDomDocument AxioVisionV4Converter::ImportAsXML(const QString& axioVisionFile, Observable* obs)
{
  // Parse the XML file to get all the meta-data information and create all the
  // data structure that is needed.
  QFile xmlFile(axioVisionFile);
  QDomDocument domDocument;

  QString errorStr;
  int errorLine = -1;
  int errorColumn = -1;

  if(!domDocument.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn))
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
      obs->setErrorCondition(-3000, ss);
    }
    return {};
  }
  QDomElement root = domDocument.documentElement();

  // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
  QDomElement tags = root.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
  if(tags.isNull())
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("Could not find the <ROOT><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file");
      obs->setErrorCondition(-3001, ss);
    }
    return {};
  }

  bool ok = false;
  QDomElement countEle = tags.namedItem(ITKImageProcessingConstants::Xml::Count).toElement();
  int count = countEle.text().toInt(&ok, 10);
  if(!ok)
  {
    QString ss = QObject::tr("Error Parsing 'Count' Tag in Root 'Tags' DOM element");
    if(obs != nullptr)
    {
      obs->setErrorCondition(-70001, ss);
    }
    return {};
  }

  ZeissTagsXmlSection::Pointer rootTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
  if(nullptr == rootTagsSection.get())
  {
    return {};
  }

  ZeissTagMapping::Pointer tagMapping = ZeissTagMapping::instance();

  for(int c = 0; c < count; c++)
  {
    QString Vx = QString("V%1").arg(c);
    QString Ix = QString("I%1").arg(c);
    QString Ax = QString("A%1").arg(c);

    QDomElement vxEle = tags.namedItem(Vx).toElement();
    QDomElement ixEle = tags.namedItem(Ix).toElement();
    QDomElement axEle = tags.namedItem(Ax).toElement();

    qint32 idValue = ixEle.text().toInt(&ok, 10);
    QString value = vxEle.text();

    QString key = tagMapping->nameForId(idValue);
    if(!key.isEmpty())
    {
      QDomElement newVxEle = domDocument.createElement(key);
      QDomText newVxEleText = domDocument.createTextNode(value);
      newVxEle.appendChild(newVxEleText);
      tags.insertBefore(newVxEle, ixEle);
    }

    tags.removeChild(vxEle);
    tags.removeChild(ixEle);
    tags.removeChild(axEle);
  }

  //  QDomElement scaling = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling);
  //  ZeissTagsXmlSection::Pointer scalingTagsSection = MetaXmlUtils::ParseScalingSection(nullptr, scaling);
  //  if(nullptr == scalingTagsSection.get())
  //  {
  //    if(obs != nullptr)
  //    {
  //      QString ss = QObject::tr("The Scaling Tags Section inside the input AxioVision XML File was null.");
  //      obs->setErrorCondition(-3003, ss);
  //    }
  //    return {};
  //  }

  //  auto tagMetaData = scalingTagsSection->getMetaDataMap();
  //  for(const auto& entry : tagMetaData)
  //  {
  //    int32_t tagId = entry->getZeissIdTag();
  //    QString name = tagMapping->nameForId(tagId);
  //    stream << name << delimiter << entry->toString() << "\n";
  //  }

  //  stream << "####### End Scaling #######\n";
  //  stream << "\n";

  int32_t imageCount = MetaXmlUtils::GetInt32Entry(nullptr, rootTagsSection.get(), Zeiss::MetaXML::ImageCountRawId);

  int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(imageCount);

  // Loop over every image in the _meta.xml file
  for(int p = 0; p < imageCount; p++)
  {
    QString pTag = GeneratePTag(p, imageCountPadding);

    // Drill down into the XML document....
    QDomElement photoEle = root.firstChildElement(pTag);
    if(photoEle.isNull())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Could not find the <ROOT><%1> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3004, ss);
      }
      return {};
    }
    // Get the TAGS section
    QDomElement tags = photoEle.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
    if(tags.isNull())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Could not find the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3005, ss);
      }
      return {};
    }
    // Now Parse the TAGS section
    ZeissTagsXmlSection::Pointer photoTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
    if(nullptr == photoTagsSection.get())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Error Parsing the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss AxioVision _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3006, ss);
      }
      return {};
    }

    bool ok = false;
    QDomElement countEle = tags.namedItem(ITKImageProcessingConstants::Xml::Count).toElement();
    int count = countEle.text().toInt(&ok, 10);
    if(!ok)
    {
      QString ss = QObject::tr("Error Parsing 'Count' Tag in %1 'Tags' DOM element").arg(pTag);
      if(obs != nullptr)
      {
        obs->setErrorCondition(-70001, ss);
      }
      return {};
    }

    ZeissTagsXmlSection::Pointer rootTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
    if(nullptr == rootTagsSection.get())
    {
      return {};
    }

    for(int c = 0; c < count; c++)
    {
      QString Vx = QString("V%1").arg(c);
      QString Ix = QString("I%1").arg(c);
      QString Ax = QString("A%1").arg(c);

      QDomElement vxEle = tags.namedItem(Vx).toElement();
      QDomElement ixEle = tags.namedItem(Ix).toElement();
      QDomElement axEle = tags.namedItem(Ax).toElement();

      qint32 idValue = ixEle.text().toInt(&ok, 10);
      QString value = vxEle.text();

      QString key = tagMapping->nameForId(idValue);
      if(!key.isEmpty())
      {
        QDomElement newVxEle = domDocument.createElement(key);
        QDomText newVxEleText = domDocument.createTextNode(value);
        newVxEle.appendChild(newVxEleText);
        tags.insertBefore(newVxEle, ixEle);
      }

      tags.removeChild(vxEle);
      tags.removeChild(ixEle);
      tags.removeChild(axEle);
    }
  }

  return domDocument;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4Converter::ConvertToHumanReadableXMLFile(const QString& axioVisionFile, const QString& outputXmlFile, Observable* obs)
{
  QDomDocument doc = ImportAsXML(axioVisionFile, obs);

  QFile outputFile(outputXmlFile);
  QFileInfo fi(outputXmlFile);
  if(!outputFile.open(QIODevice::WriteOnly))
  {
    if(obs != nullptr)
    {
      QString ss = tr("Could not open the output XML file '%1' for writing.").arg(fi.fileName());
      obs->setErrorCondition(-3005, ss);
    }
    return;
  }

  QTextStream stream(&outputFile);
  stream << doc.toString();
  outputFile.close();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4Converter::ConvertToTextFile(const QString& axioVisionFile, const QString& outputTextFile, Observable* obs)
{
  QString text = ImportAsText(axioVisionFile, obs);
  if(text.isEmpty())
  {
    return;
  }

  QFile outputFile(outputTextFile);
  QFileInfo fi(outputTextFile);
  if(!outputFile.open(QFile::WriteOnly))
  {
    if(obs != nullptr)
    {
      QString ss = tr("Could not open the output text file '%1' for writing.").arg(fi.fileName());
      obs->setErrorCondition(-3005, ss);
    }
    return;
  }

  outputFile.write(text.toUtf8());
  outputFile.close();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QJsonObject AxioVisionV4Converter::ImportAsJson(const QString& axioVisionFile, Observable* obs)
{
  QJsonObject rootObj;

  // Parse the XML file to get all the meta-data information and create all the
  // data structure that is needed.
  QFile xmlFile(axioVisionFile);
  QDomDocument domDocument;

  QString errorStr;
  int errorLine = -1;
  int errorColumn = -1;

  if(!domDocument.setContent(&xmlFile, true, &errorStr, &errorLine, &errorColumn))
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
      obs->setErrorCondition(-3000, ss);
    }
    return {};
  }
  QDomElement root = domDocument.documentElement();

  // First parse the <ROOT><Tags> section to get the values of how many images we are going to have
  QDomElement tags = root.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
  if(tags.isNull())
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("Could not find the <ROOT><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file");
      obs->setErrorCondition(-3001, ss);
    }
    return {};
  }

  ZeissTagsXmlSection::Pointer rootTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
  if(nullptr == rootTagsSection.get())
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("The Root Tags Section inside the input AxioVision XML File was null.");
      obs->setErrorCondition(-3002, ss);
    }
    return {};
  }

  ZeissTagMapping::Pointer tagMapping = ZeissTagMapping::instance();

  ZeissTagsXmlSection::MetaDataType tagMetaData = rootTagsSection->getMetaDataMap();
  QJsonObject tagObj;
  for(const auto& entry : tagMetaData)
  {
    int32_t tagId = entry->getZeissIdTag();
    QString name = tagMapping->nameForId(tagId);
    tagObj[name] = entry->toString();
  }
  rootObj["Root"] = tagObj;

  QDomElement scaling = root.firstChildElement(ITKImageProcessingConstants::Xml::Scaling);
  ZeissTagsXmlSection::Pointer scalingTagsSection = MetaXmlUtils::ParseScalingSection(nullptr, scaling);
  if(nullptr == scalingTagsSection.get())
  {
    if(obs != nullptr)
    {
      QString ss = QObject::tr("The Scaling Tags Section inside the input AxioVision XML File was null.");
      obs->setErrorCondition(-3003, ss);
    }
    return {};
  }

  tagMetaData = scalingTagsSection->getMetaDataMap();
  QJsonObject scalingObj;
  for(const auto& entry : tagMetaData)
  {
    int32_t tagId = entry->getZeissIdTag();
    QString name = tagMapping->nameForId(tagId);
    scalingObj[name] = entry->toString();
  }
  rootObj["Scaling"] = scalingObj;

  int32_t imageCount = MetaXmlUtils::GetInt32Entry(nullptr, rootTagsSection.get(), Zeiss::MetaXML::ImageCountRawId);

  int imageCountPadding = MetaXmlUtils::CalculatePaddingDigits(imageCount);

  // Loop over every image in the _meta.xml file
  for(int p = 0; p < imageCount; p++)
  {
    // Generate the xml tag that is for this image
    QString pTag = GeneratePTag(p, imageCountPadding);

    // Drill down into the XML document....
    QDomElement photoEle = root.firstChildElement(pTag);
    if(photoEle.isNull())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Could not find the <ROOT><%1> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3004, ss);
      }
      return {};
    }
    // Get the TAGS section
    QDomElement tags = photoEle.firstChildElement(ITKImageProcessingConstants::Xml::Tags);
    if(tags.isNull())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Could not find the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3005, ss);
      }
      return {};
    }
    // Now Parse the TAGS section
    ZeissTagsXmlSection::Pointer photoTagsSection = MetaXmlUtils::ParseTagsSection(nullptr, tags);
    if(nullptr == photoTagsSection.get())
    {
      if(obs != nullptr)
      {
        QString ss = QObject::tr("Error Parsing the <ROOT><%1><Tags> element. Aborting Parsing. Is the file a Zeiss AxioVision _meta.xml file").arg(pTag);
        obs->setErrorCondition(-3006, ss);
      }
      return {};
    }
    ZeissTagsXmlSection::MetaDataType tagMetaData = photoTagsSection->getMetaDataMap();

    QJsonObject pTagObj;
    for(const auto& entry : tagMetaData)
    {
      int32_t tagId = entry->getZeissIdTag();
      QString name = tagMapping->nameForId(tagId);
      pTagObj[name] = entry->toString();
    }
    rootObj[pTag] = pTagObj;
  }

  return rootObj;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AxioVisionV4Converter::ConvertToJsonFile(const QString& axioVisionFile, const QString& outputJsonFile, Observable* obs)
{
  QJsonObject jsonObj = ImportAsJson(axioVisionFile, obs);
  if(jsonObj.isEmpty())
  {
    return;
  }

  QJsonDocument doc(jsonObj);
  QFile outputFile(outputJsonFile);
  QFileInfo fi(outputJsonFile);
  if(!outputFile.open(QFile::WriteOnly))
  {
    if(obs != nullptr)
    {
      QString ss = tr("Could not open the output JSON File '%1' for writing.").arg(fi.fileName());
      obs->setErrorCondition(-3005, ss);
    }
    return;
  }

  outputFile.write(doc.toJson());
  outputFile.close();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString AxioVisionV4Converter::GeneratePTag(int imageNumber, int padding)
{
  // Generate the xml tag that is for this image
  QString pTag;
  QTextStream out(&pTag);
  out << "p";
  out.setFieldWidth(padding);
  out.setFieldAlignment(QTextStream::AlignRight);
  out.setPadChar('0');
  out << imageNumber;
  return pTag;
}
