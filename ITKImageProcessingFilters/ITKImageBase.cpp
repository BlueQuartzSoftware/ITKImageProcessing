/*
 * Your License or Copyright can go here
 */

#include "ITKImageBase.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageBase::ITKImageBase()
{
  initialize();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKImageBase::~ITKImageBase() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::initialize()
{
  clearErrorCondition();
  clearWarningCondition();
  setCancel(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::execute()
{
  initialize();
  this->dataCheckInternal();
  if(getErrorCode() < 0)
  {
    return;
  }
  if(getCancel())
  {
    return;
  }
  this->filterInternal();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  this->dataCheckInternal();
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false);    // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::dataCheckInternal()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKImageBase::filterInternal()
{
}
