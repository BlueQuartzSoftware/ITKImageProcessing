/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
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

#include "ITKImageReader.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "SIMPLib/FilterParameters/InputFileFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

// ITK includes
#include <itkImageIOFactory.h>
#include <itkPNGImageIOFactory.h>

// ITK-SCIFIO
#include <itkSCIFIOImageIOFactory.h>

// Include the MOC generated file for this class
#include "moc_ITKImageReader.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageReader::ITKImageReader() :
  AbstractFilter(),
  m_FileName(""),
  m_DataContainerName(SIMPL::Defaults::ImageDataContainerName)
{
  // As for now, register factories by hand. There is probably a better place
  // to do this than here.
  itk::PNGImageIOFactory::RegisterOneFactory();
  itk::SCIFIOImageIOFactory::RegisterOneFactory();

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageReader::~ITKImageReader()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::setupFilterParameters()
{
  FilterParameterVector parameters;
  QString supportedExtensions = ""; // \todo Change to actual supported formats
  parameters.push_back(
    InputFileFilterParameter::New("File", "FileName", getFileName(), FilterParameter::Parameter, supportedExtensions, "Image"));
  parameters.push_back(
    StringFilterParameter::New("Data Container", "DataContainerName", getDataContainerName(), FilterParameter::CreatedArray));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setFileName(reader->readString("FileName", getFileName()));
  setDataContainerName(reader->readString("DataContainerName", getDataContainerName()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::initialize()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::dataCheck()
{
  //check file name exists
  QString filename = getFileName();
  if (filename.isEmpty())
  {
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), "Invalid filename.", getErrorCondition());
    return;
  }

  // Try to print image properties:
  itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(filename.toLatin1(), itk::ImageIOFactory::ReadMode);
  if (NULL == imageIO)
  {
    setErrorCondition(-2);
    QString errorMessage = "ITK could not read the given file \"%1\". Format is likely unsupported.";
    notifyErrorMessage(getHumanLabel(), errorMessage.arg(filename), getErrorCondition());
    return;
  }
  imageIO->SetFileName(filename.toLatin1());
  imageIO->ReadImageInformation();

  typedef itk::ImageIOBase::IOComponentType ScalarPixelType;
  const ScalarPixelType pixelType = imageIO->GetComponentType();
  // Notifying a warning because it can't get the status messages in the GUI.
  notifyWarningMessage(getHumanLabel(), QString("Component Type: %1").arg(imageIO->GetComponentTypeAsString(pixelType).c_str()), 1);
  notifyWarningMessage(getHumanLabel(), QString("NumDimensions: %1").arg(imageIO->GetNumberOfDimensions()), 1);
  notifyWarningMessage(getHumanLabel(), QString("Component size:  %1").arg(imageIO->GetComponentSize()), 1);
  notifyWarningMessage(getHumanLabel(), QString("Pixel Type (string): %1").arg(imageIO->GetPixelTypeAsString(imageIO->GetPixelType()).c_str()), 1);
  notifyWarningMessage(getHumanLabel(), QString("Pixel Type : %1").arg(imageIO->GetPixelType()), 1);

  // If we got here, that means that there is no error
  setErrorCondition(0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageReader::execute()
{
  setErrorCondition(0);
  dataCheck();

  // \todo !
  /*if(getErrorCondition() < 0) { return; }

  if (getCancel() == true) { return; }

  if (getErrorCondition() < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }*/

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImageReader::newFilterInstance(bool copyFilterParameters)
{
  ITKImageReader::Pointer filter = ITKImageReader::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getGroupName()
{ return "Image Processing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getSubGroupName()
{ return "ITKImageProcessing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageReader::getHumanLabel()
{ return "[ITK] ImageReader (KW)"; }

