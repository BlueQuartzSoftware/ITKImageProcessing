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

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/DataArray.hpp"
#include "SIMPLib/SIMPLib.h"

#include "UnitTestSupport.hpp"

#include "ITKImageProcessing/ITKImageProcessingFilters/AxioVisionV4ToJson.h"
#include "ITKImageProcessingTestFileLocations.h"

class AxioVisionV4ToJsonTest
{

public:
  AxioVisionV4ToJsonTest() = default;
  ~AxioVisionV4ToJsonTest() = default;
  AxioVisionV4ToJsonTest(const AxioVisionV4ToJsonTest&) = delete;            // Copy Constructor
  AxioVisionV4ToJsonTest(AxioVisionV4ToJsonTest&&) = delete;                 // Move Constructor
  AxioVisionV4ToJsonTest& operator=(const AxioVisionV4ToJsonTest&) = delete; // Copy Assignment
  AxioVisionV4ToJsonTest& operator=(AxioVisionV4ToJsonTest&&) = delete;      // Move Assignment

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES
    QFile::remove(UnitTest::AxioVisionV4ToJsonTest::OutputFile);
#endif
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  int TestAxioVisionV4ToJsonTest()
  {
    AxioVisionV4ToJson::Pointer filter = AxioVisionV4ToJson::New();
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -387)

    QFileInfo fi(UnitTest::AxioVisionV4ToJsonTest::InputFile);
    QString nonexistantFilePath = fi.path() + QDir::separator() + "junk.xml";
    filter->setInputFile(nonexistantFilePath);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -388)

    filter->setInputFile(fi.path());
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -389)

    filter->setInputFile(UnitTest::AxioVisionV4ToJsonTest::InputFile);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -390)

    fi.setFile(UnitTest::AxioVisionV4ToJsonTest::OutputFile);
    filter->setOutputFile(fi.path());
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), -391)

    filter->setOutputFile(UnitTest::AxioVisionV4ToJsonTest::OutputFile);
    filter->preflight();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), 0)

    filter->execute();
    DREAM3D_REQUIRE_EQUAL(filter->getErrorCode(), 0)

    QFile outputFile(UnitTest::AxioVisionV4ToJsonTest::OutputFile);
    DREAM3D_REQUIRE_EQUAL(outputFile.open(QFile::ReadOnly), true)
    QFile exemplaryOutputFile(UnitTest::AxioVisionV4ToJsonTest::ExemplarOutputFile);
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
  void operator()()
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(TestAxioVisionV4ToJsonTest())

    DREAM3D_REGISTER_TEST(RemoveTestFiles())
  }

private:
};
