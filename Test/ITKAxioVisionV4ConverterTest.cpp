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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "UnitTestSupport.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKAxioVisionV4Converter.h"
#include "ITKImageProcessingTestFileLocations.h"

class ITKAxioVisionV4ConverterTest
{

public:
  ITKAxioVisionV4ConverterTest() = default;
  ~ITKAxioVisionV4ConverterTest() = default;
  ITKAxioVisionV4ConverterTest(const ITKAxioVisionV4ConverterTest&) = delete;            // Copy Constructor
  ITKAxioVisionV4ConverterTest(ITKAxioVisionV4ConverterTest&&) = delete;                 // Move Constructor
  ITKAxioVisionV4ConverterTest& operator=(const ITKAxioVisionV4ConverterTest&) = delete; // Copy Assignment
  ITKAxioVisionV4ConverterTest& operator=(ITKAxioVisionV4ConverterTest&&) = delete;      // Move Assignment

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES
    QFile::remove(UnitTest::ITKAxioVisionV4ConverterTest::JsonOutputFile);
    QFile::remove(UnitTest::ITKAxioVisionV4ConverterTest::TextOutputFile);
#endif
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestAxioVisionV4ToJson()
  {
    ITKAxioVisionV4Converter::Pointer filter = ITKAxioVisionV4Converter::New();
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -387)

    QFileInfo fi(UnitTest::ITKAxioVisionV4ConverterTest::InputFile);
    QString nonexistantFilePath = fi.path() + QDir::separator() + "junk.xml";
    filter->setInputFile(nonexistantFilePath);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -388)

    filter->setInputFile(fi.path());
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -389)

    filter->setInputFile(UnitTest::ITKAxioVisionV4ConverterTest::InputFile);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -390)

    fi.setFile(UnitTest::ITKAxioVisionV4ConverterTest::JsonOutputFile);
    filter->setOutputFile(fi.path());
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -391)

    filter->setOutputFile(UnitTest::ITKAxioVisionV4ConverterTest::JsonOutputFile);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), 0)

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), 0)

    QFile outputFile(UnitTest::ITKAxioVisionV4ConverterTest::JsonOutputFile);
    DREAM3D_REQUIRE_EQUAL(outputFile.open(QFile::ReadOnly), true)
    QFile exemplaryOutputFile(UnitTest::ITKAxioVisionV4ConverterTest::ExemplarJsonOutputFile);
    DREAM3D_REQUIRE_EQUAL(exemplaryOutputFile.open(QFile::ReadOnly), true)

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(outputFile.readAll(), &parseError);
    DREAM3D_REQUIRE_EQUAL(parseError.error, QJsonParseError::NoError)

    QJsonDocument exemplaryDoc = QJsonDocument::fromJson(exemplaryOutputFile.readAll(), &parseError);
    DREAM3D_REQUIRE_EQUAL(parseError.error, QJsonParseError::NoError)

    QJsonObject jsonObj = doc.object();
    QJsonObject exemplaryJsonObj = exemplaryDoc.object();

    if(jsonObj != exemplaryJsonObj)
    {
      // The contents of the json file does not equal the contents of the exemplary json file
      DREAM3D_REQUIRE_EQUAL(1, 0)
    }

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestAxioVisionV4ToText()
  {
    ITKAxioVisionV4Converter::Pointer filter = ITKAxioVisionV4Converter::New();
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -387)

    QFileInfo fi(UnitTest::ITKAxioVisionV4ConverterTest::InputFile);
    QString nonexistantFilePath = fi.path() + QDir::separator() + "junk.xml";
    filter->setInputFile(nonexistantFilePath);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -388)

    filter->setInputFile(fi.path());
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -389)

    filter->setInputFile(UnitTest::ITKAxioVisionV4ConverterTest::InputFile);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -390)

    fi.setFile(UnitTest::ITKAxioVisionV4ConverterTest::TextOutputFile);
    filter->setOutputFile(fi.path());
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -391)

    filter->setOutputFile(UnitTest::ITKAxioVisionV4ConverterTest::TextOutputFile);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), 0)

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), 0)

    QFile outputFile(UnitTest::ITKAxioVisionV4ConverterTest::TextOutputFile);
    DREAM3D_REQUIRE_EQUAL(outputFile.open(QFile::ReadOnly), true)
    QFile exemplaryOutputFile(UnitTest::ITKAxioVisionV4ConverterTest::ExemplarTextOutputFile);
    DREAM3D_REQUIRE_EQUAL(exemplaryOutputFile.open(QFile::ReadOnly), true)

    QString str = outputFile.readAll();
    QString exemplaryStr = exemplaryOutputFile.readAll();

    if(str != exemplaryStr)
    {
      DREAM3D_REQUIRE_EQUAL(0, 1)
    }

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestAxioVisionV4ToXml()
  {
    ITKAxioVisionV4Converter::Pointer filter = ITKAxioVisionV4Converter::New();
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -387)

    QFileInfo fi(UnitTest::ITKAxioVisionV4ConverterTest::InputFile);
    QString nonexistantFilePath = fi.path() + QDir::separator() + "junk.xml";
    filter->setInputFile(nonexistantFilePath);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -388)

    filter->setInputFile(fi.path());
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -389)

    filter->setInputFile(UnitTest::ITKAxioVisionV4ConverterTest::InputFile);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -390)

    fi.setFile(UnitTest::ITKAxioVisionV4ConverterTest::XmlOutputFile);
    filter->setOutputFile(fi.path());
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -391)

    filter->setOutputFile(UnitTest::ITKAxioVisionV4ConverterTest::XmlOutputFile);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), 0)

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), 0)

    QFile outputFile(UnitTest::ITKAxioVisionV4ConverterTest::XmlOutputFile);
    DREAM3D_REQUIRE_EQUAL(outputFile.open(QFile::ReadOnly), true)
    QFile exemplaryOutputFile(UnitTest::ITKAxioVisionV4ConverterTest::ExemplarXmlOutputFile);
    DREAM3D_REQUIRE_EQUAL(exemplaryOutputFile.open(QFile::ReadOnly), true)

    QString str = outputFile.readAll();
    QString exemplaryStr = exemplaryOutputFile.readAll();

    if(str != exemplaryStr)
    {
      DREAM3D_REQUIRE_EQUAL(0, 1)
    }

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()()
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestAxioVisionV4ToJson())

    DREAM3D_REGISTER_TEST(TestAxioVisionV4ToText())

    DREAM3D_REGISTER_TEST(TestAxioVisionV4ToXml())

    DREAM3D_REGISTER_TEST(RemoveTestFiles())
  }

private:
};
