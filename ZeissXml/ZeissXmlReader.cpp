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
#include "ZeissXMLReader.h"

#include <stdio.h>
#include <iostream>


#include "ZeissImport/ZeissXml/ZeissTagMapping.h"



#define BUFFER_SIZE 1024


namespace Zeiss
{
  namespace MicroScope
  {
    const std::string AxioObserverZ1("Axio Observer.Z1"); // CMU's Microscope
    const std::string Axiovert200MMAT("Axiovert 200 M MAT"); // WPAFB Microscope
  }

  namespace XML
  {
    // Mosaic XML file
//    const std::string V3("V3");
//    const std::string V4("V4");
//    const std::string V15("V15");
//    const std::string V16("V16");
//    const std::string V71("V71");
//    const std::string V72("V72");
//    const std::string V73("V73");
//    const std::string V74("V74");
//    const std::string V87("V87");
//    const std::string V116("V116");
//    const std::string V117("V117");
//    const std::string V118("V118");
//    const std::string V119("V119");
//    const std::string Factor_0("Factor_0");

    /* These are for the Slice XML File */
//    const std::string V25("V25");
//    const std::string V26("V26");
//    const std::string V29("V29");
//    const std::string V30("V30");
//    const std::string V32("V32");
//    const std::string V33("V33");
    const std::string Root("ROOT");
    const std::string Tags("Tags");

//   const std::string Scaling("Scaling");

    const std::string V2072("V2072");
    const std::string V2121("V2121");
    const std::string V2149("V2149");

