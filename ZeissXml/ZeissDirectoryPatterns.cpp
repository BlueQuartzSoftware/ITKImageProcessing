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
 //                           F33615-03-C-5220
 //
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "ZeissDirectoryPatterns.h"
#include <sstream>
#include <iomanip>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissDirectoryPatterns::ZeissDirectoryPatterns()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ZeissDirectoryPatterns::~ZeissDirectoryPatterns() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string ZeissDirectoryPatterns::generateZeissDirectory(int slice)
{
  std::stringstream ss;
  ss << m_Prefix << slice << m_Suffix << Zeiss::DirectoryPatterns::Dot << m_Extension << Zeiss::DirectoryPatterns::_Files;
  return ss.str();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string ZeissDirectoryPatterns::generateZeissDirectory(const std::string& slice)
{
  std::stringstream ss;
  ss << m_Prefix << slice << m_Suffix << Zeiss::DirectoryPatterns::Dot << m_Extension << Zeiss::DirectoryPatterns::_Files;
  return ss.str();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string ZeissDirectoryPatterns::generateZeissMetaXMLName(int slice)
{
  std::stringstream ss;
  ss <<  generateZeissDirectory(slice) << MXADir::Separator << Zeiss::DirectoryPatterns::MetaXML;
  return ss.str();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string ZeissDirectoryPatterns::generateZeissMetaXMLName(const std::string& slice)
{
  std::stringstream ss;
  ss <<  generateZeissDirectory(slice) << MXADir::Separator << Zeiss::DirectoryPatterns::MetaXML;
  return ss.str();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::string ZeissDirectoryPatterns::generateZeissTileName(int slice, int numberWidth, int tileIndex)
{
  std::stringstream ss;
  ss.setf(std::ios::fixed);
  ss.fill('0');
  int32_t width = numberWidth;
  ss << m_Prefix << slice << m_Suffix << Zeiss::DirectoryPatterns::_P << std::setw(width) << tileIndex << Zeiss::DirectoryPatterns::Dot << m_Extension;
  return ss.str();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ZeissDirectoryPatterns::extractSlice(const std::string& dirName)
{
  std::string::size_type pos = m_Prefix.length();
  const char* data = dirName.data();
  // Now walk past any slice number
  while (data[pos] >= '0' && data[pos] <= '9')
  {
    ++pos;
  }

  std::string::size_type sliceLen = pos - m_Prefix.length();
  std::string sliceStr = dirName.substr(m_Prefix.length(),  sliceLen);
  int sliceVal = -1;
  bool ok = StringUtils::stringToNum<int>(sliceVal, sliceStr);
  if (!ok)
  {
    sliceVal = -1;
  }

  return sliceVal;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ZeissDirectoryPatterns::parseDocumentName(const std::string& guessedPrefix,
                                              const std::string& zeissDocumentName)
{
  //std::cout << "ZeissDirectoryPatterns::parseDocumentName: " << zeissDocumentName << std::endl;
  m_Extension = MXAFileInfo::extension(zeissDocumentName);

  std::string::size_type m_PrefixPos = zeissDocumentName.find(guessedPrefix);
  if (m_PrefixPos != std::string::npos  && m_PrefixPos == 0)
  {
    std::string::size_type pos = guessedPrefix.size() - m_PrefixPos;
    // Start looking for numeric characters in case the guessed prefix is not correct
    const char* data = zeissDocumentName.data();
    while (data[pos] < '0' || data[pos] > '9' )
    {
      ++pos;
    }
    m_Prefix = zeissDocumentName.substr(m_PrefixPos, pos);

    // Now walk past any slice number
    while (data[pos] >= '0' && data[pos] <= '9')
    {
      ++pos;
    }

    // Now find the '.' character
    std::string::size_type dotPos = zeissDocumentName.find_first_of('.', pos);
    m_Suffix = zeissDocumentName.substr(pos, dotPos - pos);
  }
  else
  {
    return -1;
  }
  return 1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ZeissDirectoryPatterns::print(std::ostream& ostream)
{
  ostream << "Prefix: " << getPrefix() << std::endl;
  ostream << "Suffix: " << getSuffix() << std::endl;
  ostream << "Extension: " << getExtension() << std::endl;
}

