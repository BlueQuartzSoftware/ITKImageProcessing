/* ============================================================================
 * Copyright (c) 2018 BlueQuartz Software, LLC
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
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
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
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "ITKTestBase.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/ITKImportRoboMetMontage.h"
#include "SIMPLib/CoreFilters/DataContainerWriter.h"
#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/Filtering/FilterPipeline.h"

class ITKImportRoboMetMontageTest : public ITKTestBase
{
  const QString m_DataContainerName = QString("ImageMontage");
  const QString m_CellAMName = QString("CellData");
  const QString m_MetaDataAMName = QString("MetaData");
  const QString m_RegistrationCoordinatesArrayName = QString("RegistrationCoordinates");
  const QString m_ArrayNamesArrayName = QString("ArrayNames");

  FloatVec3Type m_Origin;
  FloatVec3Type m_Resolution;

public:
  ITKImportRoboMetMontageTest() = default;

  ~ITKImportRoboMetMontageTest() override = default;

  ITKImportRoboMetMontageTest(const ITKImportRoboMetMontageTest&) = delete;            // Copy Constructor Not Implemented
  ITKImportRoboMetMontageTest(ITKImportRoboMetMontageTest&&) = delete;                 // Move Constructor
  ITKImportRoboMetMontageTest& operator=(const ITKImportRoboMetMontageTest&) = delete; // Copy Assignment Not Implemented
  ITKImportRoboMetMontageTest& operator=(ITKImportRoboMetMontageTest&&) = delete;      // Move Assignment
  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void RemoveTestFiles()
  {
#if REMOVE_TEST_FILES
    QFile::remove(UnitTest::ImportRobometMontage::OutputDREAM3DFile);
#endif
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestITKImportRoboMetMontageTest()
  {
    ITKImportRoboMetMontage::Pointer import = ITKImportRoboMetMontage::New();

    import->setInputFile(UnitTest::ImportRobometMontage::InputFile);

    import->setChangeOrigin(false);
    import->setChangeSpacing(false);
    import->setConvertToGrayScale(false);
    import->setDataContainerPath(DataArrayPath("Mosaic", "", ""));
    import->setImageDataArrayName("Image");
    import->setImageFileExtension(UnitTest::ImportRobometMontage::InputFileExtension);
    import->setImageFilePrefix(UnitTest::ImportRobometMontage::InputFilePrefix);
    import->setSliceNumber(0);

    DataContainerWriter::Pointer writer = DataContainerWriter::New();
    writer->setOutputFile(UnitTest::ImportRobometMontage::OutputDREAM3DFile);

    // Create a Pipeline and execute it.
    FilterPipeline::Pointer pipeline = FilterPipeline::New();
    pipeline->pushBack(import);
    pipeline->pushBack(writer);

    pipeline->preflightPipeline();
    // Check for errors during the execution.
    int err = pipeline->getErrorCode();
    DREAM3D_REQUIRE_EQUAL(err, 0)

    pipeline->execute();
    // Check for errors during the execution.
    err = pipeline->getErrorCode();
    DREAM3D_REQUIRE_EQUAL(err, 0)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    std::cout << "--------------------- ITKImportRoboMetMontage ---------------------" << std::endl;

    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ITKImportRoboMetMontage"));

    DREAM3D_REGISTER_TEST(TestITKImportRoboMetMontageTest());

    DREAM3D_REGISTER_TEST(RemoveTestFiles())
  }
};
