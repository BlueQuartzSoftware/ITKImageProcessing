/*
 * Your License or Copyright can go here
 */

#include <memory>

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
  clearErrorCode();
  clearWarningCode();
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

// -----------------------------------------------------------------------------
ITKImageBase::Pointer ITKImageBase::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<ITKImageBase> ITKImageBase::New()
{
  struct make_shared_enabler : public ITKImageBase  
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString ITKImageBase::getNameOfClass() const
{
  return QString("ITKImageBase");
}

// -----------------------------------------------------------------------------
QString ITKImageBase::ClassName()
{
  return QString("ITKImageBase");
}


