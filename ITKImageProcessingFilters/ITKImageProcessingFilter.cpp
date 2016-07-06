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

#include "ITKImageProcessingFilter.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"



#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

// Include the MOC generated file for this class
#include "moc_ITKImageProcessingFilter.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingFilter::ITKImageProcessingFilter() :
  AbstractFilter()
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageProcessingFilter::~ITKImageProcessingFilter()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingFilter::setupFilterParameters()
{
  FilterParameterVector parameters;

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingFilter::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);

  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingFilter::initialize()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingFilter::dataCheck()
{
  setErrorCondition(0);

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageProcessingFilter::preflight()
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
void ITKImageProcessingFilter::execute()
{
  setErrorCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  if (getCancel() == true) { return; }

  if (getErrorCondition() < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKImageProcessingFilter::newFilterInstance(bool copyFilterParameters)
{
  ITKImageProcessingFilter::Pointer filter = ITKImageProcessingFilter::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingFilter::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingFilter::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingFilter::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingFilter::getGroupName()
{ return "Image Processing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingFilter::getSubGroupName()
{ return "ITKImageProcessing"; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageProcessingFilter::getHumanLabel()
{ return "[ITK] Image Processing Filter (KW)"; }

