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

#error THIS FILE IS NO LONGER USED. WE are now using the QtXML framework instead of Expat
//-- C++ Includes
#include <memory>

#include <sstream>
#include <string>
#include <vector>

#include "SIMPLib/SIMPLib.h"

#include "ITKImageProcessing/ITKImageProcessingDLLExport.h"
#include "ZeissTagsXmlSection.h"

/**
 * @class ZeissXMLReader ZeissXMLReader.h Importer/ZeissXMLReader.h
 * @brief  This class reads the xml file that is produced by the Zeiss software
 * during an image export.
 * @author Mike Jackson
 * @date Oct 2007
 * @version $Revision: 1.1 $
 */
class ITKImageProcessing_EXPORT ZeissXMLReader : public ExpatEvtHandler
{
public:
  using Self = ZeissXMLReader;
  using Pointer = std::shared_ptr<Self>;
  using ConstPointer = std::shared_ptr<const Self>;
  using WeakPointer = std::weak_ptr<Self>;
  using ConstWeakPointer = std::weak_ptr<const Self>;
  static Pointer NullPointer();

  /**
   * @brief Returns the name of the class for ZeissXMLReader
   */
  QString getNameOfClass() const override;
  /**
   * @brief Returns the name of the class for ZeissXMLReader
   */
  static QString ClassName();

  /**
   * @brief Creates a new Object which is wrapped in a shared_ptr
   * @param filename The filename of the xml file to read
   * @slice The slice number that the xml file corresponds to
   * @return Shared Pointer
   */
  static Pointer New(const std::string& filename)
  {
    Pointer ptr(new ZeissXMLReader(filename));
    return ptr;
  }

  virtual ~ZeissXMLReader();

  /**
   * @brief Sets the input file to parse
   * @param inputFile The path to an xml file
   */
  void setXMLInputFile(const std::string& inputFile)
  {
    this->_xmlFilename = inputFile;
  }

  /**
   * @brief returns the XML File that this importer is using
   */
  std::string getXMLInputFile()
  {
    return this->_xmlFilename;
  }

  /**
   * @brief Reads the Data Model from an XML File/
   * @return Error: Negative is error Condition
   */
  int32_t parse();

  // -----------------------------------------------------------------------------
  //  ExpatEvtHandler Implementation
  // -----------------------------------------------------------------------------
  // Over ride from ExpatEvtHandler class
  void OnStartElement(const XML_Char* name, const XML_Char** attrs);

  // Over ride from ExpatEvtHandler class
  void OnEndElement(const XML_Char* name);

  // Over ride from ExpatEvtHandler class
  void OnCharacterData(const XML_Char* data, int32_t len);

  /**
   * @brief Returns the current parser error.
   */
  int32_t getParseError();

  /**
   * @brief Print some statistics about this parser and the values it extracted
   * from the xml file
   * @param out A std::ostream Object reference
   */
  void printStats(std::ostream& out);

  /**
   * @brief Setter property for TagsSection;
   */
  void setTagsSection;
  (const ZeissTagsXmlSection::Pointer& value);
  /**
   * @brief Getter property for TagsSection;
   * @return Value of TagsSection;
   */
  ZeissTagsXmlSection::Pointer getTagsSection;
  () const;

  /**
   * @brief Setter property for ImageTags;
   */
  void setImageTags;
  (const std::vector<ZeissTagsXmlSection::Pointer>& value);
  /**
   * @brief Getter property for ImageTags;
   * @return Value of ImageTags;
   */
  std::vector<ZeissTagsXmlSection::Pointer> getImageTags;
  () const;

protected:
  ZeissXMLReader(const std::string& xmlFile);

private:
  ZeissTagsXmlSection::Pointer m_TagsSection;
  = {};
  std::vector<ZeissTagsXmlSection::Pointer> m_ImageTags;
  = {};

  std::string _xmlFilename;
  int32_t _xmlParseError;
  ExpatParser* _parser;
  std::string _errorMessage;
  std::string _charData;
  bool _parseData;

  int _xmlSection;
  std::string _vTagContent;
  int _tileSection;

  ZeissTagsXmlSection::Pointer m_CurrentTagSection;

  ZeissXMLReader(const ZeissXMLReader&); // Not Implemented
  void operator=(const ZeissXMLReader&); // Not Implemented

  /**
   * @brief Runs the actual parsing of the XML File
   * @return Error condition
   */
  int32_t _parseXMLFile();

  // -----------------------------------------------------------------------------
  //  Tags that deliniate sections of the XML file:
  // -----------------------------------------------------------------------------
  // <ROOT>
  void onRoot_StartTag(const XML_Char* name, const XML_Char** attrs);
  void onRoot_EndTag(const XML_Char* name);

  // <p00>
  void onTags_StartTag(const XML_Char* name, const XML_Char** attrs);
  void onTags_EndTag(const XML_Char* name);

  void onTile_StartTag(const XML_Char* name, const XML_Char** attrs);
  void onTile_EndTag(const XML_Char* name);

  // <V00>
  void onVx_StartTag(const XML_Char* name, const XML_Char** attrs);
  void onVx_EndTag(const XML_Char* name);

  // <I00>
  void onIx_StartTag(const XML_Char* name, const XML_Char** attrs);
  void onIx_EndTag(const XML_Char* name);

  /**
   * @brief This will extract a single value from Character Data
   * @param data The Character data to extract the value from
   * @param value The variable to store the value into
   */
  template <typename T>
  void extractValue(const std::string& data, T& value)
  {

    std::istringstream istream(data);
    while(istream.good())
    {
      istream >> value;
    }
  }
};