    int RootSection = 1;
//    int MosaicSection = 2;
//    int ScaleSection = 3;
    int TileSection = 4;
    int TagsSection = 5;

  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissXMLReader::ZeissXMLReader(const std::string& xmlFile) :
  _xmlFilename(xmlFile),
  _xmlParseError(0),
  _parseData(false)
{
  _xmlSection = Zeiss::XML::RootSection;
  _tileSection = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissXMLReader::~ZeissXMLReader()
{
}

// -----------------------------------------------------------------------------
// Call back struct to sort Commands and properties based on their name
// -----------------------------------------------------------------------------
template <typename T>
struct Compare: std::binary_function<const T, const T, bool>
{
  bool operator()(const T lhs, const T rhs) const
  {

    return (lhs.first < rhs.first);
  }
};


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t ZeissXMLReader::parse()
{

  if (this->_xmlFilename.empty() )
  {
    std::cout << "Input XML File must be set." << std::endl;
    _xmlParseError = -3;
    return _xmlParseError;
  }

  // Parse the XML Configuration file
  _xmlParseError = 1; // Clear the error flag
  _xmlParseError = this->_parseXMLFile();

  if (_xmlParseError < 0) { return _xmlParseError; }

  return _xmlParseError;
}


// -----------------------------------------------------------------------------
int32_t ZeissXMLReader::_parseXMLFile()
{

  // Clear any error messages that have been hanging around from previous imports
  _errorMessage.clear();
  _charData.clear();
  char buf[BUFFER_SIZE];
  // Create and initialise an instance of the parser.
  ExpatParser parser( static_cast<ExpatEvtHandler*>( this ) );
  //this->_parser = &parser;
  parser.Create(nullptr, nullptr);
  parser.EnableElementHandler();
  parser.EnableCharacterDataHandler();
  // Load the XML file.
  FILE*  fh    = fopen(_xmlFilename.c_str(), "r");
  if (nullptr == fh)
  {
    std::cout << "Could Not Open XML File for reading: " << _xmlFilename << std::endl;
    return -1;
  }
  bool   atEnd = false;
  size_t nRead;
  while (!atEnd && this->_xmlParseError >= 0)
  {
    // Read a block from the XML file and pass it to the parser
    nRead = fread(buf, 1, BUFFER_SIZE, fh);
    atEnd = feof(fh);
    parser.Parse(buf, nRead, atEnd);
  }
  fclose(fh);

  return this->_xmlParseError;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::OnStartElement(const XML_Char* name, const XML_Char** attrs)
{
  std::string currentTag(name);
  // std::cout << "currentTag: " << name << std::endl;
  if ( currentTag.compare(Zeiss::XML::Tags) == 0 )
  {
    onTags_StartTag(name, attrs);
    _xmlSection = Zeiss::XML::TagsSection;
  }
  else if (name[0] == 'V')  // The value of this tag
  {
    onVx_StartTag(name, attrs);
  }
  else if (name[0] == 'I')  // The ID of the unique Tag
  {
    onIx_StartTag(name, attrs);
  }
  else if (name[0] == 'A')
  {

  }
  else if (name[0] == 'p')
  {
    onTile_StartTag(name, attrs);
  }

//    else
//    {
//      std::cout << logTime() << "No XML Section Defined" << std::endl;
//    }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::OnEndElement(const XML_Char* name)
{
  std::string currentTag(name);
  if ( currentTag.compare(Zeiss::XML::Tags) == 0 )
  {
    onTags_EndTag(name);
  }
  else if (name[0] == 'V')  // The value of this tag
  {
    onVx_EndTag(name);
  }
  else if (name[0] == 'I')  // The ID of the unique Tag
  {
    onIx_EndTag(name);
  }
  else if (name[0] == 'p')
  {
    onTile_EndTag(name);
  }
//  else if (name[0] == 'A')
//  {
//
//  }
//  else
//  {
//    std::cout << logTime() << "No XML Section Defined" << std::endl;
//  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onTags_StartTag(const XML_Char* name, const XML_Char** attrs)
{
//  std::cout << "Starting: " << name << std::endl;
  if (m_CurrentTagSection.get() == nullptr)
  {
    m_CurrentTagSection = ZeissTagsXmlSection::New();
  }
  _xmlSection = Zeiss::XML::TagsSection;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onTags_EndTag(const XML_Char* name)
{
//   std::cout << "  Ending Section: " << name << std::endl;
  _xmlSection = Zeiss::XML::RootSection;
  if (_tileSection == 0)
  {
    m_TagsSection = m_CurrentTagSection;
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onTile_StartTag(const XML_Char* name, const XML_Char** attrs)
{
//  std::cout << "Starting: " << name << std::endl;
  _xmlSection = Zeiss::XML::TileSection;
  _tileSection = 1;
  m_CurrentTagSection = ZeissTagsXmlSection::New();

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onTile_EndTag(const XML_Char* name)
{
//  std::cout << "  Ending Section: " << name << std::endl;
  if (_tileSection == 1)
  {
    m_ImageTags.push_back(m_CurrentTagSection);
  }
  _tileSection = 0;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onVx_StartTag(const XML_Char* name, const XML_Char** attrs)
{
  this->_charData.clear();
  this->_parseData = true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onVx_EndTag(const XML_Char* name)
{
  if (this->_xmlParseError < 0) { return; }
  this->_vTagContent = this->_charData;
  this->_parseData = false; // Stop parsing character data
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onIx_StartTag(const XML_Char* name, const XML_Char** attrs)
{
  this->_charData.clear();
  this->_parseData = true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onIx_EndTag(const XML_Char* name)
{
  if (this->_xmlParseError < 0) { return; }
  int32_t idValue;
  this->extractValue(this->_charData, idValue);
  this->_parseData = false; // Stop parsing character data
  AbstractZeissMetaData::Pointer ptr = ZeissTagMapping::instance()->metaDataForId(idValue, this->_vTagContent);
  if (nullptr != ptr.get() && _vTagContent.size() > 0)
  {
    if (_xmlSection == Zeiss::XML::TagsSection)
    {
      m_CurrentTagSection->addMetaDataEntry(ptr);
    }
  }
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::OnCharacterData(const XML_Char* data, int32_t len)
{
  if (this->_parseData)
  {
    this->_charData.append(data, len);
  }
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::printStats(std::ostream& out)
{
  std::cout << "RoboMet XML Reader Statistics" << std::endl;

}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

void ZeissXMLReader::onRoot_StartTag(const XML_Char* name, const XML_Char** attrs)
{
  this->_charData.clear();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissXMLReader::onRoot_EndTag(const XML_Char* name)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ZeissXMLReader::getParseError()
{
  return _xmlParseError;
}

