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
  dataCheck();
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
ITKImageBase::Pointer ITKImageBase::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
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
