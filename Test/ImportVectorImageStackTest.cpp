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
 * The code contained herein was partially funded by the following contracts:
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "ITKTestBase.h"

#include <QtCore/QFileInfo>

#include "SIMPLib/FilterParameters/FileListInfoFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/Filtering/FilterPipeline.h"

#include "ITKImageProcessing/FilterParameters/ImportVectorImageStackFilterParameter.h"

class ImportVectorImageStackTest : public ITKTestBase
{
  const QString m_DataContainerName = QString("ImageMontage");
  const QString m_CellAMName = QString("CellData");
  const QString m_VectorDataArrayName = QString("VectorData");

  FloatVec3Type m_Origin;
  FloatVec3Type m_Spacing;

public:
  ImportVectorImageStackTest() = default;

  ~ImportVectorImageStackTest() override = default;

  ImportVectorImageStackTest(const ImportVectorImageStackTest&) = delete;            // Copy Constructor Not Implemented
  ImportVectorImageStackTest(ImportVectorImageStackTest&&) = delete;                 // Move Constructor
  ImportVectorImageStackTest& operator=(const ImportVectorImageStackTest&) = delete; // Copy Assignment Not Implemented
  ImportVectorImageStackTest& operator=(ImportVectorImageStackTest&&) = delete;      // Move Assignment

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void TestImportVectorImageStackTest()
  {
    // We need to generate some test files....
    QString sourceImage = QString("%1/VectorImageImport/100.tif").arg(UnitTest::ITKImageProcessingDataDir);

    for(int i = 0; i < 6; i++)
    {
      for(int j = 0; j < 10; j++)
      {
        QString outImage = QString("%1/ImportVectorImage_%2-%3.tif").arg(UnitTest::TestTempDir).arg(i).arg(j);
        QFileInfo fi(outImage);
        if(!fi.exists())
        {
          QFile inImage(sourceImage);
          bool didCopy = inImage.copy(outImage);
          DREAM3D_REQUIRE_EQUAL(didCopy, true)
        }
        FilesToRemove << outImage;
      }
    }

    // Now build and execute a pipeline

    QVariant var;
    bool propWasSet = false;
    FilterManager* fm = FilterManager::Instance();

    QString filtName = "ImportVectorImageStack";
    IFilterFactory::Pointer filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_VALID_POINTER(filterFactory.get());
    AbstractFilter::Pointer import = filterFactory->create();
    import->getFilterParameters();

    var.setValue(DataArrayPath(m_DataContainerName, "", ""));
    propWasSet = import->setProperty("DataContainerName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    var.setValue(m_CellAMName);
    propWasSet = import->setProperty("CellAttributeMatrixName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    var.setValue(m_VectorDataArrayName);
    propWasSet = import->setProperty("VectorDataArrayName", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    VectorFileListInfo_t fli;
    fli.PaddingDigits = 1;
    fli.Ordering = 0;
    fli.StartIndex = 0;
    fli.EndIndex = 5;
    fli.IncrementIndex = 1;
    fli.InputPath = UnitTest::TestTempDir;
    fli.FilePrefix = "ImportVectorImage_";
    fli.FileSuffix = "";
    fli.FileExtension = "tif";
    fli.StartComponent = 0;
    fli.EndComponent = 9;
    fli.Separator = "-";

    var.setValue(fli);
    propWasSet = import->setProperty("InputFileListInfo", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    m_Spacing[0] = 0.25f;
    m_Spacing[1] = 0.50f;
    m_Spacing[2] = 1.25f;
    var.setValue(m_Spacing);
    propWasSet = import->setProperty("Spacing", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    m_Origin[0] = 0.0f;
    m_Origin[1] = 0.0f;
    m_Origin[2] = 0.0f;
    var.setValue(m_Origin);
    propWasSet = import->setProperty("Origin", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    // Create a DataContainerWriter Filter instance
    filtName = "DataContainerWriter";
    filterFactory = fm->getFactoryFromClassName(filtName);
    DREAM3D_REQUIRE_VALID_POINTER(filterFactory.get());
    AbstractFilter::Pointer writer = filterFactory->create();

    QString output = QString("%1/ImportVectorImageStackTest.dream3d").arg(UnitTest::TestTempDir);
    FilesToRemove << output;

    var.setValue(output);
    propWasSet = writer->setProperty("OutputFile", var);
    DREAM3D_REQUIRE_EQUAL(propWasSet, true);

    Observer obs;
    // Create a Pipeline and execute it.
    FilterPipeline::Pointer pipeline = FilterPipeline::New();
    pipeline->addMessageReceiver(&obs);
    pipeline->pushBack(import);
    pipeline->pushBack(writer);
    pipeline->execute();

    // Check for errors during the execution.
    int err = pipeline->getErrorCode();
    DREAM3D_REQUIRE_EQUAL(err, 0)
  }

  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    std::cout << "# --------- Starting ImportVectorImageStack -------" << std::endl;
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST(this->TestFilterAvailability("ImportVectorImageStack"));

    DREAM3D_REGISTER_TEST(TestImportVectorImageStackTest());

    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST(this->RemoveTestFiles())
    }
  }
};
