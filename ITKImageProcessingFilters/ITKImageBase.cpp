/*
 * Your License or Copyright can go here
 */

#include "ITKImageBase.h"

#include "SIMPLib/Geometry/ImageGeom.h"



#include "ITKImageProcessing/ITKImageProcessingConstants.h"
#include "ITKImageProcessing/ITKImageProcessingVersion.h"

#include "moc_ITKImageBase.cpp"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageBase::ITKImageBase() :
  AbstractFilter(),
  m_SelectedCellArrayPath("", "", ""),
  m_NewCellArrayName(""),
  m_SaveAsNewArray(true),
  m_NewCellArray(nullptr)
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageBase::~ITKImageBase()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::initialize()
{
  setErrorCondition(0);
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  this->dataCheckInternal();
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::execute()
{
  initialize();
  this->dataCheckInternal();
  if (getErrorCondition() < 0) { return; }
  if (getCancel() == true) { return; }
  this->filterInternal();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getCompiledLibraryName()
{ return ITKImageProcessingConstants::ITKImageProcessingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getBrandingString()
{
  return "ITKImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getFilterVersion()
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  ITKImageProcessing::Version::Major() << "." << ITKImageProcessing::Version::Minor() << "." << ITKImageProcessing::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKImageBase::getGroupName()
{ return "Image Processing"; }
